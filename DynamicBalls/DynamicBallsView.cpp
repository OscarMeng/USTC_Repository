
// DynamicBallsView.cpp : implementation of the CDynamicBallsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DynamicBalls.h"
#endif

#include "DynamicBallsView.h"
#include "DynamicCurve.h"
#include "CurveData.h"
#include "ControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDynamicBallsView

IMPLEMENT_DYNCREATE(CDynamicBallsView, CScrollView)

BEGIN_MESSAGE_MAP(CDynamicBallsView, CScrollView)
	// Standard printing commands
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CDynamicBallsView construction/destruction

CDynamicBallsView::CDynamicBallsView()
{
	// TODO: add construction code here
	m_bDragMode = false;

	m_pDynamicCurve = new CDynamicCurve();
	m_pCurveData = new CCurveData();
}

CDynamicBallsView::~CDynamicBallsView()
{
	delete m_pDynamicCurve;
	delete m_pCurveData;
}

void CDynamicBallsView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


BOOL CDynamicBallsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	CSize size;
	size.cx = m_pDynamicCurve->GetWidth();
	size.cy = m_pDynamicCurve->GetHeight();
	SetScrollSizes(MM_TEXT,size);

	return CScrollView::PreCreateWindow(cs);
}

// CDynamicBallsView drawing
void CDynamicBallsView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

//	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CDynamicBallsView::OnDraw(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CDC	memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bBmp;
	bBmp.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());
	CBitmap *pOldBitmap = memDC.SelectObject(&bBmp);

	m_pDynamicCurve->Draw(&memDC,&rcClient);
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

	bBmp.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	// TODO: add draw code for native data here
}

BOOL CDynamicBallsView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

// CDynamicBallsView diagnostics

#ifdef _DEBUG
void CDynamicBallsView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDynamicBallsView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG


// CDynamicBallsView message handlers


void CDynamicBallsView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ,&si);		//获取滚动条信息，SB_VERT指垂直滚动条
	int nMaxPos = si.nMax-si.nPage;	//可滚动范围最大值
	int scrValue = 0;//滚动值
	switch(nSBCode)
	{
	case SB_LINELEFT:							//上按钮被单击了一下
		if(si.nPos != 0)						//还可以滚动
		{
			if(si.nPos-m_nLineSize < 0)		//可滚动像素不够LINESIZE
				scrValue = si.nPos;
			else
				scrValue = m_nLineSize;
		}
		break;
	case SB_LINERIGHT:						//下按钮被单击了一下
		if(si.nPos != nMaxPos)				//还可以滚动
		{
			if(nMaxPos-si.nPos < m_nLineSize)//可滚动像素不够LINESIZE
				scrValue = -(nMaxPos-si.nPos);//负数图片向上滚动
			else
				scrValue = -m_nLineSize;
		}
		break;
	case SB_PAGELEFT:							//滚动条上通道被单击了一下
		if(si.nPos != 0)						//还可以滚动
		{
			if(si.nPos-(int)si.nPage < 0)		//可滚动像素不够si.nPage
				scrValue = si.nPos;
			else 
				scrValue = si.nPage;
		}
		break;
	case SB_PAGERIGHT:						//滚动条下通道被单击了一下
		if(si.nPos != nMaxPos)				//还可以滚动
		{
			if(nMaxPos-si.nPos < (int)si.nPage)	//可滚动像素不够si.nPage
				scrValue = -(nMaxPos-si.nPos);
			else
				scrValue = -(int)si.nPage;
		}
		break;
	case SB_THUMBTRACK:						//拖动滚动条滑柄，nPos保存位置
		scrValue = si.nPos - nPos;
		break;
	}

	CPoint startPoint = m_pDynamicCurve->GetStartPoint();
	startPoint.x = startPoint.x + scrValue; 
	m_pDynamicCurve->SetStartPoint(startPoint);
	Invalidate();
	SetScrollPos(SB_HORZ,si.nPos-scrValue);//设置滚动条位置*/
}


