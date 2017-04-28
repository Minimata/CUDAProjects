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

#include "MonteCarloMultiGPU.h"

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useMonteCarloMultiGPU();

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useMonteCarloMultiGPU()
    {
    elem n = UINT_MAX;  //number of samples, here max(unsigned long long int) / 4
    float targetHeight = 4.0;  //height of target
    float tolerance = 0.01;  //tolerance between calculated pi and it's real value

    // Grid cuda
    int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

    dim3 dg = dim3(16, 1, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    dim3 db = dim3(1024, 1, 1);   	// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    Grid grid(dg, db);

    MonteCarloMultiGPU montecarlo(grid, n, targetHeight, tolerance); // on passse la grille à AddVector pour pouvoir facilement la faire varier de l'extérieur (ici) pour trouver l'optimum
    float pi = montecarlo.run();

    montecarlo.display();

    pi *= 4.f;

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

