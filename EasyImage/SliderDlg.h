#pragma once
#include "afxcmn.h"


// CSliderDlg 对话框

class CSliderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSliderDlg)

public:
	CSliderDlg(LPCTSTR _strTitle, double _defaultVal, double _maxVal, double _minVal, double _diff,int _message, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CSliderDlg();
	

// 对话框数据
	enum { IDD = IDD_SLIDERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
	int message;
	CString strTitle;
	double defaultVal, maxVal, minVal;
	double diff;
public:
	CSliderCtrl m_slide;
};
