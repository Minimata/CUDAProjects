#pragma once

#include "Sphere.h"
class SphereCreator
    {
    public:
	SphereCreator(int nbSpheres, int w, int h, int bord = 200);
	virtual ~SphereCreator(void);

    public:
	Sphere* getTabSphere();
    private:
	void createSphere(void);

    private:
// Inputs
	int nbSpheres;
	int w;
	int h;
	int bord;
// Tools
	Sphere* tabSphere;
    };

