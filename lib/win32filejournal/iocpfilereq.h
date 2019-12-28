
/*
	模块：IO完成端口读写数据时，所需的OVERLAPPED结构
	作者：denglf
	日期：2019-04-24
	说明：IO完成端口需要OVERLAPPED结构与外界进行交换，因此每个IO请求都需要一个OVERLAPPED结构和一个数据缓存。
*/

#ifndef __IOCPREQ_H__
#define __IOCPREQ_H__

#include <windows.h>

class IoCompletionPortFileReq : public OVERLAPPED
{
public:
	SIZE_T m_nBuffSize;
	PVOID  m_pvData;

	IoCompletionPortFileReq()
	{
		Internal = InternalHigh = 0;
		Offset = OffsetHigh = 0;
		hEvent = NULL;
		m_nBuffSize = 0;
		m_pvData = NULL;
	}

	~IoCompletionPortFileReq()
	{
		//销毁缓存
		if (m_pvData != NULL)
		{
			BOOL bOk = VirtualFree(m_pvData, 0, MEM_RELEASE);
		}
	}

	//分配缓存，IO完成端口在执行IO请求时用来缓存数据
	BOOL Alloc(SIZE_T dwSize)
	{
		m_nBuffSize = dwSize;
		m_pvData = VirtualAlloc(NULL, m_nBuffSize, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		return(m_pvData != NULL);
	}

	//读取文件记录
	__forceinline BOOL ReadUsn(HANDLE hDevice, PMFT_ENUM_DATA pmed, PLARGE_INTEGER pliOffset = NULL)
	{
		//进行定位
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		//读取数据
		return(
			DeviceIoControl(hDevice,
			FSCTL_ENUM_USN_DATA,
			pmed,
			sizeof(MFT_ENUM_DATA),
			m_pvData,
			m_nBuffSize,
			NULL,
			this));
	}

	//从设备读取数据
	__forceinline BOOL Read(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL)
	{
		//进行定位
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		//读取数据
		return(::ReadFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}

	//向设备写入数据
	__forceinline BOOL Write(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL)
	{
		//进行定位
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		//写入数据
		return(::WriteFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}
};

#endif //__IOCPREQ_H__