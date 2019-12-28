
/*
	ģ�飺�ڴ����
	���ߣ�denglf
	���ڣ�2019-04-24
	˵���������ֵ�������ڴ����
	ע�⣺
	ʵ��֤�������ڴ��ʺ�һ�η���MB���Ͻϴ���ڴ�����ٶȿ죬
	malloc�Ƚ��ʺϷ����С���ڴ�����ٶȿ죨��HeapAlloc��VirtualAlloc���죩
*/

#ifndef __VMEMALLOC_H_
#define __VMEMALLOC_H_

#include <windows.h>

class VMemAlloc
{
public:

	VMemAlloc()
	{
		//
	}

	virtual ~VMemAlloc()
	{
		//
	}

	//�����ڴ�
	LPVOID Alloc(SIZE_T size)
	{

		/*
		������
		LPVOID VirtualAlloc(
		  LPVOID lpAddress,
		  SIZE_T dwSize,
		  DWORD  flAllocationType,
		  DWORD  flProtect
		);

		���ܣ�
		�ڵ��ý��̵������ַ�ռ��б������ύ�����ҳ�������״̬��
		�������������ڴ汻�Զ���ʼ��Ϊ�㡣
			
		lpAddress��
		�ڴ��ַ����������ϵͳ������ҪԤ����ַ�ռ��е���һ�飬����ϵͳ���¼�������õ�����
		���ֻ��Ҫ���ò������ó�NULL���ɣ�����ڸ���ϵͳ�Զ���һ�顣

		dwSize��
		Ԥ���������С�����ֽ�Ϊ��λ��
			
		flAllocationType��
		��������ϵͳֻ��Ԥ��������Ҫ�������������������ڴ�ֻ�е��������������Ժ����ʹ�á�
		������Ǵ���Ԥ��һ������ܳ�һ��ʱ�䣬��ϣ��ϵͳ�Ӿ����ܸߵ��ڴ��ַ��Ԥ������
		�Է�ֹ�ڽ��̵�ַ�ռ���м�Ԥ�����򣬴Ӷ����������ڴ���Ƭ��Ϊ�˴ﵽ��Ŀ��Ӧ�������µ�
		��ʽ����VirtualAlloc��
				
			VirtualAlloc(NULL, dwSize, MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE);
			
		flProtect��
		�����������ı������ԡ�����ı������Խ�����������ַ�������Ե�������������������������κ����á�

		��Ԥ������������ǻ���Ҫ��������������������������ܷ������е��ڴ��ַ��
		ϵͳ���ҳ�潻���ļ��е���������������Ϊ�˵����������������Ǳ����ٴε���VirtualAlloc��
		�����Ҫ��MEM_COMMIT��flAllocationType���������£�

			VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);

		��Ȼ���������Ԥ�������ͬʱ���������������������ɣ�

			VirtualAlloc(NULL, dwSize, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);

		*/

		LPVOID pAddress = NULL;
		
		//��������ɹ�������ֵ�Ƿ����ҳ������Ļ�����ַ��
		//�������ʧ�ܣ�����ֵΪNULL��Ҫ��ȡ��չ�Ĵ�����Ϣ�������GetLastError��
		pAddress = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		
		//����
		return pAddress;

	}

	//�ͷ��ڴ�
	BOOL Free(LPVOID pAddress)
	{
		/*
		������
		BOOL VirtualFree(
		  LPVOID lpAddress,
		  SIZE_T dwSize,
		  DWORD  dwFreeType
		);

		���ܣ�
		�ͷŵ��������������������

		lpAddress��
		VirtualAlloc���صĵ�ַ��

		dwSize��
		Ҫ�ͷŵ�����洢����С������һ����ַ��ϵͳ֪��λ�ڸõ�ַ��������Ĵ�С����˴�0��dwSize���ɡ�

		dwFreeType��
		�������ͣ������Ҫ���������������һ�����������������ֲ����ͷ��������򣬿���������

			VirtualFree(lpAddress, dwSize, MEM_DECOMMIT);

		���dwSizeΪ0����lpAddress������Ļ���ַ����ôVirtualFree�᳷�������������������ҳ�棬���£�
			
			VirtualFree(lpAddress, 0, MEM_RELEASE);

		һ��ϵͳ�����˵�����ҳ����������������ͷŵ����������Ϳ�������ϵͳ�е��������̣�
		��ͼ�����ѳ����������ڴ��ַ����������Υ�档

		*/
		
		BOOL bOk = FALSE;
		if (pAddress != NULL)
		{
			//��������ɹ�������ֵΪ���㡣
			//�������ʧ�ܣ�����ֵΪ0(0)��Ҫ��ȡ��չ�Ĵ�����Ϣ�������GetLastError��
			bOk = VirtualFree(pAddress, 0, MEM_RELEASE);
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

#endif //__VMEMALLOC_H_

