#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"

#include "FractalMath.h"
#include "MandelbrotMath.h"
#include "JuliaMath.h"

#include "IndiceTools_GPU.h"
#include "DomaineMath_GPU.h"
using namespace gpu;



__global__ void mandelbrot(uchar4* ptrDevPixels, uint w, uint h, uint t, DomaineMath domaineMath);
__global__ void julia(uchar4* ptrDevPixels, uint w, uint h, uint t, DomaineMath domaineMath);
__device__ void process(uchar4* ptrDevPixels, uint w, uint h, DomaineMath domaineMath, FractalMath* fractalMath);
__device__ void workPixel(uchar4* ptrColorIJ, int i, int j, DomaineMath domaineMath, FractalMath* fractalMath);



__global__ void mandelbrot(uchar4* ptrDevPixels, uint w, uint h, uint t, DomaineMath domaineMath) {
    FractalMath* fractal = new MandelbrotMath(t);
    process(ptrDevPixels, w, h, domaineMath, fractal);
    delete fractal;
}

__global__ void julia(uchar4* ptrDevPixels, uint w, uint h, uint t, DomaineMath domaineMath) {
    FractalMath* fractal = new JuliaMath(t);
    process(ptrDevPixels, w, h, domaineMath, fractal);
    delete fractal;
}

__device__ void process(uchar4* ptrDevPixels, uint w, uint h, DomaineMath domaineMath, FractalMath* fractalMath)
    {
    const int TID = Indice2D::tid();
    const int NB_THREAD = Indice2D::nbThread();

    int s = TID;
    int i = 0, j = 0;
    while(s < w*h)
	{
	IndiceTools::toIJ(s, w, &i, &j);
	workPixel(&ptrDevPixels[s], i, j, domaineMath, fractalMath);
	s += NB_THREAD;
	}
    }

__device__ void workPixel(uchar4* ptrColorIJ, int i, int j, DomaineMath domaineMath, FractalMath* fractalMath)
    {
	double x;
	double y;
	domaineMath.toXY(i, j, &x, &y);
	fractalMath->colorXY(ptrColorIJ, x, y);
    }
