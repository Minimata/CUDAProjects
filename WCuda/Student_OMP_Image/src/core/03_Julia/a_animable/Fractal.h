#pragma once

#include "Animable_I_CPU.h"
#include "Variateur_CPU.h"
#include "FractalMath.h"
using namespace cpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class Fractal: public Animable_I<uchar4>
    {
    public:
	Fractal(uint w, uint h, float dt, const DomaineMath& domaineMath);
	virtual ~Fractal();

	virtual void processEntrelacementOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath);
	virtual void processForAutoOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath);
	virtual void animationStep();

    private:
	void workPixel(uchar4* ptrColorIJ, int i, int j, const DomaineMath& domaineMath, FractalMath* fractalMath);

	Variateur<uint> variateurAnimation;
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
