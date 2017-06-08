#pragma once


// CColorCtrl
#define  WM_GRADIENT WM_USER+11
class CColorCtrl : public CStatic
{
	DECLARE_DYNAMIC(CColorCtrl)

public:
	CColorCtrl();
	virtual ~CColorCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	bool		m_bEnalbe;
	COLORREF	m_colLeft;
	COLORREF	m_colRight;
	HCURSOR		m_Cursor;
	int		m_nLeft;
	int		m_nRight;
public:
	void Init(bool enable,COLORREF color1,COLORREF color2);
	void SetColor(COLORREF color);
	void SetColor(COLORREF color1,COLORREF color2);
	void SetLeftRight(int left,int right);
public:
	inline COLORREF GetColor()	{return m_colLeft;}
private:
	COLORREF GetColGradient(double gradient);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


