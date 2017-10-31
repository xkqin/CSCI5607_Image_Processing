//
//  directionalLight.cpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "directionalLight.hpp"

Vector directionalLight::direction(Point p)
{
    return Vector(x,y,z);
}

Point directionalLight::getLightColor(Point intersecP)
{
    return Point(lr,lg,lb);
}

bool operator< (const directionalLight &p, const directionalLight &q)
{
    return true;
}
