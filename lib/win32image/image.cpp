#include "image.h"

//
// Win32ImageDC
//

Win32ImageDC::Win32ImageDC(Win32Image& image): m_image(image), m_hDC(image.GetDC())
{
	if(this->m_hDC == NULL) throw E_OUTOFMEMORY;
}

Win32ImageDC::~Win32ImageDC()
{
	m_image.ReleaseDC();
}

HDC Win32ImageDC::GetDC()
{
	return this->m_hDC;
}

//
// Win32Image::Win32GDIPlus
//

Win32Image::Win32GDIPlus::Win32GDIPlus(): m_dwToken(0), m_nImageObjects(0)
{
	__try
	{
		InitializeCriticalSection(&m_sect);
	}
	__except(STATUS_NO_MEMORY == GetExceptionCode())
	{
		//AtlThrow(E_OUTOFMEMORY);
	}
}

Win32Image::Win32GDIPlus::~Win32GDIPlus()
{
	this->ReleaseGDIPlus();
	DeleteCriticalSection(&m_sect);
}

BOOL Win32Image::Win32GDIPlus::Init()
{
	EnterCriticalSection(&m_sect);
	BOOL fRet = TRUE;
	if(this->m_dwToken == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;
		Gdiplus::Status status = Gdiplus::GdiplusStartup(&this->m_dwToken, &input, &output);
		if(status != Gdiplus::Ok)
			fRet = FALSE;
	}
	LeaveCriticalSection(&m_sect);
	return fRet;
}

VOID Win32Image::Win32GDIPlus::ReleaseGDIPlus()
{
	EnterCriticalSection(&m_sect);
	if(this->m_dwToken != 0)
	{
		Gdiplus::GdiplusShutdown(this->m_dwToken);
	}
	this->m_dwToken = 0;
	LeaveCriticalSection(&m_sect);
}

void Win32Image::Win32GDIPlus::IncreaseImageCount()
{
	EnterCriticalSection(&m_sect);
	this->m_nImageObjects++;
	LeaveCriticalSection(&m_sect);
}

void Win32Image::Win32GDIPlus::DecreaseImageCount()
{
	EnterCriticalSection(&m_sect);
	if(--this->m_nImageObjects == 0) this->ReleaseGDIPlus();
	LeaveCriticalSection(&m_sect);
}


Win32Image::Win32CacheDC::Win32CacheDC()
{
	for(int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++)
	{
		this->m_ahDCs[iDC] = NULL;
	}
}

Win32Image::Win32CacheDC::~Win32CacheDC()
{
	for(int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++)
	{
		if(this->m_ahDCs[iDC] != NULL )
		{
			::DeleteDC(this->m_ahDCs[iDC]);
		}
	}
}

HDC Win32Image::Win32CacheDC::GetDC()
{
	HDC hDC;

	for(int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++)
	{
		hDC = static_cast<HDC>(InterlockedExchangePointer(reinterpret_cast<void**>(&this->m_ahDCs[iDC]), NULL));
		if(hDC != NULL)
		{
			return(hDC);
		}
	}

	hDC = ::CreateCompatibleDC(NULL);

	return(hDC);
}

void Win32Image::Win32CacheDC::ReleaseDC(HDC hDC)
{
	for(int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++)
	{
		HDC hOldDC;

		hOldDC = static_cast<HDC>(InterlockedExchangePointer(reinterpret_cast<void**>(&this->m_ahDCs[iDC]), hDC));
		if(hOldDC == NULL)
		{
			return;
		}
		else
		{
			hDC = hOldDC;
		}
	}
	if(hDC != NULL)
	{
		::DeleteDC(hDC);
	}
}

Win32Image::Win32Image()
{
	this->m_hBitmap = NULL;
	this->m_pBits = NULL;
	this->m_hDC = NULL;
	this->m_nDCRefCount = 0;
	this->m_hOldBitmap = NULL;
	this->m_nWidth = 0;
	this->m_nHeight = 0;
	this->m_nPitch = 0;
	this->m_nBPP = 0;
	this->m_iTransparentColor = -1;
	this->m_bHasAlphaChannel = FALSE;
	this->m_bIsDIBSection =  FALSE;
	s_gdiPlus.IncreaseImageCount();
}

