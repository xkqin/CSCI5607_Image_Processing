 //
//  main.cpp
//  Raytracer
//
//  Created by Zheng Zhang on 10/6/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include "Object.hpp"
#include "Vertex.hpp"
#include <fstream>
#include "vector.hpp"
#include "image.h"
#include <cstring>
#include "sphere.hpp"
#include "triangle.hpp"
#include <set>
#include "AreaLight.hpp"
#include "box.hpp"

#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "stb_image_write.h"

using namespace std;

float px=0,py=0,pz=0;
float dx=0,dy=0,dz=1;
float ux=0,uy=1,uz=0;
float ha=45;

float width=640,height=480;

int supersampling_size = 5;

float ar=0,ag=0,ab=0,dr=1,dg=1,db=1,sr=0,sg=0,sb=0,ns=5,tr=0,tg=0,tb=0,ior=1;

float dir,dig,dib,dix,diy,diz;

float pir,pig,pib,pix,piy,piz;

float slr,slg,slb,slpx,slpy,slpz,sldx,sldy,sldz,sl_angle,sl_angle2;

float amr=0,amg=0,amb=0;

float vx,vy,vz;

float nx,ny,nz;

int v1,v2,v3; //three vertices index for triangle

int n1,n2,n3; //three normal index for triangle

float bp1,bp2,bp3,bl;

int max_depth=5;

float r=0,g=0,b=0;

sphere * spheres = new sphere[1000];
vertex * vertice = new vertex[1000];
Vector * normals = new Vector[1000];
triangle * triangles = new triangle[1000];
box * boxes = new box[1000];

set<pointLight> pointlightSet;
set<directionalLight> directionlightSet;
set<spotLight> spotlightSet;

char outFile[1024] = "raytraced.bmp";
Image * TextureImagePointer = NULL;
char TextureImageName[1024];

