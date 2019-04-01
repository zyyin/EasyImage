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
   

	Point2f center = Point2f(width / 2, height / 2);
	Mat map_matrix = getRotationMatrix2D(center, degree, 1.0);
	//map_matrix.at<float>(0, 2) += (w - width) / 2;
	//map_matrix.at<float>(1, 3) += (h - height) / 2;

	warpAffine(img, img, map_matrix, Size(w, h),
		INTER_LINEAR | WARP_FILL_OUTLIERS,BORDER_CONSTANT, Scalar::all(0));

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
	cvtColor(img, img, COLOR_BGR2GRAY);
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
	cvtColor(tmp, dst, COLOR_GRAY2BGR);
}
bool Gamma(Mat& img, float gamma, float gain)
{
    CFunctionLog fl(__FUNCTION__);
    if (gamma <= 0.0f) return false;
    double dinvgamma = 1/gamma*gain;
    double dMax = pow(255.0, dinvgamma) / 255.0;
    int dim(256);  
    Mat lut(1,&dim,CV_8U);  
    for(int i = 0; i < 256; i++)  
    {
        lut.at<uchar>(i) = saturate_cast<uchar>( pow((double)i, dinvgamma) / dMax);
    }
    LUT(img, lut, img);
    return true;
}

void Curve(Mat& img)
{
    int w = img.cols;
    int h = img.rows;
    int size = GetYUVSize(w, h, YUV_444);
    uchar *pBuffer = new uchar[size];

    static uchar curveTable[256] = 
    {
        0,3,6,9,12,14,17,20,23,26,29,32,34,37,39,42,44,47,
        49,51,53,55,57,59,60,62,63,64,66,67,68,69,70,71,72,
        73,74,75,75,76,77,78,79,80,80,81,82,83,83,84,85,85,
        86,87,87,88,89,89,90,90,91,91,92,93,93,94,94,95,95,
        96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,
        103,104,104,105,106,106,107,107,108,108,109,110,110,111,
        112,112,113,114,114,115,116,116,117,118,118,119,120,120,
        121,122,122,123,124,124,125,126,126,127,128,128,128,128,
        128,128,128,129,130,131,132,133,134,135,136,137,138,139,
        140,141,142,143,144,145,146,147,148,149,150,151,152,153,
        154,155,156,157,158,159,160,161,162,163,164,165,166,167,
        168,169,170,171,172,173,174,175,176,177,178,179,180,181,
        182,183,184,185,186,187,188,189,190,191,192,193,194,195,
        196,197,198,199,200,201,202,203,204,205,206,207,208,209,
        210,211,212,213,214,215,216,217,218,219,220,221,222,223,
        224,225,226,227,228,229,230,231,232,233,234,235,236,237,
        238,239,240,241,242,243,244,245,246,247,248,249,250,251,
        252,253,254,255,
    };

    ImageBGR2YUV(img, pBuffer, YUV_444);
    int temp;
    for(int i = 0; i < w*h; i++)
    {
        temp = pBuffer[i];
        pBuffer[i] = curveTable[temp];

    }

    ImageYUV2BGR(img, pBuffer, YUV_444);
    delete pBuffer;
}
void  Dehaze(Mat& src, Mat& dst)
{
    CFunctionLog fl(__FUNCTION__);

    Scalar avg = mean(src);
    double y=0.3*avg.val[2]+0.59*avg.val[1]+0.11*avg.val[0];
    y = min(150., max(20., y));
    LOGE(L"y=%f\n", y);
    const uchar LOW_THREASHOLD = 128;
    const double DEHAZE_RATIO = (y-20)*0.005;
    const double nGamma = 1.857-0.00615*y;
    int w = src.cols;
    int h = src.rows;

    uchar* pData  = src.data;
    uchar* pDst  = dst.data;

    double minArray[3];
    double minVal;

    Gamma(dst,  nGamma,  1.0);
    if(DEHAZE_RATIO > 0.0)
    {

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
	cvtColor(img, img, COLOR_BGR2GRAY);
	Mat tmp = img.clone();
	for(int x = 2; x < img.cols; x++)
	{
		for(int y = 2; y < img.rows; y++)
		{
			int dif = img.at<uchar>(y, x) - img.at<uchar>(y-2, x-2);
			tmp.at<uchar>(y, x) = saturate_cast<uchar>(dif + 100);
		}
	}
	cvtColor(tmp, dst, COLOR_GRAY2BGR);
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
	cvtColor(img, img, COLOR_BGR2HLS);
	for(int x = 0; x < img.cols; x++)
	{
		for(int y = 0; y < img.rows; y++)
		{
			Vec3b c = img.at<Vec3b>(y, x);
			c[0] = (c[0]+h)%256;
			img.at<Vec3b>(y, x) = c;
		}
	}
	cvtColor(img, img, COLOR_HLS2BGR);
}