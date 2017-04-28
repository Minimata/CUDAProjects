#include <iostream>
#include <stdlib.h>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;


using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

extern bool useHello(void);
extern bool useAddVecteur(void);
extern bool useSlice();
extern bool useMonteCarlo();
extern bool useMonteCarloMultiGPU();
extern bool useHistogram();
extern bool useHistogramCustom();

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

int mainCore();

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/



/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

int mainCore()
    {
    bool isOk = true;


    auto t1 = Clock::now();

    //isOk &= useHello();
    //isOk &= useAddVecteur();
    //isOk &= useSlice();
    //isOk &= useMonteCarlo();
    //isOk &= useMonteCarloMultiGPU();
    isOk &= useHistogram();
    //isOk &= useHistogramCustom();


    auto t2 = Clock::now();

    cout << "\nisOK = " << isOk << endl;
    std::cout << "Performance : "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " nanoseconds" << std::endl;

    cout << "\nEnd : mainCore" << endl;

    return isOk ? EXIT_SUCCESS : EXIT_FAILURE;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/



/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

