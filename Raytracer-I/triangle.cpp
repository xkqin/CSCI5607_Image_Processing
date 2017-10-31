//
//  triangle.cpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "triangle.hpp"

void triangle::setVertice(vertex v1, vertex v2, vertex v3)
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
}

void triangle::setMaterial(material m_)
{
    m = m_;
}

Vector triangle::getReflection(Vector ray, Vector hit_normal)
{
    ray = normalization(ray);
    hit_normal = normalization(hit_normal);
    Vector reflex = ray - hit_normal*2*(ray*hit_normal);
    return reflex;
}

Pixel triangle::calculateColorThroughRay(Point intersecP, Vector raydir, int depth)
{
    if(isInShadow(intersecP))
    {
        return Pixel(0,0,0);
    }
    //use texture mapping
    if(textureImage != NULL)
    {
        return calculateTextureColor(intersecP);
    }
    
    Vector normalOftriangle = calculateInterpolationNormal(intersecP);
    normalOftriangle = normalization(normalOftriangle);
    Vector reflection = getReflection(raydir, normalOftriangle);
    
    //add a tiny distance to intersection point to avoid hit itself
    Point add_intersec = intersecP + reflection*0.01;
    
    Point reflect = object::evaluateRayTree(add_intersec, reflection, depth--);
    Pixel reflectionColor;
    reflectionColor.SetClamp(reflect.x*m.sr,reflect.y*m.sg,reflect.z*m.sb);
    
    Pixel p = calculateDiffuseColor(intersecP,raydir)+calculateAmbientColor()+calculateSpecualrColor(intersecP, raydir)+reflectionColor;
    //printf("pixel r %d, g %d, b %d\n",p.r,p.g,p.b);
    return p;
}

bool triangle::isInShadow(Point intersecP)
{
    bool isShadow = false;
    //generate a ray from interestion point to each light source and check if it will hit any light source.
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        //generate the ray
        //we should add a little distance to intersecP so that it won't hit itself
        Vector lightray = -lgt.direction(intersecP);
        //lightray = normalization(lightray);
        intersecP = intersecP + lightray*0.001;
        for(set<object *>::iterator it = object::objectSet.begin(); it != object::objectSet.end(); ++it)
        {
            object * obj = *it;
            if(obj->isIntersectionWith(intersecP, lightray) && obj->length_of_t(intersecP, lightray)>0.01)
            {
                isShadow = true;
                break;
            }
            
        }
        if(isShadow)
        {
            break;
        }
    }
    
    for(set<spotLight>::iterator it = spotlightSet.begin(); it != spotlightSet.end(); ++it)
    {
        spotLight lgt = *it;
        //generate the ray
        //we should add a little distance to intersecP so that it won't hit itself
        Vector lightray = -lgt.direction();
        //lightray = normalization(lightray);
        intersecP = intersecP + lightray*0.001;
        for(set<object *>::iterator it = object::objectSet.begin(); it != object::objectSet.end(); ++it)
        {
            object * obj = *it;
            if(obj->isIntersectionWith(intersecP, lightray) && obj->length_of_t(intersecP, lightray)>0.01)
            {
                isShadow = true;
                break;
            }
            
        }
        if(isShadow)
        {
            break;
        }
    }
    
    for(set<directionalLight>::iterator it = dirlightSet.begin(); it != dirlightSet.end(); ++it)
    {
        directionalLight lgt = *it;
        //generate the ray
        //we should add a little distance to intersecP so that it won't hit itself
        Vector lightray = -lgt.direction(intersecP);
        //lightray = normalization(lightray);
        intersecP = intersecP + lightray*0.001;
        for(set<object *>::iterator it = object::objectSet.begin(); it != object::objectSet.end(); ++it)
        {
            object * obj = *it;
            if(obj->isIntersectionWith(intersecP, lightray) && obj->length_of_t(intersecP, lightray)>0.01)
            {
                isShadow = true;
                break;
            }
            
        }
        if(isShadow)
        {
            break;
        }
    }
    return isShadow;
}

Pixel triangle::calculateAmbientColor()
{
    return Pixel(amr*m.ar*255,amg*m.ag*255,amb*m.ab*255);
}

