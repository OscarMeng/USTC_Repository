#pragma once
#include "afx.h"
class CCurveData :
	public CObject
{
public:
	CCurveData(void);
	~CCurveData(void);
public:
	const static int m_nMaxFrame = 30;			//最大帧数	
	const static int m_nBallNum = 100;			//小球数
	int	m_nFrameNum;							//帧数
	int m_nTime[m_nMaxFrame];					//每帧显示时间
	int m_nCurveData[m_nMaxFrame][m_nBallNum];	//小球位置
public:
	bool ReadCurveData(CString path);
};

