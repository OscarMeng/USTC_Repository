#pragma once
#include "afx.h"
#include "MyConstants.h"

class CCanet :
	public CObject
{
public:
	CCanet(DWORD deviceInd,DWORD canInd,int ip);
	~CCanet(void);
private:
	const DWORD m_nDeviceInd;
	const DWORD m_nCanInd;
	const DWORD m_nIP;

	double m_dShowHigh;		//显示高度（m_dWToS+显示的距离）
	double m_dLToS;			//limit与stop之间的距离
	double m_dZToW;			//zero与work之间的距离
	double m_dFZhen;

private:
	void Connect();
	void Close();
	bool Transmit(PVCI_CAN_OBJ vciCanObj,ULONG len);
public:
	bool SendCmd(const UINT& motor,const int& cmd,const int& data);
public:
	bool LoadMotor(const UINT& motor);				//加载电机
	bool SetModePos(const UINT& motor);				//设置位置模式
	bool EmergencyStop(const UINT& motor);			//急停
	bool CancelEmergencyStop(const UINT& motor);	//取消急停
	bool SetPID(const UINT& motor,const int& param,const int& data);		//设置PID
	bool SavePID(const UINT& motor);				//用于设置PID后的保存	
	bool SetPosZero(const UINT& motor);				//设置零位
	bool SetPosition(const UINT& motor,const int& data);					//设置位置
	bool SetSpeed(const UINT& motor,const int& data);		//设置速度
	bool Start(const UINT& motor);					//启动位置运动
	bool Zero(const UINT& motor);					//回零位
};