Win32Image::~Win32Image()
{
	this->Destroy();
	s_gdiPlus.DecreaseImageCount();
}

Win32Image::operator HBITMAP() const
{
	return(this->m_hBitmap);
}

BOOL Win32Image::AlphaBlend(HDC hDestDC, int xDest, int yDest, BYTE bSrcAlpha, BYTE bBlendOp)
{
	return(this->AlphaBlend(hDestDC, xDest, yDest, this->m_nWidth, this->m_nHeight, 0, 0, 
		this->m_nWidth, this->m_nHeight, bSrcAlpha, bBlendOp));
}

BOOL Win32Image::AlphaBlend(HDC hDestDC, const POINT& pointDest, BYTE bSrcAlpha, BYTE bBlendOp) 
{
	return(this->AlphaBlend(hDestDC, pointDest.x, pointDest.y, this->m_nWidth, this->m_nHeight, 0, 0, 
		this->m_nWidth, this->m_nHeight, bSrcAlpha, bBlendOp));
}

BOOL Win32Image::AlphaBlend(HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, 
	int nSrcHeight, BYTE bSrcAlpha, BYTE bBlendOp)
{
	BLENDFUNCTION blend;
	BOOL bResult;

	blend.SourceConstantAlpha = bSrcAlpha;
	blend.BlendOp = bBlendOp;
	blend.BlendFlags = 0;
	if(this->m_bHasAlphaChannel)
	{
		blend.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		blend.AlphaFormat = 0;
	}

	this->GetDC();

	bResult = ::AlphaBlend(hDestDC, xDest, yDest, nDestWidth, nDestHeight, this->m_hDC, 
		xSrc, ySrc, nSrcWidth, nSrcHeight, blend);

	this->ReleaseDC();

	return(bResult);
}

BOOL Win32Image::AlphaBlend(HDC hDestDC, const RECT& rectDest, 
	const RECT& rectSrc, BYTE bSrcAlpha, BYTE bBlendOp)
{
	return(this->AlphaBlend(hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left, rectSrc.top, 
		rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top, bSrcAlpha, 
		bBlendOp));
}

void Win32Image::Attach(HBITMAP hBitmap, DIBOrientation eOrientation)
{
	if (this->m_hBitmap == NULL || hBitmap != NULL)
	{
		this->m_hBitmap = hBitmap;
		this->UpdateBitmapInfo(eOrientation);
	}
}

BOOL Win32Image::BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP)
{
	return(this->BitBlt(hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, dwROP));
}

BOOL Win32Image::BitBlt( HDC hDestDC, const POINT& pointDest, DWORD dwROP )
{
	return(this->BitBlt(hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0, 0, dwROP));
}

BOOL Win32Image::BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, 
	int nDestHeight, int xSrc, int ySrc, DWORD dwROP)
{
	BOOL bResult = FALSE;

	if (this->m_hBitmap != NULL && hDestDC != NULL)
	{
		this->GetDC();
		bResult = ::BitBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC, xSrc, ySrc, dwROP);
		this->ReleaseDC();
	}
	return(bResult);
}

BOOL Win32Image::BitBlt(HDC hDestDC, const RECT& rectDest, 
	const POINT& pointSrc, DWORD dwROP)
{
	return(this->BitBlt(hDestDC, rectDest.left, rectDest.top, rectDest.right - rectDest.left, 
		rectDest.bottom - rectDest.top, pointSrc.x, pointSrc.y, dwROP));
}

BOOL Win32Image::Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags)
{
	return(this->CreateEx(nWidth, nHeight, nBPP, BI_RGB, NULL, dwFlags));
}

