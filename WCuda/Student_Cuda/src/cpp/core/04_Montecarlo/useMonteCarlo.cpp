#include <iostream>
#include "Grid.h"
#include "Device.h"
#include <climits>

using std::cout;
using std::endl;

using elem = unsigned int;


#define PI 3.14159265358979323846264338327950288419716939937510f

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

#include "MonteCarlo.h"

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useMonteCarlo();

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useMonteCarlo()
    {
    elem n = UINT_MAX>>2;  //total number of samples
    float targetHeight = 1.0;  //height of target, keep it positive
    float right = 1.f; //Keep it positive
    float left = 0.f;  //Keep it positive
    float tolerance = 0.01;  //tolerance between calculated integral and its real value

    // Grid cuda
    int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

    dim3 dg = dim3(16, 1, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    dim3 db = dim3(1024, 1, 1);   	// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    Grid grid(dg, db);

    MonteCarlo montecarlo(grid, n, targetHeight, right, left, tolerance, 0);
    float pi = montecarlo.run();

    montecarlo.display();

    pi *= 4;

    cout << endl;
    cout << "Estimation de Pi - valide uniquement si functionID = 0" << endl;
    cout << "Pi = " << PI << endl;
    cout << "Estimation = " << pi << endl;
    cout << endl;

    bool isOk = montecarlo.check(pi / 4.f);

    return isOk;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

