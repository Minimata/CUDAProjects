#pragma once

#include "Animable_I_CPU.h"
#include "Variateur_CPU.h"
#include "MandelMath.h"
using namespace cpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class Mandelbrot: public Animable_I<uchar4>
    {
    public:
	Mandelbrot(uint w, uint h, float dt, const DomaineMath& domaineMath);
	virtual ~Mandelbrot();

	virtual void processEntrelacementOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath);
	virtual void processForAutoOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath);
	virtual void animationStep();

    private:
	void workPixel(uchar4* ptrColorIJ, int i, int j, const DomaineMath& domaineMath, MandelMath* mandelMath);

	Variateur<uint> variateurAnimation;
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