BOOL Win32Image::CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, 
	const DWORD* pdwBitfields, DWORD dwFlags)
{
	//USES_ATL_SAFE_ALLOCA;
	LPBITMAPINFO pbmi;
	HBITMAP hBitmap;

	/*if((eCompression != BI_RGB) || (eCompression != BI_BITFIELDS))
	{
		return FALSE;
	}*/
	if(dwFlags & createAlphaChannel)
	{
		if((nBPP != 32) && (eCompression != BI_RGB)) return FALSE;
	}
	
	//pbmi = (LPBITMAPINFO)_ATL_SAFE_ALLOCA(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
	pbmi = (LPBITMAPINFO)new BITMAPINFO[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256];
	if( pbmi == NULL )
		return FALSE;

	qmemset(&pbmi->bmiHeader, 0, sizeof( pbmi->bmiHeader));
	pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
	pbmi->bmiHeader.biWidth = nWidth;
	pbmi->bmiHeader.biHeight = nHeight;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = USHORT(nBPP);
	pbmi->bmiHeader.biCompression = eCompression;
	if(nBPP <= 8)
	{
		//ATLASSERT( eCompression == BI_RGB );
		qmemset(pbmi->bmiColors, 0, 256*sizeof(RGBQUAD));
	}
	else 
	{
		if( eCompression == BI_BITFIELDS )
		{
			//ATLASSERT( pdwBitfields != NULL);
			qmemcpy(pbmi->bmiColors, pdwBitfields, 3*sizeof(DWORD));
		}
	}

	hBitmap = ::CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, &m_pBits, NULL, 0);
	if(hBitmap == NULL)
	{
		return FALSE;
	}

	this->Attach(hBitmap, (nHeight < 0) ? DIBOR_TOPDOWN : DIBOR_BOTTOMUP);

	if( dwFlags&createAlphaChannel )
	{
		m_bHasAlphaChannel = true;
	}

	return TRUE ;
}

void Win32Image::Destroy()
{
	HBITMAP hBitmap;

	if(this->m_hBitmap != NULL)
	{
		hBitmap = this->Detach();
		::DeleteObject(hBitmap);
	}
}

HBITMAP Win32Image::Detach()
{
	HBITMAP hBitmap;

	//ATLASSUME( m_hBitmap != NULL );
	//ATLASSUME( m_hDC == NULL );

	hBitmap = this->m_hBitmap;
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBPP = 0;
	m_nPitch = 0;
	m_iTransparentColor = -1;
	m_bHasAlphaChannel = FALSE;
	m_bIsDIBSection = FALSE;

	return(hBitmap);
}

BOOL Win32Image::Draw(HDC hDestDC, const RECT& rectDest)
{
	return(this->Draw(hDestDC, rectDest.left, rectDest.top, rectDest.right - rectDest.left,
		rectDest.bottom - rectDest.top, 0, 0, this->m_nWidth, this->m_nHeight));
}

BOOL Win32Image::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight)
{
	return(this->Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 0, this->m_nWidth, this->m_nHeight));
}

BOOL Win32Image::Draw(HDC hDestDC, const POINT& pointDest)
{
	return(this->Draw(hDestDC, pointDest.x, pointDest.y, this->m_nWidth, this->m_nHeight, 0, 0, this->m_nWidth, this->m_nHeight));
}

BOOL Win32Image::Draw(HDC hDestDC, int xDest, int yDest)
{
	return(this->Draw(hDestDC, xDest, yDest, this->m_nWidth, this->m_nHeight, 0, 0, this->m_nWidth, this->m_nHeight));
}

BOOL Win32Image::Draw(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc)
{
	return(this->Draw(hDestDC, rectDest.left, rectDest.top, rectDest.right - rectDest.left, 
		rectDest.bottom - rectDest.top, rectSrc.left, rectSrc.top, 
		rectSrc.right - rectSrc.left, rectSrc.bottom - rectSrc.top));
}

BOOL Win32Image::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth,
	int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight)
{
	BOOL bResult;

	if(hDestDC == NULL || nDestWidth <= 0 || nDestHeight <= 0 
		|| nSrcWidth <= 0 || nSrcHeight <= 0)
	{
		return FALSE;
	}

	this->GetDC();

	if((this->m_iTransparentColor != -1) && IsTransparencySupported())
	{
		bResult = ::TransparentBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight,
			this->m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, this->GetTransparentRGB());
	}
	else if(this->m_bHasAlphaChannel && IsTransparencySupported())
	{
		BLENDFUNCTION bf;

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 0xff;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bResult = ::AlphaBlend(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 
			this->m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, bf);
	}
	else
	{
		bResult = ::StretchBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 
			this->m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, SRCCOPY);
	}

	this->ReleaseDC();

	return(bResult);
}

