// EasyResizeDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "EasyImage.h"
#include "EasyResizeDialog.h"


// EasyResizeDialog 对话框

IMPLEMENT_DYNAMIC(EasyResizeDialog, CDialog)

EasyResizeDialog::EasyResizeDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RESIZE, pParent)
	, mWidth(0)
	, mHeight(0)
{

}

EasyResizeDialog::~EasyResizeDialog()
{
}

void EasyResizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, mWidth);
	DDX_Text(pDX, IDC_HEIGHT, mHeight);
}


BEGIN_MESSAGE_MAP(EasyResizeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &EasyResizeDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// EasyResizeDialog 消息处理程序


void EasyResizeDialog::OnBnClickedOk()
{
	UpdateData();

	CDialog::OnOK();
}

BOOL EasyResizeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_WIDTH)->EnableWindow(TRUE);
	UpdateData(FALSE);
	return TRUE;
}