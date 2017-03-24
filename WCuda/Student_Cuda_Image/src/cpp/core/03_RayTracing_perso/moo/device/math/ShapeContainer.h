#pragma once

#include "Shape3D.h"
#include "SpherePerso.h"


/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class ShapeContainer {
    public:
	__device__
	ShapeContainer() {}

	__device__
	ShapeContainer(int w, int h, int bord = 200, int numShapes = 2) {
	    this->w = w;
	    this->h = h;
	    this->bord = bord;
	    this->numShapes = numShapes;
	    createSphere();
	}
	__device__
	~ShapeContainer() {
	    for(size_t i = 0; i < this->numShapes; i++) {
		delete shapes[i];
	    }
	}
	__device__
	void createSphere()
	    {
	    for (int i = 0; i < this->numShapes; i++)
		{
		float3 centre;
		centre.x = i;
		centre.y = i;
		centre.z = i;
		float rayon = 1;
		float hue01 = 1. / i;
		this->shapes[i] = new SpherePerso(centre, rayon, hue01);
		}
	    }

	__device__
	Shape3D** getShapes()
	    {
	    return this->shapes;
	    }

	__device__
	size_t getNumShapes() { return this->numShapes; }


    private:
	Shape3D* shapes[];
	size_t numShapes;


	int nbSpheres;
	int w;
	int h;
	int bord;
};






/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
