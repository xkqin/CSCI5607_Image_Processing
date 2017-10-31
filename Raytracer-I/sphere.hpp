//
//  sphere.hpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef sphere_hpp
#define sphere_hpp

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "vector.hpp"
#include "Object.hpp"
#include "Ray.hpp"

class sphere:public object{
    
    float sx,sy,sz;
    float sradius;
    Point center;
    
    set<pointLight> pointlightSet;
    set<directionalLight> dirlightSet;
    set<spotLight> spotlightSet;
    
    float ar,ag,ab; //ambient color(coefficient)
    float dr,dg,db; //diffuse color(coefficient)
    float sr,sg,sb; //specular color(coefficient)
    float pn;
    
    float amr, amg, amb;
    
    
    float tr,tg,tb; //transmissive color
    float ior; //index of refraction
    
    Pixel calculateAmbientColor();
    
    Pixel calculateSpecualrColor(Point intersecP, Vector raydir);
    
    Pixel calculateDiffuseColor(Point intersecP);
    //calculate the point where the refraction ray goes out the sphere
    ray calculateRefractionOutPoint(Point intersecP, Vector raydir);
    
public:
    sphere(){}
    
    sphere(float x_,float y_,float z_, float r):sx(x_),sy(y_),sz(z_),sradius(r){center = Point(x_,y_,z_);}
    
    virtual bool isIntersectionWith(Point origin, Vector raydir);
    
    virtual float length_of_t(Point origin, Vector raydir);
    
    virtual Pixel calculateColorThroughRay(Point intersecP, Vector raydir, int depth);

    void setAmbientLightSource(float ambr, float ambg, float ambb);
    
    void setAmbientColor(float ar_, float ag_, float ab_);
    
    void setDiffuseColor(float dr_, float dg_, float db_);
    
    void setSpecularColor(float sr_, float sg_, float sb_);
    
    void setTransmissiveColor(float tr_, float tg_, float tb_);
    
    void setIndexOfRefraction(float ior_);
    
    void setPointLights(set<pointLight> pointset);
    
    void setDirectionLight(set<directionalLight> dirset);
    
    void setPn(float p);
    
    Vector getReflection(Vector ray, Vector hit_normal);
    
    void setSpotLights(set<spotLight> spotset);
    
    bool isInShadow(Point intersecP);
    
    float getX() const;
    
    float getY() const;
    
    float getZ() const;
    
    float getR() const;
    
};

bool operator< (const sphere &p, const sphere &q);



#endif /* sphere_hpp */
