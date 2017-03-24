#pragma once

#include <math.h>
#include "MathTools.h"
#include "FractalMath.h"


/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class MandelbrotMath : public FractalMath
    {
    public:
	__device__ MandelbrotMath(uint n) :
	    FractalMath(n)
	    {
	    }
	__device__ virtual ~MandelbrotMath() {}



    protected:
	__device__ float f(double x, double y)
	    {
	    float n = -1;
	    float a = 0, b = 0;
	    for(int i = 0; i < N; i++) {
		float z = a*a + b*b;
		float tmp = a;
		a = a*a - b*b + x;
		b = 2*tmp*b + y;
		n = i;
		if(isDivergent(z)) return n;
	    }
	    return -1;
	    }
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
