
#ifndef __WIN32_EXCEPTION_H__
#define __WIN32_EXCEPTION_H__

#include <crtdbg.h>
#include <errno.h>
#include <stdlib.h>
#include <sal.h>

static BOOL OAssertFailedLine(LPCSTR lpszFileName, int nLine)
{
	// we remove WM_QUIT because if it is in the queue then the message box
	// won't display
	//MSG msg;
	//BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
	//BOOL bResult = _CrtDbgReport(_CRT_ASSERT, lpszFileName, nLine, NULL, NULL);
	//if (bQuit) PostQuitMessage((int)msg.wParam);
	//return bResult;
	return 0;
}

#ifndef OASSERT
#define OASSERT(expr) _ASSERTE(expr)
#endif

#ifndef OASSUME
#define OASSUME(expr) do { OASSERT(expr); _Analysis_assume_(!!(expr)); } __pragma(warning(suppress:4127)) while (0)
#endif 

#ifndef OVERIFY
#define DEBUG_ONLY(f) (f)
#define OVERIFY(f)	DEBUG_ONLY((void) ((f) || !OAssertFailedLine(__FILE__, __LINE__) || (__debugbreak(), 0)))
#endif

#endif /* __WIN32_EXCEPTION_H__ */