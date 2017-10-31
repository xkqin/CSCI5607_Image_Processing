//
//  point.h
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef point_h
#define point_h

#include "vector.hpp"

struct Point{
    float x,y,z;
    
    Point (float x_, float y_) : x(x_), y(y_){}
    Point (float x_, float y_, float z_) : x(x_), y(y_), z(z_){}
    Point (){}
    
    void set(float x_, float y_, float z_){x = x_; y = y_; z = z_;}
};

Point operator+ (const Point& p, const Point& q);

Vector operator- (const Point& p, const Point& q);

Point operator* (const Point& p, float a);

Point operator+ (const Point& p, const Vector& q);

Point operator- (const Point& p, const Vector& q);

float operator* (const Vector& p, const Point& q);

#endif /* point_h */
