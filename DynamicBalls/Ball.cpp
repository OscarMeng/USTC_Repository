#include "StdAfx.h"
#include "Ball.h"
#include "DynamicCurve.h"
#include "MyConstants.h"
#include "math.h"
#include <gl/glut.h>


CBall::CBall(int nID,int nCanetInd,int nMotor,int nCenX,int nCenY,CDynamicCurve* pDynamicCurve)
	:m_nID(nID),
	m_nCanetInd(nCanetInd),
	m_nMotor(nMotor),
	m_nCenX(nCenX),
	m_nCenY(nCenY),
	m_pDynamicCurve(pDynamicCurve)
{
	m_nLastPosition = 0;
	m_nCurrentPosition = 0;
	m_nNextPosition = 0;
	m_nPos = 0;	
	m_nSpeed = 0;	
	m_bSelected = false;
}


CBall::~CBall(void)
{
}

void CBall::SetCurrentPosition(DWORD nStartTime)
{
	double circle = m_nSpeed*((GetTickCount()-nStartTime)/1000.0/60.0);
	double dis = circle*PI*PAN_D;
	double pos = dis/(m_pDynamicCurve->GetPosDown()-m_pDynamicCurve->GetPosUp())*9999;
	if(m_nLastPosition <= m_nNextPosition)
	{
		m_nCurrentPosition = m_nLastPosition + pos;
		if(m_nCurrentPosition > m_nNextPosition)
			m_nCurrentPosition = m_nNextPosition;
	}
	else
	{
		m_nCurrentPosition = m_nLastPosition - pos;
		if(m_nCurrentPosition < m_nNextPosition)
			m_nCurrentPosition = m_nNextPosition;
	}
}

void CBall::Draw(CDC* pDC,CRect rRect)
{
	int left = m_nCenX - m_pDynamicCurve->m_nRadius;
	int top = m_nCenY - m_pDynamicCurve->m_nRadius;
	int right = m_nCenX + m_pDynamicCurve->m_nRadius;
	int bottom = m_nCenY + m_pDynamicCurve->m_nRadius;

	CPoint leftTop = m_pDynamicCurve->Lp2Dp(CPoint(left,top));
	CPoint rightBottom = m_pDynamicCurve->Lp2Dp(CPoint(right,bottom));
	if(leftTop.x > rRect.right)
		return;
	if(leftTop.y > rRect.bottom)
		return;
	if(rightBottom.x < rRect.left)
		return;
	if(rightBottom.y < rRect.top)
		return;
	CRect ballRect(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);

	CPen pen;
	if(m_bSelected)
		pen.CreatePen(PS_SOLID,1,m_pDynamicCurve->GetColBoardSelected());
	else
		pen.CreatePen(PS_SOLID,1,m_pDynamicCurve->GetColBoardNormal());

	CBrush brush(m_pDynamicCurve->CalGradientColor(m_nCurrentPosition,9999));
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	//»­Ô²ÐÎ
	pDC->Ellipse(&ballRect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	brush.DeleteObject();
	pen.DeleteObject();

	if(m_pDynamicCurve->GetTextOn() && m_pDynamicCurve->GetZoom() >= 0.2)
	{
		COLORREF textColor = RGB(255,255,255)-m_pDynamicCurve->CalGradientColor(m_nCurrentPosition,9999);
		CFont* pOldFont = pDC->SelectObject(&m_pDynamicCurve->m_textFont);
		pDC->SetTextColor(textColor);
		pDC->SetBkMode(TRANSPARENT);
		CString text;
		text.Format("(%03d)",m_nID);
		CRect textRect(ballRect);
		textRect.top = ballRect.top + (ballRect.Height()-20)/2;
		textRect.bottom = textRect.top + 10;
		pDC->DrawText(text,&textRect,DT_WORDBREAK|DT_VCENTER|DT_CENTER|DT_NOPREFIX);

		text.Format("(%02d,%02d)",m_nCanetInd,m_nMotor);
		textRect.top = textRect.bottom;
		textRect.bottom = ballRect.bottom - (ballRect.Height()-20)/2;
		pDC->DrawText(text,&textRect,DT_WORDBREAK|DT_VCENTER|DT_CENTER|DT_NOPREFIX);

		pDC->SelectObject(pOldFont);
	}
}

void CBall::Draw3D()
{
	GLfloat no_mat[] = {0.0f,0.0f,0.0f,1.0f};
	GLfloat mat_ambient[] = {0.2f,0.2f,0.2f,1.0f};
	GLfloat mat_diffusse[] = {0.4f,0.4f,0.4f,1.0f};
	GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat low_shininess[] = {15.0f};
	GLfloat high_shininess[] = {100.0f};
	GLfloat mat_emission1[] = {0.4f,0.0f,0.0f,0.0f};
	GLfloat mat_emission2[] = {0.0f,0.0f,0.4f,0.0f};


	glEnable(GL_NORMALIZE);

	glPushMatrix();
	glTranslated(m_nCenX/5000.0,m_nCenY/5000.0,0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffusse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,low_shininess);
	glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);

	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	gluCylinder(pObj, 0.002, 0.002, m_nCurrentPosition/5000.0, 100, 50);
	gluDeleteQuadric(pObj);
	glPopMatrix();

	glPushMatrix();
	glTranslated(m_nCenX/5000.0,m_nCenY/5000.0,m_nCurrentPosition/5000.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffusse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
	if(m_nID <= 54)
		glMaterialfv(GL_FRONT,GL_EMISSION,mat_emission1);
	else
		glMaterialfv(GL_FRONT,GL_EMISSION,mat_emission2);
	glutSolidSphere(0.05,50,50);
	glPopMatrix();
}

int CBall::CalSpeed(int time)
{
	double dLastPos = m_nLastPosition/9999.0*(m_pDynamicCurve->GetPosDown()-m_pDynamicCurve->GetPosUp())+m_pDynamicCurve->GetPosUp();
	double dNextPos = m_nNextPosition/9999.0*(m_pDynamicCurve->GetPosDown()-m_pDynamicCurve->GetPosUp())+m_pDynamicCurve->GetPosUp();
	double dis = fabs(dNextPos-dLastPos);
	double circle = dis/(PI*PAN_D);
	m_nPos = int(dNextPos*LINENUM/(PI*PAN_D));
	m_nSpeed = int(circle/(time/60.0));

	if(m_nSpeed == 0 && circle != 0)
		m_nSpeed = 1;
	else if(m_nSpeed > MAX_SPEED)
	{
		time = int(circle*1.5/MAX_SPEED*60);
		m_nSpeed = int(circle/(time/60.0));
	}

	return time;
}

int CBall::CalTime(int speed)
{
	double dLastPos = m_nLastPosition/9999.0*(m_pDynamicCurve->GetPosDown()-m_pDynamicCurve->GetPosUp())+m_pDynamicCurve->GetPosUp();
	double dNextPos = m_nNextPosition/9999.0*(m_pDynamicCurve->GetPosDown()-m_pDynamicCurve->GetPosUp())+m_pDynamicCurve->GetPosUp();
	double dis = fabs(dNextPos-dLastPos);
	double circle = dis/(PI*PAN_D);

	return int(circle/speed*60.0);
}