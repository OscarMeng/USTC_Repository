#pragma once

class CMySplitterWnd:public CSplitterWnd  
{  
	DECLARE_DYNCREATE(CMySplitterWnd)  
public:   
	CMySplitterWnd();  
	virtual ~CMySplitterWnd(void);  

private:  
	int m_nHorVer;
public:
	DECLARE_MESSAGE_MAP()     
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	inline void SetHorVer(int type)		{m_nHorVer = type;}
};  
