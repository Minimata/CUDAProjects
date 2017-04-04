#pragma once


template <typename T>
__device__ void reductionAdd(T* tabSM, T* ptrDevResultat);


template <typename T>
static __device__ void reductionIntraBlock(T* tabSM);


template <typename T>
static __device__ void reductionInterblock(T* tabSM, T* ptrDevResultat);




template <typename T>
__device__ void reductionAdd(T* tabSM, T* ptrDevResultat)
    {
    reductionIntraBlock(tabSM);

    __syncthreads();

    reductionInterblock(tabSM, ptrDevResultat);
    }


template <typename T>
static __device__ void reductionIntraBlock(T* tabSM)
    {
    const int FULL_SIZE = blockDim.x * blockDim.y * blockDim.z; //in case of 3D
    const int ID_LOCAL = threadIdx.x;  //no 3d pls

    int half = FULL_SIZE/2;

    while(half >= 1)
	{
	if(ID_LOCAL < half) tabSM[ID_LOCAL] += tabSM[ID_LOCAL + half];
	half >>= 1;
	__syncthreads();
	}
    }

template <typename T>
static __device__ void reductionInterblock(T* tabSM, T* ptrDevResultat)
    {
    if(threadIdx.x == 0 and threadIdx.y == 0 and threadIdx.z == 0)
	{
	atomicAdd(ptrDevResultat, tabSM[0]);
	}
    }
