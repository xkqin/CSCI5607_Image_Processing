
#include "vector.h"
#include "math.h"
Vector operator+ (const Vector& p, const Vector& q)
{
    Vector result = Vector(p.x+q.x,p.y+q.y,p.z+q.z);
    return result;
}

Vector operator- (const Vector& p, const Vector& q)
{
    Vector result = Vector(p.x-q.x,p.y-q.y,p.z-q.z);
    return result;
}

Vector operator* (const Vector& p, float a)
{
    return Vector(p.x*a,p.y*a,p.z*a);
}

Vector operator- (const Vector& p)
{
    return Vector(-p.x,-p.y,-p.z);
}

float operator* (const Vector& p, const Vector& q)
{
    return p.x*q.x+p.y*q.y+p.z*q.z;
}

Vector operator/(const Vector&p, float a) {
	return Vector(p.x/a, p.y/a, p.z/a);
}

Vector crossProduct(Vector & u, Vector & v)
{
    float cx = u.y*v.z-u.z*v.y;
    float cy = u.z*v.x-u.x*v.z;
    float cz = u.x*v.y-u.y*v.x;
    return Vector(cx,cy,cz);
}

Vector normalize(Vector & p)
{
	float a = sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2));
	return Vector(p.x / a, p.y / a, p.z / a);
}
