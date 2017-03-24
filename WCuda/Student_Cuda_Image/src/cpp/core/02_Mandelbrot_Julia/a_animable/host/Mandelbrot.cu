#include "Mandelbrot.h"
#include "Device.h"
/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

extern __global__ void mandelbrot(uchar4* ptrDevPixels, uint w, uint h, uint t, DomaineMath domaineMath);

Mandelbrot::Mandelbrot(const Grid& grid, uint w, uint h, float dt, const DomaineMath& domaineMath) :
	Fractal(grid, w, h, dt, domaineMath)
    {

    }

Mandelbrot::~Mandelbrot() {}

void Mandelbrot::process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath) {

    mandelbrot<<<dg, db>>>(ptrDevPixels, w, h, this->t, domaineMath);
}

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

