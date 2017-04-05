#include "Indice1D.h"
#include "cudaTools.h"
#include "reductionADD.h"
#include <curand_kernel.h>
#include <limits.h>

#include <stdio.h>



__global__ void montecarlo(float* ptrDevNx, int nbSamples, curandState* tabDevGenerator);
__global__ void setup_kernel_rand(curandState* tabDevGenerator, int deviceId);

__device__ void reducIntraThread(float* tab_SM, int nbSamples, curandState* tabDevGenerator);
__device__ float fPi(float x);
__device__ int work(float* tab_SM, float x, float y);

/**
 * output : void required !!
 */
__global__ void montecarlo(float* ptrDevNx, int nbSamples, curandState* tabDevGenerator)
    {
    extern __shared__ float tab_SM[]; //size defined in calling of kernel

    reducIntraThread(tab_SM, nbSamples, tabDevGenerator);
    __syncthreads();
    reductionADD<float>(tab_SM, ptrDevNx);
    }

__device__ void reducIntraThread(float* tab_SM, int nbSamples, curandState* tabDevGenerator)
    {
    const int TID=Indice1D::tid();
    const int TID_LOCAL = Indice1D::tidLocal();

    curandState localGenerator = tabDevGenerator[TID];
    float xAlea;
    float yAlea;
    float tmp = 0;
    for (long i = 1; i <= nbSamples; i++)
	{
	xAlea = curand_uniform(&localGenerator);
	yAlea = curand_uniform(&localGenerator);

	tmp += work(tab_SM, xAlea, yAlea);
	}

    tab_SM[TID_LOCAL] = tmp;

    tabDevGenerator [TID] = localGenerator;
    }

__device__ int work(float* tab_SM, float x, float y)
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
