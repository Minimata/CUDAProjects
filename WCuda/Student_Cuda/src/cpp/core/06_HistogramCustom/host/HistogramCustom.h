#pragma once

#include "cudaTools.h"
#include "Grid.h"

class HistogramCustom
    {

    public:
	HistogramCustom(const Grid& grid, int dataMax = 255);

	virtual ~HistogramCustom();

    public:

	void run();
	bool check();
	void display();

    private:
	//Grid
	dim3 dg;
	dim3 db;

	//Data
	int dataMax;
	int dataSize;
	int* data;

	//Output
	int* histogram;

	//Threads
	int nbThreads;

	//GM
	int** ptrDevData;
	int sizeOctet;

	//SM
	size_t sizeSM;
	int dataSizeSM;

    };
