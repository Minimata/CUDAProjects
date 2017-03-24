#pragma once

#include <math.h>
#include "MathTools.h"

#include "Calibreur_GPU.h"
#include "ColorTools_GPU.h"
using namespace gpu;


/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class FractalMath
    {
    public:
	__device__ FractalMath(uint n) :
	    calibreur(Interval<float>(0, n), Interval<float>(0, 1))
	{
	    this->N = n;
	}
	__device__ virtual ~FractalMath() {}

	__device__ void colorXY (uchar4* ptrColor, double x, double y)
	    {

	    float k = f(x, y);

	    if(k > 0) {
		calibreur.calibrer(k);
		ColorTools::HSB_TO_RVB(k, ptrColor);
	    }
	    else {
		ptrColor->x = 0;
		ptrColor->y = 0;
		ptrColor->z = 0;
	    }


	    ptrColor->w = 255;
	    }


    protected:

	__device__ virtual float f(double x, double y) = 0;
	__device__ bool isDivergent(float z)
	{
	    return abs(z) > 4;
	}

	uint N;
	Calibreur<float> calibreur;

    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
