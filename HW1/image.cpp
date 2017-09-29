#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <iostream>

/**
 * Image
 **/
Image::Image (int width_, int height_){

    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels*4];
	int b = 0; //which byte to write to
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
		}
	}

    assert(data.raw != NULL);
}

Image::Image (const Image& src){

	width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels*4];

    //memcpy(data.raw, src.data.raw, num_pixels);
    *data.raw = *src.data.raw;
}

Image::Image (char* fname){

	int numComponents; //(e.g., Y, YA, RGB, or RGBA)
	data.raw = stbi_load(fname, &width, &height, &numComponents, 4);

	if (data.raw == NULL){
		printf("Error loading image: %s", fname);
		exit(-1);
	}


	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;

}

Image::~Image (){
    delete data.raw;
    data.raw = NULL;
}

void Image::Write(char* fname){

	int lastc = strlen(fname);

	switch (fname[lastc-1]){
	   case 'g': //jpeg (or jpg) or png
	     if (fname[lastc-2] == 'p' || fname[lastc-2] == 'e') //jpeg or jpg
	        stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
	     else //png
	        stbi_write_png(fname, width, height, 4, data.raw, width*4);
	     break;
	   case 'a': //tga (targa)
	     stbi_write_tga(fname, width, height, 4, data.raw);
	     break;
	   case 'p': //bmp
	   default:
	     stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}

void Image::AddNoise (double factor)
{
  double range = factor;
  double noise;
    int x,y;
  	for (x = 0 ; x < Width() ; x++)
  	{
  		for (y = 0 ; y < Height() ; y++)
  		{
        noise = pow(fabs(((double)rand() / (double) (RAND_MAX))*range - 1) , 8);
  			Pixel p = GetPixel(x, y);
  			Pixel scaled_p;
        scaled_p.SetClamp(p.r*noise,p.g*noise,p.b*noise);
  			GetPixel(x,y) = scaled_p;
  		}
  	}
}

void Image::Brighten (double factor)
{
	int x,y;
	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
			Pixel scaled_p = p*factor;
			GetPixel(x,y) = scaled_p;
		}
	}
}


void Image::ChangeContrast (double factor)
{
	/* WORK HERE */
  int x,y;
  long double sum = 0, mean = 0;

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      sum = sum + p.Luminance();
    }
  }
  mean = sum / num_pixels;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
			Pixel scaled_p;
      // mean = p.Luminance();
      // mean+(p.r-mean)*(factor)
      scaled_p.SetClamp(mean+(p.r-mean)*(factor),mean+(p.g-mean)*(factor),mean+(p.b-mean)*(factor));
			GetPixel(x,y) = scaled_p;
    }
  }

}


void Image::ChangeSaturation(double factor)
{
	/* WORK HERE */
    int x,y;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      double a;
      Pixel p = GetPixel(x, y);
      Pixel scaled_p;
      a = sqrt(p.r * p.r *0.299 + p.g*p.g*0.587+p.b*p.b*0.144);
      scaled_p.SetClamp(a+(p.r -a)*factor,a+(p.g -a)*factor,a+(p.b -a)*factor );
      GetPixel(x,y) = scaled_p;
    }
  }
}


Image* Image::Crop(int x, int y, int w, int h)
{
	/* WORK HERE */
  Image* i = new Image(w,h);

  int a,b;
  for (a = 0 ; a < w ; a++)
  {
    for (b = 0 ; b < h ;b++)
    {
      Pixel p = GetPixel(x+a,y+b);
      i->SetPixel(a,b,p);
    }
  }
  // int a,b;
  // for (a = x ; a < i->Width() ; a = a + ((Width())/(i->Width())))
  // {
  //   for (b = y ; b < i->Height() ; b = b + ((Height())/(i->Height())))
  //   {
  //     Pixel p = GetPixel((a-x)*(i->Width()), (b-y)*(i->Height()));
  //     i->SetPixel(a,b,p);
  //   }
  // }
	return i;
}


