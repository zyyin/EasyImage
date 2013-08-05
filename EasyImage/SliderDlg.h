#pragma once
#include "afxcmn.h"


// CSliderDlg �Ի���

class CSliderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSliderDlg)

public:
	CSliderDlg(LPCTSTR _strTitle, double _defaultVal, double _maxVal, double _minVal, double _diff,int _message, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CSliderDlg();
	

// �Ի�������
	enum { IDD = IDD_SLIDERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
