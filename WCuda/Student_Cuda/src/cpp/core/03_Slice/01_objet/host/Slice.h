#pragma once

#include "cudaTools.h"
#include "Grid.h"



class Slice
    {

    public:

	Slice(const Grid& grid, int nbSlice, float tolerance = 0.001);

	virtual ~Slice();

    public:

	float run();
	bool check();
	void display();

    private:

	dim3 dg;
	dim3 db;

	int nbSlice;
	float tolerance;

	float pi;
	float* ptrDevPi;

	size_t sizeOctet;

    };
