#include "StdAfx.h"
#include "EasyImage.h"
#include "ImagePlayer.h"
#include "algo.h"
#include "yuv.h"
CImagePlayer::CImagePlayer(void)
{
	historyInddex = 0;
}

CImagePlayer::~CImagePlayer(void)
{

}

BEGIN_MESSAGE_MAP(CImagePlayer, CImageView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CImagePlayer::Load(LPCTSTR strFileName)
{
	CFunctionLog fl(__FUNCTION__);
	USES_CONVERSION;
	Clear();
	image = imread(T2A(strFileName));
	if(image.empty())
		return;
	Restore();
	Refresh();
	RefreshThumb();

}
void CImagePlayer::Save(LPCTSTR strFileName)
{
	CFunctionLog fl(__FUNCTION__);
	USES_CONVERSION;
	if(image.empty())
		return;
	imwrite(T2A(strFileName), image);
}
void CImagePlayer::Clear()
{
	m_history.clear();
	historyInddex = 0;
	w = h = 0;
	scale = 1.0;
	image.release();
	bDrag = false;
}
BOOL CImagePlayer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	ScreenToClient(&pt);

	if(zDelta > 0)
		scale += 0.05;
	else 
		scale -= 0.05;
	if(scale < 1.0)
		scale = 1.0;

	Scale( pt);
	return TRUE;
}

void CImagePlayer::Scale(CPoint pt)
{
	if(scale == 1.0)
	{
		rcImage = CRect(0, 0, w, h);
	}
	else
	{
		ScreenToImage(pt);
		double dw = w/scale;
		double dh = h/scale;
		double ds = dw/rcImage.Width();
		rcImage.left = pt.x - ds*(pt.x-rcImage.left);
		rcImage.right = rcImage.left + dw;
		rcImage.top = pt.y - ds*(pt.y-rcImage.top);
		rcImage.bottom = rcImage.top + dh;
		if(rcImage.left < 0) rcImage.left = 0;
		if(rcImage.top < 0) rcImage.top = 0;
		if(rcImage.bottom > h) rcImage.bottom = h;
		if(rcImage.right > w) rcImage.right = w;
	}
	SendScaleInfo();
	Invalidate(FALSE);

}
void CImagePlayer::SendScaleInfo()
{
	CString str;
	double s = max(1.0*w/rcShow.Width(), 1.0*h/rcShow.Height());
	str.Format(L"Scale: %.2f", scale/s);
	theApp.pMainDlg->scaleInfo.SetText(str);
}
void CImagePlayer::ScreenToImage(CPoint& pt)
{
	CPoint ret;
	ret.x = rcImage.left + rcImage.Width()*(pt.x-rcShow.left)/rcShow.Width();
	ret.y = rcImage.top + rcImage.Height()*(pt.y-rcShow.top)/rcShow.Height();
	pt = ret;
}

void CImagePlayer::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	bDrag = 1;
	startPoint = point;
}
void CImagePlayer::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	bDrag = 0;
}
void CImagePlayer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(scale != 1.0)
		scale = 1.0;
	else
	{
		double s = max(1.0*w/rcShow.Width(), 1.0*h/rcShow.Height());
		scale = s;
	}
	Scale(point);
}
void CImagePlayer::OnMouseMove(UINT nFlags, CPoint point)
{
	if(image.empty() || !PtInRect(rcShow, point))
		return; 
	CPoint endPoint = point;
	ScreenToImage(point);
	if(!bDrag)
	{
		CString str;
		str.Format(L"Coord: %d, %d", point.x, point.y);
		theApp.pMainDlg->xyPos.SetText(str);
		Vec3b rgb = image.at<Vec3b>( point.y, point.x);
		str.Format(L"RGB: %d, %d, %d", rgb[2], rgb[1], rgb[0]);
		theApp.pMainDlg->textRGB.SetText(str);

		Vec3b yuv;
		_BGRtoYUV(rgb, yuv);
		str.Format(L"YUV: %d, %d, %d", yuv[0], yuv[1], yuv[2]);
		theApp.pMainDlg->textYUV.SetText(str);
	}
	else
	{
		int dx = endPoint.x-startPoint.x;
		int dy = endPoint .y - startPoint.y;
		dx = -dx/scale;
		dy = -dy/scale;
		if(    ( rcImage.left + dx < 0   )
			|| ( rcImage.right + dx > w  )
			|| ( rcImage.top + dy < 0    ) 
			|| ( rcImage.bottom + dy > h ) )
			return;
		rcImage.left	+= dx;
		rcImage.right	+= dx;
		rcImage.top		+= dy;
		rcImage.bottom  += dy;
		startPoint = endPoint;
		Invalidate(FALSE);
	}
}

