#ifndef __WIN32IMAGE_H__
#define __WIN32IMAGE_H__

#include <windows.h>
#include <gdiplus.h>
#include "../utility/utility.h"

const int CIMAGE_DC_CACHE_SIZE = 4;

class Win32Image;
class Win32ImageDC
{
public:
	Win32ImageDC(Win32Image& image);
	~Win32ImageDC();
	HDC GetDC();

private:
	Win32Image& m_image;
	HDC m_hDC;

};

class Win32Image
{
private:

	//
	class Win32CacheDC
	{
	public:
		Win32CacheDC();
		virtual ~Win32CacheDC();

		HDC GetDC();
		VOID ReleaseDC(HDC hDC);

	private:
		HDC m_ahDCs[CIMAGE_DC_CACHE_SIZE];
	};

	//
	class Win32GDIPlus
	{
	public:
		Win32GDIPlus();
		virtual ~Win32GDIPlus();

		BOOL Init();
		VOID ReleaseGDIPlus();
		VOID IncreaseImageCount();
		VOID DecreaseImageCount();

	private:
		ULONG_PTR m_dwToken;
		CRITICAL_SECTION m_sect;
		LONG m_nImageObjects;
	};

public:
	enum DIBOrientation
	{
		DIBOR_DEFAULT,
		DIBOR_TOPDOWN,
		DIBOR_BOTTOMUP
	};

	static const DWORD createAlphaChannel = 0x01;

	static const DWORD excludeGIF = 0x01;
	static const DWORD excludeBMP = 0x02;
	static const DWORD excludeEMF = 0x04;
	static const DWORD excludeWMF = 0x08;
	static const DWORD excludeJPEG = 0x10;
	static const DWORD excludePNG = 0x20;
	static const DWORD excludeTIFF = 0x40;
	static const DWORD excludeIcon = 0x80;
	static const DWORD excludeOther = 0x80000000;
	static const DWORD excludeDefaultLoad = 0;
	static const DWORD excludeDefaultSave = excludeIcon|excludeEMF|excludeWMF;
	static const DWORD excludeValid = 0x800000ff;

	Win32Image();
	virtual ~Win32Image();

	operator HBITMAP() const;

	BOOL AlphaBlend(HDC hDestDC, int xDest, int yDest, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER);
	BOOL AlphaBlend( HDC hDestDC, const POINT& pointDest, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER);
	BOOL AlphaBlend( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER);
	BOOL AlphaBlend( HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER);

