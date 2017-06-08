// C3DView.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicBalls.h"
#include "3DView.h"
#include "DynamicCurve.h"
#include <gl/glut.h>
// C3DView

IMPLEMENT_DYNCREATE(C3DView, CView)

C3DView::C3DView()
{
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;

	m_bLButtonDown = false;
}

C3DView::~C3DView()
{
}


BEGIN_MESSAGE_MAP(C3DView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// C3DView drawing

BOOL C3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}


void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// C3DView message handlers

int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	HDC hDC = ::GetDC(GetSafeHwnd());
	if(!SetWindowPixelFormat(hDC))
		return 0;
	if(!CreateViewGLContext(hDC))
		return 0;

	return 0;
}

void C3DView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	m_pDynamicCurve->Draw3D();
//	RenderScene();
	SwapBuffers(dc.m_ps.hdc);
}


void C3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	GLsizei width = cx;
	GLsizei height = cy;
	GLdouble aspect;
	if (cy==0)
		aspect = (GLdouble)width;
	else
		aspect = (GLdouble)width/(GLdouble)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, aspect, 1,4);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}


void C3DView::OnDestroy()
{
	if(wglGetCurrentContext() != NULL)
	{
		// make the rendering context not current
		wglMakeCurrent(NULL,NULL) ;
	}
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	CView::OnDestroy();
}

BOOL C3DView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex==0) // Let's choose a default index.
	{
		m_GLPixelIndex = 1;
		if(!DescribePixelFormat(hDC,m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc))
		{
			return FALSE;
		}
	}
	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL C3DView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if(m_hGLContext == NULL)
	{
		return FALSE;
	}
	if(!wglMakeCurrent(hDC, m_hGLContext))
	{
		return FALSE;
	}

	return TRUE;
}


void C3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = true;
	m_ptLButtonDownPos = point;

	CView::OnLButtonDown(nFlags, point);
}


void C3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = false;

	CView::OnLButtonUp(nFlags, point);
}

void C3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLButtonDown)
	{
		double xRotation = (m_ptLButtonDownPos.x - point.x)/10.0;
		m_pDynamicCurve->SetXRotation(m_pDynamicCurve->GetXRotation()+xRotation);
		InvalidateRect(NULL,FALSE);

		m_ptLButtonDownPos = point;
	}

	CView::OnMouseMove(nFlags, point);
}

