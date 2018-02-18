// ImageView.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyImage.h"
#include "ImageView.h"
#include "memdc.h"
#include "algo.h"
// CImageView

IMPLEMENT_DYNAMIC(CImageView, CStatic)

CImageView::CImageView()
{

}

CImageView::~CImageView()
{

}


BEGIN_MESSAGE_MAP(CImageView, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CImageView::CalcImageShowRect(const CRect& rc, const CRect& rcImage, CRect& rcShow)
{
	double dw= rcImage.Width()/(1.0*rc.Width());
	double dh= rcImage.Height()/(1.0*rc.Height());
	int cx = rc.Width()/2;
	int cy = rc.Height()/2;
	rcShow = rc;
	if(dw< 1.0 && dh < 1.0)
	{
		rcShow.left = cx - rcImage.Width()/2;
		rcShow.top = cy - rcImage.Height()/2;
		rcShow.right = rcShow.left + rcImage.Width();
		rcShow.bottom = rcShow.top + rcImage.Height();
		scale = 1.0;
	}
	else 
	{
		double s = max(dw, dh);
		double fw = rcImage.Width()/s;
		double fh = rcImage.Height()/s;
		rcShow.left = cx - fw/2;
		rcShow.right = cx + fw/2;
		rcShow.top = cy - fh/2;
		rcShow.bottom = cy + fh/2;

	}
}
void CImageView::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	YMemDC memdc(GetDC(), rc);
	image.Show(memdc.m_hDC, rcShow, rcImage);
}

void CThumbnail::Refresh()
{
	w = image.cols;
	h = image.rows;
	CRect rc;
	GetClientRect(rc);
	rcImage = CRect(0, 0, w, h);
	if(!(rc.Width() > 0) || !(rc.Height() > 0))
		return;
	CalcImageShowRect(rc, rcImage, rcShow);
	if(rcShow.Width() > 0 && rcShow.Height() > 0)
		resize(theApp.pMainDlg->m_player.image, image, Size(rcShow.Width(), rcShow.Height()));
	else return;
	w = image.cols;
	h = image.rows;
	rotateImage(image, 0, w, h);
	rcImage = CRect(0, 0, w, h);
	CalcImageShowRect(rc, rcImage, rcShow);
	Invalidate(FALSE);
}
void CHist::Refresh()
{
	
	w = image.cols;
	h = image.rows;

	rotateImage(image, 0, w, h);
	CRect rc;
	GetClientRect(rc);
	rcImage = CRect(0, 0, w, h);

	CalcImageShowRect(rc, rcImage, rcShow);
	Invalidate(FALSE);
}