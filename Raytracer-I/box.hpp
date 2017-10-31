//
//  box.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/21/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef box_hpp
#define box_hpp

#include <stdio.h>
#include "Object.hpp"
#include "material.hpp"
#include "Plane.hpp"
class box: public object{
    Point center;
    float length;
    Plane planes[6];
    float min_t;
    material m;
    set<pointLight> pointlightSet;
    set<directionalLight> dirlightSet;
    set<spotLight> spotlightSet;
    int min_plane;
    float amr, amg, amb; //ambient light
    
    
    
public:
    virtual bool isIntersectionWith(Point origin, Vector raydir);
    
    virtual float length_of_t(Point origin, Vector ray);
    
    virtual Pixel calculateColorThroughRay(Point intersecP, Vector raydir, int depth);
    
    void setMaterial(material m_);
    
    Vector getReflection(Vector ray, Vector hit_normal);
    
    void setAmbientLightSource(float ambr, float ambg, float ambb);
    
    void setPointLights(set<pointLight> pointset);
    
    void setDirectionLight(set<directionalLight> dirset);
    
    bool isInShadow(Point intersecP);
    
    void setSpotLights(set<spotLight> spotset);
    
    Pixel calculateAmbientColor();
    
    Pixel calculateSpecualrColor(Point intersecP, Vector raydir);
    
    Pixel calculateDiffuseColor(Point intersecP);
    
    void setCenter(Point p)
    {
        center = p;
    }
    
    void setLength(float l)
    {
        length = l;
    }
    
    box(Point center_, float length_): center(center_), length(length_)
    {
        planes[0] = Plane(Point(center_.x+length_/2,center_.y,center_.z), Vector(1,0,0));
        planes[1] = Plane(Point(center_.x-length_/2,center_.y,center_.z), Vector(-1,0,0));
        planes[2] = Plane(Point(center_.x,center_.y+length_/2,center_.z), Vector(0,1,0));
        planes[3] = Plane(Point(center_.x,center_.y-length_/2,center_.z), Vector(0,-1,0));
        planes[4] = Plane(Point(center_.x,center_.y,center_.z+length_/2), Vector(0,0,1));
        planes[5] = Plane(Point(center_.x,center_.y,center_.z-length_/2), Vector(0,0,-1));
    }
    
    box(){}
};

#endif /* box_hpp */
