#include "stdafx.h"  
#include "MySplitterWnd.h"  
 
IMPLEMENT_DYNCREATE(CMySplitterWnd, CSplitterWnd)  
  
CMySplitterWnd::CMySplitterWnd()
{  
}  
   
CMySplitterWnd::~CMySplitterWnd(void)  
{  
}  

BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)  
      
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()  


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	if(m_nHorVer == 1)
	{
		if(point.x < 500 || point.x > (rect.Width()-800))
			CWnd::OnMouseMove(nFlags, point);
		else
			CSplitterWnd::OnMouseMove(nFlags, point);
	}
	else
	{
		if(point.y < 550 || point.y > (rect.Height()-200))
			CWnd::OnMouseMove(nFlags, point);
		else
			CSplitterWnd::OnMouseMove(nFlags, point);
	}
	CWnd::OnMouseMove(nFlags, point);
}
