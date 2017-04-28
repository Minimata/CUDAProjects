#include "RayTracing.h"
#include "length_cm.h"

#include <iostream>
#include <assert.h>

#include "Device.h"

#include "Sphere.h"
#include "SphereCreator.h"

using std::cout;
using std::endl;

extern __global__ void raytracing_GM(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t);
extern __global__ void raytracing_CM(uchar4* ptrDevPixels, uint w, uint h, float t);
extern __global__ void raytracing_SM(Sphere* ptrDevTabSphere, int nbSphere, uchar4* ptrDevPixels, uint w, uint h, float t);

extern __host__ void uploadGPU(Sphere* tabValue);

RayTracing::RayTracing(const Grid& grid, uint w, uint h, float dt, int nbSphere) :
	Animable_I<uchar4>(grid, w, h, "Raytracing_Cuda_RGBA_uchar4")
    {
    // Inputs
    this->dt = dt;

    // Tools
    this->t = 0; // protected dans Animable
    this->nbSphere = nbSphere;

    this->sizeOctet = nbSphere * sizeof(Sphere);

    SphereCreator sphereCreator(nbSphere, w, h); // sur la pile
    Sphere* ptrTabSphere = sphereCreator.getTabSphere();
    // transfert to GM
    toGM(ptrTabSphere);
    // transfert to CM
    toCM(ptrTabSphere);

    }

RayTracing::~RayTracing()
    {
    Device::free(ptrDevTabSphere);
    }

/*-------------------------*\
 |*	Methode		    *|
 \*-------------------------*/

void RayTracing::toGM(Sphere* ptrTabSphere)
    {
    Device::malloc(&ptrDevTabSphere, sizeOctet);
    Device::memclear(ptrDevTabSphere, sizeOctet);
    Device::memcpyHToD(ptrDevTabSphere, ptrTabSphere, sizeOctet);

    }
void RayTracing::toCM(Sphere* ptrTabPixel)
    {
    uploadGPU(ptrTabPixel);
    }

/**
 * Override
 * Call periodicly by the API
 *
 * Note : domaineMath pas use car pas zoomable
 */
void RayTracing::process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    Device::lastCudaError("raytracing rgba uchar4 (before kernel)"); // facultatif, for debug only, remove for release

    static int i = 0;
    if (i % 3 == 0)
	{
	 raytracing_GM<<<dg,db>>>(this->ptrDevTabSphere, this->nbSphere, ptrDevPixels, w, h, t);
	}
    else if (i % 3 == 1)
	{
	 raytracing_CM<<<dg,db>>>(ptrDevPixels, w, h, t);
	}
    else if (i % 3 == 2)
	{
	 raytracing_SM<<<dg,db, this->sizeOctet>>>(this->ptrDevTabSphere, this->nbSphere, ptrDevPixels, w, h, t);
	}
    i++;

    Device::lastCudaError("raytracing rgba uchar4 (after kernel)"); // facultatif, for debug only, remove for release
    }

/**
 * Override
 * Call periodicly by the API
 */
void RayTracing::animationStep()
{
    t += dt;
}

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

