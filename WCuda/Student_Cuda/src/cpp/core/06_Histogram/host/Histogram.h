#pragma once

#include "cudaTools.h"
#include "Grid.h"

class Histogram
    {

    public:
	Histogram(const Grid& grid, int dataMax = 255);

	virtual ~Histogram();

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
	int* ptrDevData;
	size_t sizeOctet;

	//SM
	size_t sizeSM;
    };