VOID* Win32Image::GetBits()
{
	if (this->m_hBitmap != NULL && this->IsDIBSection())
		return(this->m_pBits);
	return NULL;
}

int Win32Image::GetBPP()
{
	if(m_hBitmap != NULL)
		return(this->m_nBPP);
	return NULL;
}

void Win32Image::GetColorTable(UINT iFirstColor, UINT nColors, RGBQUAD* prgbColors)
{
	if (this->m_hBitmap != NULL && this->m_pBits != NULL && this->IsIndexed())
	{
		this->GetDC();
		::GetDIBColorTable(this->m_hDC, iFirstColor, nColors, prgbColors);
		this->ReleaseDC();
	}
}

HDC Win32Image::GetDC()
{
	this->m_nDCRefCount++;
	if(this->m_hDC == NULL )
	{
		this->m_hDC = s_cacheDC.GetDC();
		this->m_hOldBitmap = HBITMAP(::SelectObject(this->m_hDC, this->m_hBitmap));
	}

	return(this->m_hDC);
}

BOOL Win32Image::ShouldExcludeFormat(REFGUID guidFileType, DWORD dwExclude)
{
	static const GUID* apguidFormats[] =
	{
		&Gdiplus::ImageFormatGIF,
		&Gdiplus::ImageFormatBMP,
		&Gdiplus::ImageFormatEMF,
		&Gdiplus::ImageFormatWMF,
		&Gdiplus::ImageFormatJPEG,
		&Gdiplus::ImageFormatPNG,
		&Gdiplus::ImageFormatTIFF,
		&Gdiplus::ImageFormatIcon,
		NULL
	};

	if((dwExclude|excludeValid) == excludeValid)
	{
		for(int iFormat = 0; apguidFormats[iFormat] != NULL; iFormat++)
		{
			if(guidFileType == *apguidFormats[iFormat] )
			{
				return((dwExclude&(1<<iFormat)) != 0);
			}
		}
	}
	return((dwExclude&excludeOther) != 0);
}

int Win32Image::GetHeight()
{
	return(this->m_nHeight);
}

int Win32Image::GetMaxColorTableEntries()
{
	int n = 0;
	if(this->IsIndexed())
	{
		n = 1<<this->m_nBPP;
	}
	return n;
}

int Win32Image::GetPitch()
{
	if(this->m_hBitmap != NULL && this->IsDIBSection())
		return(this->m_nPitch);
	else
		return 0;
}

COLORREF Win32Image::GetPixel(int x, int y) 
{
	COLORREF clr = 0;

	if(this->m_hBitmap != NULL && ((x >= 0) && (x < this->m_nWidth)) && ((y >= 0) && (y < this->m_nHeight)))
	{
		this->GetDC();
		clr = ::GetPixel(this->m_hDC, x, y);
		this->ReleaseDC();
	}
	return(clr);
}

void* Win32Image::GetPixelAddress(int x, int y)
{
	if(this->m_hBitmap != NULL && this->IsDIBSection() && ((x >= 0) && (x < this->m_nWidth)) && ((y >= 0) && (y < this->m_nHeight)))
		return(LPBYTE(this->m_pBits ) + (y * this->m_nPitch) + ((x * this->m_nBPP) / 8));
	else
		return NULL;
}

 
LONG Win32Image::GetTransparentColor()
{
	if(this->m_hBitmap != NULL && (this->m_nBPP == 4) || (this->m_nBPP == 8))
		return(this->m_iTransparentColor);
	else
		return 0;
}

int Win32Image::GetWidth()
{
	if(this->m_hBitmap != NULL)
		return(this->m_nWidth);
	else
		return 0;
}

BOOL Win32Image::IsDIBSection()
{
	return(this->m_bIsDIBSection);
}

BOOL Win32Image::IsIndexed()
{
	if(m_hBitmap != NULL && this->IsDIBSection())
		return(this->m_nBPP <= 8);
	else
		return FALSE;
}

BOOL Win32Image::IsNull()
{
	return(this->m_hBitmap == NULL);
}

