/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

#include "Mandelbrot.h"
#include "MandelMath.h"

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

Mandelbrot::Mandelbrot(uint w, uint h, float dt, const DomaineMath& domaineMath) :
	Animable_I<uchar4>(w, h, "Mandelbrot", domaineMath),
	variateurAnimation(Interval<uint>(20, 250), 1)
    {
    this->t = 0;
    //this->parallelPatern = ParallelPatern::OMP_MIXTE;
    this->parallelPatern = ParallelPatern::OMP_FORAUTO;

    cout << "\n[Mandelbrot] : OMP : nbThread = " << this->nbThread << endl;
    }

Mandelbrot::~Mandelbrot()
    {}

void Mandelbrot::animationStep()
    {
    this->t = variateurAnimation.varierAndGet();
    }

void Mandelbrot::processEntrelacementOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    MandelMath mandelMath(t);

    #pragma omp parallel
	{
	    const int TID = OmpTools::getTid();
	    const int NB_THREADS = OmpTools::getNbThread();

	    int s = TID;
	    int i = 0, j = 0;
	    while(s < w*h) {
		IndiceTools::toIJ(s, w, &i, &j);
		workPixel(&ptrTabPixels[s], i, j, domaineMath, &mandelMath);
		s += NB_THREADS;
	    }
	}
    }

void Mandelbrot::processForAutoOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
	MandelMath mandelMath(t);

#pragma omp parallel for
	for(int i = 0; i < h; i++) {
	    for(int j = 0; j < w; j++) {
		int s = IndiceTools::toS(w, i, j);
		workPixel(&ptrTabPixels[s], i, j, domaineMath, &mandelMath);
	    }
	}
    }
/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

void Mandelbrot::workPixel(uchar4* ptrColorIJ, int i, int j, const DomaineMath& domaineMath, MandelMath* mandelMath)
    {
	double x;
	double y;
	domaineMath.toXY(i, j, &x, &y);
	mandelMath->colorXY(ptrColorIJ, x, y);
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

