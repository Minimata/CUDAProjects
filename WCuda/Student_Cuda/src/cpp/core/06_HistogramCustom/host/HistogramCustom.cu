#include "HistogramCustom.h"

#include <iostream>
#include <cmath>

#include "Device.h"

#include "DataCreator.h"

using std::cout;
using std::endl;

extern __global__ void histogramCustom(int** ptrDevData, int dataMax, int dataSizeSM);

HistogramCustom::HistogramCustom(const Grid& grid, int dataMax)
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

    //Output

    // MM
	{

	// MM (malloc Device)
	    {
	    Device::malloc(&ptrDevData, sizeOctet);
	    Device::memclear(ptrDevData, sizeOctet);
	    //Device::memcpyHToD(ptrDevData, data, sizeOctet);
	    }

	Device::lastCudaError("Histogram (end allocation)"); // temp debug, facultatif
	}

    }

HistogramCustom::~HistogramCustom(void)
    {
    //MM (device free)
	{
	//Device::free (ptrDevNx);

	Device::lastCudaError("Histogram MM (end deallocation)"); // temp debug, facultatif
	}
    }

void HistogramCustom::run()
    {
    Device::lastCudaError("Histogram (before)"); // temp debug
    histogramCustom<<<dg, db, sizeSM + dataSizeSM>>>(ptrDevData, dataMax, dataSizeSM); // assynchrone
    Device::lastCudaError("Histogram (after)"); // temp debug

    Device::memcpyDToH(&histogram, ptrDevData, dataMax*sizeof(int)); // barriere synchronisation implicite
    Device::memclear(ptrDevData, sizeOctet); // On supprime les valeurs laissées en trop
    }

void HistogramCustom::display()
    {
    cout << "///////////////////////" << endl;
    cout << "//////TP histogram/////" << endl;
    cout << "///////////////////////" << endl;

    cout << "///////////////////////" << endl;
    cout << "//////////END//////////" << endl;
    cout << "///////////////////////" << endl;
    }

bool HistogramCustom::check()
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
