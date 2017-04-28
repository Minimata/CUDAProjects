#include <iostream>
#include "Grid.h"
#include "Device.h"
#include <climits>

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

#include "Histogram.h"

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useHistogram();

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useHistogram()
    {
    // Grid cuda
    int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

    dim3 dg = dim3(16, 1, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    dim3 db = dim3(256, 1, 1);   	// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    Grid grid(dg, db);

    Histogram histogram(grid);
    histogram.run();
    histogram.display();

    bool isOk = histogram.check();

    return isOk;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