HRESULT Win32Image::Load(LPCTSTR pszFileName)
{
	if(!this->InitGDIPlus())
	{
		return E_FAIL;
	}

	Gdiplus::Bitmap bmSrc((LPCWSTR)pszFileName);
	if( bmSrc.GetLastStatus() != Gdiplus::Ok )
	{
		return E_FAIL;
	}

	return this->CreateFromGdiplusBitmap(bmSrc);
}

HRESULT Win32Image::CreateFromGdiplusBitmap(Gdiplus::Bitmap& bmSrc)
{
	Gdiplus::PixelFormat eSrcPixelFormat = bmSrc.GetPixelFormat();
	UINT nBPP = 32;
	DWORD dwFlags = 0;
	Gdiplus::PixelFormat eDestPixelFormat = PixelFormat32bppRGB;

	if(eSrcPixelFormat & PixelFormatGDI)
	{
		nBPP = Gdiplus::GetPixelFormatSize(eSrcPixelFormat);
		eDestPixelFormat = eSrcPixelFormat;
	}
	if( Gdiplus::IsAlphaPixelFormat(eSrcPixelFormat))
	{
		nBPP = 32;
		dwFlags |= createAlphaChannel;
		eDestPixelFormat = PixelFormat32bppARGB;
	}

	BOOL bSuccess = this->Create(bmSrc.GetWidth(), bmSrc.GetHeight(), nBPP, dwFlags);
	if(!bSuccess)
	{
		return E_FAIL;
	}

	//USES_ATL_SAFE_ALLOCA;
	Gdiplus::ColorPalette* pPalette = NULL;
	if(Gdiplus::IsIndexedPixelFormat(eSrcPixelFormat))
	{
		UINT nPaletteSize = bmSrc.GetPaletteSize();
		/*pPalette = static_cast<Gdiplus::ColorPalette*>(_ATL_SAFE_ALLOCA(nPaletteSize, _ATL_SAFE_ALLOCA_DEF_THRESHOLD));*/
		pPalette = static_cast<Gdiplus::ColorPalette*>(new Gdiplus::ColorPalette[nPaletteSize]);

		if( pPalette == NULL )
			return E_OUTOFMEMORY;

		bmSrc.GetPalette(pPalette, nPaletteSize);

		RGBQUAD argbPalette[256];
		//ATLENSURE_RETURN((pPalette->Count > 0) && (pPalette->Count <= 256));
		for( UINT iColor = 0; iColor < pPalette->Count; iColor++ )
		{
			Gdiplus::ARGB color = pPalette->Entries[iColor];
			argbPalette[iColor].rgbRed = (BYTE)( (color>>RED_SHIFT) & 0xff );
			argbPalette[iColor].rgbGreen = (BYTE)( (color>>GREEN_SHIFT) & 0xff );
			argbPalette[iColor].rgbBlue = (BYTE)( (color>>BLUE_SHIFT) & 0xff );
			argbPalette[iColor].rgbReserved = 0;
		}

		this->SetColorTable(0, pPalette->Count, argbPalette);
	}

	if( eDestPixelFormat == eSrcPixelFormat )
	{
		// The pixel formats are identical, so just memcpy the rows.
		Gdiplus::BitmapData data;
		Gdiplus::Rect rect( 0, 0, GetWidth(), GetHeight() );
		if(bmSrc.LockBits( &rect, Gdiplus::ImageLockModeRead, eSrcPixelFormat, &data )!=Gdiplus::Ok)
		{
			return E_OUTOFMEMORY;
		}

		/*size_t nBytesPerRow = AtlAlignUp( nBPP*GetWidth(), 8 )/8;*/
		size_t nBytesPerRow = nBPP*GetWidth()/8;
		BYTE* pbDestRow = static_cast< BYTE* >( GetBits() );
		BYTE* pbSrcRow = static_cast< BYTE* >( data.Scan0 );
		for( int y = 0; y < GetHeight(); y++ )
		{
			qmemcpy(pbDestRow, pbSrcRow, nBytesPerRow);
			pbDestRow += GetPitch();
			pbSrcRow += data.Stride;
		}

		bmSrc.UnlockBits( &data );
	}
	else
	{
		// Let GDI+ work its magic
		Gdiplus::Bitmap bmDest(this->GetWidth(), this->GetHeight(), this->GetPitch(), eDestPixelFormat, static_cast<BYTE*>(this->GetBits()));
		Gdiplus::Graphics gDest(&bmDest);
		gDest.DrawImage(&bmSrc, 0, 0);
	}

	return S_OK;
}

