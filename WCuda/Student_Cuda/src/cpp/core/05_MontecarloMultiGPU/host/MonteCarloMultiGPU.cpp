#include "MonteCarloMultiGPU.h"
#include <cmath>
#include <iostream>
#include "Device.h"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

using std::cout;
using std::endl;


MonteCarloMultiGPU::MonteCarloMultiGPU(const Grid& grid, elem nbSamples, float targetHeight, float right, float left, float tolerance, int functionID) :
	MonteCarlo(grid, nbSamples, targetHeight, right, left, tolerance, functionID)
    {
    this->grid = grid;
    }

MonteCarloMultiGPU::~MonteCarloMultiGPU()
    {

    }


float MonteCarloMultiGPU::run()
    {
    elem r = 0;
    int nbDevices = Device::getDeviceCount();
    elem nbSamplesPerGPU = nbSamples / nbDevices;

    auto t1 = Clock::now();

#pragma omp parallel for reduction(+ : r)
    for(int id = 0; id < nbDevices; id++)
	{
	Device::setDevice(id);
	MonteCarlo m(grid, nbSamplesPerGPU, targetHeight, tolerance, functionID);
	m.run();
	r = m.getNbSuccessSamples();
	}


    auto t2 = Clock::now();

    std::cout << "Performance Montecarlo MultiGPU : "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " nanoseconds" << std::endl;

    r /= nbDevices;
    integral =  r * targetHeight / (nbSamplesPerGPU);
    return integral;
    }


void MonteCarloMultiGPU::display()
    {
    cout << "///////////////////////" << endl;
    cout << "/TP montecarloMultiGPU/" << endl;
    cout << "///////////////////////" << endl;

    cout << "Estimation : \t" << this->integral << endl;

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }


