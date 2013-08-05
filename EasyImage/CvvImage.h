#pragma once
#ifndef CVVIMAGE_CLASS_DEF
#define CVVIMAGE_CLASS_DEF
#include "opencv2/opencv.hpp"


class  CvvImage : public Mat
{
public:
   CvvImage();
   virtual ~CvvImage();
   CvvImage(Mat& mat);
 
   virtual bool  Create( int width, int height, int bits_per_pixel );

   virtual void  CopyOf( Mat& image, int desired_color = -1 );
   int Width() { return cols; };
   int Height() { return rows;};
   int Bpp() { 
	   return ((8<<((1<<depth()/2)>>1)) )*channels(); };
   
   virtual void  Show( const char* window );

#if defined WIN32 || defined _WIN32
 
   virtual void  Show( HDC dc, LPRECT rcShow, LPRECT rcImage);
#endif

};
typedef CvvImage CImage;
#endif
