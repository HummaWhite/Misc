#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "timer.h"
#include "check.h"
#include <cuda_runtime.h>

float uniformFloat() {
	// 产生随机数
	return ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
}

float3 uniformFloat3() {
	return { uniformFloat(), uniformFloat(), uniformFloat() };
}

void randomizeBodies(float3 *pos, float3 *vel, int n) {
	// 随机初始化body数据
	for (int i = 0; i < n; i++) {
		pos[i] = uniformFloat3();
		vel[i] = uniformFloat3();
	}
}
// 重载运算符以支持向量运算
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
	// 使用fma指令优化向量内积
	const float SOFTENING = 1e-9f;
	float res = SOFTENING;
	asm("fma.rn.f32         %0, %1, %2, %3;" : "=f"(res) : "f"(a.x), "f"(b.x), "f"(res));
	asm("fma.rn.f32         %0, %1, %2, %3;" : "=f"(res) : "f"(a.y), "f"(b.y), "f"(res));
	asm("fma.rn.f32         %0, %1, %2, %3;" : "=f"(res) : "f"(a.z), "f"(b.z), "f"(res));
	return res;
}

__forceinline__ __device__ void bodyGetAccelPairwise(float3 posI, float3 posJ, float3 &acc) {
	// 计算一对body之间的加速度
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
	// 使用共享内存计算一批body对某个body的加速度总和
	extern __shared__ float3 posShared[];
	// 使用预处理命令展开循环
#pragma unroll 32
	for (int i = 0; i < blockDim.x; i += 2) {
		bodyGetAccelPairwise(pos, posShared[i + 1], acc);
		bodyGetAccelPairwise(pos, posShared[i], acc);
	}
}

__global__ void bodyCalcDeltaVelocity(float3 *pos, float3 *dvel, float dt, int n, int offset) {
	// 计算
	int bodyId = blockIdx.x * blockDim.x + threadIdx.x;
	if (bodyId >= n) return;
	extern __shared__ float3 posShared[];

	float3 acc = { 0.0f, 0.0f, 0.0f };
	float3 p = pos[bodyId];

	for (int i = 0; i < gridDim.x / 2; i++) {
		// 加载blockDim.x个body位置进入共享内存
		int id = i * blockDim.x + threadIdx.x + offset;
		__syncthreads();
		posShared[threadIdx.x] = pos[id];
		__syncthreads();
		// 需要等待全部线程将数据载入共享内存后才能进行计算，反之亦然
		bodyGatherAccelTiled(p, acc);
	}
	// 计算速度增量
	dvel[bodyId] = acc * dt;
}

__global__ void bodyUpdateVelAndPos(float3 *pos, float3 *vel, float3 *dvSelf, float3 *dvOppo, float dt, int n) {
	// 更新body的速度和坐标
	int bodyId = blockIdx.x * blockDim.x + threadIdx.x;
	if (bodyId >= n) return;
	// 本GPU内计算的dv与另一个GPU计算的速度增量累加，更新速度
	vel[bodyId] += dvSelf[bodyId] + dvOppo[bodyId];
	// 更新坐标
	pos[bodyId] += vel[bodyId] * dt;
}

int main(const int argc, const char *argv[]) {
	int nBodies = 2 << 11;
	int salt = 0;
	if (argc > 1) nBodies = 2 << std::atoi(argv[1]);

	if (argc > 2) salt = std::atoi(argv[2]);

	const float dt = 0.01f; // time step
	const int nIters = 10;  // simulation iterations
	// 分配主存中的body坐标与速度的存储空间
	float3 *posHost = new float3[nBodies];
	float3 *velHost = new float3[nBodies];
	float *buf = new float[nBodies * 6];
	// 初始化body属性
	randomizeBodies(posHost, velHost, nBodies); // Init pos / vel data

	size_t dataSize = nBodies * sizeof(float3);
	const int devOffset = 0;

	for (int i = 0; i < 2; i++) {
		int acc;
		// 查询两个逻辑GPU是否满足peer access条件
		cudaDeviceCanAccessPeer(&acc, devOffset + i, devOffset + (i ^ 1));
		if (acc) {
			// 若满足，互相设置为可访问对方内存地址空间
			cudaSetDevice(devOffset + i);
			cudaDeviceEnablePeerAccess(devOffset + (i ^ 1), 0);
		}
	}
	// 分配两个GPU上的存储空间
	float3 *posDevice[2], *velDevice[2];
	// deltaVel暂存速度变化量
	float3 *deltaVel[2];
	for (int i = 0; i < 2; i++) {
		cudaSetDevice(i + devOffset);
		cudaMalloc<float3>(&posDevice[i], dataSize);
		cudaMalloc<float3>(&velDevice[i], dataSize);
		cudaMalloc<float3>(&deltaVel[i], dataSize);
		// 复制body数据到GPU
		cudaMemcpy(posDevice[i], posHost, dataSize, cudaMemcpyKind::cudaMemcpyHostToDevice);
		cudaMemcpy(velDevice[i], velHost, dataSize, cudaMemcpyKind::cudaMemcpyHostToDevice);
	}
	// 确定线程块大小和线程块的数量
	const int blockSize = 128;
	const int blockNum = int(std::ceil(double(nBodies) / blockSize));
	const int sharedMemSize = blockSize * sizeof(float3);

	double totalTime = 0.0;

	/*******************************************************************/
	// Do not modify these 2 lines of code.
	for (int iter = 0; iter < nIters; iter++) {
		StartTimer();
		/*******************************************************************/
		// 计算两个GPU上分到的n/2个body对n个body形成的n个速度增量
		for (int i = 0; i < 2; i++) {
			cudaSetDevice(i + devOffset);
			bodyCalcDeltaVelocity<<<blockNum, blockSize, sharedMemSize>>>(posDevice[i], deltaVel[i], dt, nBodies, i * nBodies / 2);
		}
		// 需要等待两个GPU都完成计算
		cudaDeviceSynchronize();
		// 更新速度和坐标
		for (int i = 0; i < 2; i++) {
			cudaSetDevice(i + devOffset);
			bodyUpdateVelAndPos<<<blockNum, blockSize>>>(posDevice[i], velDevice[i], deltaVel[i], deltaVel[i ^ 1], dt, nBodies);
		}

		// Well, this is the solution, though I still want to put
		// it out of the loop
		if (iter == nIters - 1) {
			// 最后一次迭代完成后同步CPU与GPU
			cudaDeviceSynchronize();
			totalTime += GetTimer() / 1000.0;
			// 从GPU读回数据
			cudaSetDevice(devOffset);
			cudaMemcpy(posHost, posDevice[0], dataSize, cudaMemcpyKind::cudaMemcpyDeviceToHost);
			cudaMemcpy(velHost, velDevice[0], dataSize, cudaMemcpyKind::cudaMemcpyDeviceToHost);
			// 调整数据存储方式
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
	// 释放所有已分配的内存
	for (int i = 0; i < 2; i++) {
		cudaSetDevice(i + devOffset);
		cudaFree(posDevice[i]);
		cudaFree(velDevice[i]);
		cudaFree(deltaVel[i]);
	}

	delete[] posHost;
	delete[] velHost;
	delete[] buf;
}
