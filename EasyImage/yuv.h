#pragma once

enum {
	YUV_GRAY = 0,
	YUV_NV21,
	YUV_NV12,
	YUV_I420,
	YUV_YV12,
	YUV_YUYV,
	YUV_UYVY,
	YUV_422SP,
	YUV_422I,
	YUV_444,
	YUV_NUMBER,
};

#define ALIGN_DOWN(x, align) ((x) & ~((align)-1))

int GetYUVSize(int w, int h, int type);

void ImageBGR2YUV(Mat& img, PBYTE pYUV, int type);
void ImageYUV2BGR(Mat& img, PBYTE pYUV, int type);

void _YUVtoBGR(Vec3b& rgb, const Vec3b& yuv);
void _BGRtoYUV(const Vec3b& rgb, Vec3b& yuv);
