#pragma once
#include "afx.h"

class CDynamicCurve;
class CBall : public CObject
{
public:
	CBall(int nID,int nCanetInd,int nMotor,int nCenX,int nCenY,CDynamicCurve* pDynamicCurve);
	~CBall(void);

public:


private:
	CDynamicCurve*	m_pDynamicCurve;
	const int		m_nID;			//小球编号
	const int		m_nCanetInd;	//控制板编号
	const int		m_nMotor;		//电机编号
	const int		m_nCenX;		//中心坐标x
	const int		m_nCenY;		//中心坐标y
	
	int		m_nLastPosition;	//上一次位置
	double	m_nCurrentPosition;	//当前位置(0~9999)
	int		m_nNextPosition;	//目标位置(0~9999)
	int		m_nPos;				//实际要运行到的位置
	int		m_nSpeed;			//小球移动速度:圈/min
	bool	m_bSelected;		//小球是否被选中
public:
	inline void SetNextPosition(int pos)		{m_nLastPosition = m_nNextPosition;m_nNextPosition = pos;}
	inline void SetSpeed(int speed)				{m_nSpeed = speed;}
	inline void SetSelected(bool sel)			{m_bSelected = sel;}
	inline int	GetID()							{return m_nID;}
	inline int	GetCanetInd()					{return m_nCanetInd;}
	inline int	GetMotor()						{return m_nMotor;}
	inline int  GetCenX()						{return m_nCenX;}
	inline int	GetCenY()						{return m_nCenY;}
	inline int  GetLastPosition()				{return m_nLastPosition;}
	inline int  GetNextPosition()				{return m_nNextPosition;}
	inline int  GetPos()						{return m_nPos;}
	inline int	GetSpeed()						{return m_nSpeed;}
	inline bool GetSelected()					{return m_bSelected;}
public:
	void	SetCurrentPosition(DWORD nStartTime);
public:
	void Draw(CDC* pDC,CRect rRect);
	void Draw3D();

	int	 CalSpeed(int time);
	int  CalTime(int speed);
};