void CImagePlayer::RefreshThumb()
{
	if(image.empty())
		return;
	if(!theApp.pMainDlg)
		return;
	if(!theApp.pMainDlg->m_thumb.GetSafeHwnd())
		return;

	theApp.pMainDlg->m_thumb.image = image.clone();
	theApp.pMainDlg->m_thumb.Refresh();

}
void CImagePlayer::	Refresh()
{
	if(image.empty())
		return;
	CRect rc;
	GetClientRect(rc);
	w = image.cols;
	h = image.rows;
	rotateImage(image, 0, w, h);
	rcImage = CRect(0, 0, w, h);
	CalcImageShowRect(rc, rcImage, rcShow);

	SendScaleInfo();
	CString str;
	str.Format(L"Size: %d, %d", w, h);
	theApp.pMainDlg->imageInfo.SetText(str);
	RefreshHist();
	Invalidate(FALSE);
}
void CImagePlayer::RefreshHist()
{
	if(image.empty())
		return;
	Mat bgr_planes[3];
	split( image, bgr_planes );

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(48,48,48) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
			Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
			Scalar( 255, 0, 0), 1, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
			Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
			Scalar( 0, 255, 0), 1, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
			Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
			Scalar( 0, 0, 255), 1, 8, 0  );
	}

	/// Display
	theApp.pMainDlg->m_hist.image = histImage;
	theApp.pMainDlg->m_hist.Refresh();

}

void CImagePlayer::Restore()
{
	if(m_history.size()  == 10)
	{
		m_history.erase(m_history.begin());
		historyInddex= 9;
	}

	while(m_history.size() > historyInddex)
	{
		m_history.pop_back();
	}
	m_history.push_back(image);
	historyInddex ++;

}
void CImagePlayer::HistoryPre()
{
	if(historyInddex < 2)
		return;
	historyInddex--;
	image = m_history[historyInddex-1].clone();
	Refresh();
	RefreshThumb();
}
void CImagePlayer::HistoryNext()
{
	if(historyInddex >= m_history.size())
		return;
	image = m_history[historyInddex++].clone();
	Refresh();
	RefreshThumb();
}

void CImagePlayer::Process(int id)
{
	if(image.empty())
		return;
	Mat dst = image.clone();

	switch(id)
	{
	case ID_MIRROR:
		flip(image, dst, 1);
		break;
	case ID_FLIP:
		flip(image, dst, 0);
		break;
	case ID_ROTATE90:
		rotateImage(dst, 90, w, h);
		break;
	case ID_ROTATE270:
		rotateImage(dst, 270, w, h);	
		break;
	case ID_SCALEUP:
		pyrUp(image, dst);
		break;
	case ID_SCALEDOWN:
		pyrDown(image, dst);
		break;
	case ID_FILTER_CROSSSHADER:
		CrossShader(image, dst);
		break;

	case ID_FILTER_OLDPICTURE:
		OldPicture(image, dst);
		break;

	case ID_FILTER_L:
		Lomo(image, dst);
		break;
	case ID_FILTER_SOFTGLOW:
		SoftGlow(image, dst);
		break;
	case ID_FILTER_EMBOSS:
		Emboss(image, dst);
		break;
	case ID_FILTER_SKETCH:
		Sketch(image, dst);
		break;
	case ID_FILTER_DEHAZE:
		Dehaze(image, dst);
		break;
	case ID_COLOR_NEGATIVE:
		dst= Scalar(255, 255, 255) - image;
		break;
	case ID_COLOR_GRAYSCALE:
		cvtColor(image, dst, CV_BGR2GRAY);
		cvtColor(dst, dst, CV_GRAY2BGR);
		break;
	case ID_COLOR_THRESHOLD:
		cvtColor(image, dst, CV_BGR2GRAY);
		threshold(dst, dst, 100, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
		cvtColor(dst, dst, CV_GRAY2BGR);


	}
	image = dst.clone();
	Restore();
	Refresh();
	//RefreshHist();
	RefreshThumb();
}

void CImagePlayer::OnDropFiles(HDROP hDropInfo)
{
	UINT iFileCount;
	TCHAR FILE_NAME[MAX_PATH];
	iFileCount = ::DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	::DragQueryFile(hDropInfo, iFileCount-1, FILE_NAME, MAX_PATH);

	theApp.pMainDlg->strFileName = FILE_NAME;
	theApp.pMainDlg->OnReload();
}


void CImagePlayer::LoadYUV(int _w, int _h, int type)
{
	int size = GetYUVSize(_w, _h, type);
	uchar *pYUV = new uchar[size];
	CFile file;
	file.Open(theApp.pMainDlg->strFileName, CFile::modeRead);
	file.Read(pYUV, size);
	file.Close();
	image.create(_h, _w, CV_8UC3);
	ImageYUV2BGR(image, pYUV, type);
	delete pYUV;
	Restore();
	Refresh();
	RefreshThumb();
}

void CImagePlayer::SaveYUV(int type)
{
	if(image.empty())
		return;
	int w = image.cols;
	int h = image.rows;

	int size = GetYUVSize(w, h, type);
	uchar *pYUV = new uchar[size];
	
	ImageBGR2YUV(image, pYUV, type);
	CFile file;
	file.Open(theApp.pMainDlg->strFileName, CFile::modeCreate | CFile::modeWrite);
	file.Write(pYUV, size);
	file.Close();

	delete pYUV;
}