
// DynamicBallsView.h : interface of the CDynamicBallsView class
//

#pragma once

class CDynamicCurve;
class CCurveData;
class CControlView;
//class CDynamicBallsDoc;
class CDynamicBallsView : public CScrollView
{
protected: // create from serialization only
	CDynamicBallsView();
	DECLARE_DYNCREATE(CDynamicBallsView)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

// Implementation
public:
	virtual ~CDynamicBallsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
	const static int m_nLineSize = 20;


private:
	CDynamicCurve*	m_pDynamicCurve;
	CCurveData*		m_pCurveData;
	CControlView*	m_pControlView;

	bool			m_bDragMode;
	HCURSOR			m_Cursor;
	CPoint			m_ptLast;

public:
	inline CDynamicCurve*	GetDynamicCurve()		{return m_pDynamicCurve;}
	inline CCurveData*		GetCurveData()			{return m_pCurveData;}
	inline void				SetControlView(CControlView* pControlView)		{m_pControlView = pControlView;}
public:
	void	ZoomInOut(double dZoom,CPoint ptCenter);
};

#ifndef _DEBUG  // debug version in DynamicBallsView.cpp
inline CDynamicBallsDoc* CDynamicBallsView::GetDocument() const
   { return reinterpret_cast<CDynamicBallsDoc*>(m_pDocument); }
#endif

