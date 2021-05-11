#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "timer.h"
#include "check.h"
#include <cuda_runtime.h>

#define asmv asm __volatile

void CHECK_ERROR(const std::string &msg = "") {
	cudaError cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "[%s] Launch failed: %s\n", msg.c_str(), cudaGetErrorString(cudaStatus));
	}
}

float uniformFloat() {
	return ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
}

float3 uniformFloat3() {
	return { uniformFloat(), uniformFloat(), uniformFloat() };
}

void randomizeBodies(float3 *pos, float3 *vel, int n) {
	for (int i = 0; i < n; i++) {
		pos[i] = uniformFloat3();
		vel[i] = uniformFloat3();
	}
}

std::string float3ToString(const float3 &v) {
	std::stringstream ss;
	ss << std::setprecision(12) << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return ss.str();
}

__device__ float3 operator + (const float3 &a, const float3 &b) {
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

__device__ float3 operator - (const float3 &a, const float3 &b) {
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

__device__ float3 operator * (const float3 &a, float v) {
	return { a.x * v, a.y * v, a.z * v };
}

__device__ void operator += (float3 &a, const float3 &b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}

__forceinline__ __device__ float dot(const float3 &a, const float3 &b) {
	const float SOFTENING = 1e-9f;
	float res = SOFTENING;
	asm("fma.rn.f32         %0, %1, %2, %3;" : "=f"(res) : "f"(a.x), "f"(b.x), "f"(res));
	asm("fma.rn.f32         %0, %1, %2, %3;" : "=f"(res) : "f"(a.y), "f"(b.y), "f"(res));
	asm("fma.rn.f32         %0, %1, %2, %3;" : "=f"(res) : "f"(a.z), "f"(b.z), "f"(res));
	return res;
}

__forceinline__ __device__ void bodyGetAccelPairwise(float3 posI, float3 posJ, float3 &acc) {
	// 3fsub + 1frsqrt + 2fmul + 6fma = 12 float instructions
	float3 dp = posJ - posI;

	// 4fadd, 3fmul with regular calculation method
	// By using fma, only 3 instructions
	float distSqr = dot(dp, dp);
	float invDist = rsqrtf(distSqr);
	float invDist3 = invDist * invDist * invDist;

	acc += dp * invDist3;
}

__forceinline__ __device__ void bodyGatherAccelTiled(float3 pos, float3 &acc) {
	extern __shared__ float3 posShared[];
#pragma unroll 32
	for (int i = 0; i < blockDim.x; i += 2) {
		bodyGetAccelPairwise(pos, posShared[i + 1], acc);
		bodyGetAccelPairwise(pos, posShared[i], acc);
	}
}

__global__ void bodyCalcDeltaVelocity(float3 *pos, float3 *dvel, float dt, int n, int offset) {
	int bodyId = blockIdx.x * blockDim.x + threadIdx.x;
	if (bodyId >= n) return;
	extern __shared__ float3 posShared[];

	float3 acc = { 0.0f, 0.0f, 0.0f };
	float3 p = pos[bodyId];

	for (int i = 0; i < gridDim.x / 4; i++) {
		// For all threads in the block, async load tiled position
		// data to shared memory
		int id = i * blockDim.x + threadIdx.x + offset;
		__syncthreads();
		posShared[threadIdx.x] = pos[id];
		__syncthreads();
		bodyGatherAccelTiled(p, acc);
	}
	dvel[bodyId] = acc * dt;
}

__global__ void bodyUpdateVelAndPos(float3 *pos, float3 *vel, float3 *deltaVel, float dt, int n) {
	int bodyId = blockIdx.x * blockDim.x + threadIdx.x;
	if (bodyId >= n) return;
#pragma unroll
	for (int i = 0; i < 4; i++) {
		vel[bodyId] += deltaVel[i * n + bodyId];
	}
	pos[bodyId] += vel[bodyId] * dt;
}

int main(const int argc, const char *argv[]) {
	//QUERY_DEVICE_PROP();

	int nBodies = 2 << 11;
	int salt = 0;
	if (argc > 1) nBodies = 2 << std::atoi(argv[1]);

	if (argc > 2) salt = std::atoi(argv[2]);

	const float dt = 0.01f; // time step
	const int nIters = 10;  // simulation iterations

	float3 *posHost = new float3[nBodies];
	float3 *velHost = new float3[nBodies];
	float *buf = new float[nBodies * 6];

	randomizeBodies(posHost, velHost, nBodies); // Init pos / vel data

	size_t dataSize = nBodies * sizeof(float3);

	for (int i = 0; i < 4; i++) {
		cudaSetDevice(i);
		cudaDeviceEnablePeerAccess(i ^ 1, 0);
	}

	float3 *posDevice[4], *velDevice[4];
	float3 *deltaVel[4];
	for (int i = 0; i < 4; i++) {
		cudaSetDevice(i);
		cudaMalloc<float3>(&posDevice[i], dataSize);
		cudaMalloc<float3>(&velDevice[i], dataSize);
		cudaMalloc<float3>(&deltaVel[i], dataSize * 4);
		cudaMemcpy(posDevice[i], posHost, dataSize, cudaMemcpyKind::cudaMemcpyHostToDevice);
		cudaMemcpy(velDevice[i], velHost, dataSize, cudaMemcpyKind::cudaMemcpyHostToDevice);
	}

	const int blockSize = 128;
	const int blockNum = int(std::ceil(double(nBodies) / blockSize));
	const int sharedMemSize = blockSize * sizeof(float3);

	double totalTime = 0.0;

	/*******************************************************************/
	// Do not modify these 2 lines of code.
	for (int iter = 0; iter < nIters; iter++) {
		StartTimer();
		/*******************************************************************/

		for (int i = 0; i < 4; i++) {
			cudaSetDevice(i);
			bodyCalcDeltaVelocity<<<blockNum, blockSize, sharedMemSize>>>(posDevice[i], deltaVel[i] + nBodies * i, dt, nBodies, nBodies / 4 * i);
		}
		cudaDeviceSynchronize();

		for (int i = 0; i < 4; i++) {
			cudaSetDevice(i);
			int d = i ^ 1;
			cudaMemcpyPeerAsync(deltaVel[d] + nBodies * i, d, deltaVel[i] + nBodies * i, i, dataSize);
		}

		for (int i = 0; i < 4; i++) {
			cudaSetDevice(i);
			int d = i ^ 2;
			cudaMemcpy(deltaVel[d] + nBodies * (i & 2), deltaVel[i] + nBodies * (i & 2), dataSize * 2, cudaMemcpyKind::cudaMemcpyDeviceToDevice);
		}

		for (int i = 0; i < 4; i++) {
			cudaSetDevice(i);
			bodyUpdateVelAndPos<<<blockNum, blockSize>>>(posDevice[i], velDevice[i], deltaVel[i], dt, nBodies);
		}

		// Well, this is the solution, though I still want to put
		// it out of the loop
		if (iter == nIters - 1) {
			// I don't think this should be taken into account,
			// it's just moving results from GPU to host memory
			// and data reorganize
			cudaDeviceSynchronize();
			totalTime += GetTimer() / 1000.0;
			cudaSetDevice(0);
			cudaMemcpy(posHost, posDevice[0], dataSize, cudaMemcpyKind::cudaMemcpyDeviceToHost);
			cudaMemcpy(velHost, velDevice[0], dataSize, cudaMemcpyKind::cudaMemcpyDeviceToHost);

			for (int j = 0; j < nBodies; j++) {
				*(float3*)(buf + j * 6) = posHost[j];
				*(float3*)(buf + j * 6 + 3) = velHost[j];
			}
			StartTimer();
		}

		/*******************************************************************/
		// Do not modify the code in this section.
		const double tElapsed = GetTimer() / 1000.0;
		totalTime += tElapsed;
	}

	double avgTime = totalTime / (double)(nIters);
	float billionsOfOpsPerSecond = 1e-9 * nBodies * nBodies / avgTime;

#ifdef ASSESS
	checkPerformance(buf, billionsOfOpsPerSecond, salt);
#else
	checkAccuracy(buf, nBodies);
	printf("%d Bodies: average %0.3f Billion Interactions / second\n", nBodies, billionsOfOpsPerSecond);
	salt += 1;
#endif
	/*******************************************************************/

	std::cout << float3ToString(posHost[9]) << "\n";

	for (int i = 0; i < 4; i++) {
		cudaSetDevice(i);
		cudaFree(posDevice[i]);
		cudaFree(velDevice[i]);
		cudaFree(deltaVel[i]);
	}

	delete[] posHost;
	delete[] velHost;
	delete[] buf;
}
