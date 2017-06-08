// ColorCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ColorCtrl.h"

// CColorCtrl
IMPLEMENT_DYNAMIC(CColorCtrl, CStatic)

CColorCtrl::CColorCtrl()
{
	m_nLeft = 0;
	m_nRight = 0;
}

CColorCtrl::~CColorCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CColorCtrl message handlers


void CColorCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	CBrush brush(HOLLOW_BRUSH);
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.Rectangle(&rect);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();
	for(int i = 1;i < rect.Width()-1;i++)
	{
		double gradient = (double)i/rect.Width();
		COLORREF colMid = GetColGradient(gradient);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,colMid);
		CPen* pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(i,1);
		dc.LineTo(i,rect.Height()-1);
		dc.SelectObject(pOldPen);
		pen.DeleteObject();
	}
}

void CColorCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	
	if(!m_bEnalbe)
		return ;
	CColorDialog colDlg(m_colLeft,CC_FULLOPEN,this);
	if(colDlg.DoModal() == IDOK)
	{
		m_colLeft = colDlg.GetColor(); 
		m_colRight = m_colLeft;
		Invalidate();

		::PostMessage(this->GetParent()->GetSafeHwnd(),WM_GRADIENT,m_nLeft,m_nRight);
	}
//	CStatic::OnLButtonUp(nFlags, point);
}

void CColorCtrl::Init(bool enable,COLORREF color1,COLORREF color2)
{
	m_bEnalbe = enable;
	m_colLeft = color1;
	m_colRight = color2;
}

void CColorCtrl::SetColor(COLORREF color)
{
	m_colLeft = color;
	m_colRight = color;
	Invalidate();
}

void CColorCtrl::SetColor(COLORREF color1,COLORREF color2)
{
	m_colLeft = color1;
	m_colRight = color2;
	Invalidate();
}

void CColorCtrl::SetLeftRight(int left,int right)
{
	m_nLeft = left;
	m_nRight = right;
}

COLORREF CColorCtrl::GetColGradient(double gradient)
{
	int R = GetRValue(m_colLeft) + (int)(gradient*(GetRValue(m_colRight) - GetRValue(m_colLeft)));
	int G = GetGValue(m_colLeft) + (int)(gradient*(GetGValue(m_colRight) - GetGValue(m_colLeft)));
	int B = GetBValue(m_colLeft) + (int)(gradient*(GetBValue(m_colRight) - GetBValue(m_colLeft)));

	return RGB(R,G,B);
}

BOOL CColorCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bEnalbe)
	{
		CWinApp *pApp = AfxGetApp();	
		m_Cursor = pApp->LoadStandardCursor(IDC_HAND);  
		::SetCursor(m_Cursor);
		return TRUE;	
	}
	else
		return CStatic::OnSetCursor(pWnd,nHitTest,message);
}