void Win32Image::LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName)
{
	HBITMAP hBitmap;

	hBitmap = HBITMAP(::LoadImage(hInstance, pszResourceName, IMAGE_BITMAP, 0, 
		0, LR_CREATEDIBSECTION ) );

	this->Attach(hBitmap);
}

void Win32Image::LoadFromResource( HINSTANCE hInstance, UINT nIDResource)
{
	this->LoadFromResource(hInstance, MAKEINTRESOURCE(nIDResource));
}

BOOL Win32Image::MaskBlt(HDC hDestDC, int xDest, int yDest, int nWidth, 
	int nHeight, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD dwROP)
{
	BOOL bResult = FALSE;

	if (this->m_hBitmap != NULL && hDestDC != NULL)
	{
		this->GetDC();
		bResult = ::MaskBlt(hDestDC, xDest, yDest, nWidth, nHeight, m_hDC, xSrc, ySrc, hbmMask, xMask, yMask, dwROP);
		this->ReleaseDC();
	}
	return bResult;
}

BOOL Win32Image::MaskBlt(HDC hDestDC, const RECT& rectDest, 
	const POINT& pointSrc, HBITMAP hbmMask, const POINT& pointMask, DWORD dwROP)
{
	return(this->MaskBlt(hDestDC, rectDest.left, rectDest.top, rectDest.right - rectDest.left, 
		rectDest.bottom-rectDest.top, pointSrc.x, pointSrc.y, 
		hbmMask, pointMask.x, pointMask.y, dwROP));
}

BOOL Win32Image::MaskBlt(HDC hDestDC, int xDest, int yDest, HBITMAP hbmMask, DWORD dwROP)
{
	return(this->MaskBlt(hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, hbmMask, 0, 0, dwROP));
}

BOOL Win32Image::MaskBlt(HDC hDestDC, const POINT& pointDest, HBITMAP hbmMask, DWORD dwROP)
{
	return(this->MaskBlt( hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0, 0, hbmMask, 0, 0, dwROP));
}

BOOL Win32Image::PlgBlt( HDC hDestDC, const POINT* pPoints, int xSrc, 
	int ySrc, int nSrcWidth, int nSrcHeight, HBITMAP hbmMask, int xMask, int yMask)
{
	BOOL bResult = FALSE;

	if(this->m_hBitmap != NULL && hDestDC != NULL)
	{
		this->GetDC();
		bResult = ::PlgBlt(hDestDC, pPoints, m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, hbmMask, xMask, yMask);
		this->ReleaseDC();
	}
	return bResult;
}

BOOL Win32Image::PlgBlt(HDC hDestDC, const POINT* pPoints, 
	const RECT& rectSrc, HBITMAP hbmMask, const POINT& pointMask)
{
	return(this->PlgBlt( hDestDC, pPoints, rectSrc.left, rectSrc.top, rectSrc.right - rectSrc.left, 
		rectSrc.bottom - rectSrc.top, hbmMask, pointMask.x, pointMask.y));
}

BOOL Win32Image::PlgBlt(HDC hDestDC, const POINT* pPoints, HBITMAP hbmMask)
{
	return(this->PlgBlt(hDestDC, pPoints, 0, 0, this->m_nWidth, this->m_nHeight, hbmMask, 0, 0));
}

void Win32Image::ReleaseDC()
{
	if(this->m_hDC != NULL)
	{
		this->m_nDCRefCount--;
		if(this->m_nDCRefCount == 0)
		{
			HBITMAP hBitmap;
			hBitmap = HBITMAP(::SelectObject(m_hDC, m_hOldBitmap));
			if(hBitmap == this->m_hBitmap)
			{
				s_cacheDC.ReleaseDC(this->m_hDC);
				this->m_hDC = NULL;
			}
		}
	}
}

void Win32Image::SetColorTable( UINT iFirstColor, UINT nColors, const RGBQUAD* prgbColors )
{
	if(this->IsIndexed())
	{
		this->GetDC();
		::SetDIBColorTable(this->m_hDC, iFirstColor, nColors, prgbColors);
		this->ReleaseDC();
	}
}

