

#ifndef spotLight_INCLUDED
#define spotLight_INCLUDED

#include <stdio.h>
#include "vector.h"
#include "point.h"

class spotLight{
    float r,g,b;
    float px,py,pz;
    float dx,dy,dz;
    float angle1,angle2;

public:
    spotLight(float r_, float g_, float b_, float px_, float py_, float pz_, float dx_, float dy_, float dz_, float angle1_, float angle2_)
    {
        r = r_;
        g = g_;
        b = b_;
        px = px_;
        py = py_;
        pz = pz_;
        dx = dx_;
        dy = dy_;
        dz = dz_;
        angle1 = angle1_;
        angle2 = angle2_;
    }

    Vector direction();

    Point getLightColor(Point IntersecP);
};
#endif 
