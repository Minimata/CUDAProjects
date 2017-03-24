#pragma once

#include <math.h>
#include "MathTools.h"

#include "ColorTools_GPU.h"

#include "Sphere.h"

using namespace gpu;



class RayTracingMath
    {


    public:

	__device__ RayTracingMath(int nbSphere, Sphere* ptrDevTabSphere)
	    {
	    this->nbSphere = nbSphere;
	    this->ptrDevTabSphere = ptrDevTabSphere;
	    }

	__device__
	 virtual ~RayTracingMath()
	    {
	    // rien
	    }

	__device__
	void colorIJ(uchar4* ptrColor, int i, int j, float t)
	    {
	    float min = 1000.f;
	    float hueMin = -1000.f;
	    float brightnessMin = -1000.f;

	    float2 position;
	    position.x = i;
	    position.y = j;

	    for(int i = 0; i < this->nbSphere; i++)
		{
		Sphere sphere = this->ptrDevTabSphere[i];
		float hCarre = sphere.hCarre(position);

		if(sphere.isEnDessous(hCarre))
		    {
		    float dz = sphere.dz(hCarre);
		    float distance = sphere.distance(dz);

		    if(distance < min)
			{
			min = distance;
			hueMin = sphere.hue(t);
			brightnessMin = sphere.brightness(dz);
			}
		    }
		}

	    if (hueMin >= 0)
		{
		ColorTools::HSB_TO_RVB(hueMin, 1.f, brightnessMin, ptrColor);
		}
	    else
		{
		ptrColor->x = 0;
		ptrColor->y = 0;
		ptrColor->z = 0;
		}

	    ptrColor->w = 255;
	    }

    private:

	// Inputs
	int nbSphere;
	Sphere* ptrDevTabSphere;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
