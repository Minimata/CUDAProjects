#pragma once


/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/


class Shape3D
    {
    public:
	__device__
	Shape3D(float hue){
	this->hue = hue;
	}
	__device__
	virtual ~Shape3D(){}
	__device__
	float getHue(){
	    return this->hue;
	}
	__device__
	virtual float getBrightnessAtPoint(int x, int y, float t)=0;

    protected:
	float hue;
    };
/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
