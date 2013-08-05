// EasyImageDlg.h : ͷ�ļ�
//

#pragma once
#include "imageplayer.h"
#include "afxwin.h"
#include "Label.h"

// CEasyImageDlg �Ի���
class CEasyImageDlg : public CDialog
{
// ����
public:
	CEasyImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EASYIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT, int w, int h);
	afx_msg LRESULT HandleMessage(WPARAM wp, LPARAM lp);
	
	afx_msg void OnReload();
	afx_msg void OnOpenfile();
	afx_msg void OnSavefile();

	afx_msg void OnRotate90();
	afx_msg void OnRotate270();
	afx_msg void OnAnyRotate();
	afx_msg void OnMirror();
	afx_msg void OnFlip();
	afx_msg void OnScaleUp();
	afx_msg void OnScaleDown();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Log(LPCTSTR);

	CImagePlayer m_player;
	Mat oldImage;
	CThumbnail m_thumb;
	CLabel textRGB;
	CLabel textYUV;
	CLabel xyPos;
	CLabel imageInfo;
	CLabel scaleInfo;
	CString strFileName;
	CHist m_hist;
	afx_msg void OnAnyRotation();
	afx_msg void OnFilterOldpicture();
	afx_msg void OnFilterL();
	afx_msg void OnFilterSoftglow();
	afx_msg void OnFilterEmboss();
	afx_msg void OnFilterSketch();
	afx_msg void OnFilterCrossshader();
	afx_msg void OnFilterDehaze();
	afx_msg void OnColorLight();
	afx_msg void OnColorContrast();
	afx_msg void OnColorSaturation();
	afx_msg void OnColorHue();
	afx_msg void OnColorNegative();
	afx_msg void OnColorGrayScale();
	afx_msg void OnColorThreshold();
	CEdit m_log;
};
