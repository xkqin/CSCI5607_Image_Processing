//
//  Object.cpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "Object.hpp"

Point object::background;

int object::number;

set<object *> object::objectSet;

//origin is the point where reflection ray transmits; mirror is the direction of the reflection ray; depth is the maximum recusion number
Point object::evaluateRayTree(Point origin, Vector mirror, int depth)
{
    if(depth==0)
    {
        return object::background;
    }
    float tmin = 9999;
    object * targetObejct = NULL; //the object corresponding to the smallest t
    bool is_hit = false;
    for(set<object *>::iterator it = object::objectSet.begin(); it != object::objectSet.end(); ++it)
    {
        object * obj = *it;
        if(obj->isIntersectionWith(origin, mirror))
        {
            
            float t = obj->length_of_t(origin, mirror);
            //printf("t = %f\n",t);
            //printf("raydir = %f %f %f\n",mirror.x,mirror.y,mirror.z);
            //tmin = 0, tmax = 100
            if(t<tmin && t>0 && t<100)
            {
                targetObejct = obj;
                tmin = t;
                is_hit = true;
            }
        }
    }

    //if hit some objects, then return the intersection point's color value.
    if(is_hit)
    {
        //calculate the intersection point with the targetobject
        Point intersecP = origin+mirror*tmin;
        //get the color of the intersection point with target obj
        //where the recursion happens
        //printf("1\n");
        Pixel reflexIntensity = targetObejct->calculateColorThroughRay(intersecP, mirror, depth);
        //printf("2\n");
        Point result = Point(reflexIntensity.r,reflexIntensity.g,reflexIntensity.b);
        //printf("result: %f %f %f\n", result.x,result.y,result.z);
        return result;
    }
    else{
        return object::background;
    }
}
