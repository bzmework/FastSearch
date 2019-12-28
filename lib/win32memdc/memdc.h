#ifndef __WIN32MEMDC_H__
#define __WIN32MEMDC_H__

#include <windows.h>
#include <tchar.h>

//
//ÄÚ´æDC
//
class Win32MemDC
{
public:
	Win32MemDC();
	virtual ~Win32MemDC();

	HDC GetDC() const;

	int Width() const;
	VOID Width(int nValue);

	int Height() const;
	VOID Height(int nValue);

	BOOL Create(RECT rcArea);
	BOOL Create(HDC hDC = NULL, int nWidth = 0, int nHeight = 0);

	VOID Destroy();

private:
	HDC m_hDC;
	HBITMAP m_hBmp;
	HBITMAP m_hBmpOrg;

	int m_nWidth;
	int m_nHeight;

};

#endif //__WIN32MEMDC_H__