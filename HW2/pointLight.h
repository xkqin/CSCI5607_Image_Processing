#ifndef POINTLIGHT_INCLUDED
#define POINTLIGHT_INCLUDED

#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include "pixel.h"
#include "point.h"
#include <set>



class pointLight {

	Point ori; //the position  of point light source
	float lr, lg, lb;

public:
	pointLight(float pr_, float pg_, float pb_, float orix_, float oriy_, float oriz_)
	{
		ori.x = orix_; ori.y = oriy_; ori.z = oriz_;
		lr = pr_; lg = pg_; lb = pb_;
	}
	pointLight() {}



	Point getori() {
		return ori;
	}

	float getLr() {
		return lr;
	}

	float getLg() {
		return lg;
	}
	float getLb() {
		return lb;
	}

};

#endif