void Image::ExtractChannel(int channel)
{
	/* WORK HERE */
  int x,y;
	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
			Pixel scaled_p;
      if(channel == 1)
      {
      scaled_p.SetClamp(p.r,0,0);
    }
    if(channel == 2)
    {
            scaled_p.SetClamp(0,p.g,0);
    }
    if(channel == 3)
    {
            scaled_p.SetClamp(0,0,p.b);
    }
			GetPixel(x,y) = scaled_p;
		}
	}
}



void Image::Quantize (int nbits)
{
	/* WORK HERE */
}

void Image::RandomDither (int nbits)
{
	/* WORK HERE */
}


static int Bayer4[4][4] =
{
    {15,  7, 13,  5},
    { 3, 11,  1,  9},
    {12,  4, 14,  6},
    { 0,  8,  2, 10}
};


void Image::OrderedDither(int nbits)
{
	/* WORK HERE */
}

/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
	/* WORK HERE */
}

void Image::Blur(int n)
{
	/* WORK HERE */
  int x,y;
  int r = n/2;
  double sigma = 1 / (6*(pow(r,2))+4*r+1) ;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      Pixel scaled_p;
      this -> SetPixel(pow(2, -(pow ((x/sigma),2))),pow(2, -(pow ((x/sigma),2))),scaled_p);
      // GetPixel(x,y) =  scaled_p;
    }
  }
}

void Image::Sharpen(int n)
{
  // if(n >= -1 || n <= 1)
  // {
  //   int x,y;
  //   for (x = 0 ; x < Width() ; x++)
  //   {
  //     for (y = 0 ; y < Height() ; y++)
  //     {
  //       Pixel p = GetPixel(x, y);
  //       Pixel scaled_p;
  //       scaled_p.SetClamp(0.5*((-3)*pow((1-fabs(p.r)), 3)+4*(pow(1-fabs(p.r),2)) + (1 - fabs(p.r))),
  //       0.5*((-3)*pow((1-fabs(p.g)), 3)+4*(pow(1-fabs(p.g),2)) + (1 - fabs(p.g))),
  //       0.5*((-3)*pow((1-fabs(p.b)), 3)+4*(pow(1-fabs(p.b),2)) + (1 - fabs(p.b))) );
  //       GetPixel(x,y) = scaled_p;
  //     }
  //   }
  // }
	/* WORK HERE */
}

static int edge[3][3] =
{
    {-1,  -1, -1},
    {-1,  8,  -1},
    {-1,  -1, -1}
};
void Image::EdgeDetect()
{
	/* WORK HERE */
  int x,y;
  long double sum1 = 0, sum2 = 0, sum3 = 0;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      for (int a = 0; a < 3 ;a++)
      {
        for (int b = 0; b < 3 ;b++)
        {
      Pixel p = GetPixel(a, b);
      sum1 = sum1 + (p.r * edge[x-a][y-b]);
      sum2 = sum2 + (p.g * edge[x-a][y-b]);
      sum3 = sum3 + (p.b * edge[x-a][y-b]);
      Pixel scaled_p;
      scaled_p.SetClamp(sum1,sum2,sum3);
      GetPixel(a,b) = scaled_p;
        }
      }
      // Pixel p = GetPixel(x, y);

    }
  }
  // int q,w;
  // for (q = 0 ; q < Width() ; q++)
  // {
  //   for (w = 0 ; w < Height() ; w++)
  //   {
  //     Pixel p = GetPixel(q, w);
  //     Pixel scaled_p;
  //     scaled_p.SetClamp(sum1,sum2,sum3);
  //     GetPixel(q,w) = scaled_p;
  //   }
  // }
}


Image* Image::Scale(double sx, double sy)
{
	/* WORK HERE */
	return NULL;
}

Image* Image::Rotate(double angle)
{
	/* WORK HERE */
	return NULL;
}

void Image::Fun()
{
	/* WORK HERE */
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}


Pixel Image::Sample (double u, double v){
    /* WORK HERE */
	return Pixel();
}
