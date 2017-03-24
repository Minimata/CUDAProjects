#pragma once

#include "MathTools.h"
#include "cudaTools.h"
#include "Shape3D.h"

using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class SpherePerso: public Shape3D
    {
    public:
	__device__
	SpherePerso(float3 centre, float rayon, float hue) :
		Shape3D(hue)
	    {
	    this->centre = centre;
	    this->r = rayon;
	    }
	__device__
	~SpherePerso()
	    {
	    }
	__device__
	float getBrightnessAtPoint(int x, int y, float t = 0)
	    {
	    return 1 - (getDistanceToCenter(x, y) / this->r);
	    }

    private:

	__device__
	float getDistanceToCenter(int x, int y) {
	    return sqrt(pow(x - centre.x, 2) + pow(x - centre.y, 2));
	}

	float r;
	float3 centre;

    };
/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/


