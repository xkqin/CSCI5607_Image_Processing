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
    //    noise = pow(fabs(((double)rand() / (double) (RAND_MAX))*range - 1) , 8);
  			Pixel p = GetPixel(x, y);
  			Pixel scaled_p;
        scaled_p.SetClamp(p.r + factor*PixelRandom().r,
        p.g+factor*PixelRandom().g,
        p.b+factor*PixelRandom().b );
         //    scaled_p.SetClamp(p.r*noise,p.g*noise,p.b*noise);
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
  int x,y;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      Pixel scaled_p =PixelQuant(p,nbits);
      GetPixel(x,y) = scaled_p;
    }
  }
}

void Image::RandomDither (int nbits)
{
	/* WORK HERE */
//   int x,y;
 float noise = -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.0)));
int l = pow(2,nbits);
double a = 256 / l;
double b = 255.0/(l - 1);
int x,y;
for (x = 0 ; x < Width() ; x++)
{
  for (y = 0 ; y < Height() ; y++)
  {
    Pixel p = GetPixel(x, y);
    p = PixelQuantN(p,nbits);
    GetPixel(x,y) = p;
  }
}
// for (x = 0 ; x < Width() ; x++)
// {
//   for (y = 0 ; y < Height() ; y++)
//   {
//     Pixel p = GetPixel(x, y);
//     p.SetClamp(floor(p.r/a + noise)*b,
//     floor(p.g/a + noise)*b,
//     floor(p.b/a + noise)*b);
//     GetPixel(x,y) = p;
//   }
// }

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
  int l = pow(2,nbits);
  double a = 256 / l;
  double b = 255.0/(l - 1);
  int x,y;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      p.SetClamp(floor(p.r / a - 0.5 + Bayer4[x%4][y%4] / 15.0)*b,
      floor(p.g / a - 0.5 + Bayer4[x%4][y%4] / 15.0)*b,
      floor(p.b / a - 0.5 + Bayer4[x%4][y%4] / 15.0)*b);
      GetPixel(x,y) = p;
    }
  }
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
  int x,y;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      Pixel scaled_p =PixelQuant(p,nbits);
      GetPixel(x, y) = scaled_p;

       GetPixel(x+1,y) + ALPHA * diff;
       GetPixel(x-1,y+1) + BETA * diff;
         GetPixel(x,y+1) + GAMMA * diff;
        GetPixel(x+1,y+1) + DELTA * diff;

    }
  }
	/* WORK HERE */
}

float gaussian(float x,float y)
{
    return 1.0/(2*3.1415926)*exp(-(x*x+y*y)/2.0);
}

void Image::Blur(int n)
{
	/* WORK HERE */
  Image* z = new Image (Width(),Height());
  for (int i = 0; i < Width(); i++)
  {
    for(int j=0; j<Height(); j++)
    {
     z->SetPixel(i,j,this->GetPixel(i,j));
    }
  }
  int x,y;
  for (x = n ; x <= Width()-n-1 ; x++)
  {
    for (y = n; y <= Height()-n-1 ; y++)
    {
         double pr=0;
         double pg=0;
         double pb=0;
      for (int a = x-n; a <=  x+n; a++)
      {
        for (int b = y-n; b <= y+n; b++)
        {
          pr += float(z -> GetPixel(a,b).r)*gaussian(x-a,y-b);
          pg += float(z -> GetPixel(a,b).g)*gaussian(x-a,y-b);
          pb += float(z -> GetPixel(a,b).b)*gaussian(x-a,y-b);
        }
      }
      Pixel p;
      if(pr < 0 || pb <0 || pg <0)
      {
        pr =0;
        pg =0;
        pb =0;
      }
      p.Set(pr,pg,pb);
      SetPixel(x,y,p);
    }
  }
}

void Image::Sharpen(int n)
{
  Image* z = new Image (Width(),Height());
  for (int i = 0; i < Width(); i++)
  {
    for(int j=0; j<Height(); j++)
    {
     z->SetPixel(i,j,this->GetPixel(i,j));
    }
  }
  z -> Blur(n);
  int x,y;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      Pixel scaled_p;
      scaled_p.SetClamp(ComponentLerp(z -> GetPixel(x,y).r,this->GetPixel(x,y).r,n),
    ComponentLerp(z -> GetPixel(x,y).g,this->GetPixel(x,y).g,n),
  ComponentLerp(z -> GetPixel(x,y).b,this->GetPixel(x,y).b,n));
  GetPixel(x,y) = scaled_p;

    }
  }

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
  Image* z = new Image (Width(),Height());
  for (int i = 0; i < Width(); i++)
  {
    for(int j=0; j<Height(); j++)
    {
     z->SetPixel(i,j,this->GetPixel(i,j));
    }
  }


  for (x = 0 ; x < Width() ; x++)
  {

    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p = GetPixel(x, y);
      Pixel scaled_p = p*0.01;
      GetPixel(x,y) = scaled_p;
    }
  }

  for (x = 1 ; x < Width()-1 ; x++)
  {

    for (y = 1 ; y < Height()-1 ; y++)
    {
         double pr=0;
         double pg=0;
         double pb=0;
      for (int a = x-1; a <=  x+1  ;a++)
      {
        for (int b = y-1; b <= y+1   ;b++)
        {
          pr += float(z -> GetPixel(a,b).r)*edge[x-a+1][y-b+1];
          pg += float(z -> GetPixel(a,b).g)*edge[x-a+1][y-b+1];
          pb += float(z -> GetPixel(a,b).b)*edge[x-a+1][y-b+1];
        }
      }
      Pixel p;
      if(pr < 0 || pb <0 || pg <0)
      {
        pr =0;
        pg =0;
        pb =0;
      }
      p.Set(pr,pg,pb);
      SetPixel(x,y,p);
    }
  }
}


Image* Image::Scale(double sx, double sy)
{
  Image* z = new Image (sx*Width(),sy*Height());
  for(int i=1 ; i < sx*Width() -1 ; i++)
  {
    for(int j = 1; j< sy*Height() - 1; j++)
    {
      //  z->GetPixel(i/sx,j/sy)
      int rx = ceil(i/sx);
      int ry = ceil(j/sy);
      int lx = floor(i/sx);
      int ly = floor(j/sy);
      // if((i/sx )/ 1 == 0)
      // {
      //     rx = i/sx + 1;
      //     lx = j/sy;
      // }
      // if((j/sy) / 1 ==0)
      // {
      //   ry = j/sy + 1;
      //   ly = j/sy;
      // }
      // if(i/sx == Width() -1 )
      // {
      //   rx = i/sx;
      //   lx = i/sx - 1;
      // }
      // if(j/sy == Height() - 1)
      // {
      //   ry = j/sy;
      //   ly = (j/sy) - 1;
      // }

    Pixel x1 = PixelLerp(GetPixel(lx,ry),GetPixel(rx,ry),(i/sx) - lx);
    Pixel x2 = PixelLerp(GetPixel(lx,ly),GetPixel(rx,ly),(i/sx) - lx);
    Pixel f = PixelLerp(x1,x2,(j/sy) - ly);
    z->SetPixel(i,j,f);

    }
  }




	/* WORK HERE */
	return z;
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