	void Attach(HBITMAP hBitmap, DIBOrientation eOrientation = DIBOR_DEFAULT);
	HBITMAP Detach();
	
	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, DWORD dwROP = SRCCOPY);
	BOOL BitBlt( HDC hDestDC, const POINT& pointDest, DWORD dwROP = SRCCOPY);
	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP = SRCCOPY);
	BOOL BitBlt( HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, DWORD dwROP = SRCCOPY);
	
	BOOL Create( int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0);
	BOOL CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, const DWORD* pdwBitmasks = NULL, DWORD dwFlags = 0);
	VOID Destroy();

	BOOL Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);
	BOOL Draw(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc);
	BOOL Draw(HDC hDestDC, int xDest, int yDest);
	BOOL Draw(HDC hDestDC, const POINT& pointDest);
	BOOL Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight);
	BOOL Draw(HDC hDestDC, const RECT& rectDest);

	HDC GetDC();
	VOID ReleaseDC();

	VOID* GetBits();
	int GetBPP();
	int GetMaxColorTableEntries();
	int GetHeight();
	int GetWidth();
	int GetPitch();
	VOID GetColorTable(UINT iFirstColor, UINT nColors, RGBQUAD* prgbColors);
	VOID* GetPixelAddress(int x, int y);
	COLORREF GetPixel(int x, int y);
	LONG GetTransparentColor();
	
	BOOL IsDIBSection();
	BOOL IsIndexed();
	BOOL IsNull();

	HRESULT Load(LPCTSTR pszFileName);
	VOID LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName);
	VOID LoadFromResource(HINSTANCE hInstance, UINT nIDResource);

	BOOL MaskBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD dwROP = SRCCOPY);
	BOOL MaskBlt(HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, HBITMAP hbmMask, const POINT& pointMask, DWORD dwROP = SRCCOPY);
	BOOL MaskBlt(HDC hDestDC, int xDest, int yDest, HBITMAP hbmMask, DWORD dwROP = SRCCOPY);
	BOOL MaskBlt(HDC hDestDC, const POINT& pointDest, HBITMAP hbmMask, DWORD dwROP = SRCCOPY);
	
	BOOL PlgBlt(HDC hDestDC, const POINT* pPoints, HBITMAP hbmMask = NULL);
	BOOL PlgBlt(HDC hDestDC, const POINT* pPoints, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, HBITMAP hbmMask = NULL, int xMask = 0, int yMask = 0);
	BOOL PlgBlt(HDC hDestDC, const POINT* pPoints, const RECT& rectSrc, HBITMAP hbmMask, const POINT& pointMask);
	
	//CLSID FindCodecForExtension(LPCTSTR pszExtension, const Gdiplus::ImageCodecInfo* pCodecs, UINT nCodecs);
	//CLSID FindCodecForFileType(REFGUID guidFileType, const Gdiplus::ImageCodecInfo* pCodecs, UINT nCodecs);
	//HRESULT Save(LPCTSTR pszFileName, REFGUID guidFileType = GUID_NULL);

	VOID SetColorTable( UINT iFirstColor, UINT nColors, const RGBQUAD* prgbColors );
	VOID SetPixel(int x, int y, COLORREF color);
	VOID SetPixelIndexed( int x, int y, int iIndex);
	VOID SetPixelRGB(int x, int y, BYTE r, BYTE g, BYTE b);
	LONG SetTransparentColor(LONG iTransparentColor);

	BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP = SRCCOPY);
	BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP = SRCCOPY);
	BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP = SRCCOPY);
	BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP = SRCCOPY );

	BOOL TransparentBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, UINT crTransparent = CLR_INVALID);
	BOOL TransparentBlt(HDC hDestDC, const RECT& rectDest, UINT crTransparent = CLR_INVALID);
	BOOL TransparentBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, UINT crTransparent = CLR_INVALID);
	BOOL TransparentBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, UINT crTransparent = CLR_INVALID);
	/*static*/ BOOL IsTransparencySupported();

private:
	HBITMAP m_hBitmap;
	VOID* m_pBits;
	int m_nWidth;
	int m_nHeight;
	int m_nPitch;
	int m_nBPP;
	BOOL m_bIsDIBSection;
	BOOL m_bHasAlphaChannel;
	LONG m_iTransparentColor;
	/*static*/ Win32GDIPlus s_gdiPlus;

	mutable HDC m_hDC;
	mutable int m_nDCRefCount;
	mutable HBITMAP m_hOldBitmap;
	/*static*/ Win32CacheDC s_cacheDC;

private:
	static BOOL ShouldExcludeFormat(REFGUID guidFileType, DWORD dwExclude);
	VOID UpdateBitmapInfo(DIBOrientation eOrientation);
	HRESULT CreateFromGdiplusBitmap(Gdiplus::Bitmap& bmSrc);

	/*static*/ BOOL InitGDIPlus();
	/*static*/ VOID ReleaseGDIPlus();

	/*static*/ int ComputePitch(int nWidth, int nBPP);
	/*static*/ VOID GenerateHalftonePalette(LPRGBQUAD prgbPalette);
	COLORREF GetTransparentRGB() const;

	LPWSTR Win32Image::AnsiToUnicode(LPCSTR pszText);
	LPSTR UnicodeToAnsi(LPCWSTR pszText);

};

#endif //__WIN32IMAGE_H__