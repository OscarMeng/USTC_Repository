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

	double m_dShowHigh;		//��ʾ�߶ȣ�m_dWToS+��ʾ�ľ��룩
	double m_dLToS;			//limit��stop֮��ľ���
	double m_dZToW;			//zero��work֮��ľ���
	double m_dFZhen;

private:
	void Connect();
	void Close();
	bool Transmit(PVCI_CAN_OBJ vciCanObj,ULONG len);
public:
	bool SendCmd(const UINT& motor,const int& cmd,const int& data);
public:
	bool LoadMotor(const UINT& motor);				//���ص��
	bool SetModePos(const UINT& motor);				//����λ��ģʽ
	bool EmergencyStop(const UINT& motor);			//��ͣ
	bool CancelEmergencyStop(const UINT& motor);	//ȡ����ͣ
	bool SetPID(const UINT& motor,const int& param,const int& data);		//����PID
	bool SavePID(const UINT& motor);				//��������PID��ı���	
	bool SetPosZero(const UINT& motor);				//������λ
	bool SetPosition(const UINT& motor,const int& data);					//����λ��
	bool SetSpeed(const UINT& motor,const int& data);		//�����ٶ�
	bool Start(const UINT& motor);					//����λ���˶�
	bool Zero(const UINT& motor);					//����λ
};

