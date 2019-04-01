// EasyImage.cpp : 定义应用程序的类行为。
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


// CEasyImageApp 构造

CEasyImageApp::CEasyImageApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CEasyImageApp 对象

CEasyImageApp theApp;


// CEasyImageApp 初始化
BOOL CEasyImageApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
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
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Verisilicon-EasyImage"));

	CEasyImageDlg dlg;
	m_pMainWnd = &dlg;
	dlg.strFileName = cmdInfo.m_strFileName;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
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


