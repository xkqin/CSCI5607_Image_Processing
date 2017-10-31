//
//  directionalLight.hpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef directionalLight_hpp
#define directionalLight_hpp

#include <stdio.h>
#include "vector.hpp"
#include "point.h"

#endif /* directionalLight_hpp */

class directionalLight{
    
    float lr,lg,lb;
    float x,y,z;
    
public:
    directionalLight(float lr_,float lg_, float lb_, float x_, float y_, float z_):lr(lr_), lg(lg_), lb(lb_){x = x_;y = y_; z = z_;}
    directionalLight(){}
    
    //Vector direction(Point p); //p is the intersection point
    Vector direction(Point p);
    Point getLightColor(Point intersecP);
};

bool operator< (const directionalLight &p, const directionalLight &q);
