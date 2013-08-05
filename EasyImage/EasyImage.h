// EasyImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "EasyImageDlg.h"


#define  WM_IMAGE_PROC  WM_USER + 1
#define  LOGE(szFormat, ...) {CString str; str.Format(szFormat, ##__VA_ARGS__); theApp.pMainDlg->Log(str);}

class CEasyImageApp : public CWinApp
{
public:
	CEasyImageApp();
	CEasyImageDlg * pMainDlg;

	
	double g_pos;

// ��д
	public:
	virtual BOOL InitInstance();
	CString GetPath();

// ʵ��

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