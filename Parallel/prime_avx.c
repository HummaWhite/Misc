#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "Include/mpi.h"

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

int generatePrimes(int16_t *primes, int n)
{
	// This is an optimized sieve with linear time complexity
	// For generation of primes no more than sqrt(n)
	// Parameter *table must be allocated in advance
	int size = (n - 1) >> 1;
	uint8_t *isPrime = (uint8_t*)malloc(size * sizeof(uint8_t));
	memset(isPrime, 0x10001, size * sizeof(uint8_t));

	int pCnt = 0;

	for (int i = 0; i < size; i++)
	{
		int num = (i << 1) + 3;
		if (isPrime[i]) primes[pCnt++] = num;
		for (int j = 0; j < pCnt; j++)
		{
			int multi = num * primes[j];
			if (multi >= n) break;
			isPrime[(multi - 3) >> 1] = 0;
			if (num % primes[j] == 0) break;
		}
	}

	free(isPrime);
	return pCnt;
}

int main (int argc, char *argv[])
{
	int globalCount;
	int procId;
	int procNum;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &procId);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Barrier(MPI_COMM_WORLD);
	double tElapsed = -MPI_Wtime();

	if (argc != 3)
	{
		if (procId == 0) printf("Command line: %s <m>\n", argv[0]);
		MPI_Finalize();
		return 0;
	}

	int maxNum = atoi(argv[1]);
	int sqrtNum = (int)(sqrt((float)(maxNum)));

	int lVal = (int64_t)(procId) * (maxNum - sqrtNum - 1) / procNum + sqrtNum + 2;
	int rVal = (int64_t)(procId + 1) * (maxNum - sqrtNum - 1) / procNum + sqrtNum + 1;

	if (~lVal & 1) lVal++;
	if (~rVal & 1) rVal--;
	int size = (rVal - lVal + 2) >> 1;

	// Allocate memory
	// Do NOT use any kind of auto pointer, too slow
	// Compress the marks from one per byte to one per bit,
	// this will reduce the distance between visits by 8 times,
	// thus may be able to avoid some cache misses
	
	size_t markTableSize = (size >> 3) + ((size & 0x7) > 0); // size / bits(int8)
	size_t primeTableSize = sqrtNum + 1; // (sqrtNum + 2 - 1) / 2 * sizeof(int16)

	// The two tables are joint together to form continuous memory
	uint8_t *marked = (uint8_t*)malloc(markTableSize + primeTableSize);
	if (marked == NULL)
	{
		printf("Cannot allocate enough memory\n");
		MPI_Finalize();
		return 0;
	}
	memset(marked, 0, sizeof(uint8_t) * markTableSize);

	int16_t *primes = (int16_t*)(marked + markTableSize);
	int pCnt = generatePrimes(primes, sqrtNum + 2);

	int subDivSize = atoi(argv[2]);
	int count = size;

	int plVal = lVal;
	int p0x7 = 0x7;
	int pone = 1;
	int pscale[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	// With even manual AVX assembly operations,
	// execution time is not significantly reduced.
	// It seems that the overhead is memory access
	// and cache hit rate
	//
	// Well, AVX does accelerate calculation by several times,
	// but memory latency is there...

#define asmv __asm __volatile

	asmv("vpbroadcastd	(%0), %%ymm0" : : "r"(&plVal));
	asmv("vpbroadcastd	(%0), %%ymm1" : : "r"(&p0x7));
	asmv("vpbroadcastd	(%0), %%ymm2" : : "r"(&pone));
	asmv("vmovdqu		(%0), %%ymm3" : : "r"(pscale));

	for (int offset = 0; offset < size; offset += subDivSize)
	{
		int l = lVal + (offset << 1);
		int r = min(lVal + ((offset + subDivSize) << 1) - 2, rVal);

		for (int i = 0; i < pCnt; i++)
		{
			// TODO: See if there are instructions to reduce
			int p = primes[i];
			if (p * p > r) break;

			int stride = p << 1;
			int start, end, times;

			if (i & 1)
			{
				if (r % p == 0) start = r;
				else
				{
					start = (r / p) * p;
					if (~start & 1) start -= p;
				}

				end = max(p * p, l);
				times = start - end + stride;
			}
			else
			{
				if (p * p > l) start = p * p;
				else
				{
					if (l % p == 0) start = l;
					else
					{
						start = (l / p + 1) * p;
						if (~start & 1) start += p;
					}
				}
				end = r;
				times = end - start + stride;
			}

			int dir = 1 - ((i & 1) << 1);

			int dstride = stride * dir;

			asmv("vpbroadcastd	(%0), %%ymm4" : : "r"(&dstride));	// pstride = dstride
			asmv("vpmulld		%ymm3, %ymm4, %ymm5");				// poff = pscale * pstride
			asmv("vpbroadcastd	(%0), %%ymm4" : : "r"(&start));		// fill pstride with start
			asmv("vpaddd		%ymm4, %ymm5, %ymm5");				// poff = pstride + poff = start + pscale * dstride

			stride <<= 3;
			dstride <<= 3;
			asmv("vpbroadcastd	(%0), %%ymm4" : : "r"(&dstride));

			int addi = times % stride;
			times = times / stride;

			int indices[8], bits[8];

			for (int j = 0; j < times; j++)
			{
				asmv("vpsubd	%ymm0, %ymm5, %ymm6");	// pindex = poff - lVal
				asmv("vpsrld	$1, %ymm6, %ymm6");		// pindex >>= 1
				asmv("vpand		%ymm1, %ymm6, %ymm7");	// pbits = pindex & 0x7
				asmv("vpsllvd	%ymm7, %ymm2, %ymm7");	// pbits = 1 << pbits
				asmv("vpsrld	$3, %ymm6, %ymm6");		// pindex >>= 3

				// TODO: Critical optimization area
				//       Reduce memory access here
				//       See if I can come up with some hacks...

				asmv("vmovdqu	%%ymm6, %0" : "=m"(indices) : );// store pindex -> indices
				asmv("vmovdqu	%%ymm7, %0" : "=m"(bits) : );	// store pbits -> bits

				for (int k = 0; k < 8; k++)
				{
					int ind = indices[k];
					count -= !(marked[ind] & bits[k]);
					marked[ind] |= bits[k];
				}
				asmv("vpaddd	%ymm4, %ymm5, %ymm5"); // poff += pstride
			}

			if (addi)
			{
				asmv("vmovdqu	%%ymm5, %0" : "=m"(indices) : ); // store poff -> off

				for (int k = 0; k < 8; k++)
				{
					if (i & 1)
					{
						if (indices[k] < l) break;
					}
					else
					{
						if (indices[k] > r) break;
					}
					int index = (indices[k] - lVal) >> 1;
					int bit = 1 << (index & 0x7);
					int ind = index >> 3;
					count -= !(marked[ind] & bit);
					marked[ind] |= bit;
				}
			}
		}
	}

	if (procNum > 1)
	{
		MPI_Reduce(&count, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else globalCount = count;

	tElapsed += MPI_Wtime();

	free(marked);

	if (procId == 0)
	{
		if (maxNum >= 2) globalCount += pCnt + 1;
		printf("There are %d primes less than or equal to %d\n", globalCount, maxNum);
		printf("SIEVE (%d) %.6lf", procNum, tElapsed);
	}

	MPI_Finalize();
	return 0;
}