void Win32Image::SetPixel(int x, int y, COLORREF color)
{
	if(m_hBitmap != NULL  && ((x >= 0) && (x < m_nWidth)) && ((y >= 0) && (y < m_nHeight)))
	{
		this->GetDC();
		::SetPixel(this->m_hDC, x, y, color);
		this->ReleaseDC();
	}
}

void Win32Image::SetPixelIndexed(int x, int y, int iIndex)
{
	this->SetPixel(x, y, PALETTEINDEX(iIndex));
}

void Win32Image::SetPixelRGB( int x, int y, BYTE r, BYTE g, BYTE b)
{
	this->SetPixel( x, y, RGB(r, g, b));
}

LONG Win32Image::SetTransparentColor(LONG iTransparentColor)
{
	LONG iOldTransparentColor = 0;

	if (this->m_hBitmap != NULL  && ((this->m_nBPP == 4) || (this->m_nBPP == 8)) 
		&& (iTransparentColor < this->GetMaxColorTableEntries()) 
		&& (iTransparentColor >= -1))
	{
		iOldTransparentColor = this->m_iTransparentColor;
		this->m_iTransparentColor = iTransparentColor;
	}
	return iOldTransparentColor;
}

BOOL Win32Image::StretchBlt(HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, DWORD dwROP)
{
	return(this->StretchBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 0, 
		this->m_nWidth, this->m_nHeight, dwROP));
}

BOOL Win32Image::StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP)
{
	return(this->StretchBlt(hDestDC, rectDest.left, rectDest.top, 
		rectDest.right - rectDest.left, rectDest.bottom - rectDest.top, 0, 0, m_nWidth, m_nHeight, dwROP));
}

BOOL Win32Image::StretchBlt(HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, 
	int nSrcHeight, DWORD dwROP)
{
	BOOL bResult = FALSE;

	if(this->m_hBitmap != NULL &&  hDestDC != NULL)
	{
		this->GetDC();
		bResult = ::StretchBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, this->m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwROP);
		this->ReleaseDC();
	}
	return bResult;
}

BOOL Win32Image::StretchBlt(HDC hDestDC, const RECT& rectDest, 
	const RECT& rectSrc, DWORD dwROP)
{
	return(this->StretchBlt( hDestDC, rectDest.left, rectDest.top, 
		rectDest.right - rectDest.left, rectDest.bottom - rectDest.top, rectSrc.left, rectSrc.top, 
		rectSrc.right - rectSrc.left, rectSrc.bottom - rectSrc.top, dwROP));
}

BOOL Win32Image::TransparentBlt(HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, UINT crTransparent)
{
	return(this->TransparentBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 
		0, m_nWidth, m_nHeight, crTransparent));
}

BOOL Win32Image::TransparentBlt(HDC hDestDC, const RECT& rectDest, UINT crTransparent)
{
	return(this->TransparentBlt( hDestDC, rectDest.left, rectDest.top, 
		rectDest.right - rectDest.left, rectDest.bottom - rectDest.top, 
		crTransparent));
}

BOOL Win32Image::TransparentBlt(HDC hDestDC, int xDest, int yDest, 
	int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, UINT crTransparent)
{
	BOOL bResult = FALSE;

	if(this->m_hBitmap != NULL && hDestDC != NULL)
	{
		this->GetDC();

		if(crTransparent == CLR_INVALID)
			crTransparent = this->GetTransparentRGB();
		bResult = ::TransparentBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight,
			this->m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent);

		this->ReleaseDC();
	}
	return bResult;
}

BOOL Win32Image::TransparentBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, UINT crTransparent)
{
	return(this->TransparentBlt(hDestDC, rectDest.left, rectDest.top, 
		rectDest.right - rectDest.left, rectDest.bottom - rectDest.top, rectSrc.left, 
		rectSrc.top, rectSrc.right - rectSrc.left, rectSrc.bottom - rectSrc.top, 
		crTransparent));
}

BOOL Win32Image::IsTransparencySupported()
{
	return TRUE;
}

