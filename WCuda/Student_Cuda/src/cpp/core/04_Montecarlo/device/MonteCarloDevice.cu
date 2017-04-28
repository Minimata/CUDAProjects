#include "Indice1D.h"
#include "cudaTools.h"
#include "reductionADD.h"
#include <curand_kernel.h>
#include <limits.h>

#include <stdio.h>

using elem = unsigned int;

__global__ void montecarlo(elem* ptrDevNx, elem nbSamples, curandState* tabDevGenerator, float targetHeight, float right, float left, int functionID);
__global__ void setup_kernel_rand(curandState* tabDevGenerator, int deviceId);

__device__ void reducIntraThread(elem* tab_SM, elem nbSamples, curandState* tabDevGenerator, float targetHeight, float right, float left, int functionID);
__device__ elem work(float x, float y, int functionID);
__device__ float map(float x, float left, float right);


__device__ float fPi(float x);
__device__ float identity(float x);
__device__ float squared(float x);


typedef float (*f)(float);

__device__ static f functions[3] = {
	fPi,
	identity,
	squared
};


/**
 * output : void required !!
 */
__global__ void montecarlo(elem* ptrDevNx, elem nbSamples, curandState* tabDevGenerator, float targetHeight, float right, float left, int functionID)
    {
    extern __shared__ elem tab_SM[]; //size defined in calling of kernel

    reducIntraThread(tab_SM, nbSamples, tabDevGenerator, targetHeight, right, left, functionID);
    __syncthreads();
    reductionADD<elem>(tab_SM, ptrDevNx);
    }

__device__ void reducIntraThread(elem* tab_SM, elem nbSamples, curandState* tabDevGenerator, float targetHeight, float right, float left, int functionID)
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

	xAlea = map(xAlea, left, right);

	tmp += work(xAlea, targetHeight * yAlea, functionID);
	}

    tab_SM[TID_LOCAL] = tmp;

    tabDevGenerator [TID] = localGenerator;
    }

__device__ elem work(float x, float y, int functionID)
    {
    return (elem) (functions[functionID](x) >= y);
    }

__device__ float map(float x, float left, float right)
    {
    return (right - left) * x + left;
    }

__device__ float squared(float x)
    {
    return x*x;
    }

__device__ float identity(float x)
    {
    return x;
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
