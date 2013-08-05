#pragma once
#include "afxwin.h"


// CYUVDlg 对话框

class CYUVDlg : public CDialog
{
	DECLARE_DYNAMIC(CYUVDlg)

public:
	CYUVDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CYUVDlg();
	bool bSave;
// 对话框数据
	enum { IDD = IDD_YUV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
public:
	CComboBox m_comboType;
	int w, h, m_type;
	afx_msg void OnBnClickedOk();
};
