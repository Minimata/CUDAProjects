#pragma once

#include "Variateur_GPU.h"
#include "Animable_I_GPU.h"
#include "cudaTools.h"
#include "MathTools.h"
#include "DomaineMath_GPU.h"

using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class Fractal: public Animable_I<uchar4>
    {
    public:
	Fractal(const Grid& grid, uint w, uint h, float dt, const DomaineMath& domaineMath);
	virtual ~Fractal();

	//virtual void process(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath);
	virtual void animationStep();
	virtual void process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath)=0;

    private:

	Variateur<uint> variateurAnimation;
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
