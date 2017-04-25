#include <iostream>

#include "Device.h"
#include <cmath>
#include "MonteCarlo.h"

using std::cout;
using std::endl;


extern __global__ void montecarlo(elem* ptrDevNx, elem nbSamples, curandState* ptrDevCurand, float targetHeight);
extern __global__ void setup_kernel_rand(curandState* tabDevGenerator, int deviceId);

#define PI 3.14159265358979323846264338327950288419716939937510


MonteCarlo::MonteCarlo(const Grid& grid, elem nbSamples, float targetHeight, float tolerance)
    {
    // Grid
	{
	this->dg = grid.dg;
	this->db = grid.db;
	}

    this->nbThreads = grid.threadCounts();  // one dimensionnal block
    this->sizeSM = this->db.x * sizeof(elem);  // size of SM tab, one dimensionnal block
    this->nbSamples = nbSamples;
    this->nbSamplesPerThread = nbSamples / nbThreads;
    this->targetHeight = targetHeight;
    this->tolerance = tolerance;
    this->pi = 0;
    this->nbSuccessSamples = 0;

    size_t sizeCurand = this->nbThreads * sizeof(curandState) ;
    this->ptrDevCurand=NULL;

    // MM
	{

	// MM (malloc Device)
	    {
	    Device::malloc(&ptrDevNx, sizeof(elem));
	    Device::memclear(ptrDevNx, sizeof(elem));

	    Device::malloc(&ptrDevCurand, sizeCurand);
	    Device::memclear(ptrDevCurand, sizeCurand);
	    }

	Device::lastCudaError("MonteCarlo MM (end allocation)"); // temp debug, facultatif
	}

    }

MonteCarlo::~MonteCarlo(void)
    {
    //MM (device free)
	{
	Device::free(ptrDevNx);
	Device::free(ptrDevCurand);

	Device::lastCudaError("MonteCarlo MM (end deallocation)"); // temp debug, facultatif
	}
    }


float MonteCarlo::run()
    {
    Device::lastCudaError("curand (before)"); // temp debug
    setup_kernel_rand<<<dg, db>>>(ptrDevCurand, Device::getDeviceId());
    Device::lastCudaError("montecarlo (before)"); // temp debug
    montecarlo<<<dg, db, sizeSM>>>(ptrDevNx, nbSamplesPerThread, ptrDevCurand, targetHeight); // assynchrone
    Device::lastCudaError("montecarlo (after)"); // temp debug

    Device::memcpyDToH(&nbSuccessSamples, ptrDevNx, sizeof(elem)); // barriere synchronisation implicite
    //pi prend la valeur de Nx ici

    /*

    cout << "Nx : " << pi << endl;
    cout << "target height : " << this->targetHeight << endl;
    cout << "nb Samples : " << this->nbSamples << endl;
    cout << "samples per thread : " << nbSamplesPerThread << endl;

    */

    pi = 4.0 * nbSuccessSamples;
    pi *= targetHeight / (nbSamplesPerThread * nbThreads);

    return pi;
    }

int MonteCarlo::getNbSuccessSamples()
    {
    return this->nbSuccessSamples;
    }

void MonteCarlo::display()
    {
    cout << "///////////////////////" << endl;
    cout << "/////TP montecarlo/////" << endl;
    cout << "///////////////////////" << endl;

    cout << "Exact Pi : \t" << PI << endl;
    cout << "Estimation : \t" << this->pi << endl;

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }

bool MonteCarlo::check()
    {
    return std::fabs(this->pi - PI) < this->tolerance;
    }
