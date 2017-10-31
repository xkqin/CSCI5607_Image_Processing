//
//  pointLight.hpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/8/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef pointLight_hpp
#define pointLight_hpp

#include <stdio.h>
#include <math.h>
#include "Light.hpp"

#endif /* pointLight_hpp */

class pointLight{
    
    Point pos; //the position  of point light source
    float lr,lg,lb;
    
public:
    pointLight(float pr_,float pg_, float pb_, float posx_, float posy_, float posz_)
    {pos.x = posx_; pos.y = posy_; pos.z = posz_;
        lr = pr_; lg = pg_; lb = pb_;}
    pointLight(){}
    
    Vector direction(Point p)
    {
        Vector dir = p - pos;
        return dir;
    }
    
    Point getPosition()
    {
        return pos;
    }
    
    Point getLightColor(Point intersecP)
    {
        // calculate the distance between light source and intersection point
        Vector dist = intersecP - pos;
        float distance = dist*dist;
        
        //printf("r:%f g:%f b:%f\n",lr/distance,lg/distance,lb/distance);
        Point color = Point(lr/distance,lg/distance,lb/distance);
        return color;
    }
};

bool operator< (const pointLight &p, const pointLight &q);
