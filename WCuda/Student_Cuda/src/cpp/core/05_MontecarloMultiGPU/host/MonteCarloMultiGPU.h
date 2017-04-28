#pragma once

#include "MonteCarlo.h"

using elem = unsigned int;

class MonteCarloMultiGPU
    {
    public:
	MonteCarloMultiGPU(const Grid& grid, elem nbSamples, float targetHeight = 4.0, float tolerance = 0.001);
	virtual ~MonteCarloMultiGPU();

	float run();
	bool check();
	void display();

    private:
	Grid grid;
	elem nbSamples;
	float targetHeight;
	float tolerance;
	float pi;
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
