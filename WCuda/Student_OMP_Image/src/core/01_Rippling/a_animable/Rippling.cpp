#include "RipplingMath.h"
#include "Rippling.h"

#include <iostream>
#include <omp.h>
#include "OmpTools.h"

#include "IndiceTools_CPU.h"
using cpu::IndiceTools;

using std::cout;
using std::endl;

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

Rippling::Rippling(uint w, uint h, float dt) :
	Animable_I<uchar4>(w, h, "Rippling_OMP_rgba_uchar4")
    {
    // Input
    this->dt = dt;  // animation

    // Tools
    this->t = 0;					// protected dans super classe Animable
    this->parallelPatern = ParallelPatern::OMP_MIXTE;   // protected dans super classe Animable

    // OMP
    cout << "\n[Rippling] : OMP : nbThread = " << this->nbThread << endl; // protected dans super classe Animable
    }

Rippling::~Rippling(void)
    {
    // rien
    }

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/**
 * Override
 */
void Rippling::animationStep()
    {
    t += dt;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/**
 * Override (code naturel omp)
 * Image non zoomable : domaineMath pas use ici
 */
void Rippling::processForAutoOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    RipplingMath ripplingMath(w); //comportement par défaut
    //RipplingMath ripplingMath(w, 1, 3, 1./5., 1./5.); //parametrage optionnel de ripplingMath
#pragma omp parallel for
	for(int i = 0; i < h; i++) {
	    for(int j = 0; j < w; j++) {
		int s = IndiceTools::toS(w, i, j);
		ripplingMath.colorIJ(&s[ptrTabPixels], i, j, t); // s[ptrTabPixels] == ptrTabPixels[s] => true
	    }
	}


    }

/**
 * Override (code entrainement cuda)
 * Image non zoomable : domaineMath pas use ici
 */
void Rippling::processEntrelacementOMP(uchar4* ptrTabPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    const int WH = w*h;
    RipplingMath ripplingMath(w); //comportement par défaut
    //RipplingMath ripplingMath(w, 1, 3, 1./5., 1./5.); //parametrage optionnel de ripplingMath

    #pragma omp parallel
	{
	const int TID = OmpTools::getTid();
	const int NB_THREAD = OmpTools::getNbThread();
	int s = TID;
	int i, j = 0;
	while(s < WH)
	    {
	    IndiceTools::toIJ(s, w, &i, &j);
	    ripplingMath.colorIJ(&s[ptrTabPixels], i, j, t);  // s[ptrTabPixels] == ptrTabPixels[s] => true

	    s += NB_THREAD;
	    }
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

