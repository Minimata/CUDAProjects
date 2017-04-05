#pragma once

#include "cudaTools.h"
#include "Grid.h"
#include <curand_kernel.h>


class MonteCarlo
    {

    public:

	MonteCarlo(const Grid& grid, int nbSamplesPower, float targetHeight = 4.0, float tolerance = 0.001);

	virtual ~MonteCarlo();

    public:

	float run();
	bool check();
	void display();

    private:

	dim3 dg;
	dim3 db;

	int nbSamples;
	int nbThreads;
	float targetHeight;
	float tolerance;

	float pi;
	float* ptrDevNx;
	curandState* ptrDevCurand;

	size_t sizeSM;

    };
