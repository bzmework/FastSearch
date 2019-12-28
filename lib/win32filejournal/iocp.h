
/*
	ģ�飺IO��ɶ˿ڹ���
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����IO��ɶ˿ڵĽṹ��

	��ɼ�(CompletionKey)	IO��ɶ˿�(CreateIoCompletionPort API)
						 +------------------+
					key1 |	hFile1(�ص����)	|
					key2 |	hFile2(�ص����)	|
					key3 |	hFile3(�ص����)	| <--����ɶ˿ڶ�ȡ����(GetQueuedCompletionStatus API)��ͨ����һ�������߳������ж�ȡ��
					...  |	...				|
						 +------------------+
						 ���Խ������������ص�������뵽IO��ɶ˿��С�
						 IO��ɶ˿ڵ�ִ�в������߳�����(NumberOfConcurrentThreads)�����CPU�����������Դﵽ��Ѳ�����
*/

#ifndef __IOCP_H__
#define __IOCP_H__

#include <windows.h>

class IoCompletionPort
{
public:
	IoCompletionPort(int nMaxConcurrency = -1)
	{
		m_hIOCP = NULL;
		if (nMaxConcurrency != -1)
		{
			(void)Create(nMaxConcurrency);
		}
	}

	~IoCompletionPort()
	{
		if (m_hIOCP != NULL)
		{
			CloseHandle(m_hIOCP);
		}

	}

	//�ر�IO��ɶ˿�
	__forceinline BOOL Close()
	{
		BOOL bResult = CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
		return(bResult);
	}

	//����IO��ɶ˿�
	__forceinline BOOL Create(int nMaxConcurrency = 0)
	{
		//����IO��ɶ˿ڣ���������ָ�����豸����������ļ����׽��ֵȣ����������ߴ�����δ���豸���������IO��ɶ˿ڣ��Ա��Ժ���й�����
		//���򿪵��豸�����ʵ����IO��ɶ˿��������������̽����漰���豸������첽IO����֪ͨ��
		//����FileHandleָ��INVALID_HANDLE_VALUE������һ��IO��ɶ˿ڣ������������豸�������������������£�ExistingCompletionPort��������ΪNULL, CompletionKey�����������ԡ�
		//����NumberOfConcurrentThreadsΪ�㣬����ô������������߳̽���IO��������
		m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, nMaxConcurrency);
		return(m_hIOCP != NULL);
	}

	//���豸���ӵ�IO��ɶ˿���
	//hDevice���豸������������ļ����׽��ֵȡ�
	//UniqueKey, ��ʶ�豸��Ψһ��(��ɼ�)��������һ������
	__forceinline BOOL AddDevice(HANDLE hDevice, ULONG_PTR UniqueKey)
	{
		//��IO��ɶ˿���ָ�����豸����������ļ����׽��ֵȣ�������
		//����CompletionKey����Ψһ��ʶ��IO��ɶ˿ڹ������豸��
		BOOL bOk = (CreateIoCompletionPort(hDevice, m_hIOCP, UniqueKey, 0) == m_hIOCP);
		return bOk;
	}

	//����IO��ɶ˿ڣ����乤������
	__forceinline BOOL PostStatus(ULONG_PTR UniqueKey, DWORD dwNumBytes = 0, OVERLAPPED *pOA = NULL)
	{
		BOOL bOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, UniqueKey, pOA);
		return bOk;
	}

	//��IO��ɶ˿ڶ�ȡ����
	__forceinline BOOL GetStatus(ULONG_PTR* pUniqueKey, PDWORD pdwNumBytes, OVERLAPPED** ppOA, DWORD dwMilliseconds = INFINITE)
	{
		return(GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pUniqueKey, ppOA, dwMilliseconds));
	}

private:

	HANDLE m_hIOCP;

};

#endif //__IOCP_H__