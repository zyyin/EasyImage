#include "stdafx.h"
#include "algo.h"
#include "yuv.h"
#include "EasyImage.h"


void rotateImage(Mat& img, double degree, int &w, int &h)
{
	double angle = degree  * CV_PI / 180.;
	double a = sin(angle), b = cos(angle);   
	int width = img.cols;    
	int height = img.rows;    
	w= int(height * fabs(a) + width * fabs(b));    
	h= int(width * fabs(a) + height * fabs(b));   

	w = ALIGN_UP(w, 4);
	h = ALIGN_UP(h, 4);
	float map[6];  
	CvMat map_matrix = cvMat(2, 3, CV_32F, map);    

	CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);    
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix);    
	map[2] += (w - width) / 2;    
	map[5] += (h - height) / 2;
	warpAffine(img, img, Mat(&map_matrix), Size(w, h),
		CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,BORDER_CONSTANT, Scalar::all(0));

}


void CrossShader(Mat& img, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);
	double value;
	double r, g;
	int dim(256);  
	Mat lut(1,&dim,CV_8UC3);  
	uchar *pData = lut.ptr(0);
	for(int i = 0; i < 256; i++)  
	{  
		value=i<128?i:256-i;
		*(pData) = saturate_cast<uchar>(i/2 + 0x25);
		g = pow(value, 2)/128.0;
		*(pData+1) =  (i<128?g:256-g);
		r = pow(value, 3)/64.0/256.0;
		*(pData+2) = (i<128?r:256-r);
		pData+=3;
	}  

	LUT(img, lut, dst);
	return ;
}

void OldPicture(Mat& img, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);
	dst = img.clone();
	double r, g, b;
	for (int y = 0;y < img.rows;y++)
	{
		for (int x = 0;x < img.cols;x++)
		{
			Vec3b c = img.at<Vec3b>(y, x);
			b = c[0];
			g = c[1];
			r = c[2];   
			c[0]= saturate_cast<uchar>(0.272*r + 0.534*g + 0.131*b);
			c[1]= saturate_cast<uchar>(0.349*r + 0.686*g + 0.168*b);
			c[2]= saturate_cast<uchar>(0.393*r + 0.769*g + 0.189*b);
			dst.at<Vec3b>(y, x) = c;
		}
	}
	return;
}


void Lomo(Mat& img, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);

	Mat kernel(3,3,CV_32F,Scalar(-1));   
	kernel.at<float>(1,1) = 8.9;  
	filter2D(img,dst, img.depth(),kernel);  
	double table[256];
	double temp, xform;
	double step=1.0/256.0;
	double r;
	double value;
	for(int i = 0; i < 256; i++)
	{
		r = i/256.0;
		xform=min(1.0, max(0.0, (r-0.15)*1.53846));
		temp=min(1.0, max(0.0, (r+step-0.15)*1.53846));
		value=min(1.0, max(0.0, xform + (temp - xform) * (r - 0.5)));
		value*=256.0;
		table[i] = value;
	}
	double cx = 0.5*img.cols;
	double cy = 0.5*img.rows; 
	double inv_max_dist = 1.0/sqrt(cx*cx+cy*cy);
	double lumen;
	double dist;
	CrossShader(img, img);
	dst = img.clone();
	for (int y = 0;y < img.rows;y++)
	{
		for (int x = 0;x < img.cols;x++)
		{
			Vec3b c = img.at<Vec3b>(y, x);

			dist = sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy));
			lumen = 0.9/(1.0 + exp((dist * inv_max_dist - 0.73) * 20.0)) + 0.1;

			c[0]=saturate_cast<uchar>(table[c[0]]*lumen);
			c[1]=saturate_cast<uchar>(table[c[1]]*lumen);
			c[2]=saturate_cast<uchar>(table[c[2]]*lumen);

			dst.at<Vec3b>(y, x) = c;
		}
	}
	return;
}

