//
//  Object.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include "point.h"
#include "pixel.h"
#include "set"
#include "pointLight.hpp"
#include "directionalLight.hpp"
#include "spotLight.hpp"


using namespace std;

class object{
public:
    
    static set<object *> objectSet;
    
    static Point background;
    
    static int number;
    
    virtual bool isIntersectionWith(Point origin, Vector raydir)
    {
        printf("isIntersectionWith obejct\n");
        return false;
    }
    
    virtual float length_of_t(Point origin, Vector ray)
    {
        printf("lengtj_of_t obejct\n");
        return 0;
    }
    
    virtual Pixel calculateColorThroughRay(Point intersecP, Vector raydir, int depth)
    {
        printf("calculateColorThroughRay obejct\n");
        return Pixel(0,0,0);
    }
    
    static Point evaluateRayTree(Point origin, Vector mirror, int depth);
};

#endif /* Object_hpp */
