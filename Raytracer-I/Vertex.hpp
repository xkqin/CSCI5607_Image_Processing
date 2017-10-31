//
//  Vertex.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include "vector.hpp"
#include "point.h"
class vertex{
public:
    double x,y,z; //coordinate of the vertex
};

Vector operator- (vertex const &a, vertex const &b);
double operator* (vertex const &a, Vector const &b);
Vector operator- (Point const &a, vertex const &b);
#endif /* Vertex_hpp */

