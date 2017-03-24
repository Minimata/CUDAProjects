/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/


#include <iostream>
#include <assert.h>

#include "Device.h"

#include "Fractal.h"

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


Fractal::Fractal(const Grid& grid, uint w, uint h, float dt, const DomaineMath& domaineMath) :
	Animable_I<uchar4>(grid, w, h, "Fractal_CUDA", domaineMath),
	variateurAnimation(Interval<uint>(5, 250), dt)
    {
    this->t = 0;
    }

Fractal::~Fractal()
    {}

void Fractal::animationStep()
    {
    this->t = variateurAnimation.varierAndGet();
    }


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

