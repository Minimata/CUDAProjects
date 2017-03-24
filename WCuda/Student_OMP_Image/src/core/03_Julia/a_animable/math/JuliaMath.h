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

class JuliaMath : public FractalMath
    {
    public:
	JuliaMath(uint n, double C1, double C2) :
	    FractalMath(n)
	    {
	    this->C1 = C1;
	    this->C2 = C2;
	    }

	virtual ~JuliaMath() {}

    protected:
	float f(double x, double y)
	    {
	    float n = -1;
	    float a = x, b = y;
	    for(int i = 0; i < N; i++) {
		float z = a*a + b*b;
		float tmp = a;
		a = a*a - b*b + C1;
		b = 2*tmp*b + C2;
		n = i;
		if(isDivergent(z)) return n;
	    }
	    return -1;
	    }
    private:
	double C1;
	double C2;

    };



/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
