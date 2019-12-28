
/*
	模块：IO完成端口管理
	作者：denglf
	日期：2019-04-24
	说明：IO完成端口的结构：

	完成键(CompletionKey)	IO完成端口(CreateIoCompletionPort API)
						 +------------------+
					key1 |	hFile1(重叠句柄)	|
					key2 |	hFile2(重叠句柄)	|
					key3 |	hFile3(重叠句柄)	| <--从完成端口读取数据(GetQueuedCompletionStatus API)，通常用一个工作线程来进行读取。
					...  |	...				|
						 +------------------+
						 可以将无限数量的重叠句柄加入到IO完成端口中。
						 IO完成端口的执行操作的线程数量(NumberOfConcurrentThreads)最好是CPU核心数量，以达到最佳并发。
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

	//关闭IO完成端口
	__forceinline BOOL Close()
	{
		BOOL bResult = CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
		return(bResult);
	}

	//创建IO完成端口
	__forceinline BOOL Create(int nMaxConcurrency = 0)
	{
		//创建IO完成端口，并将其与指定的设备句柄（例如文件，套接字等）关联，或者创建尚未与设备句柄关联的IO完成端口，以便稍后进行关联。
		//将打开的设备句柄的实例与IO完成端口相关联，允许进程接收涉及该设备句柄的异步IO操作通知。
		//参数FileHandle指定INVALID_HANDLE_VALUE，创建一个IO完成端口，而不将其与设备句柄关联。在这种情况下，ExistingCompletionPort参数必须为NULL, CompletionKey参数将被忽略。
		//参数NumberOfConcurrentThreads为零，则采用处理器数量的线程进行IO并发处理。
		m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, nMaxConcurrency);
		return(m_hIOCP != NULL);
	}

	//将设备增加到IO完成端口中
	//hDevice，设备句柄，可以是文件，套接字等。
	//UniqueKey, 标识设备的唯一键(完成键)，可以是一个对象
	__forceinline BOOL AddDevice(HANDLE hDevice, ULONG_PTR UniqueKey)
	{
		//将IO完成端口与指定的设备句柄（例如文件，套接字等）关联。
		//参数CompletionKey用来唯一标识与IO完成端口关联的设备。
		BOOL bOk = (CreateIoCompletionPort(hDevice, m_hIOCP, UniqueKey, 0) == m_hIOCP);
		return bOk;
	}

	//触发IO完成端口，让其工作起来
	__forceinline BOOL PostStatus(ULONG_PTR UniqueKey, DWORD dwNumBytes = 0, OVERLAPPED *pOA = NULL)
	{
		BOOL bOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, UniqueKey, pOA);
		return bOk;
	}

	//从IO完成端口读取数据
	__forceinline BOOL GetStatus(ULONG_PTR* pUniqueKey, PDWORD pdwNumBytes, OVERLAPPED** ppOA, DWORD dwMilliseconds = INFINITE)
	{
		return(GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pUniqueKey, ppOA, dwMilliseconds));
	}

private:

	HANDLE m_hIOCP;

};

#endif //__IOCP_H__