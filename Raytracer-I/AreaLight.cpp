//
//  AreaLight.cpp
//  Rayest
//
//  Created by Zheng Zhang on 10/20/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "AreaLight.hpp"

void AreaLight::setPosition(Vector pos_)
{
    pos = pos_;
}

void AreaLight::setDirection(Vector d1, Vector d2)
{
    dirx = d1;
    diry = d2;
}
