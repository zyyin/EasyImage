#pragma once
#include "afxwin.h"


// CYUVDlg �Ի���

class CYUVDlg : public CDialog
{
	DECLARE_DYNAMIC(CYUVDlg)

public:
	CYUVDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CYUVDlg();
	bool bSave;
// �Ի�������
	enum { IDD = IDD_YUV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
public:
	CComboBox m_comboType;
	int w, h, m_type;
	afx_msg void OnBnClickedOk();
};
