#include "Indice1D.h"
#include "cudaTools.h"
#include "reductionADD.h"
#include <curand_kernel.h>
#include <limits.h>

#include <stdio.h>

using elem = unsigned int;

__global__ void montecarlo(elem* ptrDevNx, elem nbSamples, curandState* tabDevGenerator, float targetHeight);
__global__ void setup_kernel_rand(curandState* tabDevGenerator, int deviceId);

__device__ void reducIntraThread(elem* tab_SM, elem nbSamples, curandState* tabDevGenerator, float targetHeight);
__device__ float fPi(float x);
__device__ elem work(float x, float y);

/**
 * output : void required !!
 */
__global__ void montecarlo(elem* ptrDevNx, elem nbSamples, curandState* tabDevGenerator, float targetHeight)
    {
    extern __shared__ elem tab_SM[]; //size defined in calling of kernel

    reducIntraThread(tab_SM, nbSamples, tabDevGenerator, targetHeight);
    __syncthreads();
    reductionADD<elem>(tab_SM, ptrDevNx);
    }

__device__ void reducIntraThread(elem* tab_SM, elem nbSamples, curandState* tabDevGenerator, float targetHeight)
    {
    const int TID=Indice1D::tid();
    const int TID_LOCAL = Indice1D::tidLocal();

    curandState localGenerator = tabDevGenerator[TID];
    float xAlea;
    float yAlea;
    float tmp = 0;
    for (elem i = 1; i <= nbSamples; i++)
	{
	xAlea = curand_uniform(&localGenerator);
	yAlea = curand_uniform(&localGenerator);

	tmp += work(xAlea, targetHeight * yAlea);
	}

    tab_SM[TID_LOCAL] = tmp;

    tabDevGenerator [TID] = localGenerator;
    }

__device__ elem work(float x, float y)
    {
    return (int) (fPi(x) >= y);
    }

__device__ float fPi(float x)
    {
    return 1.f / (1.f + x * x);
    }



// Each thread gets same seed, a different sequence number
// no offset
__global__
void setup_kernel_rand(curandState* tabDevGenerator, int deviceId)
    {
    // Customisation du generator:
    // Proposition, au lecteur de faire mieux !
    // Contrainte : Doit etre différent d'un GPU à l'autre
    // Contrainte : Doit etre différent d’un thread à l’autre
    const int TID = Indice1D::tid();
    int deltaSeed = deviceId * INT_MAX / 10000;
    int deltaSequence = deviceId * 100;
    int deltaOffset = deviceId * 100;
    int seed = 1234 + deltaSeed;
    int sequenceNumber = TID + deltaSequence;
    int offset = deltaOffset;
    curand_init(seed, sequenceNumber, offset, &tabDevGenerator[TID]);
    }
