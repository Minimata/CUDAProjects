#pragma once

#include "Animable_I_GPU.h"
#include "Provider_I_GPU.h"

using namespace gpu;
/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class FractalProvider: public Provider_I<uchar4>
    {
    public:
	virtual ~FractalProvider(){}

	Animable_I<uchar4>* createAnimable();
	Image_I* createImageGL();
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
