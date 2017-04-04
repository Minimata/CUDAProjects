#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"
#include "length_cm.h"

#include "IndiceTools_GPU.h"

#include "RayTracingMath.h"
using namespace gpu;

__host__ void uploadGPU(Sphere* tabValue);

__global__ void raytracing_GM(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t);
__global__ void raytracing_CM(uchar4* ptrDevPixels, uint w, uint h, float t);
__global__ void raytracing_SM(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t);

__device__ void copyGMToSM(Sphere* ptrDevTabGM, Sphere* ptrDevTabSM, int nbSphere);
__device__ void work(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t);

// Déclaration Constante globale
__constant__ Sphere TAB_CM[LENGTH_CM];
__constant__ Sphere TAB_SM[LENGTH_CM];



/**
 * call once by the host
 */
__host__ void uploadGPU(Sphere* tabValue)
    {
    size_t size = LENGTH_CM * sizeof(Sphere);
    int offset = 0;
    HANDLE_ERROR(cudaMemcpyToSymbol(TAB_CM, tabValue, size, offset, cudaMemcpyHostToDevice));
    }



__global__ void raytracing_GM(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t)
    {
    work(ptrDevTabSphere, nbSphere, ptrDevPixels, w, h, t);
    }
__global__ void raytracing_CM(uchar4* ptrDevPixels, uint w, uint h, float t)
    {
    work(TAB_CM, LENGTH_CM, ptrDevPixels, w, h, t);
    }
__global__ void raytracing_SM(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t)
    {
    extern __shared__ Sphere tabSM[];
    copyGMToSM(ptrDevTabSphere, tabSM, nbSphere);
    __syncthreads();
    work(tabSM, nbSphere, ptrDevPixels, w, h, t);
    }

__device__ void copyGMToSM(Sphere* ptrDevTabGM, Sphere* ptrDevTabSM, int nbSphere){

    const int TID_LOCAL = Indice2D::tidLocal();
    const int NB_THREAD_LOCAL = Indice2D::nbThreadLocal();

    int s = TID_LOCAL;

    while(s < nbSphere){
	ptrDevTabSM[s] = ptrDevTabGM[s];
	s+= NB_THREAD_LOCAL;
    }

}

__device__ void work(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t)
    {
    RayTracingMath rayTracingMath = RayTracingMath(nbSphere, ptrDevTabSphere);

    const int TID = Indice2D::tid();
    const int NB_THREAD = Indice2D::nbThread();

    int i, j;

    int s = TID;
    while (s < w * h)
	{
	IndiceTools::toIJ(s, w, &i, &j);

	rayTracingMath.colorIJ(&s[ptrDevPixels], i, j, t);

	s += NB_THREAD;
	}
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

