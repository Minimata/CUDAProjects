#pragma once

#include "Fractal.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class Julia : public Fractal
    {
    public:
	Julia(const Grid& grid, uint w, uint h, float dt, const DomaineMath& domaineMath);
	virtual ~Julia();

	virtual void process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath);
    };



/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
