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
	const int		m_nID;			//С����
	const int		m_nCanetInd;	//���ư���
	const int		m_nMotor;		//������
	const int		m_nCenX;		//��������x
	const int		m_nCenY;		//��������y
	
	int		m_nLastPosition;	//��һ��λ��
	double	m_nCurrentPosition;	//��ǰλ��(0~9999)
	int		m_nNextPosition;	//Ŀ��λ��(0~9999)
	int		m_nPos;				//ʵ��Ҫ���е���λ��
	int		m_nSpeed;			//С���ƶ��ٶ�:Ȧ/min
	bool	m_bSelected;		//С���Ƿ�ѡ��
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

