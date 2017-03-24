#pragma once

#include <math.h>
#include "MathTools.h"

#include "ColorTools_CPU.h"
using namespace cpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class RipplingMath
    {
	/*--------------------------------------*\
	|*		Constructeur		*|
	 \*-------------------------------------*/

    public:

	RipplingMath(uint w, float stretchX = 1, float stretchY = 1, float stretch = 1./10., float speed = 1./7.)
	    {
	    this->dim2 = w / 2;
	    this->stretchX = stretchX;
	    this->stretchY = stretchY;
	    this->stretch = stretch;
	    this->speed = speed;
	    }

	// constructeur copie: pas besoin car pas attribut ptr

	virtual ~RipplingMath(void)
	    {
	    // rien
	    }

	/*--------------------------------------*\
	|*		Methode			*|
	 \*-------------------------------------*/

    public:

	void colorIJ(uchar4* ptrColorIJ, int i, int j, float t)
	    {
	    uchar levelGris = f(j, i, t);

	    ptrColorIJ->x = levelGris;
	    ptrColorIJ->y = levelGris;
	    ptrColorIJ->z = levelGris;

	    ptrColorIJ->w = 255; //opaque
	    }

    private:

	uchar f(int i, int j, float t)
	    {
	    float result = dij(i, j, t);
	    return 128 + 127*(cos(result*this->stretch - t*this->speed) / (result*this->stretch + 1));
	    }

	float dij(int i, int j, float t)
	    {
	    float fi = i - this->dim2;
	    float fj = j - this->dim2;
	    return sqrt(this->stretchY*fi*fi + this->stretchX*fj*fj);
	    }

	/*--------------------------------------*\
	|*		Attribut			*|
	 \*-------------------------------------*/

    private:

	// Tools
	float dim2;
	float stretchX;
	float stretchY;
	float stretch;
	float speed;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
