#ifndef __WIN32SYNC_H__
#define __WIN32SYNC_H__

#include <windows.h>

//
// 临界区
//
class Win32CritSec 
{
public:
	Win32CritSec();
	virtual ~Win32CritSec();
    VOID Lock();
	VOID Unlock();

private:
	// make copy constructor and assignment operator inaccessible
	Win32CritSec(const Win32CritSec &refCritSec);
    Win32CritSec &operator=(const Win32CritSec &refCritSec);

	CRITICAL_SECTION m_CritSec;
};

//
// 自动锁
//
class Win32AutoLock 
{
public:
    Win32AutoLock(Win32CritSec * plock);
    ~Win32AutoLock();

protected:
    Win32CritSec* m_pLock;

private:
    // make copy constructor and assignment operator inaccessible
    Win32AutoLock(const Win32AutoLock &refAutoLock);
    Win32AutoLock &operator=(const Win32AutoLock &refAutoLock);
};

#endif  //__WIN32SYNC_H__
