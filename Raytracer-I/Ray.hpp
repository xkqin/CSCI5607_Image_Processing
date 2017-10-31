//
//  Ray.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/19/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "vector.hpp"
#include "point.h"

struct ray{
    Vector direction;
    Point origin;
    
    ray(){direction = Vector(0,0,0); origin = Point(0,0,0);}
};


#endif /* Ray_hpp */
