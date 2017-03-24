/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

#include "Fractal.h"

#include "FractalMath.h"
#include "MandelbrotMath.h"
#include "JuliaMath.h"
#include "MathDriver.h"

#include <iostream>
#include <omp.h>
#include "OmpTools.h"

#include "IndiceTools_CPU.h"
using cpu::IndiceTools;

using std::cout;
using std::endl;

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/


const MathDriver::Math FRACTAL_TYPE = MathDriver::JULIA;


Fractal::Fractal(uint w, uint h, float dt, const DomaineMath& domaineMath) :
	Animable_I<uchar4>(w, h, "Mandelbrot", domaineMath),
	variateurAnimation(Interval<uint>(5, 250), 1)
    {
    this->t = 0;
    //this->parallelPatern = ParallelPatern::OMP_MIXTE;
    this->parallelPatern = ParallelPatern::OMP_FORAUTO;

    cout << "\n[Mandelbrot] : OMP : nbThread = " << this->nbThread << endl;
    }

Fractal::~Fractal()
    {}

void Fractal::animationStep()
    {
    this->t = variateurAnimation.varierAndGet();
    }

void Fractal::processEntrelacementOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    MathDriver fractalMath(FRACTAL_TYPE, t);

    #pragma omp parallel
	{
	    const int TID = OmpTools::getTid();
	    const int NB_THREADS = OmpTools::getNbThread();

	    int s = TID;
	    int i = 0, j = 0;
	    while(s < w*h) {
		IndiceTools::toIJ(s, w, &i, &j);
		workPixel(&ptrTabPixels[s], i, j, domaineMath, fractalMath.getPtrMath());
		s += NB_THREADS;
	    }
	}
    }

void Fractal::processForAutoOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    MathDriver fractalMath(FRACTAL_TYPE, t);
#pragma omp parallel for
	for(int i = 0; i < h; i++) {
	    for(int j = 0; j < w; j++) {
		int s = IndiceTools::toS(w, i, j);
		workPixel(&ptrTabPixels[s], i, j, domaineMath, fractalMath.getPtrMath());
	    }
	}
    }
/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

void Fractal::workPixel(uchar4* ptrColorIJ, int i, int j, const DomaineMath& domaineMath, FractalMath* fractalMath)
    {
	double x;
	double y;
	domaineMath.toXY(i, j, &x, &y);
	fractalMath->colorXY(ptrColorIJ, x, y);
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