void CDynamicBallsView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	GetScrollInfo(SB_VERT,&si);		//获取滚动条信息，SB_VERT指垂直滚动条
	int nMaxPos = si.nMax-si.nPage;	//可滚动范围最大值
	int scrValue = 0;//滚动值
	switch(nSBCode)
	{
	case SB_LINEUP:							//上按钮被单击了一下
		if(si.nPos != 0)						//还可以滚动
		{
			if(si.nPos-m_nLineSize < 0)		//可滚动像素不够LINESIZE
				scrValue = si.nPos;
			else
				scrValue = m_nLineSize;
		}
		break;
	case SB_LINEDOWN:						//下按钮被单击了一下
		if(si.nPos != nMaxPos)				//还可以滚动
		{
			if(nMaxPos-si.nPos < m_nLineSize)//可滚动像素不够LINESIZE
				scrValue = -(nMaxPos-si.nPos);//负数图片向上滚动
			else
				scrValue = -m_nLineSize;
		}
		break;
	case SB_PAGEUP:							//滚动条上通道被单击了一下
		if(si.nPos != 0)						//还可以滚动
		{
			if(si.nPos-(int)si.nPage < 0)		//可滚动像素不够si.nPage
				scrValue = si.nPos;
			else 
				scrValue = si.nPage;
		}
		break;
	case SB_PAGEDOWN:						//滚动条下通道被单击了一下
		if(si.nPos != nMaxPos)				//还可以滚动
		{
			if(nMaxPos-si.nPos < (int)si.nPage)	//可滚动像素不够si.nPage
				scrValue = -(nMaxPos-si.nPos);
			else
				scrValue = -(int)si.nPage;
		}
		break;
	case SB_THUMBTRACK:						//拖动滚动条滑柄，nPos保存位置
		scrValue = si.nPos - nPos;
		break;
	}

	CPoint startPoint = m_pDynamicCurve->GetStartPoint();
	startPoint.y = startPoint.y + scrValue; 
	m_pDynamicCurve->SetStartPoint(startPoint);
	Invalidate();
	SetScrollPos(SB_VERT,si.nPos-scrValue);//设置滚动条位置*/
}


void CDynamicBallsView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	CPoint startPoint = m_pDynamicCurve->GetStartPoint();
	if(startPoint.x >= 0 && rect.Width() > m_pDynamicCurve->GetWidth())
		startPoint.x = (rect.Width()-m_pDynamicCurve->GetWidth())/2;
	if(startPoint.y >= 0 && rect.Height() > m_pDynamicCurve->GetHeight())
		startPoint.y = (rect.Height()-m_pDynamicCurve->GetHeight())/2;
	m_pDynamicCurve->SetStartPoint(startPoint);
}


BOOL CDynamicBallsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&pt);
	double dZoom = m_pDynamicCurve->GetZoom()*(1+zDelta/1200.0);
	ZoomInOut(dZoom,pt);

	return FALSE;
}


void CDynamicBallsView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_ptLast = point;
	m_bDragMode = true;

//	CScrollView::OnMButtonDown(nFlags, point);
}


void CDynamicBallsView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bDragMode = false;

//	CScrollView::OnMButtonUp(nFlags, point);
}


void CDynamicBallsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDragMode)
	{
		CRect rect;
		GetClientRect(&rect);

		SCROLLINFO si_h;
		GetScrollInfo(SB_HORZ,&si_h);		//获取滚动条信息，SB_VERT指垂直滚动条
		int nMaxPosX = si_h.nMax-(int)si_h.nPage;	//可滚动范围最大值
		int mx = point.x - m_ptLast.x;
		if(m_pDynamicCurve->GetWidth() <= rect.Width())
			mx = 0;
		else if(si_h.nPos - mx > nMaxPosX)
			mx = nMaxPosX - si_h.nPos;
		else if(si_h.nPos - mx < 0)
			mx = 0 - si_h.nPos;

		SCROLLINFO si_v;
		GetScrollInfo(SB_VERT,&si_v);		//获取滚动条信息，SB_VERT指垂直滚动条
		int nMaxPosY = si_v.nMax-(int)si_v.nPage;	//可滚动范围最大值
		int my = point.y - m_ptLast.y;
		if(m_pDynamicCurve->GetHeight() <= rect.Height())
			my = 0;
		else if(si_v.nPos - my > nMaxPosY)
			my = nMaxPosY - si_v.nPos;
		else if(si_v.nPos - my < 0)
			my = 0 - si_v.nPos;

		CPoint startPoint = m_pDynamicCurve->GetStartPoint();
		startPoint.x = startPoint.x + mx;
		startPoint.y = startPoint.y + my; 
		m_pDynamicCurve->SetStartPoint(startPoint);
		Invalidate();

		SetScrollPos(SB_HORZ,si_h.nPos-mx);//设置滚动条位置*/
		SetScrollPos(SB_VERT,si_v.nPos-my);//设置滚动条位置*/

		m_ptLast = point;
		return ;
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CDynamicBallsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_pControlView->IsRunning())
		return;

	CPoint lpPoint = m_pDynamicCurve->Dp2Lp(point);
	int id;
	if(m_pDynamicCurve->FindCellIDFromLp(lpPoint,id))
	{
		if(nFlags & MK_CONTROL)
			m_pDynamicCurve->SelectOne(true,id);
		else
			m_pDynamicCurve->SelectOne(false,id);
		Invalidate();
	}
	else if(!(nFlags & MK_CONTROL))
	{
		m_pDynamicCurve->SelectNone();
		Invalidate();
	}