Pixel triangle::calculateSpecualrColor(Point intersecP, Vector raydir)
{
    float r=0,g=0,b=0;
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        Vector v = lgt.direction(intersecP);
//        Vector edge1 = vertices[0]-vertices[2];
//        Vector edge2 = vertices[1]-vertices[2];
        Vector normalOftriangle = calculateInterpolationNormal(intersecP);
        //if raydir*normalOfPlane < 0, we should use the crossProduct(edg2,edge1)
//        if(raydir*normalOfPlane>0)
//        {
//            normalOfPlane = crossProduct(edge2, edge1);
//        }
        normalOftriangle = normalization(normalOftriangle);
        Vector reflection = v-2*(v*normalOftriangle)*normalOftriangle;
        reflection = normalization(reflection);
        raydir = normalization(raydir);
        float cos = reflection*(-raydir);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += m.sr*lightIntensity.x*pow(fmax(cos, 0),m.pn);
        g += m.sg*lightIntensity.y*pow(fmax(cos, 0),m.pn);
        b += m.sb*lightIntensity.z*pow(fmax(cos, 0),m.pn);
    }
    
    for(set<directionalLight>::iterator it = dirlightSet.begin(); it != dirlightSet.end(); ++it)
    {
        directionalLight lgt = *it;
        Vector v = lgt.direction(intersecP);
        Vector normalOftriangle = calculateInterpolationNormal(intersecP);
        normalOftriangle = normalization(normalOftriangle);
        Vector reflection = v-2*(v*normalOftriangle)*normalOftriangle;
        reflection = normalization(reflection);
        raydir = normalization(raydir);
        float cos = reflection*(-raydir);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += m.sr*lightIntensity.x*pow(fmax(cos, 0),m.pn);
        g += m.sg*lightIntensity.y*pow(fmax(cos, 0),m.pn);
        b += m.sb*lightIntensity.z*pow(fmax(cos, 0),m.pn);
        //printf("r: %f, g: %f, b: %f\n",r,g,b);
    }
    
    for(set<spotLight>::iterator it = spotlightSet.begin(); it != spotlightSet.end(); ++it)
    {
        spotLight lgt = *it;
        Vector v = lgt.direction();
        Vector normalOftriangle = calculateInterpolationNormal(intersecP);
        //if raydir*normalOfPlane < 0, we should use the crossProduct(edg2,edge1)
        normalOftriangle = normalization(normalOftriangle);
        Vector reflection = v-2*(v*normalOftriangle)*normalOftriangle;
        raydir = normalization(raydir);
        float cos = reflection*(-raydir);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += m.sr*lightIntensity.x*pow(fmax(cos, 0),m.pn);
        g += m.sg*lightIntensity.y*pow(fmax(cos, 0),m.pn);
        b += m.sb*lightIntensity.z*pow(fmax(cos, 0),m.pn);
        //printf("r: %f, g: %f, b: %f\n",r,g,b);
    }
    if(r>1)
    {
        r = 1;
    }
    if(g>1)
    {
        g = 1;
    }
    if(b>1)
    {
        b = 1;
    }
    //printf("diffuse color: %f %f %f\n", r,g,b);
    
    return Pixel(r*255, g*255, b*255);
}


