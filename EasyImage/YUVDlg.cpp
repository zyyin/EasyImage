// YUVDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyImage.h"
#include "YUVDlg.h"


// CYUVDlg 对话框

IMPLEMENT_DYNAMIC(CYUVDlg, CDialog)

CYUVDlg::CYUVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYUVDlg::IDD, pParent)
{

}

CYUVDlg::~CYUVDlg()
{
}

void CYUVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YUVTYPE, m_comboType);
}


BEGIN_MESSAGE_MAP(CYUVDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CYUVDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CYUVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comboType.AddString(L"NV12");
	m_comboType.AddString(L"NV21");
	m_comboType.AddString(L"I420");
	m_comboType.AddString(L"YV12");
	m_comboType.AddString(L"YUYV");
	m_comboType.AddString(L"UYVY");
	m_comboType.AddString(L"YUV444");
	
	FileStorage fs;
	CString strPath = theApp.GetPath() + L"yuv.conf";
	USES_CONVERSION;
	fs.open(T2A(strPath), FileStorage::READ);
	fs["type"] >> m_type;
	fs["width"] >> w;
	fs["height"] >> h;
	m_comboType.SetCurSel(m_type);
	if(bSave)
	{
		w = theApp.pMainDlg->m_player.w;
		h = theApp.pMainDlg->m_player.h;
		
	}
	CString str;
	str.Format(L"%d", w);
	GetDlgItem(IDC_WIDTH)->SetWindowText(str);
	str.Format(L"%d", h);
	GetDlgItem(IDC_HEIGHT)->SetWindowText(str);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(!bSave);
	GetDlgItem(IDC_WIDTH)->EnableWindow(!bSave);
	
	return TRUE;
}


void CYUVDlg::OnBnClickedOk()
{

	FileStorage fs;
	CString strPath = theApp.GetPath() + L"yuv.conf";
	m_type = m_comboType.GetCurSel();
	USES_CONVERSION;
	fs.open(T2A(strPath), FileStorage::WRITE);
	fs<<"type"<<m_type<<"width"<<w<<"height"<<h;
	OnOK();
}
