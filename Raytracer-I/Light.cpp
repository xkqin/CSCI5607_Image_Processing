//
//  Light.cpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "Light.hpp"


Pixel Light::getLightColor(Point interescP)
{
    return Pixel(lr,lg,lb);
}

bool operator< (const Light &p, const Light &q)
{
    return true;
}

Vector Light::direction(Point p_)
{
    //printf("light direction\n");
    return Vector(0,0,0);
}
