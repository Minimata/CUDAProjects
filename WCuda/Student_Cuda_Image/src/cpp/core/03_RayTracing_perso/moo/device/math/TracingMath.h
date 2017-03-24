#pragma once

#include <math.h>
#include "MathTools.h"

#include "Calibreur_GPU.h"
#include "ColorTools_GPU.h"

#include "ShapeContainer.h"
#include "Shape3D.h"

using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class TracingMath
    {

	/*--------------------------------------*\
	|*		Constructor		*|
	 \*-------------------------------------*/

    public:

	__device__ TracingMath(int w, int h)
	    {
	    this->shapes = ShapeContainer(w, h);
	    }

	__device__
	   virtual ~TracingMath()
	    {
	    // rien
	    }

	/*--------------------------------------*\
	|*		Methodes		*|
	 \*-------------------------------------*/

    public:

	__device__
	void colorIJ(uchar4* ptrColor, int i, int j, float t)
	    {
	    Shape3D** shapes = this->shapes.getShapes();
	    size_t numShapes = this->shapes.getNumShapes();

	    float h = 0, s = 1, v = 0;

	    for(int i = 0; i < numShapes; i++) {
		 Shape3D* current = shapes[i];
		 v = current->getBrightnessAtPoint(i, j, t);
		 h = current->getHue();
	    }
	    ColorTools::HSB_TO_RVB(h, s, v, ptrColor);
	    ptrColor->w = 255;
	    }

    private:

	__device__
	float f(int i, int j, float t)
	    {
	    return 128.0;
	    }

	/*--------------------------------------*\
	|*		Attributs		*|
	 \*-------------------------------------*/

    private:
	ShapeContainer shapes;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
