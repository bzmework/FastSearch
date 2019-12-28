
/*

	ģ�飺�ڴ����
	���ߣ�denglf
	���ڣ�2019-04-24
	˵���������ֵ�Ķ��ڴ�������Է��ִ�����ʱ���Ϸ��䡢��д�ٶȶ��Ƚ�����
*/

#ifndef __HMEMALLOC_H_
#define __HMEMALLOC_H_

#include <windows.h>

class HMemAlloc
{
private:
	HANDLE hHeap; //�Ѿ��

public:

	HMemAlloc()
	{
		//
		hHeap = NULL;
	}

	virtual ~HMemAlloc()
	{
		//���ٴ����Ķ�
		if (hHeap != NULL)
		{
			HeapDestroy(hHeap);
		}
	}

	//��ʼ�� 
	BOOL Initialize(SIZE_T size)
	{
		/*
		������
		HANDLE HeapCreate(
		  DWORD  flOptions,
		  SIZE_T dwInitialSize,
		  SIZE_T dwMaximumSize
		);

		���ܣ�
		�������ɵ��ý���ʹ�õ�˽�жѶ���

		dwInitialSize��
		һ��ʼҪ�������ѵ��ֽ�����

		dwMaximumSize��
		�����ޣ�Ϊ0��ʾ�Ӷ��з����ڴ��ʹ���Զ�������ֱ���þ����������ڴ�λ�á�
		*/

		//��������ɹ�������ֵ���´����ѵľ����
		//�������ʧ�ܣ�����ֵΪNULL��Ҫ��ȡ��չ�Ĵ�����Ϣ�������GetLastError��
		hHeap = HeapCreate(0, size, 0);
	
		/*
		if (hHeap != NULL)
		{
			//���ѵ����ɵ���Ƭ��
			//��������ɹ�������ֵΪ���㡣
			//�������ʧ�ܣ�����ֵΪ0��Ҫ��ȡ��չ�Ĵ�����Ϣ�������GetLastError��
			//HeapCompatibilityInformation=0
			ULONG ic = 2;
			BOOL bOk = FALSE;
			bOk = HeapSetInformation(hHeap, HeapCompatibilityInformation, &ic, sizeof(ic));
			if (bOk)
			{
				//�����ɵ���Ƭ�ѳɹ�
			}
		}
		*/

		return (hHeap != NULL);

	}

	//�����ڴ�
	LPVOID Alloc(SIZE_T size)
	{

		/*
		������
		DECLSPEC_ALLOCATOR LPVOID HeapAlloc(
		  HANDLE hHeap,
		  DWORD  dwFlags,
		  SIZE_T dwBytes
		);

		���ܣ�
		�Ӷ��з����ڴ�顣������ڴ治���ƶ���

		*/

		LPVOID pAddress = NULL;

		//��������ɹ�������ֵ��ָ���ѷ����ڴ���ָ�롣
		//�������ʧ�ܣ�����û��ָ��dwFlagsΪHEAP_GENERATE_EXCEPTION(0x00000004)���򷵻�ֵΪNULL��
		//Ҫ��ȡ��չ�Ĵ�����Ϣ�������GetExceptionCode/GetLastError��
		if (hHeap != NULL)
		{
			pAddress = HeapAlloc(hHeap, 0, size);
		}

		//����
		return pAddress;

	}

	//�ͷ��ڴ�
	BOOL Free(LPVOID pAddress)
	{
		/*
		������
		BOOL HeapFree(
		  HANDLE                 hHeap,
		  DWORD                  dwFlags,
		  _Frees_ptr_opt_ LPVOID lpMem
		);

		���ܣ�
		�ͷ���HeapAlloc��HeapReAlloc�����Ӷ��з�����ڴ�顣

		hHeap��
		Ҫ�ͷ��ڴ��Ķѵľ������������HeapCreate��GetProcessHeap�������ء�

		lpMem��
		ָ��Ҫ�ͷŵ��ڴ���ָ�롣���ָ����HeapAlloc��HeapReAlloc�������ء������ָ��Ϊ�գ�����Ϊδ֪��

		*/

		BOOL bOk = FALSE;
		if (hHeap != NULL && pAddress != NULL)
		{
			//��������ɹ�������ֵΪ���㡣
			//�������ʧ�ܣ�����ֵΪ�㡣Ӧ�ó�����Ե���GetLastError��ȡ��չ�Ĵ�����Ϣ��
			bOk = HeapFree(hHeap, 0, pAddress);
		}
		return bOk;

	}

private:

	void OutputErr()
	{
		LPTSTR lpMsgBuf;
		DWORD nErrno = GetLastError();
		if (nErrno != 0)
		{
			FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrno,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);

			MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION);

			LocalFree(lpMsgBuf);
		}
	}

};

#endif //__HMEMALLOC_H_

