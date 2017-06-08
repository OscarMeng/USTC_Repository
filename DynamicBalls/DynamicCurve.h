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
	static const int	m_nRadius = 120;		//小球的直径
	static const int	m_nBallNum = 100;		//小球的总数
	static const int	m_nOutBallNum = 54;		//外圈小球数量
	static const int	m_nInnerBallNum = 46;	//内圈小球数量
	static const int	m_nEllipse1A = 4975;	//椭圆1长轴
	static const int	m_nEllipse1B = 2875;	//椭圆1短轴
	static const int	m_nEllipse2A = 4600;	//椭圆2长轴
	static const int	m_nEllipse2B = 2500;	//椭圆2短轴
	static const int	m_nEllipse3A = 4097;	//椭圆3长轴
	static const int	m_nEllipse3B = 2000;	//椭圆3短轴
	static const int	m_nEllipse4A = 3890;	//椭圆4长轴
	static const int	m_nEllipse4B = 1790;	//椭圆4短轴
	static const int	m_nMargin = 1000;
	static const int	m_nCanetNum = 10;
	static const int	m_nPCINum = 1;
	static const int	m_nTimerTime = 100;
	const double m_dMaxZoom;
	const double m_dMinZoom;

	CFont		m_textFont;				//文字字体
private:
	CPoint	m_ptStartPoint;				//开始点
	double	m_dZoom;					//缩放比
	bool	m_bTextOn;					//是否显示文字

	double  m_dXRotation;	

	COLORREF m_colBackGround;
	COLORREF m_colElliepseReal;		//实椭圆的颜色
	COLORREF m_colElliepseVirtual;	//虚椭圆的颜色			
	COLORREF m_colCoordinate;		//坐标轴的颜色
	COLORREF m_colBoardNormal;		//普通状态下边框的颜色
	COLORREF m_colBoardSelected;	//选中时边框颜色
	double	 m_colUpH;				//最高点HSV颜色H值
	double	 m_colUpS;				//最高点HSV颜色S值
	double	 m_colUpV;				//最高点HSV颜色V值
	double	 m_colDownH;			//最低点HSV颜色H值
	double	 m_colDownS;			//最低点HSV颜色S值
	double	 m_colDownV;			//最低点HSV颜色V值

	int		 m_nLimitToZero;	    //机械限位和零位的距离
	int		 m_nPosUp;				//上限位位置
	int		 m_nPosDown;			//下限位位置

	bool	 m_bAutoRotation;		//自动旋转
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
	CBall*		m_pBalls[m_nBallNum];	//小球
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
	void	LoadMotorSetMode();			//初始化，加载电机，设置位置模式
	void	ToLimitManually();			//运行到机械限位处--手动操作
	void	FromLimitToZeroManually();	//从机械限位到零位，(人为规定的位置)--手动操作
	void	BackToZeroManually();		//回到人为规定的零位--手动操作

	int		SetPosSpeedContents(int pos[],int time);
	int		SetPosSpeedManually(int pos,int time);
	int		SetPosSpecialContents(int mode);
	int		SetPosSpecialManually(int mode);
	void	RunContents();
	void	RunManually();
};

