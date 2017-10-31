//
//  spotLight.hpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/8/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef spotLight_hpp
#define spotLight_hpp

#include <cstdio>
#include "vector.hpp"
#include "point.h"
#include <math.h>

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
    
    Point getPosition()
    {
        return Point(px,py,pz);
    }
};

bool operator< (const spotLight& p, const spotLight& q);
#endif /* spotLight_hpp */
