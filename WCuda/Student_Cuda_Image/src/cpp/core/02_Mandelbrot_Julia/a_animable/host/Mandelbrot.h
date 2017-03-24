#pragma once

#include "Fractal.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class Mandelbrot : public Fractal
    {
    public:
	Mandelbrot(const Grid& grid, uint w, uint h, float dt, const DomaineMath& domaineMath);
	virtual ~Mandelbrot();

	virtual void process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath);
    };



/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