void Win32Image::UpdateBitmapInfo(DIBOrientation eOrientation)
{
	DIBSECTION dibsection;
	int nBytes;

	nBytes = ::GetObject(this->m_hBitmap, sizeof(DIBSECTION), &dibsection);
	if(nBytes == sizeof(DIBSECTION))
	{
		this->m_bIsDIBSection = true;
		this->m_nWidth = dibsection.dsBmih.biWidth;
		this->m_nHeight = abs( dibsection.dsBmih.biHeight );
		this->m_nBPP = dibsection.dsBmih.biBitCount;
		this->m_nPitch = ComputePitch( m_nWidth, m_nBPP );
		this->m_pBits = dibsection.dsBm.bmBits;
		if(eOrientation == DIBOR_DEFAULT)
		{
			eOrientation = (dibsection.dsBmih.biHeight > 0) ? DIBOR_BOTTOMUP : DIBOR_TOPDOWN;
		}
		if( eOrientation == DIBOR_BOTTOMUP )
		{
			this->m_pBits = LPBYTE(this->m_pBits) + ((this->m_nHeight - 1) * this->m_nPitch);
			this->m_nPitch = -this->m_nPitch;
		}
	}
	else
	{
		// Non-DIBSection
		//if(nBytes == sizeof(BITMAP))
		this->m_bIsDIBSection = FALSE;
		this->m_nWidth = dibsection.dsBm.bmWidth;
		this->m_nHeight = dibsection.dsBm.bmHeight;
		this->m_nBPP = dibsection.dsBm.bmBitsPixel;
		this->m_nPitch = 0;
		this->m_pBits = 0;
	}
	this->m_iTransparentColor = -1;
	this->m_bHasAlphaChannel = FALSE;
}

int Win32Image::ComputePitch(int nWidth, int nBPP) 
{ 
	return((((nWidth*nBPP)+31)/32)*4); 
}

void Win32Image::GenerateHalftonePalette(LPRGBQUAD prgbPalette)
{
	int r;
	int g;
	int b;
	int gray;
	LPRGBQUAD prgbEntry;

	prgbEntry = prgbPalette;
	for( r = 0; r < 6; r++ )
	{
		for( g = 0; g < 6; g++ )
		{
			for( b = 0; b < 6; b++ )
			{
				prgbEntry->rgbBlue = BYTE(b*255/5);
				prgbEntry->rgbGreen = BYTE(g*255/5);
				prgbEntry->rgbRed = BYTE(r*255/5);
				prgbEntry->rgbReserved = 0;
				prgbEntry++;
			}
		}
	}

	for( gray = 0; gray < 20; gray++ )
	{
		prgbEntry->rgbBlue = BYTE(gray*255/20);
		prgbEntry->rgbGreen = BYTE(gray*255/20);
		prgbEntry->rgbRed = BYTE(gray*255/20);
		prgbEntry->rgbReserved = 0;
		prgbEntry++;
	}
}

COLORREF Win32Image::GetTransparentRGB() const
{
	RGBQUAD rgb = {0};

	if(this->m_hDC != NULL && this->m_iTransparentColor != -1)
	{
		::GetDIBColorTable( m_hDC, m_iTransparentColor, 1, &rgb);
	}
	return(RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue));
}

BOOL Win32Image::InitGDIPlus()
{
	return s_gdiPlus.Init();
}

VOID Win32Image::ReleaseGDIPlus()
{	
	s_gdiPlus.ReleaseGDIPlus(); 
}

LPWSTR Win32Image::AnsiToUnicode(LPCSTR pszText)
{     
    int nLen = MultiByteToWideChar(CP_ACP, 0, pszText, -1, NULL, 0);
	if (nLen == 0) return NULL;
    LPWSTR pwText = new WCHAR[nLen];  
    nLen = MultiByteToWideChar(CP_ACP, 0, pszText, -1, pwText, nLen);   
    return pwText;
}

LPSTR UnicodeToAnsi(LPCWSTR pszText)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pszText, -1, NULL, 0, NULL, NULL);
	if (nLen == 0) return NULL;
	LPSTR pText = new CHAR[nLen];
	nLen = WideCharToMultiByte(CP_ACP, 0, pszText, -1, pText, nLen, NULL, NULL);
	return pText;
}
