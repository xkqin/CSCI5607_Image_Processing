//
//  spotLight.cpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/8/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "spotLight.hpp"

bool operator< (const spotLight& p, const spotLight& q)
{
    return true;
}

Vector spotLight::direction()
{
    return Vector(dx,dy,dz);
}

Point spotLight::getLightColor(Point IntersecP)
{
    Point color;
    
    Vector ray = IntersecP - Point(px,py,pz);
    
    Vector normalRay = normalization(ray);
    
    Vector spotdir = Vector(dx,dy,dz);
    spotdir = normalization(spotdir);
    
    double cos = normalRay*spotdir;
    double cosa1 = cosf(angle1*3.1415926/180);
    double cosa2 = cosf(angle2*3.1415926/180);
    if(cos>cosa1)
    {
        //we treat it as the point light
        //printf("angle: %f ray*ray = %f\n", ray*ray);
        color = Point(1*r/(ray*ray),1*g/(ray*ray),1*b/(ray*ray));
    }
    else if(cos>cosa2)
    {
        //the intensity will fall off linearly in source
        printf("cosa1<cos<cosa2\n");
        float angle = acos(cos);
        float fallOffInSource = (angle2-angle*180/3.1415926)/(angle2-angle1);
        printf("angle: %f ray*ray= %f\n",angle*180/3.1415926,ray*ray);
        color = Point(fallOffInSource*r/(ray*ray),fallOffInSource*g/(ray*ray),fallOffInSource*b/(ray*ray));
    }
    else{
        printf("cos>cosa2\n");
        color = Point(0,0,0);
    }
    
    return color;
}
