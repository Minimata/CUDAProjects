#include <iostream>
#include <chrono>
#include "Device.h"
#include <cmath>
#include "MonteCarlo.h"
typedef std::chrono::high_resolution_clock Clock;

using std::cout;
using std::endl;


extern __global__ void montecarlo(elem* ptrDevNx, elem nbSamples, curandState* ptrDevCurand, float targetHeight, float right, float left, int functionID);
extern __global__ void setup_kernel_rand(curandState* tabDevGenerator, int deviceId);



MonteCarlo::MonteCarlo(const Grid& grid, elem nbSamples, float targetHeight, float right, float left, float tolerance, int functionID)
    {
    // Grid
	{
	this->dg = grid.dg;
	this->db = grid.db;
	}

    this->left = left;
    this->right = right;
    this->functionID = functionID;
    this->nbThreads = grid.threadCounts();  // one dimensionnal block
    this->sizeSM = this->db.x * sizeof(elem);  // size of SM tab, one dimensionnal block
    this->nbSamples = nbSamples;
    this->nbSamplesPerThread = nbSamples / nbThreads;
    this->targetHeight = targetHeight;
    this->tolerance = tolerance;
    this->integral = 0;
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

    auto t1 = Clock::now();

    Device::lastCudaError("curand (before)"); // temp debug
    setup_kernel_rand<<<dg, db>>>(ptrDevCurand, Device::getDeviceId());
    Device::lastCudaError("montecarlo (before)"); // temp debug
    montecarlo<<<dg, db, sizeSM>>>(ptrDevNx, nbSamplesPerThread, ptrDevCurand, targetHeight, right, left, functionID); // assynchrone
    Device::lastCudaError("montecarlo (after)"); // temp debug

    Device::memcpyDToH(&nbSuccessSamples, ptrDevNx, sizeof(elem)); // barriere synchronisation implicite
    //pi prend la valeur de Nx ici

    auto t2 = Clock::now();

    std::cout << "Performance Montecarlo : "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " nanoseconds" << std::endl;

    /*

    cout << "Nx : " << pi << endl;
    cout << "target height : " << this->targetHeight << endl;
    cout << "nb Samples : " << this->nbSamples << endl;
    cout << "samples per thread : " << nbSamplesPerThread << endl;

    */

    integral = nbSuccessSamples;
    integral *= targetHeight / (nbSamplesPerThread * nbThreads);

    return integral;
    }

int MonteCarlo::getNbSuccessSamples()
    {
    std::cout << "Success samples : " << this->nbSuccessSamples << std::endl;
    return this->nbSuccessSamples;
    }

void MonteCarlo::display()
    {
    cout << "///////////////////////" << endl;
    cout << "/////TP montecarlo/////" << endl;
    cout << "///////////////////////" << endl;

    cout << "Estimation intégrale : \t" << this->integral << endl;

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }

bool MonteCarlo::check(float value)
    {
    return std::fabs(this->integral - value) < this->tolerance;
    }
