#include "StdAfx.h"
#include "CvvImage.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void  FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp)
{
   assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

   BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

   memset( bmih, 0, sizeof(*bmih));
   bmih->biSize = sizeof(BITMAPINFOHEADER);
   bmih->biWidth = width;
   bmih->biHeight =  abs(height);
   bmih->biPlanes = 1;
   bmih->biBitCount = (unsigned short)bpp;
   bmih->biCompression = BI_RGB;
   if( bpp == 8 )
   {
      RGBQUAD* palette = bmi->bmiColors;
      int i;
      for( i = 0; i < 256; i++ )
      {
         palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
         palette[i].rgbReserved = 0;
      }
   }
}
CvvImage::CvvImage()
{
  
}

CvvImage::~CvvImage()
{
}
bool  CvvImage::Create( int w, int h, int bpp )
{
   const unsigned max_img_size = 10000;

   if( (bpp != 8 && bpp != 24 && bpp != 32) ||
      (unsigned)w >=  max_img_size || (unsigned)h >= max_img_size)
   {
      assert(0); // most probably, it is a programming error
      return false;
   }
   if( Bpp() != bpp || Width() != w || Height() != h )
   {
	  create(h, w, CV_MAKETYPE(CV_8U, bpp/8));
   }
  
   return !empty();
}


void  CvvImage::CopyOf( Mat& img, int desired_color )
{
	if( !img.empty() )
	{
      int color = desired_color;
      Size size = img.size(); 
      if( color < 0 )
         color = img.channels() > 1;
      if( Create( size.width, size.height,
         (!color ? 1 : img.channels() > 1 ? img.channels() : 3)*8))
      {
			img.convertTo(*this, 0);
      }
   }
}

void  CvvImage::Show( const char* window )
{
   imshow(window, *this);
}
void  CvvImage::Show( HDC dc, LPRECT rcShow, LPRECT rcImage)
{
   if( depth() == CV_8U )
   {
      uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
      BITMAPINFO* bmi = (BITMAPINFO*)buffer;
      int bmp_w = Width(), bmp_h = Height();
      FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp());
    
	  SetStretchBltMode(dc, HALFTONE);
	  ::StretchDIBits(dc, rcShow->left, rcShow->top, rcShow->right-rcShow->left, rcShow->bottom-rcShow->top,
				rcImage->left, rcImage->bottom, rcImage->right-rcImage->left, -(rcImage->bottom-rcImage->top),
				data,
				bmi, DIB_RGB_COLORS, SRCCOPY);
 
   }
}


CvvImage::CvvImage(Mat& mat)
{
	CopyOf(mat);
}