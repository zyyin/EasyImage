#pragma 
#include "stdafx.h"
#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align)-1))


void rotateImage(Mat& img, double degree, int &w, int &h);

void CrossShader(Mat& img, Mat& dst);

void OldPicture(Mat& img, Mat& dst);

void Lomo(Mat& img, Mat& dst);

void Sketch(Mat& img, Mat& dst);

void Dehaze(Mat& img, Mat& dst);

void SoftGlow(Mat& img, Mat& dst);

void Emboss(Mat& img, Mat& dst);

void Light(Mat& img, long brightness, long contrast);

void Saturation(Mat& img, int sat);

void Hue(Mat& img, int h);

