// EasyImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyImage.h"
#include "EasyImageDlg.h"
#include "algo.h"
#include "SliderDlg.h"
#include "types.h"
#include "yuvDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyImageDlg 对话框
static void Move(CWnd* pWnd, int l, int t, ULONG cx, ULONG cy,
				 int xScale, int yScale, int width, int height)
{

	ASSERT(IsWindow(pWnd->GetSafeHwnd()));
	pWnd->MoveWindow(l + static_cast<int>((cx * xScale) / 1000.0), 
		t + static_cast<int>((cy * yScale) / 1000.0),
		static_cast<int>((cx * width) / 1000.0),
		static_cast<int>((cy * height)/ 1000.0));


}

CEasyImageDlg::CEasyImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasyImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_player);
	DDX_Control(pDX, IDC_THUMBNAIL, m_thumb);
	DDX_Control(pDX, IDC_RGB, textRGB);
	DDX_Control(pDX, IDC_YUV, textYUV);
	DDX_Control(pDX, IDC_POS, xyPos);
	DDX_Control(pDX, IDC_IMAGEINFO, imageInfo);
	DDX_Control(pDX, IDC_SCALEINFO, scaleInfo);
	DDX_Control(pDX, IDC_HIST, m_hist);
	DDX_Control(pDX, IDC_EDIT1, m_log);
}

BEGIN_MESSAGE_MAP(CEasyImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CEasyImageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEasyImageDlg::OnBnClickedCancel)
	ON_COMMAND(ID_OPENFILE, &CEasyImageDlg::OnOpenfile)
	ON_COMMAND(ID_SAVEFILE, &CEasyImageDlg::OnSavefile)
	ON_COMMAND(ID_RELOAD, &CEasyImageDlg::OnReload)
	ON_COMMAND(ID_ROTATE90, &CEasyImageDlg::OnRotate90)
	ON_COMMAND(ID_ROTATE270, &CEasyImageDlg::OnRotate270)
	ON_COMMAND(ID_MIRROR, &CEasyImageDlg::OnMirror)
	ON_COMMAND(ID_FLIP, &CEasyImageDlg::OnFlip)
	ON_COMMAND(ID_SCALEUP, &CEasyImageDlg::OnScaleUp)
	ON_COMMAND(ID_SCALEDOWN, &CEasyImageDlg::OnScaleDown)
	ON_COMMAND(ID_ANY_ROTATION, &CEasyImageDlg::OnAnyRotation)
	ON_MESSAGE(WM_IMAGE_PROC, &CEasyImageDlg::HandleMessage)

	ON_COMMAND(ID_FILTER_OLDPICTURE, &CEasyImageDlg::OnFilterOldpicture)
	ON_COMMAND(ID_FILTER_L, &CEasyImageDlg::OnFilterL)
	ON_COMMAND(ID_FILTER_SOFTGLOW, &CEasyImageDlg::OnFilterSoftglow)
	ON_COMMAND(ID_FILTER_EMBOSS, &CEasyImageDlg::OnFilterEmboss)
	ON_COMMAND(ID_FILTER_SKETCH, &CEasyImageDlg::OnFilterSketch)
	ON_COMMAND(ID_FILTER_CROSSSHADER, &CEasyImageDlg::OnFilterCrossshader)
	ON_COMMAND(ID_FILTER_DEHAZE, &CEasyImageDlg::OnFilterDehaze)
	ON_COMMAND(ID_COLOR_LIGHT, &CEasyImageDlg::OnColorLight)
	ON_COMMAND(ID_COLOR_CONTRAST, &CEasyImageDlg::OnColorContrast)
	ON_COMMAND(ID_COLOR_SATURATION, &CEasyImageDlg::OnColorSaturation)
	ON_COMMAND(ID_COLOR_HUE, &CEasyImageDlg::OnColorHue)

	ON_COMMAND(ID_COLOR_NEGATIVE, &CEasyImageDlg::OnColorNegative)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CEasyImageDlg::OnColorGrayScale)
	ON_COMMAND(ID_COLOR_THRESHOLD, &CEasyImageDlg::OnColorThreshold)
END_MESSAGE_MAP()


// CEasyImageDlg 消息处理程序

BOOL CEasyImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	theApp.pMainDlg = this;
	//m_player.Load(L"lena.jpg");
	PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	textRGB.SetBkColor(0x303030);
	textYUV.SetBkColor(0x303030);
	xyPos.SetBkColor(0x303030);
	imageInfo.SetBkColor(0x303030);
	scaleInfo.SetBkColor(0x303030);
	textRGB.SetTransparent(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEasyImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		CPaintDC dc(this); // 用于绘制的设备上下文
		dc.FillSolidRect(rect, 0x303030);
		CDialog::OnPaint();
	}
}

