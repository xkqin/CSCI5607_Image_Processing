//
//  sphere.cpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/7/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include "sphere.hpp"

Pixel sphere::calculateColorThroughRay(Point intersecP, Vector raydir, int depth)
{
    Vector normal = intersecP - center;
    normal = normalization(normal);
    Vector reflection = getReflection(raydir, normal);
    depth--;
    //printf("in shadow\n");
    //add a tiny distance to intersection point to avoid hit itself
    Point add_intersec = intersecP + reflection*0.01;
    
    Point reflect = object::evaluateRayTree(add_intersec, reflection, depth);
    Pixel reflectionColor;
    reflectionColor.SetClamp(reflect.x*sr,reflect.y*sg,reflect.z*sb);
    
    ray refraction_ray;
    //only if it isn't the total inner refraction, calculate the refraction color
    refraction_ray = calculateRefractionOutPoint(intersecP, raydir);
    Point refraction = object::evaluateRayTree(refraction_ray.origin, refraction_ray.direction, depth);
    Pixel refractionColor;
    refractionColor.SetClamp(refraction.x*tr, refraction.y*tg, refraction.z*tb);
    //printf("refraction color: %d %d %d\n",refractionColor.r,refractionColor.g,refractionColor.b);
    
    Pixel p = calculateDiffuseColor(intersecP)+calculateAmbientColor()+calculateSpecualrColor(intersecP, raydir)+reflectionColor+refractionColor;
    //printf("pixel r %d, g %d, b %d\n",p.r,p.g,p.b);
    if(isInShadow(intersecP))
    {

        return Pixel(0,0,0)+calculateAmbientColor()+calculateSpecualrColor(intersecP, raydir)+reflectionColor+refractionColor;
    }
    return p;
}

//return the eyepoint of the outward refraction ray
ray sphere::calculateRefractionOutPoint(Point intersecP, Vector raydir)
{
//    //assume the air refractive index(n) is 1.00
      Vector normal = intersecP - center;
      normal = normalization(normal);
      raydir = normalization(raydir);
      Point p = intersecP;
//    //Vector t_part1 = (raydir-normal*(raydir*normal))*(1/ior);
//    Vector t_part1 = (raydir-normal*(raydir*normal))*(1/ior);
//    Vector t_part2 = normal*sqrt(1-(1-pow(raydir*normal,2))/pow(ior, 2));
//    Vector inner_refration = t_part1-t_part2;
//    inner_refration = normalization(inner_refration);
//    //create ray starting in intersection point in direction of inner_refraction (add a tiny distance to avoid hitting itself)
//    intersecP = intersecP + inner_refration*0.01;
//    
//    Point intersecP_innerRayiwthSphere;
//    
//    if(isIntersectionWith(intersecP, inner_refration))
//    {
//        float t = 0;
//        float b = inner_refration*(intersecP-center);
//        float a = inner_refration*inner_refration;
//        float c = (intersecP-center)*(intersecP-center)-sradius*sradius;
//        
//        float t1 = (-b+sqrt(b*b-a*c))/a;
//        float t2 = (-b-sqrt(b*b-a*c))/a;
//        
//        //printf("t1=%f,t2=%f\n",t1,t2);
//        if(t1>0)
//        {
//            t = t1;
//        }
//        else{
//            t = t2;
//        }
//        //printf("t = %f\n",t);
//        intersecP_innerRayiwthSphere = intersecP+inner_refration*t;
//    }
//    
//    //calculate the outgoing direction
//    Vector exit_normal = center-intersecP_innerRayiwthSphere;
//    exit_normal = normalization(exit_normal);
//    Vector exit_t_part1 = (ior)*(inner_refration-exit_normal*(inner_refration*exit_normal));
//    Vector exit_t_part2 = exit_normal*sqrt(1-(1-pow(inner_refration*exit_normal, 2))*pow(ior, 2));
//    Vector exit_direction = exit_t_part1-exit_t_part2;
//    exit_direction = normalization(exit_direction);
//    ray exitRay;
//    exitRay.origin = intersecP_innerRayiwthSphere;
//    exitRay.direction = exit_direction;
//    return exitRay;
    
    // circumstance: ray enter the sphere
    double cos_in = -(raydir*normal);
    double sin_in = sqrt(1-cos_in*cos_in);
    double cos_out = sqrt(1-(1-(cos_in*cos_in))/pow(ior,2));
    double sin_out = sqrt(1-cos_out*cos_out);
    
    Vector base = (raydir+normal*cos_in)*(1/sin_in);
    Vector inner_refraction = base*sin_out-normal*cos_out;
    inner_refraction = normalization(inner_refraction);
    p = intersecP+inner_refraction*0.01;
    Point intersecP_innerRayiwthSphere;
        if(isIntersectionWith(p, inner_refraction))
        {
            float t = 0;
            float b = inner_refraction*(p-center);
            float a = inner_refraction*inner_refraction;
            float c = (p-center)*(p-center)-sradius*sradius;
    
            float t1 = (-b+sqrt(b*b-a*c))/a;
            float t2 = (-b-sqrt(b*b-a*c))/a;
    
            //printf("t1=%f,t2=%f\n",t1,t2);
            if(t1>t2)
            {
                t = t1;
            }
            else{
                t = t2;
            }
            //printf("t = %f\n",t);
            intersecP_innerRayiwthSphere = p+inner_refraction*t;
        }
    Vector normalOut = center-intersecP_innerRayiwthSphere;
    normalOut = normalization(normalOut);
    double cos_2_in = -(inner_refraction*normalOut);
    double sin_2_in = sqrt(1-cos_2_in*cos_2_in);
    double cos_2_out = sqrt(1-pow(ior, 2)*(1-(cos_2_in*cos_2_in)));
    double sin_2_out = sqrt(1-cos_2_out*cos_2_out);
    
    Vector base2 = (inner_refraction+normalOut*cos_2_in)*(1/sin_2_in);
    Vector out_direction = base2*sin_2_out-normalOut*cos_2_out;
    out_direction = normalization(out_direction);
    intersecP_innerRayiwthSphere = intersecP_innerRayiwthSphere + out_direction*0.01;
    ray refraction_ray;
    refraction_ray.origin = intersecP_innerRayiwthSphere;
    refraction_ray.direction = out_direction;
    return refraction_ray;
}

