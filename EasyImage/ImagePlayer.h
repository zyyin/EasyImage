#pragma once
#include "imageview.h"

class CImagePlayer :
	public CImageView
{
public:
	CImagePlayer(void);
	~CImagePlayer(void);
	void Load(LPCTSTR strFileName);
	void Save(LPCTSTR strFileName);
	void LoadYUV(int _w, int _h, int type);
	void SaveYUV(int type);

	void Clear();
	void Scale( CPoint pt);
	void SendScaleInfo();
	void ScreenToImage(CPoint& pt);
	void RefreshThumb();
	void Refresh();
	void RefreshHist();
	void Process(int id);

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	void Restore();
	void HistoryPre();
	void HistoryNext();
	
private:
	bool bDrag;
	CPoint startPoint;

	vector<Mat> m_history;
	int historyInddex;

public:
	afx_msg void OnSplit();
};
