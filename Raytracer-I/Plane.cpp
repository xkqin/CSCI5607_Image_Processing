//
//  Plane.cpp
//  Rayest
//
//  Created by Zheng Zhang on 10/21/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "Plane.hpp"

bool Plane::is_hit(Point p, Vector raydir)
{
    if(raydir*normal!=0)
    {
        return true;
    }
    else{
        return false;
    }
}

float Plane::length_of_t(Point origin, Vector raydir)
{
    double d = p.x*normal.x+p.y*normal.y+p.z*normal.z;
    double VN = raydir*normal;
    double eN = normal.x*origin.x+normal.y*origin.y+normal.z*origin.z;
    //printf("d=%f,VN=%f,eN=%f\n",d,VN,eN);
    double t = (d-eN)/VN;
    return t;
}

