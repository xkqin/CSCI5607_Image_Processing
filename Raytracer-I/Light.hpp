//
//  Light.hpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include "pixel.h"
#include "point.h"
#include <set>

using namespace std;

class Light{
    
protected:
    float lr,lg,lb;

    
public:
    Light(float lr_,float lg_, float lb_):lr(lr_),lg(lg_),lb(lb_){}
    Light(){}
    
    //set the virtual function to work in different way for subcalss
    Pixel getLightColor(Point intersecP);
    Vector direction(Point p);
};

bool operator< (const Light &p, const Light &q);

#endif /* Light_hpp */


