//
//  AreaLight.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/20/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef AreaLight_hpp
#define AreaLight_hpp

#include <stdio.h>
#include "vector.hpp"
#include "point.h"

class AreaLight {
    float lr,lg,lb;
    Vector pos;
    Vector dirx,diry;
    
public:
    AreaLight(float lr_,float lg_,float lb_):lr(lr_),lg(lg_),lb(lb_){}
    void setPosition(Vector pos_);
    void setDirection(Vector d1, Vector d2);
    Vector getPosition()
    {
        return pos;
    }
};

#endif /* AreaLight_hpp */
