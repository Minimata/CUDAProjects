#pragma once

#include "cudaTools.h"
#include "Grid.h"
#include <curand_kernel.h>

using elem = unsigned int;

class MonteCarlo
    {

    public:
	MonteCarlo();
	MonteCarlo(const MonteCarlo&);
	MonteCarlo(const Grid& grid, elem nbSamples, float targetHeight = 4.0, float right = 1.f, float left = 0.f, float tolerance = 0.001, int functionID = 0);

	virtual ~MonteCarlo();

    public:

	virtual float run();
	bool check(float value);
	virtual void display();
	int getNbSuccessSamples();

    protected:

	Grid grid;
	dim3 dg;
	dim3 db;

	elem nbSamples;
	elem nbThreads;
	elem nbSamplesPerThread;
	elem nbSuccessSamples;
	float targetHeight;
	float left, right;
	int functionID;
	float tolerance;

	float integral;
	elem* ptrDevNx;
	curandState* ptrDevCurand;

	size_t sizeSM;

    };
