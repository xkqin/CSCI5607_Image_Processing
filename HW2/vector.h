

#ifndef vector_INCLUDED
#define vector_INCLUDED

#include <stdio.h>

struct Vector{

    float x,y,z;

    Vector (float x_=0, float y_=0, float z_=0) : x(x_), y(y_), z(z_){}



};

Vector operator+ (const Vector& p, const Vector& q);

Vector operator- (const Vector& p, const Vector& q);

Vector operator- (const Vector& p);

Vector operator* (const Vector& p, float a);

Vector crossProduct(Vector & u, Vector & v);
Vector divided(const Vector&p, float a);
Vector normalize(Vector & p);
float operator* (const Vector& p, const Vector& q);
Vector operator/(const Vector&p, float a);

#endif 
