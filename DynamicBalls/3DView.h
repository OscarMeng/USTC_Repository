#pragma once

// C3DView view
class CDynamicCurve;

class C3DView : public CView
{
	DECLARE_DYNCREATE(C3DView)

protected:
	C3DView();           // protected constructor used by dynamic creation
	virtual ~C3DView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
private:
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
//	void RenderScene();
private:
	int		m_GLPixelIndex;
	HGLRC	m_hGLContext; 

private:
	CDynamicCurve*	m_pDynamicCurve;
public:
	inline void SetDynamicCurve(CDynamicCurve*	pDynamicCurve)		{m_pDynamicCurve = pDynamicCurve;}

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	bool	m_bLButtonDown;
	CPoint	m_ptLButtonDownPos;
};


