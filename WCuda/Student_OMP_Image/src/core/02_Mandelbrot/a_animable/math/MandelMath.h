#pragma once

#include <math.h>
#include "MathTools.h"

#include "Calibreur_CPU.h"
#include "ColorTools_CPU.h"
using namespace cpu;


/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class MandelMath
    {
    public:
	MandelMath(uint n) :
	    calibreur(Interval<float>(0, n), Interval<float>(0, 1))
	{
	    this->N = n;
	}
	virtual ~MandelMath() {}

	void colorXY (uchar4* ptrColor, double x, double y)
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

	float f(double x, double y)
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

	bool isDivergent(float z)
	{
	    return abs(z) > 4;
	}

    private:
	uint N;
	Calibreur<float> calibreur;
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
