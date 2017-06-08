#pragma once
#include "afx.h"
class CCurveData :
	public CObject
{
public:
	CCurveData(void);
	~CCurveData(void);
public:
	const static int m_nMaxFrame = 30;			//���֡��	
	const static int m_nBallNum = 100;			//С����
	int	m_nFrameNum;							//֡��
	int m_nTime[m_nMaxFrame];					//ÿ֡��ʾʱ��
	int m_nCurveData[m_nMaxFrame][m_nBallNum];	//С��λ��
public:
	bool ReadCurveData(CString path);
};

