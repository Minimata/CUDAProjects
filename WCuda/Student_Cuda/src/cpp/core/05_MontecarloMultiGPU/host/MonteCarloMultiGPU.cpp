#include "MonteCarloMultiGPU.h"
#include <cmath>
#include <iostream>
#include "Device.h"

using std::cout;
using std::endl;

#define PI 3.14159265358979323846264338327950288419716939937510

MonteCarloMultiGPU::MonteCarloMultiGPU(const Grid& grid, elem nbSamples, float targetHeight, float tolerance)
    {
    this->grid = grid;
    this->nbSamples = nbSamples;
    this->targetHeight = targetHeight;
    this->tolerance = tolerance;
    this->pi = 0;
    }

MonteCarloMultiGPU::~MonteCarloMultiGPU()
    {

    }


float MonteCarloMultiGPU::run()
    {
    int r = 0;
    int nbDevices = Device::getDeviceCount();
    elem nbSamplesPerGPU = nbSamples / nbDevices;
#pragma omp parallel for reduction(+ : r)
    for(int id = 0; id < nbDevices; id++)
	{
	Device::setDevice(id);
	MonteCarlo m(grid, nbSamplesPerGPU, targetHeight, tolerance);
	m.run();
	r = m.getNbSuccessSamples();
	}

    r /= nbDevices;
    pi =  4.0 * r * targetHeight / (nbSamplesPerGPU);
    return pi;
    }

bool MonteCarloMultiGPU::check()
    {
    return std::fabs(pi - PI) < tolerance;
    }

void MonteCarloMultiGPU::display()
    {
    cout << "///////////////////////" << endl;
    cout << "/TP montecarloMultiGPU/" << endl;
    cout << "///////////////////////" << endl;

    cout << "Exact Pi : \t" << PI << endl;
    cout << "Estimation : \t" << this->pi << endl;

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }


