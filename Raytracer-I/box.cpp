
  //box.cpp
  //Rayest

  //Created by Zheng Zhang on 10/21/17.
  //Copyright © 2017 Zheng Zhang. All rights reserved.


#include "box.hpp"

void box::setMaterial(material m_)
{
    m = m_;
}


bool box::isIntersectionWith(Point origin, Vector raydir)
{
    float txmin, txmax;
    float tymin, tymax;
    float tzmin, tzmax;
    float tmin, tmax;
    
    txmin = planes[0].length_of_t(origin,raydir);
    txmax = planes[1].length_of_t(origin, raydir);
    
    tymin = planes[2].length_of_t(origin,raydir);
    tymax = planes[3].length_of_t(origin, raydir);
    
    tzmin = planes[4].length_of_t(origin,raydir);
    tzmax = planes[5].length_of_t(origin, raydir);
    
    tmin = txmin;
    tmax = txmax;
    
    min_plane = 0;
    
    if(tmin>tmax)
    {
        swap(tmin, tmax);
        min_plane = 1;
    }
    
    int min_y = 2;
    if(tymin>tymax)
    {
        swap(tymin, tymax);
        min_y = 3;
    }
    
    if(tmin>tymax || tmax<tymin)
    {
        return false;
    }
    
    if(tymin>tmin)
    {
        tmin = tymin;
        min_plane = min_y;
    }
    
    if(tymax<tmax)
    {
        tmax = tymax;
    }
    
    int min_z = 4;
    if(tzmax<tzmin)
    {
        swap(tzmax, tzmin);
        min_z = 5;
    }
    
    if(tmin>tzmax || tmax<tzmin)
    {
        return false;
    }
    
    if(tzmin>tmin)
    {
        tmin = tzmin;
        min_plane = min_z;
    }
    
    if(tzmax<tmax)
    {
        tmax = tzmax;
    }
    
    if(tmin<0 && tmax<0)
    {
        return false;
    }
    
    min_t = tmin;
    
    return true;
}

float box::length_of_t(Point origin, Vector ray)
{
    return min_t;
}

void box::setPointLights(set<pointLight> lights)
{
    pointlightSet = lights;
}

void box::setDirectionLight(set<directionalLight> lights)
{
    dirlightSet = lights;
}

void box::setSpotLights(set<spotLight> spotset)
{
    spotlightSet = spotset;
}

void box::setAmbientLightSource(float ambr, float ambg, float ambb)
{
    amr = ambr;
    amg = ambg;
    amb = ambb;
}

bool box::isInShadow(Point intersecP)
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
                //                if(!(sp.center.x==center.x && sp.center.y == center.y && sp.center.z == center.z))
                //                {
                //                    isShadow = true;
                //                }
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
                //                if(!(sp.center.x==center.x && sp.center.y == center.y && sp.center.z == center.z))
                //                {
                //                    isShadow = true;
                //                }
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
                //                if(!(sp.center.x==center.x && sp.center.y == center.y && sp.center.z == center.z))
                //                {
                //                    isShadow = true;
                //                }
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

Vector box::getReflection(Vector ray, Vector hit_normal)
{
    ray = normalization(ray);
    hit_normal = normalization(hit_normal);
    Vector reflex = ray - hit_normal*2*(ray*hit_normal);
    return reflex;
}

//special: due to only one ambient light source in scene
Pixel box::calculateAmbientColor()
{
    //ambient color is the product of ambient light color with surface color
    //get ambient light, use pixel to store
    //printf("ambient color: %f %f %f\n",255*amr*ar,255*amg*ag,255*amb*ab);
    return Pixel(255*amr*m.ar,255*amg*m.ag,255*amb*m.ab);
}

Pixel box::calculateColorThroughRay(Point intersecP, Vector raydir, int depth)
{
    Vector normal = planes[min_plane].normal;
    normal = normalization(normal);
    Vector reflection = getReflection(raydir, normal);
    depth--;
    //printf("in shadow\n");
    //add a tiny distance to intersection point to avoid hit itself
    Point add_intersec = intersecP + reflection*0.01;
    
    Point reflect = object::evaluateRayTree(add_intersec, reflection, depth);
    Pixel reflectionColor;
    reflectionColor.SetClamp(reflect.x*m.sr,reflect.y*m.sg,reflect.z*m.sb);
    
    Pixel p = calculateDiffuseColor(intersecP)+calculateAmbientColor()+calculateSpecualrColor(intersecP, raydir)+reflectionColor;
    //printf("pixel r %d, g %d, b %d\n",p.r,p.g,p.b);
    if(isInShadow(intersecP))
    {
        
        return Pixel(0,0,0)+calculateAmbientColor()+calculateSpecualrColor(intersecP, raydir)+reflectionColor;
    }
    return p;
}

Pixel box::calculateSpecualrColor(Point intersecP, Vector raydir)
{
    float r=0,g=0,b=0;
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        Vector v = lgt.direction(intersecP);
        Vector unit_normal = planes[min_plane].normal;
        unit_normal = normalization(unit_normal);
        Vector reflection = v-2*(v*unit_normal)*unit_normal;
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
        Vector unit_normal = planes[min_plane].normal;
        unit_normal = normalization(unit_normal);
        Vector reflection = v-2*(v*unit_normal)*unit_normal;
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
        Vector unit_normal = planes[min_plane].normal;
        unit_normal = normalization(unit_normal);
        Vector reflection = v-2*(v*unit_normal)*unit_normal;
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

Pixel box::calculateDiffuseColor(Point intersecP)
{
    
    float r=0,g=0,b=0;
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        Vector v = -lgt.direction(intersecP);
        v = normalization(v);
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector unit_normal = planes[min_plane].normal;
        unit_normal = normalization(unit_normal);
        //printf("%f\n", unit_normal.x*unit_normal.x+unit_normal.y*unit_normal.y+unit_normal.z*unit_normal.z);
        //dot product of unit vector equals to cos(angle)
        float cos = v*unit_normal;
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
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector unit_normal = planes[min_plane].normal;
        unit_normal = normalization(unit_normal);
        float cos = v*unit_normal;
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
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector unit_normal = planes[min_plane].normal;
        unit_normal = normalization(unit_normal);
        //printf("%f\n", unit_normal.x*unit_normal.x+unit_normal.y*unit_normal.y+unit_normal.z*unit_normal.z);
        //dot product of unit vector equals to cos(angle)
        double cos = v*unit_normal;
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
