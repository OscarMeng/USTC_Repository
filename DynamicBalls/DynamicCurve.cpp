#include "StdAfx.h"
#include "DynamicCurve.h"
#include "Ball.h"
#include "Canet.h"
#include "PCI1752U.h"
#include "MyConstants.h"
#include <math.h>
#include <gl/glut.h>

CDynamicCurve::CDynamicCurve(void)
	:m_dMaxZoom(1),
	m_dMinZoom(0.06)
{
	int nCoord[m_nBallNum][2];
	int	nDeviceID[m_nBallNum];
	int nMotor[m_nBallNum];
	ReadCoordinate(nCoord);
	ReadDeviceID(nDeviceID,nMotor);
	for(int i = 0;i < m_nBallNum;i++)
		m_pBalls[i] = new CBall(i+1,nDeviceID[i],nMotor[i],nCoord[i][0],nCoord[i][1],this);

	ReadParam();

	m_ptStartPoint = CPoint(0,0);
	m_dZoom = 0.06;
	m_bTextOn = true;

	m_dXRotation = 0;

	m_colBackGround = RGB(5,9,5);
	m_colElliepseReal = RGB(9,111,130);
	m_colElliepseVirtual = RGB(255,255,0);
	m_colCoordinate = RGB(255,255,0);
	m_colBoardNormal = RGB(40,245,12);		//普通状态下边框的颜色
	m_colBoardSelected = RGB(244,39,11);	//选中时边框颜色
	m_colUpH = 0;				//最高点HSL颜色H值
	m_colUpS = 1;				//最高点HSL颜色S值
	m_colUpV = 1;				//最高点HSL颜色L值
	m_colDownH = 245;			//最低点HSL颜色H值
	m_colDownS = 1;			//最低点HSL颜色S值
	m_colDownV = 1;			//最低点HSL颜色L值

	m_bAutoRotation = false;

	m_textFont.CreateFont(12,0,0,0,600,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("宋体"));

	for(int i = 0;i < m_nCanetNum;i++)
		m_pCanet[i] = NULL;
//	InitCanetDevice();

	m_pPCI1752U = NULL;
//	InitPCIDevice();
}

CDynamicCurve::~CDynamicCurve(void)
{
}

void CDynamicCurve::ReadCoordinate(int coord[][2])
{
	CString sPath = "./coordinate.txt";
	FILE* ff;
	char buf[1000];
	CFileStatus status;
	errno_t err;
	if(CFile::GetStatus(sPath,status))
	{
		err = fopen_s(&ff,sPath,"rt");
		CString str;
		if(err != 0)
		{			
			str.Format("文件%s无法打开！",sPath);
			AfxMessageBox(str);
			return; 
		}
		for(int i = 0;i < m_nBallNum;i++)
		{
			fscanf_s(ff,"%d,%d",&coord[i][0],&coord[i][1]);
			fgets(buf,sizeof(buf)-1,ff);
		}
		fclose(ff);
	}
}

void CDynamicCurve::ReadDeviceID(int deviceID[],int motor[])
{
	CString sPath = "./deviceID.txt";
	FILE* ff;
	char buf[1000];
	CFileStatus status;
	errno_t err;
	if(CFile::GetStatus(sPath,status))
	{
		err = fopen_s(&ff,sPath,"rt");
		CString str;
		if(err != 0)
		{			
			str.Format("文件%s无法打开！",sPath);
			AfxMessageBox(str);
			return; 
		}
		int data;
		for(int i = 0;i < m_nBallNum;i++)
		{
			fscanf_s(ff,"%d",&data);
			deviceID[i] = data / 100;
			motor[i] = data % 100;

			fgets(buf,sizeof(buf)-1,ff);
		}
		fclose(ff);
	}
}

