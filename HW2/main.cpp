#include <cstdio>
#include <iostream>
#include <fstream>
#include "vector.h"
#include "image.h"
#include <cstring>
#include "sphere.h"
#include <list>

#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "stb_image_write.h"

using namespace std;

float width = 640, height = 480;
float ar = 0, ag = 0, ab = 0, dr = 1, dg = 1, db = 1, sr = 0, sg = 0, sb = 0, ns = 5, tr = 0, tg = 0, tb = 0, ior = 1;
float dir, dig, dib, dix, diy, diz;
float pir, pig, pib, pix, piy, piz;
float slr, slg, slb, slpx, slpy, slpz, sldx, sldy, sldz, sl_angle, sl_angle2;
float n;
float x, y, z, radius;
float amr, amg, amb;
float plr, plg, plb, plx, ply, plz;
float dlr, dlg, dlb, dlx, dly, dlz;
float r = 0, g = 0, b = 0;

list<sphere> SphereList;
list<pointLight> plList;
list<directionalLight> dlList;
//SphereList stores all the spheres
	float px = 0, py = 0, pz = 0, dx = 0, dy = 0, dz = 1, ux = 0, uy = 1, uz = 0, ha = 45;

	char outImage[1024] = "raytraced.bmp";
int main(int argc, const char * argv[]) {
	//parse the .scn file
  FILE *fp;
  long length;
  int i=0;
  char line[1024]; //read the file line by line, and assume no line is longer than 1024

  //examine if the input format is right
  if(argc != 2)
  {
      cout << "Usage: ./*.out scenefile\n";
      exit(0);
  }

  string fileName = argv[1];

  //open the scene file as read mode
  fp = fopen(fileName.c_str(), "r");

  //check if this file exists
  if(fp == NULL)
  {
      printf("Can't open this file %s\n",fileName.c_str());
      return 0;
  }

	// determine the file size (this is optional -- feel free to delete the 4 lines below)
	fseek(fp, 0, SEEK_END); // move position indicator to the end of the file;
	length = ftell(fp);  // return the value of the current position
	printf("File '%s' is %ld bytes long.\n\n", fileName.c_str(), length);
	fseek(fp, 0, SEEK_SET);  // move position indicator to the start of the file
							 //Loop through reading each line
	int k = 0;
	while (fgets(line, 1024, fp)) { //Assumes no line is longer than 1024 characters!
		k++;
		if (line[0] == '#') {
			printf("Skipping comment: %s", line);
			continue;
		}

		char command[100];
		int fieldsRead = sscanf(line, "%s ", command); //Read first word in the line (i.e., the command type)

		if (fieldsRead < 1) { //No command read
							  //Blank line
			continue;
		}

		if (strcmp(command, "camera") == 0) { // If the command is a camera
			sscanf(line, "camera %f %f %f %f %f %f %f %f %f %f", &px, &py, &pz, &dx, &dy, &dz, &ux, &uy, &uz, &ha);

		}

		else if (strcmp(command, "film_resolution") == 0) { //If the command is a sphere command
			sscanf(line, "film_resolution %f %f", &width, &height);
		}

		else if (strcmp(command, "material") == 0) { //If the command is a sphere command
			sscanf(line, "material %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &ar, &ag, &ab, &dr, &dg, &db, &sr, &sg, &sb, &ns, &tr, &tg, &tb, &ior);
		}

		else if (strcmp(command, "directional_light") == 0) { //If the command is a sphere command
			sscanf(line, "directional_light %f %f %f %f %f %f", &dlr, &dlg, &dlb, &dlx, &dly, &dlz);
			directionalLight dirLight = directionalLight(dlr, dlg, dlb, dlx, dly, dlz);
			dlList.push_back(dirLight);
		}

		else if (strcmp(command, "point_light") == 0) { //If the command is a sphere command
			sscanf(line, "point_light %f %f %f %f %f %f", &plr, &plg, &plb, &plx, &ply, &plz);
			pointLight light = pointLight(plr, plg, plb, plx, ply, plz);
			plList.push_back(light);
			printf("point_light %f %f %f %f %f %f\n", plr, plg, plb, plx, ply, plz);
		}

		else if (strcmp(command, "spot_light") == 0) { //If the command is a sphere command
			float r, g, b, px, py, pz, dx, dy, dz, angle1, angle2;
			sscanf(line, "spot_light %f %f %f %f %f %f %f %f %f %f %f", &r, &g, &b, &px, &py, &pz, &dx, &dy, &dz, &angle1, &angle2);
		}

		else if (strcmp(command, "ambient_light") == 0) { //If the command is a sphere command
			sscanf(line, "ambient_light %f %f %f", &amr, &amg, &amb);
			printf("ambient %f %f %f\n",amr,amg,amb);
		}

		else if (strcmp(command, "max_depth") == 0) { //If the command is a sphere command
			float n;
			sscanf(line, "max_depth %f", &n);
		}

		else if (strcmp(command, "sphere") == 0) { //If the command is a sphere command
			sscanf(line, "sphere %f %f %f %f", &x, &y, &z, &radius);
			printf("Sphere as position (%f,%f,%f) with radius %f\n", x, y, z, radius);
			sphere sphere1 = sphere(x, y, z, radius);
			sphere1.setAmbientColor(ar, ag, ab);
			sphere1.setDiffuseColor(dr, dg, db);
			sphere1.setSpecularColor(sr, sg, sb);
			sphere1.setIndexOfRefraction(ior);
			sphere1.setTransmissiveColor(tr, tg, tb);
			SphereList.push_back(sphere1);

		}
		else if (strcmp(command, "background") == 0) { //If the command is a background command
			sscanf(line, "background %f %f %f", &r, &g, &b);
			printf("Background color of (%f,%f,%f)\n", r, g, b);
		}
		else if (strcmp(command, "output_image") == 0) { //If the command is an output_image command
			sscanf(line, "output_image %s", outImage);
			printf("Render to file named: %s\n", outImage);
		}
		else {
			printf("WARNING. Do not know command: %s\n", command);
		}
	}

	Image * img = new Image(width, height);

	for (int i = 0; i<img->Width(); i++)
	{
		for (int j = 0; j<img->Height(); j++)
		{
			Pixel p = Pixel(r*255, g*255, b*255);
			img->SetPixel(i,j,p);
		}
	}

	Point origin = Point(px, py, pz);

	Vector go = Vector(dx, dy, dz);
	go = normalize(go);
	Vector top = Vector(ux, uy, uz);
	top = normalize(top);
	Vector right = crossProduct(go, top);
	right = normalize(right);



	Point leftE = origin - right * (width / 2);
	Point rightE = origin + right * (width / 2);
	Point topE = origin + top*(height / 2);
	Point bottomE = origin - top*(height / 2);
	float di = height / (2 * tan(ha*3.1415926 / 180));
    for (int i = 0; i<width; i++)
	{
		for (int j = 0; j<height; j++)
		{

			Point light = origin + (rightE- (rightE-leftE)/width*(i+0.5)) +(topE- (topE-bottomE)/height*(j+05)) +  go*di;
			Vector rayD = light - origin;
			rayD = normalize(rayD);
			float t_min = 1000000;

			for (list<sphere>::iterator next = SphereList.begin(); next != SphereList.end(); ++next)
			{
				sphere sp = *next;
				if (sp.CheckIntersection(origin, rayD))
				{
					float t = sp.calculate_t(origin, rayD);
					if ( t_min > t && t > 0) {
						t_min = t;
						Point hitPoint = origin + rayD*t_min;
						Pixel p = sp.calcuteColor(plList, dlList, amr, amg, amb, hitPoint, rayD, ns);
						img->GetPixel(i, j) = p;
					}
				}
			}
		}
	}


	img->Blur(1);
	img->Write(outImage);
}