//	CScrollView::OnLButtonDown(nFlags, point);
}


void CDynamicBallsView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_pControlView->IsRunning())
		return;

	CPoint lpPoint = m_pDynamicCurve->Dp2Lp(point);
	int id;
	if(m_pDynamicCurve->FindCellIDFromLp(lpPoint,id))
	{
		if(nFlags & MK_CONTROL)
			m_pDynamicCurve->SelectTen(true,id);
		else
			m_pDynamicCurve->SelectTen(false,id);
		Invalidate();
	}

//	CScrollView::OnRButtonDown(nFlags, point);
}


void CDynamicBallsView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_pControlView->IsRunning())
		return;

	CPoint lpPoint = m_pDynamicCurve->Dp2Lp(point);
	
	m_pDynamicCurve->SelectAll();
	Invalidate();

//	CScrollView::OnRButtonDblClk(nFlags, point);
}


BOOL CDynamicBallsView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	CWinApp *pApp = AfxGetApp();
	if(m_bDragMode)
		m_Cursor = pApp->LoadCursor(IDC_HAND_DRAG);  
	else
		m_Cursor = pApp->LoadStandardCursor(IDC_CROSS);

	::SetCursor(m_Cursor);

	return TRUE;
}


void CDynamicBallsView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CDynamicBallsView::ZoomInOut(double dZoom,CPoint ptCenter)
{
	double dOldZoom = m_pDynamicCurve->GetZoom();
	if(dZoom < m_pDynamicCurve->m_dMinZoom)
		dZoom = m_pDynamicCurve->m_dMinZoom;
	else if(dZoom > m_pDynamicCurve->m_dMaxZoom)
		dZoom = m_pDynamicCurve->m_dMaxZoom;
	m_pDynamicCurve->SetZoom(dZoom);

	CPoint startPoint;
	CRect rect;
	GetClientRect(&rect);
	int x = int((ptCenter.x-m_pDynamicCurve->GetStartPoint().x)/dOldZoom*dZoom);
	int y = int((ptCenter.y-m_pDynamicCurve->GetStartPoint().y)/dOldZoom*dZoom);
	if(m_pDynamicCurve->GetWidth() < rect.Width())
		startPoint.x = (rect.Width()-m_pDynamicCurve->GetWidth())/2;
	else if(ptCenter.x > x)
		startPoint.x = 0;
	else if(m_pDynamicCurve->GetWidth()-x < rect.Width()-ptCenter.x) 
		startPoint.x = rect.Width() - m_pDynamicCurve->GetWidth();
	else
		startPoint.x = ptCenter.x - x;
	if(m_pDynamicCurve->GetHeight() < rect.Height())
		startPoint.y = (rect.Height()-m_pDynamicCurve->GetHeight())/2;
	else if(ptCenter.y > y)
		startPoint.y = 0;
	else if(m_pDynamicCurve->GetHeight()-y < rect.Height()-ptCenter.y)
		startPoint.y = rect.Height() - m_pDynamicCurve->GetHeight();
	else
		startPoint.y = ptCenter.y - y;

	m_pDynamicCurve->SetStartPoint(startPoint);

	Invalidate();

	CSize size;
	size.cx = m_pDynamicCurve->GetWidth();
	size.cy = m_pDynamicCurve->GetHeight();

	SetScrollSizes(MM_TEXT,size);
	SetScrollPos(SB_HORZ,-startPoint.x);
	SetScrollPos(SB_VERT,-startPoint.y);
}

