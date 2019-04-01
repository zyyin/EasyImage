#pragma once
#include "afxwin.h"

// EasyResizeDialog 对话框

class EasyResizeDialog : public CDialog
{
	DECLARE_DYNAMIC(EasyResizeDialog)

public:
	EasyResizeDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EasyResizeDialog();


	enum { IDD = IDD_RESIZE };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	int mWidth;
	int mHeight;
	afx_msg void OnBnClickedOk();
};
