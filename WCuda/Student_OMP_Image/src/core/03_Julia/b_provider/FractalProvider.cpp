#include "FractalProvider.h"

#include "Fractal.h"

#include "MathTools.h"

#include "ImageAnimable_CPU.h"
#include "DomaineMath_CPU.h"
using namespace cpu;
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

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

Animable_I<uchar4>* FractalProvider::createAnimable()
    {
    DomaineMath domaineMath = DomaineMath(-2, -2, 2, 2);

    float dt = 1;

    int dw = 16*60;
    int dh = 16*60;

    return new Fractal(dw, dh, dt, domaineMath);
    }

Image_I* FractalProvider::createImageGL()
    {
    ColorRGB_01 colorText(0, 0, 0);
    return new ImageAnimable_RGBA_uchar4(createAnimable(), colorText);
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

