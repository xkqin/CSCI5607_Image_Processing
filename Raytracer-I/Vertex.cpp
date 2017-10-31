//
//  Vertex.cpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "Vertex.hpp"

Vector operator-(vertex const &a, vertex const &b)
{
    Vector v = Vector(a.x-b.x, a.y-b.y, a.z-b.z);
    return v;
}

double operator* (vertex const &a, Vector const &b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vector operator- (Point const &a, vertex const &b)
{
    Vector v = Vector(a.x-b.x, a.y-b.y, a.z-b.z);
    return v;
}
