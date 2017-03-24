#include "FractalProvider.h"

#include "Fractal.h"
#include "Julia.h"
#include "Mandelbrot.h"

#include "MathTools.h"

#include "ImageAnimable_GPU.h"
#include "DomaineMath_GPU.h"
using namespace gpu;
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

    // Grid Cuda
    int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

    dim3 dg(mp, 2, 1);
    dim3 db(coreMP, 2, 1);

    Grid grid(dg, db);  // TODO definissez une grille cuda (dg, db)

    return new Julia(grid, dw, dh, dt, domaineMath);
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

