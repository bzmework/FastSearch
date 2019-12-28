
/*
	ģ�飺IO��ɶ˿ڶ�д����ʱ�������OVERLAPPED�ṹ
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����IO��ɶ˿���ҪOVERLAPPED�ṹ�������н��������ÿ��IO������Ҫһ��OVERLAPPED�ṹ��һ�����ݻ��档
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
		//���ٻ���
		if (m_pvData != NULL)
		{
			BOOL bOk = VirtualFree(m_pvData, 0, MEM_RELEASE);
		}
	}

	//���仺�棬IO��ɶ˿���ִ��IO����ʱ������������
	BOOL Alloc(SIZE_T dwSize)
	{
		m_nBuffSize = dwSize;
		m_pvData = VirtualAlloc(NULL, m_nBuffSize, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		return(m_pvData != NULL);
	}

	//��ȡ�ļ���¼
	__forceinline BOOL ReadUsn(HANDLE hDevice, PMFT_ENUM_DATA pmed, PLARGE_INTEGER pliOffset = NULL)
	{
		//���ж�λ
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		//��ȡ����
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

	//���豸��ȡ����
	__forceinline BOOL Read(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL)
	{
		//���ж�λ
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		//��ȡ����
		return(::ReadFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}

	//���豸д������
	__forceinline BOOL Write(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL)
	{
		//���ж�λ
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		//д������
		return(::WriteFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}
};

#endif //__IOCPREQ_H__