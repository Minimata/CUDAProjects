#include "HistogramCustom.h"

#include <iostream>

#include "Device.h"

#include "DataCreator.h"

using std::cout;
using std::endl;

extern __global__ void histogramCustom(int* ptrDevData, int dataMax, int dataSize, int bufferSizeSM);

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
    bufferSizeSM = 128 / sizeof(int);

    //Output
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

HistogramCustom::~HistogramCustom(void)
    {
    //MM (device free)
	{
	Device::free(ptrDevData);

	Device::lastCudaError("Histogram MM (end deallocation)"); // temp debug, facultatif
	}
    }

void HistogramCustom::run()
    {
    Device::lastCudaError("Histogram (before)"); // temp debug
    histogramCustom<<<dg, db, sizeSM + bufferSizeSM>>>(ptrDevData, dataMax, dataSize, bufferSizeSM); // assynchrone
    Device::lastCudaError("Histogram (after)"); // temp debug

    int* result = new int[dataMax]();

    Device::memcpyDToH(result, ptrDevData, dataMax*sizeof(int)); // barriere synchronisation implicite
    Device::memclear(ptrDevData, sizeOctet); // On supprime les valeurs laissées en trop

    histogram = result;
    }

void HistogramCustom::display()
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
