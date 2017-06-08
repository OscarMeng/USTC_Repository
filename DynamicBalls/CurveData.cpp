#include "StdAfx.h"
#include "CurveData.h"


CCurveData::CCurveData(void)
{
	m_nFrameNum = 0;
	for(int i = 0;i < m_nMaxFrame;i++)
	{
		m_nTime[i] = 0;
		for(int j = 0;j < m_nBallNum;j++)
			m_nCurveData[i][j] = 0;
	}
}

CCurveData::~CCurveData(void)
{
}

bool CCurveData::ReadCurveData(CString path)
{
	CString type;
	GetPrivateProfileString("Type","type","",type.GetBuffer(100),100,path);
	type.ReleaseBuffer();
	type.Trim();
	if(type != "curveData")
	{
		CString str;
		str.Format("文件%s格式不正确！",path);
		AfxMessageBox(str);
		return false;
	}

	CString frame;
	GetPrivateProfileString("Frame","count","0",frame.GetBuffer(100),100,path);
	frame.ReleaseBuffer();
	frame.Trim();
	m_nFrameNum = atoi(frame);

	CString section;
	CString key;
	CString pos;
	for(int i = 0;i < m_nFrameNum;i++)
	{
		section.Format("Frame-%02d",i+1);
		CString time;
		GetPrivateProfileString(section,"time","1",time.GetBuffer(100),100,path);
		time.ReleaseBuffer();
		time.Trim();
		m_nTime[i] = atoi(time);

		for(int j = 0;j < 10;j++)
		{
			key.Format("Ball%02d-%02d",j*10+1,(j+1)*10);
			GetPrivateProfileString(section,key,"0",pos.GetBuffer(100),100,path);
			pos.ReleaseBuffer();
			pos.Trim();

			for(int k = 0;k < ((pos.GetLength()/4>10)?10:pos.GetLength()/4);k++)
			{
				CString data = pos.Mid(k*4,4);
				m_nCurveData[i][j*10+k] = atoi(data);
			}
			for(int k = pos.GetLength()/4;k <10;k++)
			{
				m_nCurveData[i][j*10+k] = 0;
			}
		}
	}

	return true;
}

