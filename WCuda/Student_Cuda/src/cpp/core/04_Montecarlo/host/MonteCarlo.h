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
	MonteCarlo(const Grid& grid, elem nbSamplesPower, float targetHeight = 4.0, float tolerance = 0.001);

	virtual ~MonteCarlo();

    public:

	float run();
	bool check();
	void display();
	int getNbSuccessSamples();

    private:

	dim3 dg;
	dim3 db;

	elem nbSamples;
	elem nbThreads;
	elem nbSamplesPerThread;
	elem nbSuccessSamples;
	float targetHeight;
	float tolerance;

	float pi;
	elem* ptrDevNx;
	curandState* ptrDevCurand;

	size_t sizeSM;

    };
