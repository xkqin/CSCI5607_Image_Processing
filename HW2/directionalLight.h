#ifndef DIRECTINOALLIGHT_INCLUDED
#define DIRECTINOALLIGHT_INCLUDED

#include <stdio.h>
#include "vector.h"
#include "point.h"



class directionalLight {

	float x, y, z;
	float dlr, dlg, dlb;

public:
	directionalLight(float dlrr, float dlgg, float dlbb, float xx, float yy, float zz) :dlr(dlrr), dlg(dlgg), dlb(dlbb) { x = xx; y = yy; z = zz; }
	directionalLight() {}

	Vector getV() {
		return Vector(x, y, z);
	}

	float getLr() {
		return dlr;
	}

	float getLg() {
		return dlg;
	}

	float getLb() {
		return dlb;
	}
};
#endif
