#include "Indice1D.h"
#include "cudaTools.h"
#include <algorithm>

__global__ void histogramCustom(int** ptrDevData, int dataMax, int dataSizeSM);

__device__ void reducIntraThreadCustom(int* tab_SM, int dataSizeSM);
__device__ void reducInterBlockCustom(int* tab_SM, int* tab_GM, int dataSize, int dataMax);
__device__ void copyGMToSMCustom(int* ptrDevTabGM, int* ptrDevTabSM, int dataSizeSM);
__device__ void initSMCustom(int* tab_SM, int dataMax);



__global__ void histogramCustom(int** ptrDevData, int dataMax, int dataSizeSM)
    {
    extern __shared__ int tab_SM[];
    //int tab_GM[] = *(ptrDevData);

    /*
    initSMCustom(tab_SM, dataMax);
    copyGMToSMCustom(tab_GM, tab_SM, dataSizeSM);
    reducIntraThreadCustom (tab_SM, dataSizeSM);
    __syncthreads();
    reducInterBlockCustom(tab_SM, tab_GM, dataSizeSM, dataMax);
    */
    }

__device__ void initSMCustom(int* tab_SM, int dataMax)
    {
    const int TID_LOCAL = Indice1D::tidLocal();
    const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

    int s = TID_LOCAL;

    while(s < dataMax)
	{
	tab_SM[s] = 0;
	s+=NB_THREAD_LOCAL;
	}
    }

__device__ void reducIntraThreadCustom(int* tab_SM, int dataSizeSM)
    {
    const int TID = Indice1D::tid();
    const int NB_THREAD = Indice1D::nbThread();
    const int TID_LOCAL = Indice1D::tidLocal();

    int s = TID;

    while(s < dataSizeSM)
	{
	//atomicAdd(dataSizeSM + tab_SM[s], 1);
	s += NB_THREAD;
	}
    }

__device__ void reducInterBlockCustom(int* tab_SM, int* tab_GM, int dataSizeSM, int dataMax)
    {
    const int TID = Indice1D::tid();
    const int NB_THREAD = Indice1D::nbThread();
    const int TID_LOCAL = Indice1D::tidLocal();
    const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

    int s = 0;

    while(s < dataMax)
	{
	if(TID < dataMax)
	    {
	    tab_GM[s] = 0;
	    }

	s += NB_THREAD;
	}

    __syncthreads();

    s = 0;

    while(s < dataMax)
	{
	//atomicAdd(tab_GM + s, tab_SM[dataSizeSM + s]);
	s += NB_THREAD_LOCAL;
	}
    }

__device__ void copyGMToSMCustom(int* ptrDevTabGM, int* ptrDevTabSM, int dataSizeSM){

    const int TID = Indice1D::tid();
    const int TID_LOCAL = Indice1D::tidLocal();
    const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

    if(TID_LOCAL == 0)
	{
	// memcpy(ptrDevTabSM, ptrDevTabGM, sizeof(int) * dataSizeSM);
	//std::copy(ptrDevTabGM + s, ptrDevTabGM + s + dataSizeSM, ptrDevTabSM);
	}

}

