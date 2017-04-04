#include "Indice2D.h"
#include "Indice1D.h"
#include "cudaTools.h"
//#include "reductionTools.h"
#include "reductionADD.h"

#include <stdio.h>


__global__ void slice(float* ptrDevPi, int nbSlice);

__device__ void reducIntraThread(float* tab_SM, int nbSlice);
__device__ float fpi(float x);



/**
 * output : void required !!
 */
__global__ void slice(float* ptrDevPi, int nbSlice)
    {
    extern __shared__ float tab_SM[]; //size defined in calling of kernel

    reducIntraThread(tab_SM, nbSlice);
    __syncthreads();
    //reductionAdd<float>(tab_SM, ptrDevPi);
    reductionADD<float>(tab_SM, ptrDevPi);

    }

__device__ void reducIntraThread(float* tab_SM, int nbSlice)
    {
    const int NB_THREAD=Indice2D::nbThread();
    const int TID=Indice2D::tid();
    const int TID_LOCAL=Indice2D::tidLocal();
    const float DX = 1.f / (float) nbSlice;

    int s = TID;
    float tmp = 0;
    while(s < nbSlice)
	{
	tmp += fpi(s * DX);
	s += NB_THREAD;
	}

    tab_SM[TID_LOCAL] = tmp * DX;
    }


__device__ float fpi(float x)
    {
    return 4.f / (1.f + x * x);
    }
