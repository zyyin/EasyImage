// SliderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyImage.h"
#include "SliderDlg.h"
#include "algo.h"
#include "EasyImage.h"

// CSliderDlg 对话框

IMPLEMENT_DYNAMIC(CSliderDlg, CDialog)

CSliderDlg::CSliderDlg(LPCTSTR _strTitle, double _defaultVal, double _maxVal, double _minVal, double _diff, int _message, CWnd* pParent /*=NULL*/)
	: CDialog(CSliderDlg::IDD, pParent)
{
	strTitle = _strTitle;
	defaultVal = _defaultVal;
	maxVal = _maxVal;
	minVal = _minVal;
	message = _message;
	diff = _diff;

}

CSliderDlg::~CSliderDlg()
{
}

void CSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slide);
	
}


BEGIN_MESSAGE_MAP(CSliderDlg, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSliderDlg 消息处理程序
BOOL CSliderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(strTitle);
	//m_slide.setR
	m_slide.SetRange(minVal, maxVal);
	m_slide.SetPos(defaultVal);
	CString s;
	s.Format(L"%d", (int)(defaultVal-diff));
	GetDlgItem(IDC_EDIT1)->SetWindowText(s);
	return TRUE;
}
void CSliderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	theApp.g_pos = m_slide.GetPos();
	CString s;
	s.Format(L"%d", (int)(theApp.g_pos-diff));
	GetDlgItem(IDC_EDIT1)->SetWindowText(s);
	theApp.pMainDlg->SendMessage(WM_IMAGE_PROC, message, 0);
	
}