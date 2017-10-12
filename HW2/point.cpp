

#include <stdio.h>
#include "point.h"


Point operator+ (const Point& p, const Point& q)
{
    Point result = Point(p.x+q.x,p.y+q.y,p.z+q.z);
    return result;
}

Vector operator- (const Point& p, const Point& q)
{
    Vector result = Vector(p.x-q.x,p.y-q.y,p.z-q.z);
    return result;
}

Point operator* (const Point& p, float a)
{
    return Point(p.x*a,p.y*a,p.z*a);
}

Point operator+ (const Point& p, const Vector& q)
{
    return Point(p.x+q.x,p.y+q.y,p.z+q.z);
}

Point operator- (const Point& p, const Vector& q)
{
    return Point(p.x-q.x,p.y-q.y,p.z-q.z);
}

float operator* (const Vector& p, const Point& q)
{
    return p.x*q.x+p.y*q.y+p.z*q.z;
}