void  Sketch(Mat& img, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);
	cvtColor(img, img, CV_BGR2GRAY);
	Mat tmp = img.clone();
	double minVal;
	double maxVal;
	for(int x = 2; x < img.cols; x++)
	{
		for(int y = 2; y < img.rows; y++)
		{
			Mat m(img, Rect(x-2, y-2, 3, 3));
			cv::minMaxLoc(m, &minVal, &maxVal);
			tmp.at<uchar>(y, x) = saturate_cast<uchar>(255 - (maxVal-minVal)*2);;
		}
	}
	cvtColor(tmp, dst, CV_GRAY2BGR);
}

void  Dehaze(Mat& src, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);
	const double DEHAZE_RATIO = 0.7;
	int w = src.cols;
	int h = src.rows;

	uchar* pData  = src.data;
	uchar* pDst  = dst.data;

	double minArray[3];
	double minVal;

	for(int i = 0; i < w*h*3; i+=3)
	{	
		for(int k = 0; k < 3; k++)
		{
			minArray[k] = *pData++;
		}
		minVal = min(minArray[0],min(minArray[1], minArray[2] ));

		double dval = minVal;
		dval = 255 - DEHAZE_RATIO*dval;
		dval/=255.0;
		if(dval < 0.1) dval = 0.1;
		for(int k = 0; k < 3; k++)
		{
			double ix = *pDst;
			 *pDst++ = saturate_cast<uchar>((ix - 255)/dval + 255);
		}
	}
}

void  SoftGlow(Mat& img, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);
	Mat temp = img.clone();
	cv::GaussianBlur(temp, temp, Size(21, 21), 10.0);
	addWeighted(img, 0.5, temp, 0.5, 0, dst );

}

void Emboss(Mat& img, Mat& dst)
{
	CFunctionLog fl(__FUNCTION__);
	cvtColor(img, img, CV_BGR2GRAY);
	Mat tmp = img.clone();
	for(int x = 2; x < img.cols; x++)
	{
		for(int y = 2; y < img.rows; y++)
		{
			int dif = img.at<uchar>(y, x) - img.at<uchar>(y-2, x-2);
			tmp.at<uchar>(y, x) = saturate_cast<uchar>(dif + 100);
		}
	}
	cvtColor(tmp, dst, CV_GRAY2BGR);
}

void Light(Mat& img, long brightness, long contrast)
{
	CFunctionLog fl(__FUNCTION__);
	float c=(100 + contrast)/100.0f;
	brightness+=128;

	int dim(256);  
	Mat lut(1,&dim,CV_8U);  

	for(int i = 0; i < 256; i++)  
	{  
		lut.at<uchar>(i) = saturate_cast<uchar>((i-128)*c + brightness + 0.5f);
	}  

	LUT(img, lut, img);
	return;
}

void Saturation(Mat& img, int sat)
{
	CFunctionLog fl(__FUNCTION__);
	uchar tableUV[256];
	float c=(100 + sat)/100.0f;
	for(int i = 0; i < 256; i++)
	{
		tableUV[i] = saturate_cast<uchar>((i-128)*c + 128.5f);
	}
	int w = img.cols;
	int h = img.rows;
	int size = GetYUVSize(w, h, YUV_NV12);
	uchar *pBuffer = new uchar[size];
	ImageBGR2YUV(img, pBuffer, YUV_NV12);
	int temp;
	for(int i = w*h; i < size; i++)
	{
		temp = pBuffer[i];
		pBuffer[i] = tableUV[temp];
		
	}
	
	ImageYUV2BGR(img, pBuffer, YUV_NV12);
	delete pBuffer;
}

void Hue(Mat& img, int h)
{
	CFunctionLog fl(__FUNCTION__);
	cvtColor(img, img, CV_BGR2HLS);
	for(int x = 0; x < img.cols; x++)
	{
		for(int y = 0; y < img.rows; y++)
		{
			Vec3b c = img.at<Vec3b>(y, x);
			c[0] = (c[0]+h)%256;
			img.at<Vec3b>(y, x) = c;
		}
	}
	cvtColor(img, img, CV_HLS2BGR);
}