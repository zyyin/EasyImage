#pragma once
#include "stdafx.h"
#include "CvvImage.h"
// CImageView

class CImageView : public CStatic
{
	DECLARE_DYNAMIC(CImageView)

public:
	CImageView();
	virtual ~CImageView();
	void CalcImageShowRect(const CRect& rc, const CRect& rcImage, CRect& rcShow);
	virtual void Refresh() = 0;
	CvvImage image;
	int w, h;
	CString strFileName;
	double scale;
	CRect rcShow, rcImage;
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	
private:
};


class CThumbnail : public CImageView
{
public:
	CThumbnail() {}
	virtual ~CThumbnail(){}
	void Refresh();
};

class CHist : public CImageView
{
public:
	CHist() {}
	virtual ~CHist(){}
	void Refresh();
};
