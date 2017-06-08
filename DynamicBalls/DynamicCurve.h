#pragma once
#include "afx.h"

class CBall;
class CCanet;
class CPCI1752U;
class CDynamicCurve : public CObject
{
public:
	CDynamicCurve(void);
	~CDynamicCurve(void);

public:
	static const int	m_nRadius = 120;		//С���ֱ��
	static const int	m_nBallNum = 100;		//С�������
	static const int	m_nOutBallNum = 54;		//��ȦС������
	static const int	m_nInnerBallNum = 46;	//��ȦС������
	static const int	m_nEllipse1A = 4975;	//��Բ1����
	static const int	m_nEllipse1B = 2875;	//��Բ1����
	static const int	m_nEllipse2A = 4600;	//��Բ2����
	static const int	m_nEllipse2B = 2500;	//��Բ2����
	static const int	m_nEllipse3A = 4097;	//��Բ3����
	static const int	m_nEllipse3B = 2000;	//��Բ3����
	static const int	m_nEllipse4A = 3890;	//��Բ4����
	static const int	m_nEllipse4B = 1790;	//��Բ4����
	static const int	m_nMargin = 1000;
	static const int	m_nCanetNum = 10;
	static const int	m_nPCINum = 1;
	static const int	m_nTimerTime = 100;
	const double m_dMaxZoom;
	const double m_dMinZoom;

	CFont		m_textFont;				//��������
private:
	CPoint	m_ptStartPoint;				//��ʼ��
	double	m_dZoom;					//���ű�
	bool	m_bTextOn;					//�Ƿ���ʾ����

	double  m_dXRotation;	

	COLORREF m_colBackGround;
	COLORREF m_colElliepseReal;		//ʵ��Բ����ɫ
	COLORREF m_colElliepseVirtual;	//����Բ����ɫ			
	COLORREF m_colCoordinate;		//���������ɫ
	COLORREF m_colBoardNormal;		//��ͨ״̬�±߿����ɫ
	COLORREF m_colBoardSelected;	//ѡ��ʱ�߿���ɫ
	double	 m_colUpH;				//��ߵ�HSV��ɫHֵ
	double	 m_colUpS;				//��ߵ�HSV��ɫSֵ
	double	 m_colUpV;				//��ߵ�HSV��ɫVֵ
	double	 m_colDownH;			//��͵�HSV��ɫHֵ
	double	 m_colDownS;			//��͵�HSV��ɫSֵ
	double	 m_colDownV;			//��͵�HSV��ɫVֵ

	int		 m_nLimitToZero;	    //��е��λ����λ�ľ���
	int		 m_nPosUp;				//����λλ��
	int		 m_nPosDown;			//����λλ��

	bool	 m_bAutoRotation;		//�Զ���ת
public:
	inline void		SetStartPoint(const CPoint& point)		{m_ptStartPoint = point;}
	inline void		SetZoom(const double& zoom)				{m_dZoom = zoom;}
	inline void		SetXRotation(const double& rotation)	{m_dXRotation = rotation;}
	inline void		SetAutoRotation(const bool& rotation)	{m_bAutoRotation = rotation;}
	inline CPoint	GetStartPoint()							{return m_ptStartPoint;}
	inline double	GetZoom()								{return m_dZoom;}
	inline double	GetXRotation()							{return m_dXRotation;}
	inline bool		GetTextOn()								{return m_bTextOn;}
	inline COLORREF GetColBoardNormal()						{return m_colBoardNormal;}
	inline COLORREF GetColBoardSelected()					{return m_colBoardSelected;}
	inline double   GetColUpH()								{return m_colUpH;}
	inline double   GetColUpS()								{return m_colUpS;}
	inline double   GetColUpV()								{return m_colUpV;}
	inline double   GetColDownH()							{return m_colDownH;}
	inline double   GetColDownS()							{return m_colDownS;}
	inline double   GetColDownV()							{return m_colDownV;}
	inline int		GetPosUp()								{return m_nPosUp;}
	inline int		GetPosDown()							{return m_nPosDown;}
	inline bool		GetAutoRotation()						{return m_bAutoRotation;}
private:
	CBall*		m_pBalls[m_nBallNum];	//С��
	CCanet*		m_pCanet[m_nCanetNum];	//can
	CPCI1752U*	m_pPCI1752U;
private:
	void ReadCoordinate(int coord[][2]);
	void ReadDeviceID(int deviceID[],int motor[]);
	void ReadParam();
	void InitCanetDevice();
	void InitPCIDevice();
public:
	int		GetWidth();
	int		GetHeight();
	CPoint	Lp2Dp(const CPoint& point);
	CPoint	Dp2Lp(const CPoint& point);
	bool	FindCellIDFromLp(const CPoint& point,int& id);
	void	SelectNone();
	void	SelectOne(bool add,int id);
	void	SelectTen(bool add,int id);
	void	SelectAll();

	void	SetColorContents(DWORD nStartTime);
	void	SetColorManually(DWORD nStartTime);
public:
	void	Draw(CDC* pDC,CRect rRect);
	void	Draw3D();


	COLORREF	CalGradientColor(double index,double range);
public:
	void	LoadMotorSetMode();			//��ʼ�������ص��������λ��ģʽ
	void	ToLimitManually();			//���е���е��λ��--�ֶ�����
	void	FromLimitToZeroManually();	//�ӻ�е��λ����λ��(��Ϊ�涨��λ��)--�ֶ�����
	void	BackToZeroManually();		//�ص���Ϊ�涨����λ--�ֶ�����

	int		SetPosSpeedContents(int pos[],int time);
	int		SetPosSpeedManually(int pos,int time);
	int		SetPosSpecialContents(int mode);
	int		SetPosSpecialManually(int mode);
	void	RunContents();
	void	RunManually();
};

