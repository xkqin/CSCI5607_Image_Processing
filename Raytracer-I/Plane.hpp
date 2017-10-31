//
//  Plane.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/21/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Vertex.hpp"

class Plane{
public:
    Point p;
    Vector normal;
    bool is_hit(Point p, Vector raydir);
    float length_of_t(Point p, Vector raydir);
    Plane(Point p_, Vector normal_)
    {
        p = p_;
        normal = normal_;
    }
    Plane(){}
};
#endif /* Plane_hpp */
