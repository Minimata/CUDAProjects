#pragma once
#include "FractalMath.h"
#include "MandelbrotMath.h"
#include "JuliaMath.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class MathDriver
    {
    public:

	enum Math {
	    MANDELBROT,
	    JULIA
	};

	MathDriver(Math m, uint n, double c1 = -0.51, double c2 = 0.52)
	    {
	    switch(m) {
		case MANDELBROT:
		    this->ptrMath = new MandelbrotMath(n);
		    break;
		case JULIA:
		    this->ptrMath = new JuliaMath(n, c1, c2);
		    break;
		default:
		    this->ptrMath = new JuliaMath(n, c1, c2);
		    break;
	    }
	    }

	FractalMath* getPtrMath() { return this->ptrMath; }

	~MathDriver() {
	    delete this->ptrMath;
	}


    private:
	FractalMath* ptrMath;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
