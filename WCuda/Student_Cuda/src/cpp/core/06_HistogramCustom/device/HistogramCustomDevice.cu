#include "Indice1D.h"
#include "cudaTools.h"

__global__ void histogramCustom(int* ptrDevData, int dataMax, int dataSize, int bufferSizeSM);

__device__ void reducIntraThreadCustom(int* tab_SM, int* tab_GM, int dataSize, int dataMax, int bufferSizeSM);
__device__ void reducInterBlockCustom(int* tab_SM, int* tab_GM, int dataMax);
__device__ void initSMCustom(int* tab_SM, int dataMax);
__device__ void resetGMCustom(int* tab_GM, int dataMax);

__global__ void histogramCustom(int* ptrDevData, int dataMax, int dataSize, int bufferSizeSM)
    {
    extern __shared__ int tab_SM[];

    initSMCustom(tab_SM, dataMax);
    __syncthreads();
    reducIntraThreadCustom(tab_SM, ptrDevData, dataSize, dataMax, bufferSizeSM);
    __syncthreads();

    resetGMCustom(ptrDevData, dataMax);
    __syncthreads();
    reducInterBlockCustom(tab_SM, ptrDevData, dataMax);
    }

__device__ void initSMCustom(int* tab_SM, int dataMax)
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

__device__ void reducIntraThreadCustom(int* tab_SM, int* tab_GM, int dataSize, int dataMax, int bufferSizeSM)
    {
    const int TID = Indice1D::tid();
    const int NB_THREAD = Indice1D::nbThread();
    const int TID_LOCAL = Indice1D::tidLocal();
    const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

    int s = TID;
    int t = 0;

    //Pattern d'entrelacement sur la GM avec pour unité de copie la transaction
    while (s < dataSize)
	{
	if (TID_LOCAL == 0)
	{
	/** All of these are host methods...
	 *  Apparently memory managment device-side is only int by int, float by float, etc.
	 *  Too bad =(
	 * */

	// cudaMalloc(...);
	// cudaMemcpy(&tab_SM[dataMax],&tab_GM[s],bufferSizeSM, cudaMemcpyDeviceToDevice);
	// memcpy(ptrDevTabSM, ptrDevTabGM, sizeof(int) * dataSizeSM);
	// std::copy(tab_GM + s, tab_GM + s + bufferSizeSM, tab_SM + dataMax);
	}

    __syncthreads();  //I know it's bad

    //Pattern d'entrelacement sur le buffer de la SM
    t = TID_LOCAL;
    while (t < bufferSizeSM)
	{
	atomicAdd(&tab_SM[tab_SM[dataMax + t]], 1);
	t += NB_THREAD_LOCAL;
	}

    s += NB_THREAD;
    }
}

__device__ void resetGMCustom(int* tab_GM, int dataMax)
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

__device__ void reducInterBlockCustom(int* tab_SM, int* tab_GM, int dataMax)
{
const int TID = Indice1D::tid();
const int NB_THREAD = Indice1D::nbThread();
const int TID_LOCAL = Indice1D::tidLocal();
const int NB_THREAD_LOCAL = Indice1D::nbThreadBlock();

int s = 0;

while (s < dataMax)
    {
    atomicAdd(&tab_GM[s], tab_SM[s]);
    s += NB_THREAD_LOCAL;
    }
}

