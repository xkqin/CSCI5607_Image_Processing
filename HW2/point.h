

#ifndef POINT_INCLUDED
#define POINT_INCLUDED

#include "vector.h"

struct Point{
    float x,y,z;

    Point (float xx=0, float yy=0, float zz=0) : x(xx), y(yy), z(zz){}

    void set(float xx, float yy, float zz){x = xx; y = yy; z = zz;}


};

Point operator+ (const Point& p, const Point& q);

Vector operator- (const Point& p, const Point& q);

Point operator* (const Point& p, float a);

Point operator+ (const Point& p, const Vector& q);

Point operator- (const Point& p, const Vector& q);

float operator* (const Vector& p, const Point& q);

#endif 
