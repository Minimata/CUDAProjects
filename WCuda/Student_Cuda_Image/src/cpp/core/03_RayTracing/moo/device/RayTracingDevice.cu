#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"

#include "IndiceTools_GPU.h"

#include "RayTracingMath.h"
using namespace gpu;


__global__ void raytracing(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels,uint w, uint h, float t);


__global__ void raytracing(Sphere* ptrDevTabSphere,int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t)
    {
    RayTracingMath rayTracingMath = RayTracingMath(nbSphere, ptrDevTabSphere);

    const int TID = Indice2D::tid();
    const int NB_THREAD = Indice2D::nbThread();

    int i, j;

    int s = TID;
    while (s < w*h)
	{
	IndiceTools::toIJ(s, w, &i, &j);

	rayTracingMath.colorIJ(&s[ptrDevPixels],i, j, t);

	s += NB_THREAD;
	}
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

