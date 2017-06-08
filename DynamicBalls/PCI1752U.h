#pragma once
#include "afx.h"

#include "driver.h"
class CPCI1752U :public CObject
{
public:
	CPCI1752U(ULONG deviceNum,DWORD dwBoardID);
	~CPCI1752U(void);
private:
	ULONG		m_nDeviceNum;
	DWORD		m_nBoardID;
	LONG        m_hDriverHandle;          // driver handle
public:
	bool Open();
	bool Close();
	bool WritePortByte(UINT port,UINT data);	//index:0-7,data:0-255
	bool WritePortWord(UINT port,UINT data);	//index:0-7,data:0-65535

	bool DioWriteDoPorts(UINT portStart,UINT portCount,LPBYTE buffer);
	bool DioWriteBit(UINT index,UINT data);
	bool DioWritePortByte(UINT port,UINT mask,UINT data);
	bool DioWritePortWord(UINT port,UINT mask,UINT data);
	bool DioWritePortDword(UINT port,UINT mask,UINT data);
};

