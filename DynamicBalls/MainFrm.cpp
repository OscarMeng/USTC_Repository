
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DynamicBalls.h"

#include "MainFrm.h"
#include "3DView.h"
#include "DynamicBallsView.h"
#include "ControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_wndSplitter1.SetHorVer(1);
	m_wndSplitter2.SetHorVer(2);

	m_bInitSplitter = false;
	m_nWidth = 0;
	m_nHeight = 0;


}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	HMENU nIDorHMenu = NULL;  
	return CFrameWnd::CreateEx(dwExStyle,lpszClassName,lpszWindowName,dwStyle,  
		rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,  
		pParentWnd->GetSafeHwnd(),  
		nIDorHMenu,  
		(LPVOID)pContext);  
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszName = _T("¶¯¸ÐÐ¡Çò");

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitter1.CreateStatic(this,1,2);
	m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(C3DView),CSize(0,0),pContext);
	m_wndSplitter2.CreateStatic(&m_wndSplitter1,2,1,WS_CHILD|WS_VISIBLE,m_wndSplitter1.IdFromRowCol(0,1));
	m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CDynamicBallsView),CSize(0,0),pContext);
	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CControlView),CSize(0,0),pContext);

	m_p3DView = (C3DView*)m_wndSplitter1.GetPane(0,0);
	m_pDynamicBallsView = (CDynamicBallsView*)m_wndSplitter2.GetPane(0,0);
	m_pControlView  = (CControlView*)m_wndSplitter2.GetPane(1, 0);	
	m_p3DView->SetDynamicCurve(m_pDynamicBallsView->GetDynamicCurve());
	m_pDynamicBallsView->SetControlView(m_pControlView);
	m_pControlView->SetMainFrame(this);
	m_pControlView->Set3DView(m_p3DView);
	m_pControlView->SetDynamicBallsView(m_pDynamicBallsView);
	m_pControlView->SetDynamicCurve(m_pDynamicBallsView->GetDynamicCurve());
	m_pControlView->SetCurveData(m_pDynamicBallsView->GetCurveData());


	CRect rcClient;
	GetClientRect(rcClient);

	m_bInitSplitter = true;
	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

// CMainFrame message handlers
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMinTrackSize.x = 1300;
	lpMMI->ptMinTrackSize.y = 750;

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_bInitSplitter)
	{
		CRect rect;
		GetClientRect(&rect);
		if(rect.Width() <= 0 || rect.Height() <= 0)
			return;
		if(cx == m_nWidth)
			return;
		m_nWidth = cx;
		int x = max(500,(int)(rect.Width()*0.4));
		m_wndSplitter1.SetColumnInfo(0,x,0);
		m_wndSplitter1.SetColumnInfo(1,rect.Width()-x,0); 
		m_wndSplitter1.RecalcLayout();


		if(cy == m_nHeight)
			return;
		m_nHeight = cy;
		int y = 200;
		m_wndSplitter2.SetRowInfo(0,rect.Height()-y,0);
		m_wndSplitter2.SetRowInfo(1,y,0); 
		m_wndSplitter2.RecalcLayout();

	}
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CFrameWndEx::OnEraseBkgnd(pDC);
}
