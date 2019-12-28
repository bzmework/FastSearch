
#include "timer.h"

CTimer::CTimer()
{
	m_uTimerId = 0;
	m_uInterval = 0;
	m_bEnabled = false;
	m_lpfnOnTick = NULL;
}

CTimer::~CTimer()
{
	if(m_uTimerId != 0)
	{
		KillTimer(NULL, m_uTimerId);
		m_uTimerId = 0;
	}
}

BOOL CTimer::Create(ONTICK_EVENT lpfnOnTick)
{
	if(m_uInterval == 0 || lpfnOnTick == NULL)
	{
		return FALSE;
	}
	else
	{
		m_lpfnOnTick = lpfnOnTick;
		return TRUE;
	}
}

UINT CTimer::TimerId() const
{
	return m_uTimerId;
}

UINT CTimer::Interval() const
{
	return m_uInterval;
}

VOID CTimer::Interval(UINT uValue)
{
	m_uInterval = uValue;
}

VOID CTimer::Enabled(BOOL bValue)
{
	m_bEnabled = bValue;
}

void CALLBACK CTimer::TimerProc(HWND hWnd, UINT uMsg, UINT uTimerId, DWORD dwTime)
{
	/*char timerId[16] = {0};
	sprintf_s(timerId, "T%d", idEvent);
	void* timerObject = GetProp((HWND)&idEvent, timerId);
	if(timerObject != NULL)
	{
		CTimer* timer = (CTimer*)timerObject;
		if(timer->m_enabled)
		{
			timer->m_onTick();
		}
	}*/
}