int main(int argc, const char * argv[]) {
    //parse the .scn file
    FILE *fp;
    long length;
    material * mt = new material(0,0,0,1,1,1,0,0,0,5,0,0,0,1);
    int i=0;
    int j=0;
    int triangleN = 0;
    int vertex_index = 0; //vertex_index;
    int normal_index = 0;
    int box_index = 0;
    char line[1024]; //read the file line by line, and assume no line is longer than 1024
    
    //examine if the input format is right
    if(argc != 2)
    {
        cout << "Usage: ./*.out scenefile\n";
        exit(0);
    }
    
    string filename = argv[1];
    
    //open the scene file as read mode
    fp = fopen(filename.c_str(), "r");
    
    //check if this file exists
    if(fp == NULL)
    {
        printf("Can't open this file %s\n",filename.c_str());
        return 0;
    }
    
    //get the size of this file
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    printf("File '%s' is %ld bytes long.\n\n",filename.c_str(),length);
    fseek(fp, 0, SEEK_SET);
    
    int k=0;
    //Loop through reading each line
    while( fgets(line,1024,fp) ) { //Assumes no line is longer than 1024 characters!
        k++;
        if (line[0] == '#'){
            printf("Skipping comment: %s", line);
            continue;
        }
        
        char command[100];
        int fieldsRead = sscanf(line,"%s ",command); //Read first word in the line (i.e., the command type)
        string commandStr = command;
        
        if (fieldsRead < 1){ //No command read
            //Blank line
            continue;
        }
        
        
        
        if (commandStr == "sphere"){ //If the command is a sphere command
            float x,y,z,radius; //sphere coordinate
            sscanf(line,"sphere %f %f %f %f", &x, &y, &z, &radius);
            printf("Sphere as position (%f,%f,%f) with radius %f\n",x,y,z,r);
            sphere sp = sphere(x,y,z,radius);
            sp.setDiffuseColor(dr, dg, db);
            sp.setSpecularColor(sr, sg, sb);
            sp.setAmbientColor(ar, ag, ab);
            sp.setIndexOfRefraction(ior);
            sp.setTransmissiveColor(tr, tg, tb);
            sp.setPn(ns);
            spheres[j++] = sp;
            //put this sphere into the sphereSet
            //sphereSet.insert(sp);
            //sphere::sphereSet.insert(sp);
        }
        else if (commandStr == "background"){ //If the command is a background command
            sscanf(line,"background %f %f %f", &r, &g, &b);
            printf("Background color of (%f,%f,%f)\n",r,g,b);
            object::background = Point(r*255,g*255,b*255);
        }
        else if (commandStr == "output_image"){ //If the command is an output_image command
            sscanf(line,"output_image %s", outFile);
            printf("Render to file named: %s\n", outFile);

        }
        else if(commandStr == "camera")
        {
            sscanf(line, "camera %f %f %f %f %f %f %f %f %f %f", &px, &py, &pz, &dx, &dy, &dz, &ux, &uy, &uz, &ha);
            printf("Camera posisition is %f %f %f\n",px,py,pz);
            printf("Camera direction is %f %f %f\n", dx, dy, dz);
            printf("Camera up vector is %f %f %f\n", ux, uy, uz);
            printf("One-half height angle of the viewing frustum is %f\n", ha);
        }
        else if(commandStr == "film_resolution")
        {
            sscanf(line, "film_resolution %f %f", &width, &height);
            printf("Wdith is %f, Height is %f\n",width,height);
        }
        else if(commandStr == "material")
        {
            sscanf(line, "material %f %f %f %f %f %f %f %f %f %f %f %f %f %f",&ar,&ag,&ab,&dr,&dg,&db,&sr,&sg,&sb,&ns,&tr,&tg,&tb,&ior);
            printf("material ambient color is %f %f %f\n",ar,ag,ab);
            printf("material diffuse color is %f %f %f\n",dr,dg,db);
            printf("material specular color is %f %f %f\n",sr,sg,sb);
            printf("material phong cosine power is %f\n",ns);
            printf("material transmissive color of the object is %f %f %f\n",tr,tg,tb);
            printf("material index of refraction is %f\n", ior);
            //let the m store the lastest material parameters
            mt = new material(ar,ag,ab,dr,dg,db,sr,sg,sb,ns,tr,tg,tb,ior);
        }
        else if(commandStr == "directional_light")
        {
            sscanf(line, "directional_light %f %f %f %f %f %f", &dir,&dig,&dib,&dix,&diy,&diz);
            //printf("D_light is %f %f %f %f %f %f\n", dir, dig, dib, dix, diy, diz);
            directionalLight dirLight = directionalLight(dir, dig, dib, dix, diy, diz);
            directionlightSet.insert(dirLight);
            //lightSet.insert(dirLight);
        }
        else if(commandStr == "point_light")
        {
            sscanf(line, "point_light %f %f %f %f %f %f", &pir,&pig,&pib,&pix,&piy,&piz);
            printf("P_light is %f %f %f %f %f %f\n", pir, pig, pib, pix, piy, piz);
            pointLight pLight = pointLight(pir, pig, pib, pix, piy, piz);
            pointlightSet.insert(pLight);
            
        }
        else if(commandStr == "spot_light")
        {
            sscanf(line, "spot_light %f %f %f %f %f %f %f %f %f %f %f", &slr,&slg,&slb,&slpx,&slpy,&slpz,&sldx,&sldy,&sldz,&sl_angle,&sl_angle2);
            printf("spot_light %f %f %f %f %f %f %f %f %f %f %f\n",slr,slg,slb,slpx,slpy,slpz,sldx,sldy,sldz,sl_angle,sl_angle2);
            spotLight spotlight = spotLight(slr,slg,slb,slpx,slpy,slpz,sldx,sldy,sldz,sl_angle,sl_angle2);
            spotlightSet.insert(spotlight);
        }
        else if(commandStr == "ambient_light")
        {
            //address specially, because of only one ambient light globally
            sscanf(line,"ambient_light %f %f %f", &amr, &amg, &amb);
            printf("ambient_light is %f %f %f\n",amr,amg,amb);
        }
        else if(commandStr == "max_depth")
        {
            sscanf(line,"max_depth %d", &max_depth);
            printf("max_depth: %d\n",max_depth);
        }
        else if(commandStr == "vertex")
        {
            sscanf(line,"vertex %f %f %f", &vx,&vy,&vz);
            printf("vertex is %f %f %f\n",vx,vy,vz);
            vertex v;
            v.x = vx;
            v.y = vy;
            v.z = vz;
            vertice[vertex_index++] = v;
        }
        else if(commandStr == "texturefile")
        {
            sscanf(line,"texturefile %s", TextureImageName);
            printf("texture file name is %s\n", TextureImageName);
            TextureImagePointer = new Image(TextureImageName);
        }
        else if(commandStr == "normal")
        {
            sscanf(line, "normal %f %f %f", &nx, &ny, &nz);
            printf("normal is %f %f %f\n",nx,ny,nz);
            Vector n;
            n.x = nx;
            n.y = ny;
            n.z = nz;
            normals[normal_index++] = n;
        }
        else if(commandStr == "triangle")
        {
            sscanf(line,"triangle %d %d %d", &v1, &v2, &v3);
            printf("triangle is %d %d %d\n",v1,v2,v3);
            triangle tri;
            tri.setMaterial(*mt);
            tri.setVertice(vertice[v1],vertice[v2],vertice[v3]);
            tri.setImage(TextureImagePointer);
            triangles[triangleN++] = tri;
        }
        else if(commandStr == "normal_triangle")
        {
            sscanf(line,"normal_triangle %d %d %d %d %d %d", &v1, &v2, &v3, &n1, &n2, &n3);
            printf("normal_triangle is %d %d %d %d %d %d\n", v1, v2, v3, n1, n2, n3);
            triangle tri;
            tri.setMaterial(*mt);
            tri.setVertice(vertice[v1], vertice[v2], vertice[v3]);
            tri.setNormals(normals[n1], normals[n2], normals[n3]);
            tri.setImage(TextureImagePointer);
            tri.hasNormalArray = true;
            triangles[triangleN++] = tri;
        }
        else if(commandStr == "box")
        {
            sscanf(line, "box %f %f %f %f", &bp1, &bp2, &bp3, &bl);
            printf("box is %f %f %f %f\n", bp1, bp2, bp3, bl);
            box b(Point(bp1,bp2,bp3),bl);
            b.setMaterial(*mt);
            boxes[box_index++] = b;
            
        }
        else {
            printf("WARNING. Unknow command: %s\n",command);
        }
    }
    
    printf("line number: %d\n",k);
    Image * img = new Image(width,height);
    //set up each pixel's color
    for(int i=0;i<img->Width();i++)
    {
        for(int j=0; j<img->Height(); j++)
        {
            Pixel p = Pixel(r*255,g*255,b*255);
            img->SetPixel(i, j, p);
        }
    }
    
    Point origin = Point(px,py,pz);
    
    //calculate u = d x up, v = u x d
    Vector d = Vector(dx,dy,dz); //viewing direction(forward basis)
    d = normalization(d);
    Vector up = Vector(ux,uy,uz);
    up = normalization(up);
    Vector u = crossProduct(d, up); //rightward basis
    u = normalization(u);
    Vector v = crossProduct(u, d); //upward basis
    v = normalization(v);
    
    printf("d: %f %f %f\n",d.x,d.y,d.z);
    printf("u: %f %f %f\n",u.x,u.y,u.z);
    printf("v: %f %f %f\n",v.x,v.y,v.z);
    //calculate leftedge and topedge of image
    Point leftEdge = origin - u*(width/2);
    Point rightEdge = origin + u*(width/2);
    Point topEdge = origin + v*(height/2);
    //calculate the distance between camera and image
    float distance = height/(2*tan(ha*3.1415926/180));
    
    for(int k=0; k<j; k++)
    {
        spheres[k].setAmbientLightSource(amr,amg,amb);
        spheres[k].setSpotLights(spotlightSet);
        spheres[k].setDirectionLight(directionlightSet);
        spheres[k].setPointLights(pointlightSet);
        
        object * obj = &spheres[k];
        object::objectSet.insert(obj);
    }
    
    for(int k=0; k<triangleN; k++)
    {
        triangles[k].setAmbientLightSource(amr, amg, amb);
        triangles[k].setSpotLights(spotlightSet);
        triangles[k].setDirectionLight(directionlightSet);
        triangles[k].setPointLights(pointlightSet);
        
        object * obj = &triangles[k];
        object::objectSet.insert(obj);
    }
    
    for(int k=0; k<box_index; k++)
    {
        boxes[k].setAmbientLightSource(amr, amg, amb);
        boxes[k].setSpotLights(spotlightSet);
        boxes[k].setDirectionLight(directionlightSet);
        boxes[k].setPointLights(pointlightSet);
        
        object * obj = &boxes[k];
        object::objectSet.insert(obj);
    }
    
/* box test*/
//    box b1 = box(Point(0,1.25,0),1.5);
//    b1.setAmbientLightSource(amr,amg,amb);
//    b1.setSpotLights(spotlightSet);
//    b1.setDirectionLight(directionlightSet);
//    b1.setPointLights(pointlightSet);
//    b1.setMaterial(*mt);
//    
//    object * obj = &b1;
//    object::objectSet.insert(obj);
    
    //address each pixel in image, use the jitterd supersampling
    srand((unsigned)time(NULL));
    for(int j=0;j<height;j++)
    {
        for(int i=0;i<width;i++)
        {
            Point pixel;
//            printf("i=%d,j=%d\n",i2,j2);
//            //calculate the area light
//            AreaLight areaLight = AreaLight(10,10,10);
//            areaLight.setPosition(Vector(0,5,0));
//            Vector vx = Vector(1,1,1);
//            vx = normalization(vx);
//            Vector vy = Vector(1,-2,3);
//            vy = normalization(vy);
//            Point * forlight = new Point[supersampling_size*supersampling_size];
//            Point * foreye = new Point[supersampling_size*supersampling_size];
//                
//            for(int k1=0;k1<supersampling_size*supersampling_size;k1++)
//            {
//                double rand_number1 = ((double)rand()/(double)RAND_MAX);
//                double rand_number2 = ((double)rand()/(double)RAND_MAX);
//                Point p1 = Point(rand_number1,rand_number2);
//                forlight[k1] = p1;
//                
//                double rand_number3 = ((double)rand()/(double)RAND_MAX);
//                double rand_number4 = ((double)rand()/(double)RAND_MAX);
//                Point p2 = Point(rand_number3,rand_number4);
//                foreye[k1] = p2;
//            }
//            
//            for(int n=0; n<supersampling_size*supersampling_size; n++)
//            {
//                //create a equivalent point light and put it in the new pointlightset.
//                float rvx = forlight[n].x;
//                float rvy = forlight[n].y;
//                Vector v = areaLight.getPosition()+vx*rvx+vy*rvy;
//                pointLight pl = pointLight(10,10,10,v.x,v.y,v.z);
//                set<pointLight> pointLSet;
//                pointLSet.insert(pl);
//                for(int k=0; k<j; k++)
//                {
//                    spheres[k].setPointLights(pointLSet);
//                }
//                Point s = origin+(rightEdge-u*(i2+foreye[n].x))+(topEdge-v*(j2+foreye[n].y))+d*distance;
//                Vector raydir = s-origin;
//                raydir = normalization(raydir);
//                Point color = object::evaluateRayTree(origin, raydir, max_depth);
//                pixel.x += color.x;
//                pixel.y += color.y;
//                pixel.z += color.z;
//            }
//            
//            delete [] foreye;
//            delete [] forlight;
//            pixel.x /= pow(supersampling_size,2);
//            pixel.y /= pow(supersampling_size,2);
//            pixel.z /= pow(supersampling_size,2);
//            Pixel pix;
//            pix.SetClamp(pixel.x, pixel.y, pixel.z);
//            img->SetPixel(i2, j2, pix);
    
            for(int p=0; p<supersampling_size; p++)
            {
                for(int q=0; q<supersampling_size; q++)
                {
                    double rand_number = ((double)rand()/(double)RAND_MAX);
                    //ray generation
                    //Point s = origin+(leftEdge+u*(i+0.5))+(topEdge-v*(j+0.5))+d*distance;
                    Point s = origin+(rightEdge-u*(i+(p+rand_number)/supersampling_size))+(topEdge-v*(j+(q+rand_number)/supersampling_size))+d*distance;
                    Vector raydir = s-origin;
                    raydir = normalization(raydir);
                    Point color = object::evaluateRayTree(origin, raydir, max_depth);
                    //printf("max_depth: %d\n",max_depth);
                    pixel.x += color.x;
                    pixel.y += color.y;
                    pixel.z += color.z;
                }
            }
            pixel.x /= pow(supersampling_size,2);
            pixel.y /= pow(supersampling_size,2);
            pixel.z /= pow(supersampling_size,2);
            Pixel pi;
            pi.SetClamp(pixel.x, pixel.y, pixel.z);
            img->SetPixel(i, j, pi);
          }
    }
    
    printf("image out\n");
    
    img->Write(outFile);
}