void CDynamicCurve::ReadParam()
{
	CString limtToZero;
	GetPrivateProfileString("Param","LimitToZero","0",limtToZero.GetBuffer(100),100,PARAM_PATH);
	limtToZero.ReleaseBuffer();
	limtToZero.Trim();
	m_nLimitToZero = atoi(limtToZero);

	CString posUp;
	GetPrivateProfileString("Param","PosUp","1",posUp.GetBuffer(100),100,PARAM_PATH);
	posUp.ReleaseBuffer();
	posUp.Trim();
	m_nPosUp = atoi(posUp);

	CString posDown;
	GetPrivateProfileString("Param","PosDown","20",posDown.GetBuffer(100),100,PARAM_PATH);
	posDown.ReleaseBuffer();
	posDown.Trim();
	m_nPosDown = atoi(posDown);
}

void CDynamicCurve::InitCanetDevice()
{
	for(int i = 0;i < m_nCanetNum;i++)
	{
		m_pCanet[i] = new CCanet(i,0,i);

	}
}

void CDynamicCurve::InitPCIDevice()
{
	LRESULT errCode;
	CString errMsg;
	SHORT   numOfDevices;

	//获取设备总数
	if ((errCode = DRV_DeviceGetNumOfList((SHORT far*)&numOfDevices)) != SUCCESS)
	{
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return ;
	}
	if (numOfDevices > m_nPCINum)
		numOfDevices = m_nPCINum;

	//获取设备链表
	SHORT	outEntries;
	DEVLIST m_DeviceList[m_nPCINum];
	if ((errCode = DRV_DeviceGetList((DEVLIST far*)m_DeviceList,numOfDevices,(SHORT far*)&outEntries)) != (LONG)SUCCESS)
	{
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return;
	}

	CString name = m_DeviceList[0].szDeviceName;
	name = name.Mid(18,2);
	int id = atoi(name);

	m_pPCI1752U = new CPCI1752U(m_DeviceList[0].dwDeviceNum,id);
}

int CDynamicCurve::GetWidth()
{
	return int((m_nMargin+m_nEllipse1A)*2*m_dZoom);
}

int CDynamicCurve::GetHeight()
{
	return int((m_nMargin+m_nEllipse1B)*2*m_dZoom);
}


CPoint CDynamicCurve::Lp2Dp(const CPoint& point)
{
	double x = (point.x+m_nEllipse1A+m_nMargin)*m_dZoom+m_ptStartPoint.x;
	double y = (point.y+m_nEllipse1B+m_nMargin)*m_dZoom+m_ptStartPoint.y;
	int nx,ny;
	if(x - floor(x) < 0.5)
		nx = (int)floor(x);
	else
		nx = (int)floor(x)+1;
	if(y - floor(y) < 0.5)
		ny = (int)floor(y);
	else
		ny = (int)floor(y)+1;

	return CPoint(nx,ny);
}

CPoint CDynamicCurve::Dp2Lp(const CPoint& point)
{
	double x = (point.x-m_ptStartPoint.x)/m_dZoom;
	double y = (point.y-m_ptStartPoint.y)/m_dZoom;

	int nx,ny;
	if(x - floor(x) < 0.5)
		nx = (int)floor(x);
	else
		nx = (int)floor(x)+1;
	if(y - floor(y) < 0.5)
		ny = (int)floor(y);
	else
		ny = (int)floor(y)+1;

	nx = nx - m_nEllipse1A - m_nMargin;
	ny = ny - m_nEllipse1B - m_nMargin;
	
	return CPoint(nx,ny);
}

bool CDynamicCurve::FindCellIDFromLp(const CPoint& point,int& id)
{
	for(int i = 0;i < m_nBallNum;i++)
	{
		double dis = (point.x-m_pBalls[i]->GetCenX())*(point.x-m_pBalls[i]->GetCenX())+(point.y-m_pBalls[i]->GetCenY())*(point.y-m_pBalls[i]->GetCenY());
		dis = sqrt(dis);
		if(dis <= m_nRadius)
		{
			id = i;
			return true;
		}
	}

	return false;
}

void CDynamicCurve::SelectNone()
{
	for(int i = 0;i < m_nBallNum;i++)
		m_pBalls[i]->SetSelected(false);
}

