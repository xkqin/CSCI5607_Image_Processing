//
//  triangle.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include <stdio.h>
#include "Vertex.hpp"
#include "Object.hpp"
#include "vector.hpp"
#include "material.hpp"
#include "set"
#include "image.h"

using namespace std;

class triangle: public object{
    
    vertex vertices[3]; // vertices
    Vector normals[3]; //normal
    material m;
    set<pointLight> pointlightSet;
    set<directionalLight> dirlightSet;
    set<spotLight> spotlightSet;
    float amr, amg, amb; //ambient light
    Image * textureImage = NULL;
    
    static set<triangle> sphereSet;
    
    static Point background;
    
    //method calculating the color should be private
    Pixel calculateAmbientColor();
    
    Pixel calculateSpecualrColor(Point intersecP, Vector raydir);
    
    Pixel calculateDiffuseColor(Point intersecP,Vector raydir);
    
    Pixel calculateTextureColor(Point intersecP);
    
    Vector getReflection(Vector ray, Vector hit_normal);
    
    bool sameSide(Point p1, vertex p2, vertex a, vertex );
    
    bool isInShadow(Point intersecP);
    
    Vector calculateInterpolationNormal(Point intersecP);
    
public:
    //public interface
    bool hasNormalArray = false;
    
    void setMaterial(material m_);
    
    void setImage(Image * image);
    
    void setAmbientLightSource(float ambr, float ambg, float ambb);
    
    void setPointLights(set<pointLight> pointset);
    
    void setDirectionLight(set<directionalLight> dirset);
    
    void setVertice(vertex v1, vertex v2, vertex v3);
    
    void setNormals(Vector n1, Vector n2, Vector n3);
    
    void setSpotLights(set<spotLight> spotset);
    
    virtual bool isIntersectionWith(Point origin, Vector raydir);
    
    virtual float length_of_t(Point origin, Vector ray);
    
    virtual Pixel calculateColorThroughRay(Point intersecP, Vector raydir, int depth);

};
#endif /* triangle_hpp */
