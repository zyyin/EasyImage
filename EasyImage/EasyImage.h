// EasyImage.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "EasyImageDlg.h"


#define  WM_IMAGE_PROC  WM_USER + 1
#define  LOGE(szFormat, ...) {CString str; str.Format(szFormat, ##__VA_ARGS__); theApp.pMainDlg->Log(str);}

class CEasyImageApp : public CWinApp
{
public:
	CEasyImageApp();
	CEasyImageDlg * pMainDlg;

	
	double g_pos;

// 重写
	public:
	virtual BOOL InitInstance();
	CString GetPath();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CEasyImageApp theApp;


class CFunctionLog 
{
public:
	CFunctionLog(char* sz)
	{
		m_str = sz;
		LOGE(L"Enter %s", m_str);
		
		startTick = GetTickCount();
	}
	virtual ~CFunctionLog()
	{
		LOGE(L"Leave %s,  time cost: %d ms", m_str, GetTickCount() - startTick);
	}
	CString m_str;
	DWORD startTick;

};