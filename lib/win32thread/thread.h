#ifndef __WIN32_THREAD_H__
#define __WIN32_THREAD_H__

#include <windows.h>

class Win32Thread
{
public:
	//�߳����ȼ�
	enum THREAD_PRIORITY_LEVEL
	{
		ThreadIdle			= -15,	//���У�THREAD_PRIORITY_IDLE
		ThreadLowest		= -2,	//��ͣ�THREAD_PRIORITY_LOWEST
		ThreadBelowNormal	= -1,	//����������THREAD_PRIORITY_BELOW_NORMAL
		ThreadNormal		= 0,	//������THREAD_PRIORITY_NORMAL
		ThreadAboveNormal	= 1,	//����������THREAD_PRIORITY_ABOVE_NORMAL
		ThreadHighest		= 2,	//��ߣ�THREAD_PRIORITY_HIGHEST
		ThreadRealtime		= 15	//ʵʱ��THREAD_PRIORITY_TIME_CRITICAL
	};

public:
	//�߳�ִ�к���
	typedef VOID (*THREAD_EXECUTE_FUNCTION)(LPVOID lpvArg);

public:
	Win32Thread();
	virtual ~Win32Thread();

	BOOL Create(THREAD_EXECUTE_FUNCTION lpfnFunc, LPVOID lpvArg, BOOL bImmeditExcute = TRUE);
	VOID Stop(BOOL bImmeditExcute = FALSE, DWORD dwMillisecondsTimeout = 500);
	VOID Pause();
	VOID Resume();
	VOID Join();
	BOOL Join(DWORD dwMillisecondsTimeout);
	VOID Sleep(DWORD dwMillisecondsTimeout);

	BOOL IsRunning() const;
	BOOL IsStopping() const;

	int GetPriority() const;
	int SetPriority(THREAD_PRIORITY_LEVEL enmPriority);
	int SetPriority(int nPriority);

    static DWORD CALLBACK ThreadExecuteFunction(LPVOID lpvArg);
    DWORD InternalThreadExecuteFunction(); 

private:
	HANDLE m_hThreadHandle;	
	DWORD m_dwThreadId;

    THREAD_EXECUTE_FUNCTION m_lpfnFunc;
    LPVOID m_lpvArg;

	volatile BOOL m_bIsStopping;

	VOID Destroy();

};

#endif /* __WIN32_THREAD_H__ */