void CDynamicCurve::SelectOne(bool add,int id)
{
	if(!add)
	{
		SelectNone();
		m_pBalls[id]->SetSelected(true);
	}
	else
	{
		m_pBalls[id]->SetSelected(!m_pBalls[id]->GetSelected());
	}
}


void CDynamicCurve::SelectTen(bool add,int id)
{
	if(!add)
	{
		SelectNone();
		for(int i = 0;i < 10;i++)
			m_pBalls[id/10*10+i]->SetSelected(true);
	}
	else
	{
		for(int i = 0;i < 10;i++)	
			m_pBalls[id/10*10+i]->SetSelected(true);
	}
}

void CDynamicCurve::SelectAll()
{
	for(int i = 0;i < m_nBallNum;i++)
		m_pBalls[i]->SetSelected(true);
}

void CDynamicCurve::SetColorContents(DWORD nStartTime)
{
	for(int i = 0;i < m_nBallNum;i++)
		m_pBalls[i]->SetCurrentPosition(nStartTime);
}


void CDynamicCurve::SetColorManually(DWORD nStartTime)
{
	for(int i = 0;i < m_nBallNum;i++)
		if(m_pBalls[i]->GetSelected())
			m_pBalls[i]->SetCurrentPosition(nStartTime);
}


void CDynamicCurve::Draw(CDC* pDC,CRect rRect)
{
	pDC->FillSolidRect(rRect,m_colBackGround);

	int left,top,right,bottom;
	CPoint leftTop,rightBottom;
	CRect ellipseRect;

	CPen pen;
	CPen* pOldPen;
	CBrush* pOldBrush; 

	pen.CreatePen(PS_SOLID,1,m_colElliepseReal);
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
	//画最外面的实线椭圆
	left = -m_nEllipse1A;
	top = -m_nEllipse1B;
	right = m_nEllipse1A;
	bottom = m_nEllipse1B;
	leftTop = Lp2Dp(CPoint(left,top));
	rightBottom = Lp2Dp(CPoint(right,bottom));
	ellipseRect = CRect(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
	pDC->Ellipse(&ellipseRect);
	//画最里面的实线椭圆
	left = -m_nEllipse4A;
	top = -m_nEllipse4B;
	right = m_nEllipse4A;
	bottom = m_nEllipse4B;
	leftTop = Lp2Dp(CPoint(left,top));
	rightBottom = Lp2Dp(CPoint(right,bottom));
	ellipseRect = CRect(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
	pDC->Ellipse(&ellipseRect);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pen.CreatePen(PS_DASHDOT,1,m_colElliepseVirtual);
	pOldPen = pDC->SelectObject(&pen);
	//画出中间孔所在的外椭圆
	left = -m_nEllipse2A;
	top = -m_nEllipse2B;
	right = m_nEllipse2A;
	bottom = m_nEllipse2B;
	leftTop = Lp2Dp(CPoint(left,top));
	rightBottom = Lp2Dp(CPoint(right,bottom));
	ellipseRect = CRect(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
	pDC->Ellipse(&ellipseRect);
	//画出中间孔所在的内椭圆
	left = -m_nEllipse3A;
	top = -m_nEllipse3B;
	right = m_nEllipse3A;
	bottom = m_nEllipse3B;
	leftTop = Lp2Dp(CPoint(left,top));
	rightBottom = Lp2Dp(CPoint(right,bottom));
	ellipseRect = CRect(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
	pDC->Ellipse(&ellipseRect);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();


	pen.CreatePen(PS_DASHDOT,1,m_colCoordinate);
	pOldPen = pDC->SelectObject(&pen);
	//画出坐标轴
	left = -m_nEllipse1A-20;
	top = 0;
	right = -left;
	bottom = 0;
	leftTop = Lp2Dp(CPoint(left,top));
	rightBottom = Lp2Dp(CPoint(right,bottom));
	pDC->MoveTo(leftTop);
	pDC->LineTo(rightBottom);
	left = 0;
	top =  -m_nEllipse1B-20;
	right = 0;
	bottom = -top;
	leftTop = Lp2Dp(CPoint(left,top));
	rightBottom = Lp2Dp(CPoint(right,bottom));
	pDC->MoveTo(leftTop);
	pDC->LineTo(rightBottom);


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();

	//画出小球
	for(int i = 0;i < m_nBallNum;i++)
		m_pBalls[i]->Draw(pDC,rRect);

	//画出color bar
	CRect barRect(0,rRect.Height()-40-300,120,rRect.Height());
	pDC->FillSolidRect(barRect,m_colBackGround);
	for(int i = 0;i < 300;i++)
	{
		pen.CreatePen(PS_SOLID,1,CalGradientColor(i,300));
		pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(20,i+rRect.Height()-20-300);
		pDC->LineTo(70,i+rRect.Height()-20-300);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	COLORREF textColor = RGB(255,255,255)-m_colBackGround;
	pDC->SetTextColor(textColor);
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_textFont);
	CString text = "Up";
	CRect textRect(75,rRect.Height()-20-300,120,rRect.Height()-300);
	pDC->DrawText(text,&textRect,DT_WORDBREAK|DT_VCENTER|DT_NOPREFIX);
	text = "Down";
	textRect = CRect(75,rRect.Height()-40,120,rRect.Height()-20);
	pDC->DrawText(text,&textRect,DT_WORDBREAK|DT_VCENTER|DT_NOPREFIX);
	pDC->SelectObject(pOldFont);
}

void CDynamicCurve::Draw3D()
{
	GLfloat LightAmbient[] = {0.1f,0.1f,0.1f,0.1f };
	GLfloat LightDiffuse[] = {0.7f,0.7f,0.7f,0.7f };
	GLfloat LightSpecular[] = {0.0f,0.0f,0.0f,0.1f};
	GLfloat LightPosition[] = {-5.0f,- 5.0f,5.0f,0.0f};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	glEnable(GL_NORMALIZE);

	glTranslated(0.0, 0.0, -3);
	glRotated(90, 1.0, 0.0, 0.0);
	glTranslated(0.0, 0.0, -1);

	if(m_bAutoRotation)
		m_dXRotation += 1;
	glRotated(m_dXRotation, 0.0, 0.0, 1.0);

	for(int i = 0;i < m_nBallNum;i++)
		m_pBalls[i]->Draw3D();
	//	auxSolidSphere(1.0);

	glPopMatrix();
}

COLORREF CDynamicCurve::CalGradientColor(double index,double range)
{
	double h,s,v;
	double r,g,b;
	h = m_colUpH + index/range*(m_colDownH - m_colUpH);
	s = m_colUpS + index/range*(m_colDownS - m_colUpS);
	v = m_colUpV + index/range*(m_colDownV - m_colUpV);

	int hi = int(h/60.0)%6;
	double f = h/60.0-hi;
	double p = v*(1-s);
	double q = v*(1-f*s);
	double t = v*(1-(1-f)*s);
	switch(hi)
	{
	case 0:
		r = v;g = t;b = p;
		break;
	case 1:
		r = q;g = v;b = p;
		break;
	case 2:
		r = p;g = v;b = t;
		break;
	case 3:
		r = p;g = q;b = v;
		break;
	case 4:
		r = t;g = p;b = v;
		break;
	case 5:
		r = v;g = p;b = q;
		break;;
	default:
		r = 1;g = 1;b = 1;
		break;
	}

	return RGB(int(r*255),int(g*255),int(b*255));
}

void CDynamicCurve::LoadMotorSetMode()
{
	for(int i = 0;i < m_nCanetNum;i++)
	{


		m_pCanet[i]->LoadMotor(ALL_MOTOR);
		m_pCanet[i]->SetModePos(ALL_MOTOR);
	}
}

void CDynamicCurve::ToLimitManually()
{
	for(int i = 0;i < m_nBallNum;i++)
	{
		if(m_pBalls[i]->GetSelected())
		{
			int nCanetInd = m_pBalls[i]->GetCanetInd();
			int nMotor = m_pBalls[i]->GetMotor();
			m_pCanet[nCanetInd]->SetPosition(nMotor,LIMIT_POS);
			m_pCanet[nCanetInd]->SetSpeed(nMotor,LIMIT_SPEED);
			m_pCanet[nCanetInd]->Start(nMotor);
			m_pCanet[nCanetInd]->SetPosZero(nMotor);
		}
	}
}

void CDynamicCurve::FromLimitToZeroManually()
{
	for(int i = 0;i < m_nBallNum;i++)
	{
		if(m_pBalls[i]->GetSelected())
		{
			int nCanetInd = m_pBalls[i]->GetCanetInd();
			int nMotor = m_pBalls[i]->GetMotor();
			m_pCanet[nCanetInd]->SetPosition(nMotor,m_nLimitToZero);
			m_pCanet[nCanetInd]->SetSpeed(nMotor,UNIT_SPEED);
			m_pCanet[nCanetInd]->Start(nMotor);
			m_pCanet[nCanetInd]->SetPosZero(nMotor);
		}
	}
}

void CDynamicCurve::BackToZeroManually()
{
	for(int i = 0;i < m_nBallNum;i++)
	{
		if(m_pBalls[i]->GetSelected())
		{
			int nCanetInd = m_pBalls[i]->GetCanetInd();
			int nMotor = m_pBalls[i]->GetMotor();
			m_pCanet[nCanetInd]->SetSpeed(nMotor,UNIT_SPEED);
			m_pCanet[nCanetInd]->Zero(nMotor);
		}
	}
}

int CDynamicCurve::SetPosSpeedContents(int pos[],int time)
{
	int nProperTime = time;

	for(int i = 0;i < m_nBallNum;i++)
	{
		m_pBalls[i]->SetNextPosition(pos[i]);
		int nCalTime = m_pBalls[i]->CalSpeed(time);
		if(nProperTime < nCalTime)
			nProperTime = nCalTime;
	}
	if(nProperTime > time)
	{
		for(int i = 0;i < m_nBallNum;i++)
			m_pBalls[i]->CalSpeed(nProperTime);
	}

	return nProperTime;
}

int CDynamicCurve::SetPosSpeedManually(int pos,int time)
{
	int nProperTime = time;

	for(int i = 0;i < m_nBallNum;i++)
		if(m_pBalls[i]->GetSelected())
		{
			m_pBalls[i]->SetNextPosition(pos);
			int nCalTime = m_pBalls[i]->CalSpeed(time);
			if(nProperTime < nCalTime)
				nProperTime = nCalTime;
		}
	if(nProperTime > time)
	{
		for(int i = 0;i < m_nBallNum;i++)
			if(m_pBalls[i]->GetSelected())
				m_pBalls[i]->CalSpeed(nProperTime);
	}
	return nProperTime;
}

int CDynamicCurve::SetPosSpecialContents(int mode)
{
	int nProperTime = 0;
	if(mode == MODE_LIMIT)
	{
		for(int i = 0;i < m_nBallNum;i++)
			m_pBalls[i]->SetNextPosition(0);

		nProperTime = int(fabs((LIMIT_POS/(PI*PAN_D)))/LIMIT_SPEED*60.0)+60;
	}
	else if(mode == MODE_LIMITTOZERO)
	{
		for(int i = 0;i < m_nBallNum;i++)
			m_pBalls[i]->SetNextPosition(0);
		nProperTime = int((m_nLimitToZero/(PI*PAN_D))/UNIT_SPEED*60.0)+60;
	}
	else if(mode == MODE_BACKTOZERO)
	{
		int time;
		for(int i = 0;i < m_nBallNum;i++)
		{
			m_pBalls[i]->SetNextPosition(0);
			time = m_pBalls[i]->CalTime(UNIT_SPEED);
			if(nProperTime < time)
				nProperTime = time;
		}
		if(nProperTime > 0)
		{
			for(int i = 0;i < m_nBallNum;i++)
				m_pBalls[i]->CalSpeed(nProperTime);
		}
	}
	else if(mode == MODE_MIDDLE)
	{
		int time;
		for(int i = 0;i < m_nBallNum;i++)
		{
			m_pBalls[i]->SetNextPosition(4999);
			time = m_pBalls[i]->CalTime(UNIT_SPEED);
			if(nProperTime < time)
				nProperTime = time;
		}
		if(nProperTime > 0)
		{
			for(int i = 0;i < m_nBallNum;i++)
				m_pBalls[i]->CalSpeed(nProperTime);
		}
	}
	else
	{
		nProperTime = 10;
	}

	return nProperTime;
}


int CDynamicCurve::SetPosSpecialManually(int mode)
{
	int nProperTime = 0;
	if(mode == MODE_LIMIT)
	{
		for(int i = 0;i < m_nBallNum;i++)
			if(m_pBalls[i]->GetSelected())
			{
				m_pBalls[i]->SetNextPosition(0);
			}

		nProperTime = int(fabs((LIMIT_POS/(PI*PAN_D)))/LIMIT_SPEED*60.0)+60;
	}
	else if(mode == MODE_LIMITTOZERO)
	{
		for(int i = 0;i < m_nBallNum;i++)
			if(m_pBalls[i]->GetSelected())
			{
				m_pBalls[i]->SetNextPosition(0);
			}
		nProperTime = int((m_nLimitToZero/(PI*PAN_D))/UNIT_SPEED*60.0)+60;
	}
	else if(mode == MODE_BACKTOZERO)
	{
		int time;
		for(int i = 0;i < m_nBallNum;i++)
			if(m_pBalls[i]->GetSelected())
			{
				m_pBalls[i]->SetNextPosition(0);
				time = m_pBalls[i]->CalTime(UNIT_SPEED);
				if(nProperTime < time)
					nProperTime = time;
			}
		if(nProperTime > 0)
		{
			for(int i = 0;i < m_nBallNum;i++)
				if(m_pBalls[i]->GetSelected())
					m_pBalls[i]->CalSpeed(nProperTime);
		}
	}
	else if(mode == MODE_MIDDLE)
	{
		int time;
		for(int i = 0;i < m_nBallNum;i++)
			if(m_pBalls[i]->GetSelected())
			{
				m_pBalls[i]->SetNextPosition(4999);
				time = m_pBalls[i]->CalTime(UNIT_SPEED);
				if(nProperTime < time)
					nProperTime = time;
			}
		if(nProperTime > 0)
		{
			for(int i = 0;i < m_nBallNum;i++)
				if(m_pBalls[i]->GetSelected())
					m_pBalls[i]->CalSpeed(nProperTime);
		}
	}
	else
	{
		nProperTime = 10;
	}

	return nProperTime;
}

void CDynamicCurve::RunContents()
{
	for(int i = 0;i < m_nBallNum;i++)
	{
		int nCanetInd = m_pBalls[i]->GetCanetInd();
		int nMotor = m_pBalls[i]->GetMotor();

		m_pCanet[nCanetInd]->SetPosition(nMotor,m_pBalls[i]->GetPos());
		m_pCanet[nCanetInd]->SetSpeed(nMotor,m_pBalls[i]->GetSpeed());
	}

	for(int i = 0;i < m_nCanetNum;i++)
	{
		m_pCanet[i]->Start(ALL_MOTOR);
	}
}

void CDynamicCurve::RunManually()
{
	for(int i = 0;i < m_nBallNum;i++)
	{
		if(m_pBalls[i]->GetSelected())
		{
			int nCanetInd = m_pBalls[i]->GetCanetInd();
			int nMotor = m_pBalls[i]->GetMotor();

			m_pCanet[nCanetInd]->SetPosition(nMotor,m_pBalls[i]->GetPos());
			m_pCanet[nCanetInd]->SetSpeed(nMotor,m_pBalls[i]->GetSpeed());
			m_pCanet[nCanetInd]->Start(nMotor);
		}
	}
}