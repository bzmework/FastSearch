#include "memdc.h"

Win32MemDC::Win32MemDC()
{
	this->m_hDC = NULL;
	this->m_hBmp = NULL;
	this->m_hBmpOrg = NULL;
	this->m_nWidth = 0;
	this->m_nHeight = 0;
}

Win32MemDC::~Win32MemDC()
{
	this->Destroy();
}

HDC Win32MemDC::GetDC() const
{
	return this->m_hDC;
}

int Win32MemDC::Width() const
{
	return this->m_nWidth;
}

int Win32MemDC::Height() const
{
	return this->m_nHeight;
}

VOID Win32MemDC::Width(int nValue)
{
	this->m_nWidth = nValue;
}

VOID Win32MemDC::Height(int nValue)
{
	this->m_nHeight = nValue;
}

BOOL Win32MemDC::Create(RECT rcArea)
{
	return this->Create(NULL, rcArea.right - rcArea.left, rcArea.bottom - rcArea.top);
}

BOOL Win32MemDC::Create(HDC hDC, int nWidth, int nHeight)
{
	this->Destroy();

	if(nWidth <= 0 || nHeight <= 0)
	{
		nWidth = GetSystemMetrics(SM_CXSCREEN);
		nHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	if(hDC != NULL)
	{
		this->m_hDC = CreateCompatibleDC(hDC);
		this->m_hBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
		this->m_hBmpOrg = (HBITMAP)SelectObject(this->m_hDC, this->m_hBmp);
	}
	else
	{
		HDC hTempDC = CreateDC(_T("DISPLAY"), 0, 0, 0);
		this->m_hDC = CreateCompatibleDC(hTempDC);
		this->m_hBmp = CreateCompatibleBitmap(hTempDC, nWidth, nHeight);
		this->m_hBmpOrg = (HBITMAP)SelectObject(this->m_hDC, this->m_hBmp);
		DeleteDC(hTempDC);
	}
	
	if (this->m_hBmp == NULL || this->m_hBmpOrg == NULL)
	{
		this->Destroy();
	}
	else
	{
		this->m_nWidth = nWidth;
		this->m_nHeight = nHeight;
	}

	return (this->m_hDC != NULL);
}

VOID Win32MemDC::Destroy()
{
	this->m_nWidth = 0;
	this->m_nHeight = 0;
	if (this->m_hBmpOrg != NULL)
	{
		SelectObject(this->m_hDC, this->m_hBmpOrg);
		this->m_hBmpOrg = NULL;
	}
	if (this->m_hBmp != NULL)
	{
		DeleteObject(this->m_hBmp);
		this->m_hBmp = NULL;
	}
	if (this->m_hDC != NULL)
	{
		DeleteDC(this->m_hDC);
		this->m_hDC = NULL;
	}
}