void CEasyImageDlg::OnSize(UINT, int w, int h)
{
	if(m_player.GetSafeHwnd())
	{
		Move(&m_player, 0, 0, w, h, 0, 0, 800, 1000);
		Move(&xyPos, 0, 0, w, h, 840,  130,100, 30);
		Move(&textRGB, 0, 0, w, h, 840,  170,100, 30);
		Move(&textYUV, 0, 0, w, h, 840,  210, 100,30);
		Move(&imageInfo, 0, 0, w, h, 840,  50,100, 30);
		Move(&scaleInfo, 0, 0, w, h, 840,  90, 100,30);
		Move(GetDlgItem(IDC_BAR1), 0, 0, w, h, 801,  0, 2,1000);
		Move(GetDlgItem(IDC_BAR2), 0, 0, w, h, 803,  250, 197,2);
		Move(GetDlgItem(IDC_BAR3), 0, 0, w, h, 803,  500, 197,2);
		Move(GetDlgItem(IDC_BAR4), 0, 0, w, h, 803,  750, 197,2);
		Move(&m_thumb, 0, 0, w, h, 803,  252, 197, 248);
		Move(&m_hist, 0, 0, w, h, 803,  503, 195, 248);
		Move(&m_log, 0, 0, w, h, 803,  752, 195, 245);

		m_player.Refresh();
		m_player.RefreshThumb();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEasyImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEasyImageDlg::OnBnClickedOk()
{
	
}

void CEasyImageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CEasyImageDlg::OnOpenfile()
{
	CFileDialog dlg(TRUE, 
		L"Image Files (*.*)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
		_T("jpg|*.jpg|jpeg|*.jpeg|bmp|*.bmp|png|*.png|j2k|*.j2k|mng|*.mng|tif|*.tif|ras|*.ras|pnm|*.pnm|yuv|*.yuv||"),
		NULL);
	if(dlg.DoModal() == IDOK)
	{
		strFileName = dlg.GetPathName();	
		OnReload();
	}

}

void CEasyImageDlg::OnSavefile()
{
	CFileDialog dlg(FALSE, 
		L"Image Files (*.*)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
		_T("*.*|*.*||"),
		NULL);
	if(dlg.DoModal() == IDOK)
	{
		strFileName = dlg.GetPathName();
		CString strExt = strFileName.Mid(strFileName.ReverseFind('.')+1);
		strExt.MakeLower();
		if(strExt == "yuv")
		{
			CYUVDlg dlg;
			dlg.bSave = TRUE;
			if(dlg.DoModal() == IDOK)
			{
				m_player.SaveYUV(dlg.m_type);
			}
		}
		else 
			m_player.Save(strFileName);
	}
}

void CEasyImageDlg::OnReload()
{
	CString strExt = strFileName.Mid(strFileName.ReverseFind('.')+1);
	strExt.MakeLower();
	if(strExt == L"jpg" || strExt == L"bmp" || strExt == L"png" || strExt == L"j2k" || strExt == L"mng" 
		|| strExt == L"tif" || strExt == L"ras" || strExt == L"pnm")
			m_player.Load(strFileName);
	else if(strExt == "yuv")
	{
		CYUVDlg dlg;
		dlg.bSave = false;
		if(dlg.DoModal() == IDOK)
		{
			m_player.LoadYUV(dlg.w, dlg.h, dlg.m_type);
		}
	}
}
void CEasyImageDlg::OnRotate90()
{
	m_player.Process(ID_ROTATE90);
}
void CEasyImageDlg::OnRotate270()
{
	m_player.Process(ID_ROTATE270);
}
void CEasyImageDlg::OnMirror()
{
	m_player.Process(ID_MIRROR);
}
void CEasyImageDlg::OnFlip()
{
	m_player.Process(ID_FLIP);
}
void CEasyImageDlg::OnScaleUp()
{
	m_player.Process(ID_SCALEUP);
}
void CEasyImageDlg::OnScaleDown()
{
	m_player.Process(ID_SCALEDOWN);
}



void CEasyImageDlg::OnAnyRotation()
{
	oldImage = m_thumb.image.clone();
	CSliderDlg dlg(L"Rotation", 0, 360, 0, 0,EI_ROTATION);
	if(dlg.DoModal() == IDOK)
	{
		rotateImage(m_player.image, theApp.g_pos, m_player.w, m_player.h);
		m_player.Refresh();
		m_player.Restore();
	}
	
	m_player.RefreshThumb();
	
	
}

LRESULT CEasyImageDlg::HandleMessage(WPARAM wp, LPARAM lp)
{
	Mat temp = oldImage.clone();
	switch(wp)
	{
	case EI_ROTATION:
		{
			int w = temp.cols;
			int h = temp.rows;
			rotateImage(temp, theApp.g_pos, w, h);
			m_thumb.image = temp.clone();
		
			CRect rc;
			m_thumb.GetClientRect(rc);
			m_thumb.rcImage = CRect(0, 0, w, h);
			if(!(rc.Width() > 0) || !(rc.Height() > 0))
				return 0;
			m_thumb.CalcImageShowRect(rc, m_thumb.rcImage, m_thumb.rcShow);
			m_thumb.Invalidate();
			break;
		}
	case EI_COLOR_LIGHT:
		{
			Light(temp, theApp.g_pos-100, 0);
			m_thumb.image = temp.clone();
			m_thumb.Invalidate();
			break;

		}
	case EI_COLOR_CONTRAST:
		{
			Light(temp, 0, theApp.g_pos-100);
			m_thumb.image = temp.clone();
			m_thumb.Invalidate();
			break;
		}
	case EI_COLOR_SATURATION:
		{
			Saturation(temp, theApp.g_pos-100);
			m_thumb.image = temp.clone();
			m_thumb.Invalidate();
			break;
		}
	case EI_COLOR_HUE:
		{
			Hue(temp, theApp.g_pos);
			m_thumb.image = temp.clone();
			m_thumb.Invalidate();
		}
		break;
	}

	return 0;
}

void CEasyImageDlg::OnFilterOldpicture()
{
	m_player.Process(ID_FILTER_OLDPICTURE);
}

void CEasyImageDlg::OnFilterL()
{
	m_player.Process(ID_FILTER_L);
}

void CEasyImageDlg::OnFilterSoftglow()
{
	m_player.Process(ID_FILTER_SOFTGLOW);
}

void CEasyImageDlg::OnFilterEmboss()
{
	m_player.Process(ID_FILTER_EMBOSS);
}
void CEasyImageDlg::OnFilterSketch()
{
	m_player.Process(ID_FILTER_SKETCH);
}

void CEasyImageDlg::OnFilterCrossshader()
{
	m_player.Process(ID_FILTER_CROSSSHADER);
}

void CEasyImageDlg::OnFilterDehaze()
{
	m_player.Process(ID_FILTER_DEHAZE);
}


void CEasyImageDlg::OnColorLight()
{
	oldImage = m_thumb.image.clone();
	CSliderDlg dlg(L"Light",  100, 200, 0, 100,  EI_COLOR_LIGHT);
	if(dlg.DoModal() == IDOK)
	{
		Light(m_player.image, theApp.g_pos-100, 0);
		m_player.Refresh();
		m_player.Restore();
	}

	m_player.RefreshThumb();
}

void CEasyImageDlg::OnColorContrast()
{
	oldImage = m_thumb.image.clone();
	CSliderDlg dlg(L"Contrast",  100, 200, 0,  100, EI_COLOR_CONTRAST);
	if(dlg.DoModal() == IDOK)
	{
		Light(m_player.image, 0, theApp.g_pos-100);
		m_player.Refresh();
		m_player.Restore();
	}

	m_player.RefreshThumb();
}

void CEasyImageDlg::OnColorSaturation()
{
	oldImage = m_thumb.image.clone();
	CSliderDlg dlg(L"Saturation",  100, 200, 0,  100, EI_COLOR_SATURATION);
	if(dlg.DoModal() == IDOK)
	{
		Saturation(m_player.image, theApp.g_pos-100);
		m_player.Refresh();
		m_player.Restore();
	}

	m_player.RefreshThumb();
}

void CEasyImageDlg::OnColorHue()
{
	oldImage = m_thumb.image.clone();
	CSliderDlg dlg(L"Hue",  0, 256, 0,  0, EI_COLOR_HUE);
	if(dlg.DoModal() == IDOK)
	{
		Hue(m_player.image, theApp.g_pos);
		m_player.Refresh();
		m_player.Restore();
	}

	LOGE(L"Cancel to set hue");
	m_player.RefreshThumb();
}


void CEasyImageDlg::OnColorThreshold()
{
	m_player.Process(ID_COLOR_THRESHOLD);
}

void CEasyImageDlg::OnColorGrayScale()
{
	m_player.Process(ID_COLOR_GRAYSCALE);
}

void CEasyImageDlg::OnColorNegative()
{
	m_player.Process(ID_COLOR_NEGATIVE);
}

void CEasyImageDlg::Log(LPCTSTR s)
{
	CString str;
	m_log.GetWindowText(str);
	if(str.GetLength() > 8000)
		str= L"";
	str += s;
	str += L"\r\n";
	m_log.SetWindowText(str);
}

BOOL CEasyImageDlg::PreTranslateMessage(MSG* pMsg)
{
	ModifyStyle(0, SS_NOTIFY, TRUE);
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;	
		}
		else if(pMsg->wParam == 'Z' && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			m_player.HistoryPre();
			return TRUE;	
		}
		else if(pMsg->wParam == 'Y' && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			m_player.HistoryNext();
			return TRUE;	
		}
		else if(pMsg->wParam == 'O' && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			OnOpenfile();
			return TRUE;	
		}
		else if(pMsg->wParam == 'S' && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			OnSavefile();
			return TRUE;	
		}
		else if(pMsg->wParam == 'R' && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			OnReload();
			return TRUE;	
		}

	}
	if(pMsg->message == WM_MOUSEWHEEL)
	{

		if(PtInRect(m_player.rcShow, pMsg->pt))
		{
			m_player.OnMouseWheel(LOWORD(pMsg->wParam), HIWORD(pMsg->wParam), pMsg->pt);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CEasyImageDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT iFileCount;
	TCHAR FILE_NAME[MAX_PATH];
	iFileCount = ::DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	::DragQueryFile(hDropInfo, iFileCount-1, FILE_NAME, MAX_PATH);

	strFileName = FILE_NAME;
	OnReload();
}