float sphere::length_of_t(Point origin, Vector raydir)
{
    if(!isIntersectionWith(origin, raydir))
    {
        printf("determint < 0\n");
        return -9999;
    }
    
    float b = raydir*(origin-center);
    float a = raydir*raydir;
    float c = (origin-center)*(origin-center)-sradius*sradius;
    
    float t1 = (-b+sqrt(b*b-a*c))/a;
    float t2 = (-b-sqrt(b*b-a*c))/a;
    
    
    
    if(fabs(t1)<fabs(t2))
    {
        return t1;
    }
    else{
        return t2;
    }
}


bool sphere::isIntersectionWith(Point origin, Vector raydir)
{
    float b = raydir*(origin-center);
    float a = raydir*raydir;
    float c = (origin-center)*(origin-center)-sradius*sradius;
    float determinant = b*b-a*c;
    if(determinant>0)
    {
        return true;
    }
    else{
        return false;
    }
}

bool sphere::isInShadow(Point intersecP)
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

Vector sphere::getReflection(Vector ray, Vector hit_normal)
{
    ray = normalization(ray);
    hit_normal = normalization(hit_normal);
    Vector reflex = ray - hit_normal*2*(ray*hit_normal);
    return reflex;
}

//special: due to only one ambient light source in scene
Pixel sphere::calculateAmbientColor()
{
    //ambient color is the product of ambient light color with surface color
    //get ambient light, use pixel to store
    //printf("ambient color: %f %f %f\n",255*amr*ar,255*amg*ag,255*amb*ab);
    return Pixel(255*amr*ar,255*amg*ag,255*amb*ab);
}

