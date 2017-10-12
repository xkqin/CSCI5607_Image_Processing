

#ifndef vector_INCLUDED
#define vector_INCLUDED

#include <stdio.h>

struct Vector{

    float x,y,z;

    Vector (float xx=0, float yy=0, float zz=0) : x(xx), y(yy), z(zz){}



};

Vector operator+ (const Vector& p, const Vector& q);
Vector operator- (const Vector& p, const Vector& q);
Vector operator* (const Vector& p, float a);
Vector crossProduct(Vector & u, Vector & v);
Vector divided(const Vector&p, float a);
Vector normalize(Vector & p);
float operator* (const Vector& p, const Vector& q);
Vector operator/(const Vector&p, float a);

#endif
