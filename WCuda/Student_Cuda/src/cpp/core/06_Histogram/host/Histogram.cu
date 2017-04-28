#include <iostream>
#include <cmath>

#include "Device.h"

#include "DataCreator.h"
#include "Histogram.h"

using std::cout;
using std::endl;

extern __global__ void myhistogram(int* ptrDevData, int dataMax, int dataSize);

Histogram::Histogram(const Grid& grid, int dataMax)
    {
    // Grid
	{
	this->dg = grid.dg;
	this->db = grid.db;
	}

    //Data
    this->dataMax = dataMax;
    DataCreator dataCreator(dataMax);
    dataSize = dataCreator.getLength();
    data = dataCreator.getTabData();

    //Threads
    this->nbThreads = grid.threadCounts();  // one dimensionnal block

    //GM
    sizeOctet = dataSize * sizeof(int);

    //SM
    this->sizeSM = dataMax * sizeof(int);

    //output
    histogram = nullptr;

    // MM
	{

	// MM (malloc Device)
	    {
	    Device::malloc(&ptrDevData, sizeOctet);
	    Device::memclear(ptrDevData, sizeOctet);
	    Device::memcpyHToD(ptrDevData, data, sizeOctet);
	    }

	Device::lastCudaError("Histogram (end allocation)"); // temp debug, facultatif
	}

    }

Histogram::~Histogram(void)
    {
    //MM (device free)
	{
	Device::free(ptrDevData);

	Device::lastCudaError("Histogram MM (end deallocation)"); // temp debug, facultatif
	}
    }

void Histogram::run()
    {
    Device::lastCudaError("Histogram (before)"); // temp debug
    myhistogram<<<dg, db, sizeSM>>>(ptrDevData, dataMax, dataSize); // assynchrone
    Device::lastCudaError("Histogram (after)"); // temp debug

    int* result = new int[dataMax]();

    Device::memcpyDToH(result, ptrDevData, dataMax * sizeof(int)); // barriere synchronisation implicite
    Device::memclear(ptrDevData, sizeOctet); // On supprime les valeurs laissées en trop. Pas nécessaire mais gêne pas de faire le ménage.

    histogram = result;
    }

void Histogram::display()
    {
    cout << "///////////////////////" << endl;
    cout << "//////TP histogram/////" << endl;
    cout << "///////////////////////" << endl;

    cout << "The Histogram : " << endl;
    for(int i = 0; i < dataMax; i++)
	{
	cout << "frequency of " << i << " : " << histogram[i] << endl;
	}

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }

bool Histogram::check()
    {
    bool isOk = true;
    int val = 0;
    for (int i = 0; i < dataMax; i++)
	{
	isOk &= (i + 1 == histogram[i]);
	val += histogram[i];
	}

    isOk &= (val == dataSize);
    return isOk;
    }