Pixel triangle::calculateDiffuseColor(Point intersecP,Vector raydir)
{
    
    float r=0,g=0,b=0;
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        Vector v = -lgt.direction(intersecP);
        v = normalization(v);
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector normalOftriangle = calculateInterpolationNormal(intersecP);
        normalOftriangle = normalization(normalOftriangle);
        //dot product of unit vector equals to cos(angle)
        float cos = v*normalOftriangle;
        //we calculate the sine to check if it > 0
        //printf("cos = %f, angle = %f\n",cos);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += m.dr*lightIntensity.x*fmax(cos,0);
        g += m.dg*lightIntensity.y*fmax(cos, 0);
        b += m.db*lightIntensity.z*fmax(cos, 0);
    }
    
    for(set<directionalLight>::iterator it = dirlightSet.begin(); it != dirlightSet.end(); ++it)
    {
        directionalLight lgt = *it;
        Vector v = -lgt.direction(intersecP);
        v = normalization(v);
        Vector normalOftriangle = calculateInterpolationNormal(intersecP);
        normalOftriangle = normalization(normalOftriangle);
        float cos = v*normalOftriangle;
        //printf("cos = %f\n",cos);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += m.dr*lightIntensity.x*fmax(cos,0);
        g += m.dg*lightIntensity.y*fmax(cos, 0);
        b += m.db*lightIntensity.z*fmax(cos, 0);
    }
    
    for(set<spotLight>::iterator it = spotlightSet.begin(); it != spotlightSet.end(); ++it)
    {
        spotLight lgt = *it;
        Vector v = -lgt.direction();
        v = normalization(v);
        Vector normalOftriangle = calculateInterpolationNormal(intersecP);
        normalOftriangle = normalization(normalOftriangle);
        //printf("%f\n", unit_normal.x*unit_normal.x+unit_normal.y*unit_normal.y+unit_normal.z*unit_normal.z);
        //dot product of unit vector equals to cos(angle)
        double cos = v*normalOftriangle;
        //printf("cos = %f\n",cos);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += m.dr*lightIntensity.x*fmax(cos,0);
        g += m.dg*lightIntensity.y*fmax(cos, 0);
        b += m.db*lightIntensity.z*fmax(cos, 0);
    }
    if(r>1)
    {
        r = 1;
    }
    if(g>1)
    {
        g = 1;
    }
    if(b>1)
    {
        b = 1;
    }
    //printf("spot: %ld, direc: %ld, point: %ld\n", spotlightSet.size(),dirlightSet.size(),pointlightSet.size());
    return Pixel(r*255, g*255, b*255);
}

void triangle::setPointLights(set<pointLight> lights)
{
    pointlightSet = lights;
}

void triangle::setDirectionLight(set<directionalLight> lights)
{
    dirlightSet = lights;
}

void triangle::setSpotLights(set<spotLight> spotset)
{
    spotlightSet = spotset;
}

bool triangle::sameSide(Point p1, vertex p2, vertex a, vertex b)
{
    Vector ba = b-a;
    Vector p1a = p1-a;
    Vector p2a = p2-a;
    Vector cp1 = crossProduct(ba, p1a);
    Vector cp2 = crossProduct(ba, p2a);
    return cp1*cp2>=0;
}

void triangle::setAmbientLightSource(float ambr, float ambg, float ambb)
{
    amr = ambr;
    amg = ambg;
    amb = ambb;
}

float triangle::length_of_t(Point origin, Vector ray)
{
    //calculate the normal of the plane triangle is in
    Vector edge1 = vertices[0]-vertices[2];
    Vector edge2 = vertices[1]-vertices[2];
    Vector normalOfPlane = crossProduct(edge1, edge2);
    normalOfPlane = normalization(normalOfPlane);
    printf("normal: %f %f %f\n",normalOfPlane.x,normalOfPlane.y,normalOfPlane.z);
    //if dot product of normalOfPlane and raydir is 0, it means this ray is parallel with plane and doesn't have intersection point.
    if(normalOfPlane*ray==0)
    {
        return false;
    }
    
    //choose vertrices[2] as the C0 in plane, and calculate the t
    double d = vertices[2]*normalOfPlane;
    double VN = ray*normalOfPlane;
    double eN = normalOfPlane*origin;
    //printf("d=%f,VN=%f,eN=%f\n",d,VN,eN);
    double t = (d-eN)/VN;
    return t;
}

void triangle::setNormals(Vector n1, Vector n2, Vector n3)
{
    normals[0] = n1;
    normals[1] = n2;
    normals[2] = n3;
}

void triangle::setImage(Image * image)
{
    textureImage = image;
}

