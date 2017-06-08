#include "StdAfx.h"
#include "PCI1752U.h"


CPCI1752U::CPCI1752U(ULONG deviceNum,DWORD boardID)
	:m_nDeviceNum(deviceNum),
	m_nBoardID(boardID)
{
	Open();
}


CPCI1752U::~CPCI1752U(void)
{ 
	Close();
}

bool CPCI1752U::Open()
{
	// open device
	LRESULT errCode = DRV_DeviceOpen(m_nDeviceNum,(LONG far*)&m_hDriverHandle);
	if (errCode != SUCCESS)
	{
		CString errMsg = "Device open error.";
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}

bool CPCI1752U::Close()
{
	LRESULT errCode = DRV_DeviceClose((LONG far *)&m_hDriverHandle);
	if(errCode != SUCCESS)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}
	return true;
}


bool CPCI1752U::WritePortByte(UINT port,UINT data) 
{
	// TODO: Add your control notification handler code here
	if(port< 0 || port > 7)
	{
		AfxMessageBox("WritePortByte:port must be 0 ~ 7.");
		return false;
	}

	PT_WritePortByte writePortByte;
	writePortByte.port  = (USHORT)port;
	writePortByte.ByteData = (USHORT)(data&0xff);

	LRESULT errCode = DRV_WritePortByte(m_hDriverHandle,&writePortByte);
	if(errCode != 0)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}

bool CPCI1752U::WritePortWord(UINT port,UINT data)
{
	if(port < 0 || port > 7)
	{
		AfxMessageBox("WriteWord:index must be 0 ~ 7.");
		return false;
	}

	PT_WritePortWord writePortWord;
	writePortWord.port  = (USHORT)port;
	writePortWord.WordData = (USHORT)(data&0xffff);

	LRESULT errCode = DRV_WritePortWord(m_hDriverHandle,&writePortWord);
	if(errCode != 0)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}


bool CPCI1752U::DioWriteDoPorts(UINT portStart,UINT portCount,LPBYTE buffer)
{
	if(portStart < 0 || portStart > 7)
	{
		AfxMessageBox("DioWriteDoPorts:portStart must be 0 ~ 7.");
		return false;
	}
	if((portStart+portCount) > 8)
	{
		AfxMessageBox("DioWriteDoPorts:portStart plus portCount can't be greater than 8.");
		return false;
	}

	LRESULT errCode = AdxDioWriteDoPorts(m_hDriverHandle,(ULONG)portStart,(ULONG)portCount,buffer);
	if(errCode != 0)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}

bool CPCI1752U::DioWriteBit(UINT index,UINT data)
{
	if(index < 0 || index > 63)
	{
		AfxMessageBox("DioWriteBit:index must be 0 ~ 63.");
		return false;
	}
	PT_DioWriteBit dioWriteBit;
	dioWriteBit.port = (USHORT)(index/8);
	dioWriteBit.bit = (USHORT)(index%8);
	dioWriteBit.state = (USHORT)((data==0)?0:1);

	LRESULT errCode = DRV_DioWriteBit(m_hDriverHandle,&dioWriteBit);
	if (errCode != SUCCESS)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}

bool CPCI1752U::DioWritePortByte(UINT port,UINT mask,UINT data)
{
	if(port < 0 || port > 7)
	{
		AfxMessageBox("DioWriteByte:port must be 0 ~ 7.");
		return false;
	}
	PT_DioWritePortByte dioWritePortByte;
	dioWritePortByte.port = (USHORT)port;
	dioWritePortByte.mask = (USHORT)(mask&0xff);
	dioWritePortByte.state = (USHORT)(data&0xff);

	LRESULT errCode = DRV_DioWritePortByte(m_hDriverHandle,&dioWritePortByte);
	if (errCode != SUCCESS)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}

bool CPCI1752U::DioWritePortWord(UINT port,UINT mask,UINT data)
{
	if(port < 0 || port > 7)
	{
		AfxMessageBox("DioWriteWord:port must be 0 ~ 7.");
		return false;
	}
	PT_DioWritePortWord dioWritePortWord;
	dioWritePortWord.port = (USHORT)port;
	dioWritePortWord.mask = (USHORT)(mask&0xffff);
	dioWritePortWord.state = (USHORT)(data&0xffff);

	LRESULT errCode = DRV_DioWritePortWord(m_hDriverHandle,&dioWritePortWord);
	if (errCode != SUCCESS)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}

bool CPCI1752U::DioWritePortDword(UINT port,UINT mask,UINT data)
{
	if(port < 0 || port > 7)
	{
		AfxMessageBox("DioWriteDword:port must be 0 ~ 7.");
		return false;
	}
	PT_DioWritePortDword dioWritePortDword;
	dioWritePortDword.port = (USHORT)port;
	dioWritePortDword.mask = (ULONG)mask;
	dioWritePortDword.state = (ULONG)data;

	LRESULT errCode = DRV_DioWritePortDword(m_hDriverHandle,&dioWritePortDword);
	if (errCode != SUCCESS)
	{
		CString errMsg;
		DRV_GetErrorMessage(errCode,errMsg.GetBuffer());
		errMsg.ReleaseBuffer();
		AfxMessageBox(errMsg);
		return false;
	}

	return true;
}