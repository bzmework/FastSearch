#include "sync.h"

Win32CritSec::Win32CritSec() 
{ 
	InitializeCriticalSection(&m_CritSec); 
}

Win32CritSec::~Win32CritSec() 
{ 
	DeleteCriticalSection(&m_CritSec); 
}

VOID Win32CritSec::Lock() 
{ 
	EnterCriticalSection(&m_CritSec); 
}

VOID Win32CritSec::Unlock() 
{ 
	LeaveCriticalSection(&m_CritSec); 
}

Win32AutoLock::Win32AutoLock(Win32CritSec * plock)
{
    m_pLock = plock;
    m_pLock->Lock();
}

Win32AutoLock::~Win32AutoLock() 
{
    m_pLock->Unlock();
}