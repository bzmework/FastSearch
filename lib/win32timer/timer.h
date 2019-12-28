#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>
#include "../array/array.h"

typedef VOID (CALLBACK *ONTICK_EVENT)();

//typedef struct _TIMER_OBJECT
//{
//	UINT uTimerId;
//	CTimer* pTimer;
//} TIMER_OBJECT;
//
//TArray<TIMER_OBJECT> g_Timers;

class CTimer
{
private:
	UINT m_uTimerId;
	UINT m_uInterval;
	BOOL m_bEnabled;
	
	ONTICK_EVENT m_lpfnOnTick;
	static VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT uTimerId, DWORD dwTime);

public:
	CTimer();
	virtual ~CTimer();
	
	BOOL Create(ONTICK_EVENT lpfnOnTick);
	UINT TimerId() const;
	UINT Interval() const;
	VOID Interval(UINT uValue);
	VOID Enabled(BOOL bValue);
	
};
#endif //__TIMER_H__