//
//  vector.hpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef vector_hpp
#define vector_hpp

#include <stdio.h>

struct Vector{
    
    float x,y,z;
    
    Vector (float x_=0, float y_=0, float z_=0) : x(x_), y(y_), z(z_){}
    
    void set(float x_, float y_, float z_){x = x_; y = y_; z = z_;}
    
    
};

Vector operator+ (const Vector& p, const Vector& q);

Vector operator- (const Vector& p, const Vector& q);

Vector operator- (const Vector& p);

Vector operator* (const Vector& p, float a);

Vector crossProduct(Vector & u, Vector & v);

float operator* (const Vector& p, const Vector& q);



Vector normalization(Vector & p);


#endif /* vector_hpp */
