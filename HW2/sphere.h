#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED

#include <stdio.h>
#include "point.h"
#include "pixel.h"
#include "pointLight.h"
#include "directionalLight.h"

#include <list>

class sphere {

	float x, y, z;
	float radius;
	Point center;

	float ar, ag, ab;
	float dr, dg, db;
	float sr, sg, sb;

	float tr, tg, tb; 
	float ior;

public:
	sphere(){}

	sphere(float x_, float y_, float z_, float r) :x(x_), y(y_), z(z_), radius(r) { center = Point(x_, y_, z_); }


	bool CheckIntersection(Point origin, Vector rayD);

	float calculate_t(Point origin, Vector rayD);

	Pixel calculateAmbLightCol(float amb_r, float amb_g, float amb_b);

	Pixel calculatePhong(std::list<pointLight> plList, std::list<directionalLight> dlList,Point intersecP, Vector raydir, float pn);

	Pixel calcuteColor(std::list<pointLight> plList, std::list<directionalLight> dlList, float amb_r, float amb_g, float amb_b , Point intersecP, Vector rayD,float pn);

	void setAmbientColor(float ar_, float ag_, float ab_);

	void setDiffuseColor(float dr_, float dg_, float db_);

	void setSpecularColor(float sr_, float sg_, float sb_);

	void setTransmissiveColor(float tr_, float tg_, float tb_);

	void setIndexOfRefraction(float ior_);

	Pixel getAmbientColor();

	Pixel getDiffuseColor();

	Pixel getSpecularColor();

	Pixel getTransmissiveColor();

	float getIndexOfRefraction();

	float getX() {
		return x;
	}

	float getY() {
		return y;
	}

	float getZ() {
		return z;
	}

	float getR() {
		return radius;
	}

};


#endif
