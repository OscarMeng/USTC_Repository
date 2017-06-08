#include "StdAfx.h"
#include "Canet.h"


CCanet::CCanet(DWORD deviceInd,DWORD canInd,int ip)
	:m_nDeviceInd(deviceInd),
	m_nCanInd(canInd),
	m_nIP(ip)
{
	m_dZToW = 0;
	m_dLToS = 0;

//	Connect();
}


CCanet::~CCanet(void)
{
//	Close();
}

void CCanet::Connect()
{
	if(!VCI_CloseDevice(VCI_CANETE,m_nDeviceInd))
	{
		CString msg;
		msg.Format("关闭CANET－%d 失败!",m_nDeviceInd);
		AfxMessageBox(msg);
	}

	if(VCI_OpenDevice(VCI_CANETE,m_nDeviceInd,CANETPORT+m_nDeviceInd))
	{
		char ip[20];
		sprintf_s(ip,"192.168.0.%d",m_nIP);
		int srcport = 4001;
		
		VCI_SetReference(VCI_CANETE,m_nDeviceInd,m_nCanInd,CMD_DESIP,(PVOID)ip);			//设置所要操作的CANET-E的IP地址
		VCI_SetReference(VCI_CANETE,m_nDeviceInd,m_nCanInd,CMD_DESPORT,(PVOID)&srcport);			//设置所要操作的CANET-E的工作端口
	}
	else
	{
		CString msg;
		msg.Format("打开CANET－%d 失败!",m_nDeviceInd);
		AfxMessageBox(msg);
	}
}

void CCanet::Close()
{
	if(!VCI_CloseDevice(VCI_CANETE,m_nDeviceInd))
	{
		CString msg;
		msg.Format("关闭CANET－%d 失败!",m_nDeviceInd);
		AfxMessageBox(msg);
	}
}


bool CCanet::Transmit(PVCI_CAN_OBJ vciCanObj,ULONG len)
{
	if(VCI_Transmit(VCI_CANETE,m_nDeviceInd,m_nCanInd,vciCanObj,len) != len)
	{
		AfxMessageBox("发送数据错误");
		return false;
	}

	return true;
}


bool CCanet::SendCmd(const UINT& motor,const int& cmd,const int& data)
{
	VCI_CAN_OBJ canCmd;		//数据发送结构体
	canCmd.Data[0] = 0;
	canCmd.Data[1] = cmd;
	canCmd.Data[2] = (data & 0x000f);
	canCmd.Data[3] = (data & 0x00f0) >> 8;
	canCmd.Data[4] = (data & 0x0f00) >> 16;
	canCmd.Data[5] = (data & 0xf000) >> 24;
	canCmd.Data[6] = 0;
	canCmd.Data[7] = 0;
	canCmd.DataLen = 8;
	canCmd.RemoteFlag = 0;
	canCmd.ExternFlag = 1;
	if(motor == 99)
		canCmd.ID = 0;
	else
		canCmd.ID = motor + ((m_nDeviceInd%2)?MOTOID1:MOTOID0);

	return Transmit(&canCmd,1);
}

bool CCanet::LoadMotor(const UINT& motor)
{
	int cmd = 128;
	int data = 0;
	return SendCmd(motor,cmd,data);
}

bool CCanet::SetModePos(const UINT& motor)
{
	int cmd = 42;	
	int data = 129 + (2<<8);
	return SendCmd(motor,cmd,data);
}

bool CCanet::EmergencyStop(const UINT& motor)
{
	int cmd = 132;
	int data = 0;
	return SendCmd(motor,cmd,data);
}

bool CCanet::CancelEmergencyStop(const UINT& motor)
{
	int cmd = 133;
	int data = 0;
	return SendCmd(motor,cmd,data);
}


bool CCanet::SetPID(const UINT& motor,const int& param,const int& data)
{
	int cmd = 0;
	if(param == 1)	
		cmd = 96;
	else if(param == 2)
		cmd = 98;
	else if(param == 3)
		cmd = 100;
	else if(param == 4)	
		cmd = 102;
	else if(param == 5)	
		cmd = 104;

	return SendCmd(motor,cmd,data);
}

bool CCanet::SavePID(const UINT& motor)
{
	int cmd = 130;
	int data = 0;
	return SendCmd(motor,cmd,data);
}

bool CCanet::SetPosZero(const UINT& motor)
{
	int cmd = 152;
	int data = 0;
	return SendCmd(motor,cmd,data);
}


bool CCanet::SetPosition(const UINT& motor,const int& data)
{
	int cmd = 158;
	return SendCmd(motor,cmd,data);
}

bool CCanet::SetSpeed(const UINT& motor,const int& data)
{
	int cmd = 22;
	return SendCmd(motor,cmd,data);
}

bool CCanet::Start(const UINT& motor)
{
	int cmd = 160;
	int data = 0;
	return SendCmd(motor,cmd,data);
}


bool CCanet::Zero(const UINT& motor)
{
	int cmd = 153;
	int data = 0;
	return SendCmd(motor,cmd,data);
}
