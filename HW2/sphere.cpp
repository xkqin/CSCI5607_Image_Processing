#include "sphere.h"
#include <math.h>



bool sphere::CheckIntersection(Point ori, Vector rayD)
{
	Vector ray = normalize(rayD);
	float b = ray*(ori - center)*2;
	float c = (ori - center)*(ori - center) - radius*radius;
	if (b*b - 4 *c >= 0)
		return true;
	else
		return false;
}


float sphere::calculate_t(Point ori, Vector rayD)
{


	Vector ray = normalize(rayD);
    float b = ray*(ori-center)*2;
    float c = (ori-center)*(ori-center)-radius*radius;
    float t1 = (-b+sqrt(b*b-4*c))/2;
    float t2 = (-b-sqrt(b*b-4*c))/2;
	float t;

	if (t1 < t2)
		t = t1;
	else
		t = t2;

	return t;

}


Pixel sphere::calcuteColor(std::list<pointLight> plList, std::list<directionalLight> dlList, float amb_r, float amb_g, float amb_b, Point P, Vector rayD,float pn) {
	Pixel p = calculateAmbLightCol(amb_r, amb_g, amb_b)+calculatePhong(plList,dlList, P, rayD, pn);
	return p;

}



Pixel sphere::calculateAmbLightCol(float amb_r, float amb_g, float amb_b) {
	if(amb_r > 1)
	{
		amb_r = 1;
	}
	if(amb_g > 1)
	{
		amb_g = 1;
	}
	if(amb_b > 1)
	{
		amb_b = 1;
	}
	return Pixel(255 * amb_r * ar, 255 * amb_g * ag, 255 * amb_b * ab);
}

Pixel sphere::calculatePhong(std::list<pointLight> plList, std::list<directionalLight> dlList,Point P, Vector rayD, float pn)
{
	float r = 0, g = 0, b = 0;

	for (std::list<pointLight>::iterator next = plList.begin(); next != plList.end(); next++)
	{
		pointLight plight = *next;
		Vector v = P - plight.getori();
		v = Vector(0, 0, 0) - v;
		v = normalize(v);
    Vector h = ( rayD - v);
    h =normalize(h);
		Vector un = (P-center);
    un = normalize(un);
		Vector re = v - 2 *(v*un) * un;
		re = normalize(re);
		float angle = h*re;
		float distance = (P - plight.getori())*(P - plight.getori());
		float a = 0;

		if (angle > a) {
			a = angle;
		}

		r += sr*(plight.getLr() / distance)*pow(a, pn);
		g += sg*(plight.getLg() / distance)*pow(a, pn);
		b += sb*(plight.getLb() / distance)*pow(a, pn);


		float b = 0;
		float angle2 = v*un;

		if (angle2 > b) {
			b = angle2;
		}

		r += dr*(plight.getLr() / distance)*b;
		g += dg*(plight.getLg() / distance)*b;
		b += db*(plight.getLb() / distance)*b;

	}

	for (std::list<directionalLight>::iterator next = dlList.begin(); next != dlList.end(); next++)
	{
		directionalLight dlight = *next;
		Vector v = dlight.getV();
		v = Vector(0, 0, 0) - v;
		Vector h = (rayD - v);
    h =normalize(h);
    Vector un = (P-center);
    un = normalize(un);

		Vector re = v - 2 * (v*un) * un;
		re = normalize(re);
		float angle = h*re;
		float a = 0;

		if (angle > a) {
			a = angle;
		}
		r += sr*dlight.getLr()*pow(a, pn);
		g += sg*dlight.getLg()*pow(a, pn);
		b += sb*dlight.getLb()*pow(a, pn);

		float angle2 = v*un;
		float b = 0;

		if (angle2 > b) {
			b = angle2;
		}

		r += dr*dlight.getLr()*b;
		g += dg*dlight.getLg()*b;
		b += db*dlight.getLb()*b;

	}
	if(r > 1)
	{
		r = 1;
	}
	if(g > 1)
	{
		g = 1;
	}
	if(b > 1)
	{
		b = 1;
	}

	return Pixel(r * 255, g * 255, b * 255);
}



void sphere::setAmbientColor(float ar_, float ag_, float ab_)
{
	ar = ar_;
	ag = ag_;
	ab = ab_;
}

void sphere::setDiffuseColor(float dr_, float dg_, float db_)
{
	dr = dr_;
	dg = dg_;
	db = db_;
}

void sphere::setSpecularColor(float sr_, float sg_, float sb_)
{
	sr = sr_;
	sg = sg_;
	sb = sb_;
}

void sphere::setTransmissiveColor(float tr_, float tg_, float tb_)
{
	tr = tr_;
	tg = tg_;
	tb = tb_;
}

void sphere::setIndexOfRefraction(float ior_)
{
	ior = ior_;
}

Pixel sphere::getAmbientColor()
{
	return Pixel(ar, ag, ab);
}

Pixel sphere::getDiffuseColor()
{
	return Pixel(dr, dg, db);
}

Pixel sphere::getSpecularColor()
{
	return Pixel(sr, sg, sb);
}

Pixel sphere::getTransmissiveColor()
{
	return Pixel(tr, tg, tb);
}

float sphere::getIndexOfRefraction()
{
	return ior;
}