Vector triangle::calculateInterpolationNormal(Point intersecP)
{
    //calculate the alpha, beta and gamma in barycentric coordinate, before it we has passed isIntersectionWith function,
    //so we can assume alpha, beta and gamma are all conform to the limitation.
    
    //we build the barycentric coordinate in vertex0
    if(!hasNormalArray)
    {
        Vector edge1 = vertices[0]-vertices[2];
        Vector edge2 = vertices[1]-vertices[2];
        Vector normalOfPlane = crossProduct(edge1, edge2);
        normalOfPlane = normalization(normalOfPlane);
        printf("this triangle has no normal\n");
        return normalOfPlane;
    }
    
    Vector v1 = vertices[1]-vertices[0];
    Vector v2 = vertices[2]-vertices[0];
    Vector n = crossProduct(v1,v2);
    
    v1 = vertices[2]-vertices[1];
    v2 = intersecP-vertices[1];
    Vector nv0 = crossProduct(v1,v2);
    float alpha = (n*nv0)/(n*n);
    
    v1 = vertices[0]-vertices[2];
    v2 = intersecP-vertices[2];
    Vector nv1 = crossProduct(v1,v2);
    float beta = (n*nv1)/(n*n);
    
    v1 = vertices[1]-vertices[0];
    v2 = intersecP-vertices[0];
    Vector nv2 = crossProduct(v1,v2);
    float gamma = (n*nv2)/(n*n);
    
    //interpolate the normal
    Vector intpNormal = normals[0]*alpha+normals[1]*beta+normals[2]*gamma;
    return intpNormal;
}

Pixel triangle::calculateTextureColor(Point intersecP)
{
    if(textureImage == NULL)
    {
        return Pixel(0,0,0);
    }
    
    //choose the texture coordinate position for each vertex randomly
    Point v1p = Point(200,150);
    Point v2p = Point(1000,560);
    Point v3p = Point(740,1200);
    
    Vector v1 = vertices[1]-vertices[0];
    Vector v2 = vertices[2]-vertices[0];
    Vector n = crossProduct(v1,v2);
    
    v1 = vertices[2]-vertices[1];
    v2 = intersecP-vertices[1];
    Vector nv0 = crossProduct(v1,v2);
    float alpha = (n*nv0)/(n*n);
    
    v1 = vertices[0]-vertices[2];
    v2 = intersecP-vertices[2];
    Vector nv1 = crossProduct(v1,v2);
    float beta = (n*nv1)/(n*n);
    
    v1 = vertices[1]-vertices[0];
    v2 = intersecP-vertices[0];
    Vector nv2 = crossProduct(v1,v2);
    float gamma = (n*nv2)/(n*n);
    
    Point interpolationPoint = Point(alpha*v1p.x+beta*v2p.x+gamma*v3p.x,alpha*v1p.y+beta*v2p.y+gamma*v3p.y);
    Pixel p = textureImage->GetPixel(interpolationPoint.x, interpolationPoint.y);
    return p;
}

bool triangle::isIntersectionWith(Point origin, Vector raydir)
{
    //calculate the normal of the plane triangle is in
    Vector edge1 = vertices[0]-vertices[2];
    Vector edge2 = vertices[1]-vertices[2];
    Vector normalOfPlane = crossProduct(edge1, edge2);
    normalOfPlane = normalization(normalOfPlane);
    //printf("vertex0 : x=%f, y=%f, z=%f\n",vertices[0].x, vertices[0].y,vertices[0].z);
    //if dot product of normalOfPlane and raydir is 0, it means this ray is parallel with plane and doesn't have intersection point.
    if(normalOfPlane*raydir==0)
    {
        return false;
    }
    
    //choose vertrices[2] as the C0 in plane, and calculate the t
    double d = vertices[2]*normalOfPlane;
    double VN = raydir*normalOfPlane;
    double eN = normalOfPlane*origin;
    //printf("d = %f, VN = %f, eN = %f\n",d,VN,eN);
    double t = (d-eN)/VN;

    //get the intersection point
    Point p = origin+raydir*t;
    //check if this point in the triangle
    bool v0p, v1p,v2p;
    
    v0p = sameSide(p, vertices[2], vertices[0], vertices[1]);
    v1p = sameSide(p, vertices[0], vertices[1], vertices[2]);
    v2p = sameSide(p, vertices[1], vertices[2], vertices[0]);
    
    return v0p&&v1p&&v2p;
}
