#include "Indice1D.h"
#include "cudaTools.h"

__global__ void myhistogram(int* ptrDevData, int dataMax, int dataSize);

__device__ void reducIntraThread(int* tab_SM, int* tab_GM, int dataSizeSM);
__device__ void reducInterBlock(int* tab_SM, int* tab_GM, int dataMax);
__device__ void initSM(int* tab_SM, int dataMax);
__device__ void resetGM(int* tab_GM, int dataMax);

__global__ void myhistogram(int* ptrDevData, int dataMax, int dataSize)
    {
    extern __shared__ int tab_SM[];

    initSM(tab_SM, dataMax);
    __syncthreads();
    reducIntraThread(tab_SM, ptrDevData, dataSize);
    __syncthreads();

    resetGM(ptrDevData, dataMax);
    __syncthreads();
    reducInterBlock(tab_SM, ptrDevData, dataMax);
    }

__device__ void initSM(int* tab_SM, int dataMax)
    {
    const int TID_LOCAL = Indice1D::tidLocal();
    const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

    int s = TID_LOCAL;

    while (s < dataMax)
	{
	tab_SM[s] = 0;
	s += NB_THREAD_LOCAL;
	}
    }

__device__ void reducIntraThread(int* tab_SM, int* tab_GM, int dataSize)
    {
    const int TID = Indice1D::tid();
    const int NB_THREAD = Indice1D::nbThread();

    int s = TID;

    while (s < dataSize)
	{
	atomicAdd(&tab_SM[tab_GM[s]], 1);
	s += NB_THREAD;
	}
    }

__device__ void resetGM(int* tab_GM, int dataMax)
    {
    const int TID = Indice1D::tid();
    const int NB_THREAD = Indice1D::nbThread();

    int s = TID;

    while (s < dataMax)
	{
	tab_GM[s] = 0;
	s += NB_THREAD;
	}
    }

__device__ void reducInterBlock(int* tab_SM, int* tab_GM, int dataMax)
    {
    const int TID = Indice1D::tid();
    const int NB_THREAD = Indice1D::nbThread();
    const int TID_LOCAL = Indice1D::tidLocal();
    const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

    int s = TID_LOCAL;

    while (s < dataMax)
	{
	atomicAdd(&tab_GM[s], tab_SM[s]);
	s += NB_THREAD_LOCAL;
	}
    }