Pixel sphere::calculateSpecualrColor(Point intersecP, Vector raydir)
{
    float r=0,g=0,b=0;
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        Vector v = lgt.direction(intersecP);
        Vector unit_normal = intersecP-center;
        unit_normal = normalization(unit_normal);
        Vector reflection = v-2*(v*unit_normal)*unit_normal;
        reflection = normalization(reflection);
        raydir = normalization(raydir);
        float cos = reflection*(-raydir);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += sr*lightIntensity.x*pow(fmax(cos, 0),pn);
        g += sg*lightIntensity.y*pow(fmax(cos, 0),pn);
        b += sb*lightIntensity.z*pow(fmax(cos, 0),pn);
    }
    
    for(set<directionalLight>::iterator it = dirlightSet.begin(); it != dirlightSet.end(); ++it)
    {
        directionalLight lgt = *it;
        Vector v = lgt.direction(intersecP);
        Vector unit_normal = intersecP-center;
        unit_normal = normalization(unit_normal);
        Vector reflection = v-2*(v*unit_normal)*unit_normal;
        reflection = normalization(reflection);
        raydir = normalization(raydir);
        float cos = reflection*(-raydir);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += sr*lightIntensity.x*pow(fmax(cos, 0),pn);
        g += sg*lightIntensity.y*pow(fmax(cos, 0),pn);
        b += sb*lightIntensity.z*pow(fmax(cos, 0),pn);
        //printf("r: %f, g: %f, b: %f\n",r,g,b);
    }
    
    for(set<spotLight>::iterator it = spotlightSet.begin(); it != spotlightSet.end(); ++it)
    {
        spotLight lgt = *it;
        Vector v = lgt.direction();
        Vector unit_normal = intersecP-center;
        unit_normal = normalization(unit_normal);
        Vector reflection = v-2*(v*unit_normal)*unit_normal;
        raydir = normalization(raydir);
        float cos = reflection*(-raydir);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += sr*lightIntensity.x*pow(fmax(cos, 0),pn);
        g += sg*lightIntensity.y*pow(fmax(cos, 0),pn);
        b += sb*lightIntensity.z*pow(fmax(cos, 0),pn);
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

Pixel sphere::calculateDiffuseColor(Point intersecP)
{
    
    float r=0,g=0,b=0;
    for(set<pointLight>::iterator it = pointlightSet.begin(); it != pointlightSet.end(); ++it)
    {
        pointLight lgt = *it;
        Vector v = -lgt.direction(intersecP);
        v = normalization(v);
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector unit_normal = intersecP-center;
        unit_normal = normalization(unit_normal);
        //printf("%f\n", unit_normal.x*unit_normal.x+unit_normal.y*unit_normal.y+unit_normal.z*unit_normal.z);
        //dot product of unit vector equals to cos(angle)
        float cos = v*unit_normal;
        //we calculate the sine to check if it > 0
        //printf("cos = %f, angle = %f\n",cos);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += dr*lightIntensity.x*fmax(cos,0);
        g += dg*lightIntensity.y*fmax(cos, 0);
        b += db*lightIntensity.z*fmax(cos, 0);
    }
    
    for(set<directionalLight>::iterator it = dirlightSet.begin(); it != dirlightSet.end(); ++it)
    {
        directionalLight lgt = *it;
        Vector v = -lgt.direction(intersecP);
        v = normalization(v);
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector unit_normal = intersecP-center;
        unit_normal = normalization(unit_normal);
        float cos = v*unit_normal;
        //printf("cos = %f\n",cos);
        Point lightIntensity = lgt.getLightColor(intersecP);
        r += dr*lightIntensity.x*fmax(cos,0);
        g += dg*lightIntensity.y*fmax(cos, 0);
        b += db*lightIntensity.z*fmax(cos, 0);
    }
    
    for(set<spotLight>::iterator it = spotlightSet.begin(); it != spotlightSet.end(); ++it)
    {
        spotLight lgt = *it;
        Vector v = -lgt.direction();
        v = normalization(v);
        //printf("v: x=%f y=%f z=%f\n",v.x,v.y,v.z);
        //get the unit normal at the intersection point: 2(u-c)/r
        Vector unit_normal = intersecP-center;
        unit_normal = normalization(unit_normal);
        //printf("%f\n", unit_normal.x*unit_normal.x+unit_normal.y*unit_normal.y+unit_normal.z*unit_normal.z);
        //dot product of unit vector equals to cos(angle)
        double cos = v*unit_normal;
        //printf("cos = %f\n",cos);
        Point lightIntensity = lgt.getLightColor(intersecP);
        Vector cross = crossProduct(v, unit_normal);
        r += dr*lightIntensity.x*fmax(cos,0);
        g += dg*lightIntensity.y*fmax(cos, 0);
        b += db*lightIntensity.z*fmax(cos, 0);
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

void sphere::setPointLights(set<pointLight> lights)
{
    pointlightSet = lights;
}

void sphere::setDirectionLight(set<directionalLight> lights)
{
    dirlightSet = lights;
}

void sphere::setSpotLights(set<spotLight> spotset)
{
    spotlightSet = spotset;
}

float sphere::getX() const
{
    return sx;
}

float sphere::getY() const
{
    return sy;
}

float sphere::getZ() const
{
    return sz;
}

float sphere::getR() const
{
    return sradius;
}

bool operator< (const sphere &p, const sphere &q)
{
    return 1;
}

void sphere::setDiffuseColor(float dr_, float dg_, float db_)
{
    dr = dr_;
    dg = dg_;
    db = db_;
}

void sphere::setAmbientColor(float ar_, float ag_, float ab_)
{
    ar = ar_;
    ag = ag_;
    ab = ab_;
}

void sphere::setSpecularColor(float sr_, float sg_, float sb_)
{
    sr = sr_;
    sg = sg_;
    sb = sb_;
}

void sphere::setPn(float p)
{
    pn = p;
}

void sphere::setTransmissiveColor(float tr_, float tg_, float tb_)
{
    tr = tr_;
    tg = tg_;
    tb = tb_;
}

void sphere::setIndexOfRefraction(float ior_)
{
    ior = ior_;
}

void sphere::setAmbientLightSource(float r, float g, float b)
{
    amr = r;
    amg = g;
    amb = b;
}
