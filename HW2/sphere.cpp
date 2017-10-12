#include "sphere.h"
#include <math.h>



bool sphere::CheckIntersection(Point origin, Vector rayD)
{
	Vector ray = normalize(rayD);
	float b = ray*(origin - center)*2;
	float c = (origin - center)*(origin - center) - radius*radius;
	if (b*b - 4 *c >= 0)
		return true;
	else
		return false;
}


float sphere::calculate_t(Point origin, Vector rayD)
{


	Vector ray = normalize(rayD);
    float b = ray*(origin-center)*2;
    float a = 1;
    float c = (origin-center)*(origin-center)-radius*radius;

    float t1 = (-b+sqrt(b*b-4*a*c))/(2*a);
    float t2 = (-b-sqrt(b*b-4*a*c))/(2*a);
	float t;

	if (t1 < t2)
		t = t1;
	else
		t = t2;

	return t;

}


Pixel sphere::calcuteColor(std::list<pointLight> pointLightList, std::list<directionalLight> dlList, float amb_r, float amb_g, float amb_b, Point P, Vector rayD,float pn) {
	Pixel p = calculateAmbLightCol(amb_r, amb_g, amb_b)+calculatePhong(pointLightList,dlList, P, rayD, pn);
	return p;

}



Pixel sphere::calculateAmbLightCol(float amb_r, float amb_g, float amb_b) {
	return Pixel(255 * amb_r * ar, 255 * amb_g * ag, 255 * amb_b * ab);
}

Pixel sphere::calculatePhong(std::list<pointLight> pointLightList, std::list<directionalLight> dlList,Point P, Vector rayD, float pn)
{
	float r = 0, g = 0, b = 0;

	for (std::list<pointLight>::iterator next = pointLightList.begin(); next != pointLightList.end(); next++)
	{
		pointLight plight = *next;
		Vector v = P - plight.getori();
		v = Vector(0, 0, 0) - v;
		v = normalize(v);
    Vector h = (v - rayD);
    h =normalize(h);
		Vector unit_normal = (P-center);
    unit_normal = normalize(unit_normal);
		float angle = h*unit_normal;
		float distance = (P - plight.getori())*(P - plight.getori());
		float factor1 = 0;

		if (angle > factor1) {
			factor1 = angle;
		}

		r += sr*(plight.getLr() / distance)*pow(factor1, pn);
		g += sg*(plight.getLg() / distance)*pow(factor1, pn);
		b += sb*(plight.getLb() / distance)*pow(factor1, pn);


		float factor2 = 0;
		float cos2 = v*unit_normal;

		if (cos2 > factor2) {
			factor2 = cos2;
		}

		r += dr*(plight.getLr() / distance)*factor2;
		g += dg*(plight.getLg() / distance)*factor2;
		b += db*(plight.getLb() / distance)*factor2;


	}

	for (std::list<directionalLight>::iterator next = dlList.begin(); next != dlList.end(); next++)
	{
		directionalLight dlight = *next;
		Vector v = dlight.getV();
		v = Vector(0, 0, 0) - v;
		Vector h = (v - rayD);
    h =normalize(h);
    Vector unit_normal = (P-center);
    unit_normal = normalize(unit_normal);

		float angle = h*unit_normal;
		float factor1 = 0;

		if (angle > factor1) {
			factor1 = angle;
		}
		r += sr*dlight.getLr()*pow(factor1, pn);
		g += sg*dlight.getLg()*pow(factor1, pn);
		b += sb*dlight.getLb()*pow(factor1, pn);

		float cos2 = v*unit_normal;
		float factor2 = 0;

		if (cos2 > factor2) {
			factor2 = cos2;
		}

		r += dr*dlight.getLr()*factor2;
		g += dg*dlight.getLg()*factor2;
		b += db*dlight.getLb()*factor2;

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
