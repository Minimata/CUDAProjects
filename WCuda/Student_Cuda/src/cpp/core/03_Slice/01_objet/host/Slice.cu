#include <iostream>

#include "Device.h"
#include "Slice.h"
#include <cmath>

using std::cout;
using std::endl;


extern __global__ void slice(float* ptrDevPi, int nbSlice);

#define PI 3.14159265358979323846264338327950288419716939937510


Slice::Slice(const Grid& grid, int nbSlice, float tolerance)
    {
    // Grid
	{
	this->dg = grid.dg;
	this->db = grid.db;
	}

    this->sizeOctet = db.x * db.y * db.z * sizeof(float); // octet
    this->nbSlice = nbSlice;
    this->tolerance = tolerance;
    this->pi = 0;

    // MM
	{

	// MM (malloc Device)
	    {
	    Device::malloc(&ptrDevPi, sizeof(float));
	    Device::memclear(ptrDevPi, sizeof(float));
	    }

	Device::lastCudaError("AddVector MM (end allocation)"); // temp debug, facultatif
	}

    }

Slice::~Slice(void)
    {
    //MM (device free)
	{
	Device::free(ptrDevPi);

	Device::lastCudaError("AddVector MM (end deallocation)"); // temp debug, facultatif
	}
    }


float Slice::run()
    {
    Device::lastCudaError("addVecteur (before)"); // temp debug
    slice<<<dg, db, sizeOctet>>>(ptrDevPi, nbSlice); // assynchrone
    Device::lastCudaError("addVecteur (after)"); // temp debug

    Device::memcpyDToH(&pi, ptrDevPi, sizeof(float)); // barriere synchronisation implicite

    return pi;
    }

void Slice::display()
    {
    cout << "///////////////////////" << endl;
    cout << "/////TP SLICE - PI/////" << endl;
    cout << "///////////////////////" << endl;

    cout << "Exact Pi : \t" << PI << endl;
    cout << "Estimation : \t" << this->pi << endl;

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }

bool Slice::check()
    {
    return std::fabs(this->pi - PI) < this->tolerance;
    }
