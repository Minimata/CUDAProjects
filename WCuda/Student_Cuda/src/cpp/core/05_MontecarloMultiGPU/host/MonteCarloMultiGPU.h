#pragma once

#include "MonteCarlo.h"
#include "Grid.h"

using elem = unsigned int;

class MonteCarloMultiGPU : public MonteCarlo
    {
    public:
	MonteCarloMultiGPU(const Grid& grid, elem nbSamples, float targetHeight = 4.0, float right = 1.f, float left = 0.f, float tolerance = 0.001, int functionID = 0);
	virtual ~MonteCarloMultiGPU();

	virtual float run();
	virtual void display();

    private:
	Grid grid;
    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
