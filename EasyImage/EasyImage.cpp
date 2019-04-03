// EasyImage.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EasyImage.h"
#include "EasyImageDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyImageApp

BEGIN_MESSAGE_MAP(CEasyImageApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEasyImageApp ����

CEasyImageApp::CEasyImageApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEasyImageApp ����

CEasyImageApp theApp;


// CEasyImageApp ��ʼ��
BOOL CEasyImageApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	CString str;
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	str = exeFullPath;
	USES_CONVERSION;
	CString ext = L"jpg";
	HKEY hKey, hTempKey;
	str += L" %1";
	::RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Classes", 0, KEY_ALL_ACCESS, &hKey);
	::RegCreateKey(hKey, _T("easyimage\\shell\\open\\command"), &hTempKey);
	::RegSetValueExA(hTempKey, (""), 0, REG_SZ, (const BYTE*)T2A((LPCTSTR)str), str.GetLength());
	str = exeFullPath;
	str = str.Mid(0, str.ReverseFind('\\') + 1);
	str += L"icon.ico";
	::RegCreateKey(hKey, _T("easyimage\\DefaultIcon"), &hTempKey);
	::RegSetValueExA(hTempKey, (""), 0, REG_SZ, (const BYTE*)T2A((LPCTSTR)str), str.GetLength());
	::RegCloseKey(hTempKey);
	::RegCloseKey(hKey);
	static vector<string> vec = {
		".jpg",".jpeg",".bmp",".png",".j2k",".mng",".tif",".ras",".pnm",".yuv",".pam",".pbm",".ppm",".pgm",".tiff",".dng",
	};
	const char* pDefault = "easyimage";
	for (auto item : vec) {
		::RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Classes", 0, KEY_SET_VALUE, &hKey);
		::RegCreateKey(hKey, A2T(item.c_str()), &hTempKey);
		::RegSetValueExA(hTempKey, (""), 0, REG_SZ, (const BYTE*)pDefault, strlen(pDefault));
		::RegCloseKey(hTempKey);
		::RegCloseKey(hKey);
	}

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Verisilicon-EasyImage"));

	CEasyImageDlg dlg;
	m_pMainWnd = &dlg;
	dlg.strFileName = cmdInfo.m_strFileName;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


CString CEasyImageApp::GetPath()
{
	CString str;
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	str = exeFullPath;
	str = str.Mid(0, str.ReverseFind('\\') + 1);
	return str;
}


