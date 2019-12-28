#include "gdi.h"
#include "math.h"

//
// 函数：IsWinNT
// 功能：判断操作系统的类型。返回True表明是以下操作系统：
//       WindowsNT3.5(1), NT4.0, 2000、XP或Windows Server 2003
// 参数：无
// 返回：是返回真
//
BOOL Win32GDI::IsWinNT()
{
    OSVERSIONINFO lpOSI;
    
    lpOSI.dwOSVersionInfoSize = sizeof(lpOSI);
	GetVersionEx(&lpOSI);
    return (lpOSI.dwPlatformId = VER_PLATFORM_WIN32_NT);
}

//
// 函数：SelectAObject
// 功能：将对象选入设备环境
// 参数：hDC，		设备环境
//       hObject，	对象句柄，可以是画刷、画笔、字体、图像等
// 返回：原设备环境的对象句柄
//
HGDIOBJ Win32GDI::SelectAObject(HDC hDC, HGDIOBJ hObject)
{
	return SelectObject(hDC, hObject);
}

//
// 函数：DeleteAObject
// 功能：删除对象
// 参数：hObject，对象句柄，可以是画刷、画笔、字体、图像等
// 返回：是返回真
//
BOOL Win32GDI::DeleteAObject(HGDIOBJ hObject)
{
	return DeleteObject(hObject);
}

//
// 函数：CreateABrush
// 功能：创建画刷
// 参数：eBrushType，	画刷类型
//       eBrushStyle，	画刷样式
//       eBrushHatch，	画刷阴影线样式
//       clrColor，		画刷颜色
//       hBitmap，		画刷位图
// 返回：原设备环境的画刷
//
HBRUSH Win32GDI::CreateABrush(
	BrushTypeConstants eBrushType, 
    BrushStyleConstants eBrushStyle, 
    BrushHatchConstants eBrushHatch, 
    COLORREF clrColor, 
    HBITMAP hBitmap) 
{
    LOGBRUSH lb;
	HBRUSH hBrush;

    switch (eBrushType)
	{
	case btDefaultBrush:     //缺省画刷
		hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		break;
	case btSolidBrush:       //实心画刷
		hBrush = CreateSolidBrush(TranslateColor(clrColor));
		break;
	case btIndirectBrush:    //指定样式、颜色、图案的画刷
		lb.lbStyle = eBrushStyle;
		lb.lbColor = TranslateColor(clrColor);
		lb.lbHatch = eBrushHatch;
		hBrush = CreateBrushIndirect(&lb);
		break;
	case btHatchBrush:       //指定颜色的图案画刷
		hBrush = CreateHatchBrush(eBrushHatch, TranslateColor(clrColor));
		break;
	case btPatternBrush:     //位图画刷
		hBrush = CreatePatternBrush(hBitmap);
		break;
    }
    return hBrush;
}

//
// 函数：CreateAPen
// 功能：创建画笔
// 参数：ePenType，		画笔类型
//       ePenStyle，	画笔样式
//       clrColor，		画笔颜色
//       nLineWidth，	画笔线宽
// 返回：原设备环境的画笔
//
HPEN Win32GDI::CreateAPen(
	PenTypeConstants ePenType, 
    PenStyleConstants ePenStyle, 
    COLORREF clrColor, 
    UINT nLineWidth)
{
    LOGPEN lgpn;
	HPEN hPen;

	switch(ePenType)
	{
	case ptDefaultPen:       //缺省画笔
		hPen = (HPEN)GetStockObject(WHITE_PEN);
		break;
	case ptSolidPen:         //实心画笔
		hPen = CreatePen(PS_SOLID, nLineWidth, TranslateColor(clrColor));
		break;
	case ptIndirectPen:      //指定样式、宽度、颜色的画笔
		lgpn.lopnStyle = ePenStyle;
		lgpn.lopnWidth.x = nLineWidth;
		lgpn.lopnWidth.y = 0;
		lgpn.lopnColor = TranslateColor(clrColor);
		hPen = CreatePenIndirect(&lgpn);
		break;
    }
    return hPen;
}

//
// 函数：CreateAFont
// 功能：创建字体
// 参数：hDC，					设备环境
//       eFontType，			字体类型
//       strFontName，			字体名称
//       nFontSize，			字体尺寸
//       nFontRotationDegrees，	字体旋转角度
//       bFontBold，			是否出粗体
//       bFontItalic，			是否斜体
//       bFontUnderline，		是否有下划线
//       bFontStrikethru，		是否有强调线
// 返回：原设备环境的字体
//
HFONT Win32GDI::CreateAFont(
	HDC hDC, 
    FontTypeConstants eFontType, 
    LPCTSTR strFontName,
    UINT nFontSize,
    INT  nFontRotationDegrees, 
    BOOL bFontBold, 
    BOOL bFontItalic,
    BOOL bFontUnderline,
    BOOL bFontStrikethru,
	BOOL bFontHighQuality)
{
    HFONT hFont;
 
    switch(eFontType)
	{
	case ftDefaultFont:          //缺省字体
        hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		break;
    case ftSystemFont:           //系统字体
        hFont = (HFONT)GetStockObject(SYSTEM_FONT);
		break;
    case ftIndirectFont:         //指定字体
		LOGFONT lf;
		lf.lfWidth = 0;
        lf.lfHeight = -MulDiv((nFontSize==0)?9:nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
        lf.lfEscapement = nFontRotationDegrees * 10;
        lf.lfOrientation = nFontRotationDegrees * 10;
        lf.lfWeight = (bFontBold)?FW_BOLD:FW_NORMAL;
        lf.lfItalic = bFontItalic;
        lf.lfUnderline = bFontUnderline;
        lf.lfStrikeOut = bFontStrikethru;
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS | CLIP_LH_ANGLES;
        lf.lfQuality = (bFontHighQuality)?5:DEFAULT_QUALITY; //CLEARTYPE_QUALITIY=5清晰字体
        lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		if (strFontName == _T("")) 
		{
			lstrcpy(lf.lfFaceName, _T("Tahoma"));
		}
		else
		{
			lstrcpy(lf.lfFaceName, strFontName);   
		}
        hFont = CreateFontIndirect(&lf);
		break;
    }
    return hFont;
}

//
// 函数：CalcTextArea
// 功能：计算文本区域。
// 参数：hDC，设备环境
//		 strText，	要计算的文本
//       hFont，	字体对象
//       nWidth，	返回文本区域宽度
//       nHeight，	返回区域高度
//
/*
VOID Win32GDI::CalcTextArea(
	HDC hDC, 
	LPCTSTR strText, 
	HFONT hFont,
	INT* nWidth, 
	INT* nHeight)
{
	RECT tR;
	HDC hDstDC;
	HDC hDeskDC;
	HBITMAP hMemDCBmpCurr;
	HBITMAP hMemDCBmpOrg;
	HFONT hOldFont;
	INT width, 
	INT height,
	
	//设置容纳文本的区域
	width = *nWidth;
	height = *nHeight;
	tR.left = 0;
	tR.top = 0;
	if (width <= 0) 
	{
		tR.right = GetSystemMetrics(SM_CXSCREEN) \ 15;
	}
	else
	{
		tR.right = width;
	}
	
	if (height <= 0)
	{
		tR.bottom = GetSystemMetrics(SM_CYSCREEN) \ 15;
	}
	else
	{
		tR.bottom = height;
	}
	
	//设置DC
	if (hDC != NULL)
	{
		hDstDC = hDC;
	}
	else
	{
		hDeskDC = CreateDC(_T("DISPLAY"), 0, 0, 0);
		hDstDC = CreateCompatibleDC(hDeskDC);
		hMemDCBmpCurr = CreateCompatibleBitmap(hDeskDC, width, height);
		hMemDCBmpOrg = SelectObject(hDC, hMemDCBmpCurr);
		DeleteDC(hDeskDC);
	}
	
	//设置字体
	if (hFont != NULL)
	{
		hOldFont = SelectObject(hDstDC, hFont);
	}
	
	//计算文本区域
	DrawText(hDstDC, strText, -1, &tR, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
	
	//释放资源
	if (hDC == NULL) 
	{
		SelectObject(hDstDC, hMemDCBmpOrg);
		DeleteObject(hMemDCBmpCurr);
		DeleteDC(hDstDC);
	}
	if (hFont != NULL)
	{
		SelectObject(hDstDC, hOldFont);
	}
	
	//返回
	*nWidth = tR.right;
	*nHeight = tR.bottom;
}
*/
VOID Win32GDI::CalcTextArea(
	HDC hDC, 
	LPCTSTR strText, 
	INT* nWidth, 
	INT* nHeight,
	HFONT hFont)
{
	RECT tR;
	HFONT hFontOrg;
	INT width;
	INT height;

	//设置容纳文本的区域
	width = *nWidth;
	height = *nHeight;
	tR.left = 0;
	tR.top = 0;
	if (width <= 0) 
	{
		tR.right = GetSystemMetrics(SM_CXSCREEN) / 15;
	}
	else
	{
		tR.right = width;
	}
	if (height <= 0)
	{
		tR.bottom = GetSystemMetrics(SM_CYSCREEN) / 15;
	}
	else
	{
		tR.bottom = height;
	}
	
	//设置字体
	if (hFont != NULL)
	{
		hFontOrg = (HFONT)SelectObject(hDC, hFont);
	}
	
	//计算文本区域
	DrawText(hDC, strText, -1, &tR, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
	
	//释放资源
	if (hFont != NULL)
	{
		SelectObject(hDC, hFontOrg);
	}
	
	//返回
	*nWidth = tR.right;
	*nHeight = tR.bottom;
}

//
// 函数：TextWidth
// 功能：获得文本宽度
// 参数：hDC，		设备环境
//		 strText，	要计算的文本
//
LONG Win32GDI::TextWidth(
	HDC hDC, 
	LPCTSTR strText)
{
	SIZE size;
	GetTextExtentPoint(hDC, strText, lstrlen(strText), &size);
	return size.cx;
}

//
// 函数：TextHeight
// 功能：获得文本高度
// 参数：hDC，		设备环境
//		 strText，	要计算的文本
//
LONG Win32GDI::TextHeight(
	HDC hDC,
	LPCTSTR strText)
{
	SIZE size;
	GetTextExtentPoint(hDC, strText, lstrlen(strText), &size);
	return size.cy;
}

//
// 函数：DrawAText
// 功能：画文本
// 参数：hDC，					设备环境
//       strText，				要绘制的文本
//       X，					输出区域X轴坐标
//       Y，					输出区域Y轴坐标
//       nWidth，				输出区域宽度
//       nHeight，				输出区域高度
//       clrTextColor，			文本颜色
//       eFormat，				输出的文本格式
//		 bEnabled，				是否可用
//		 bUseCustomFont，		是否使用自定义字体
//       eFontType，			字体类型
//       strFontName，			字体名称
//       nFontSize，			字体尺寸
//       nFontRotationDegrees，	字体旋转角度
//       bFontBold，			是否出粗体
//       bFontItalic，			是否斜体
//       bFontUnderline，		是否有下划线
//       bFontStrikethru，		是否有强调线
// 例如：下面代码将输出“Hello...”，因为设置的区域不能完全容纳，所以就会显示省略号
//       DrawText hDC,"Hello World", tR, RGB(255,0,0), DT_SINGLELINE Or DT_END_ELLIPSIS
//
VOID Win32GDI::DrawAText(
	HDC hDC, 
	LPCTSTR strText, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrTextColor, 
    TextFormatConstants eFormat, 
	BOOL bEnabled,
	BOOL bUseCustomFont,
    FontTypeConstants eFontType, 
	LPCTSTR strFontName, 
	UINT nFontSize, 
	INT  nFontRotationDegrees, 
	BOOL bFontBold,
	BOOL bFontItalic,
	BOOL bFontUnderline,
	BOOL bFontStrikethru,
	BOOL bFontHighQuality)
{
	RECT tR;
    INT nBkMode;
    COLORREF clrColor;
	
	tR.left = X;
	tR.top = Y;
	tR.right = X + nWidth;
	tR.bottom = Y + nHeight;

	if(bUseCustomFont)
	{
		HFONT hFont;
		HFONT hFontOrg;

		hFont = CreateAFont(hDC, eFontType, strFontName, nFontSize, nFontRotationDegrees, bFontBold, bFontItalic, bFontUnderline, bFontStrikethru, bFontHighQuality);
		hFontOrg = (HFONT)::SelectObject(hDC, hFont);
		nBkMode = ::SetBkMode(hDC, TRANSPARENT);
		
		if (bEnabled)
		{
			clrColor = ::SetTextColor(hDC, TranslateColor(clrTextColor));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}
		else
		{
			clrColor = ::SetTextColor(hDC, RGB(255, 255, 255));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::OffsetRect(&tR, -1, -1);
			::SetTextColor(hDC, RGB(128, 128, 128));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}

		::SetBkMode(hDC, nBkMode);
		::SelectObject(hDC, hFontOrg);
		::DeleteObject(hFont);
	}
	else
	{
		nBkMode = ::SetBkMode(hDC, TRANSPARENT);

		if (bEnabled)
		{
			clrColor = ::SetTextColor(hDC, TranslateColor(clrTextColor));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}
		else
		{
			clrColor = ::SetTextColor(hDC, RGB(255, 255, 255));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::OffsetRect(&tR, -1, -1);
			::SetTextColor(hDC, RGB(128, 128, 128));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}

		::SetBkMode(hDC, nBkMode);
	}
}

VOID Win32GDI::DrawAText(
	HDC hDC,
	LPCTSTR strText,
	RECT tR,
	COLORREF clrTextColor,
	TextFormatConstants eFormat,
	BOOL bEnabled,
	BOOL bUseCustomFont,
	FontTypeConstants eFontType,
	LPCTSTR strFontName,
	UINT nFontSize,
	INT  nFontRotationDegrees,
	BOOL bFontBold,
	BOOL bFontItalic,
	BOOL bFontUnderline,
	BOOL bFontStrikethru,
	BOOL bFontHighQuality)
{
	INT nBkMode;
	COLORREF clrColor;

	if (bUseCustomFont)
	{
		HFONT hFont;
		HFONT hFontOrg;

		hFont = CreateAFont(hDC, eFontType, strFontName, nFontSize, nFontRotationDegrees, bFontBold, bFontItalic, bFontUnderline, bFontStrikethru, bFontHighQuality);
		hFontOrg = (HFONT)::SelectObject(hDC, hFont);
		nBkMode = ::SetBkMode(hDC, TRANSPARENT);

		if (bEnabled)
		{
			clrColor = ::SetTextColor(hDC, TranslateColor(clrTextColor));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}
		else
		{
			clrColor = ::SetTextColor(hDC, RGB(255, 255, 255));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::OffsetRect(&tR, -1, -1);
			::SetTextColor(hDC, RGB(128, 128, 128));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}

		::SetBkMode(hDC, nBkMode);
		::SelectObject(hDC, hFontOrg);
		::DeleteObject(hFont);
	}
	else
	{
		nBkMode = ::SetBkMode(hDC, TRANSPARENT);

		if (bEnabled)
		{
			clrColor = ::SetTextColor(hDC, TranslateColor(clrTextColor));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}
		else
		{
			clrColor = ::SetTextColor(hDC, RGB(255, 255, 255));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::OffsetRect(&tR, -1, -1);
			::SetTextColor(hDC, RGB(128, 128, 128));
			::DrawText(hDC, strText, -1, &tR, eFormat);
			::SetTextColor(hDC, clrColor);
		}

		::SetBkMode(hDC, nBkMode);
	}
}

//
// 函数：DrawControl
// 功能：画控件形状。例如，复选框、单选框、最大化、最小化、关闭按扭、菜单箭头等
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nWidth，	输出区域宽度
//       nHeight，	输出区域高度
//       eType，	控件类型
//       eFlags，	控件格式
// 返回：无
//
VOID Win32GDI::DrawControl(
	HDC hDC, 
	INT X,
	INT Y,
	INT nWidth, 
	INT nHeight,
	FrameControlTypeConstants eType, 
	FrameControlStateConstants eState)
{
	RECT tR; 

	tR.left = X;
	tR.top = Y;
	tR.right = X + nWidth;
	tR.bottom = Y + nHeight;
	DrawFrameControl(hDC, &tR, eType, eState);		   
}

//
// 函数：DrawControlEdge
// 功能：画控件边缘特效。例如：凹下、突起等
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nWidth，	输出区域宽度
//       nHeight，	输出区域高度
//       eStyle，	边缘格式
//       eFlags，	区域类型
// 返回：无
//
VOID Win32GDI::DrawControlEdge(
	HDC hDC, 
	INT X, 
	INT Y,
	INT nWidth, 
	INT nHeight,
	EdgeStyleConstants eStyle, 
	EdgeFlagConstants eFlags)
{
	RECT tR;

	tR.left = X;
	tR.top = Y;
	tR.right = X + nWidth;
	tR.bottom = Y + nHeight;
	DrawEdge(hDC, &tR, eStyle, eFlags);					   
}

//
// 函数：DrawLine
// 功能：画线
// 参数：hDC，		设备环境
//       X1，		输出区域X1轴坐标
//       Y1，		输出区域Y1轴坐标
//       X2，		输出区域X2轴坐标
//       Y2，		输出区域Y2轴坐标
//       clrColor，	颜色
//		 ePenType	画笔类型
//       ePenStyle，画笔样式
//       nWidth，	宽度
// 返回：无
//
VOID Win32GDI::DrawLine(
	HDC hDC,
	INT X1,
	INT Y1,
	INT X2,
	INT Y2,
	COLORREF clrLineColor, 
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle, 
	INT nLineWidth)
{
    HPEN hPen;
    HPEN hPenOrg;
    
    hPen = CreateAPen(ePenType, ePenStyle, clrLineColor, nLineWidth);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
    MoveToEx(hDC, X1, Y1, NULL);
    LineTo(hDC, X2, Y2);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//画圆
VOID Win32GDI::DrawCircle(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrFillColor, 
	COLORREF clrBorderColor,
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch,
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle,
	INT nLineWidth)
{
	HPEN hPen;
    HPEN hPenOrg;
    HBRUSH hBrush;
	HBRUSH hBrushOrg;

    hPen = CreateAPen(ePenType, ePenStyle, clrBorderColor, nLineWidth);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrFillColor, NULL);
    hBrushOrg = (HBRUSH)SelectObject(hDC, hBrush);

    Ellipse(hDC, X, Y, X + nWidth, Y + nHeight);

	SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
	SelectObject(hDC, hBrushOrg);
    DeleteObject(hBrush);
	
	/* 抗锯齿
	int x1, y1, x2, y2;
	x1 = X;
	x2 = X + nWidth;
	y1 = Y;
	y2 = Y + nHeight;
	 
	int centerX = (x1 + x2) / 2;
	int centerY = (y1 + y2) / 2;
	int dx = (x2 - x1);
	int dy = (y2 - y1);
	int radiusX = (int)(fabs(dx) / 2.0f);
	int radiusY = (int)(fabs(dy) / 2.0f);
	float rel = (float)sqrt(radiusX*radiusX + radiusY*radiusY) / (float)radiusY;
	COLORREF bgColor;
	int temp;
	float distance;
	UCHAR red, green, blue;
	
	if (x2 < x1)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
		
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	int xs;
	float yt;
	for (xs=-radiusX; xs<=radiusX; xs++)
	{
		yt = (float)(radiusY * sqrt(1-(float)(xs*xs)/(float)(radiusX*radiusX)));
		distance = (float)(yt - (int)yt);
		
		bgColor = ::GetPixel(hDC, centerX+xs, centerY+(int)yt);
		red = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f-distance)*GetRValue(clrBorderColor));
		green = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f-distance)*GetGValue(clrBorderColor));
		blue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f-distance)*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX+xs, centerY+(int)yt, RGB(red,green,blue));
		
		bgColor = ::GetPixel(hDC, centerX+xs, centerY+(int)yt+1);
		red = (UCHAR)((1.0f-distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(clrBorderColor));
		green = (UCHAR)((1.0f-distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(clrBorderColor));
		blue = (UCHAR)((1.0f-distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX+xs, centerY+(int)yt+1, RGB(red,green,blue));
		
		bgColor = ::GetPixel(hDC, centerX-xs, centerY-(int)yt);
		red = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f-distance)*GetRValue(clrBorderColor));
		green = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f-distance)*GetGValue(clrBorderColor));
		blue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f-distance)*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX-xs, centerY-(int)yt, RGB(red,green,blue));
		
		bgColor = ::GetPixel(hDC, centerX-xs, centerY-(int)yt-1);
		red = (UCHAR)((1.0f-distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(clrBorderColor));
		green = (UCHAR)((1.0f-distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(clrBorderColor));
		blue = (UCHAR)((1.0f-distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX-xs, centerY-(int)yt-1, RGB(red,green,blue));
	}
	
	int ys;
	float xt;
	for (ys=-(int)((float)radiusY/rel); ys<=(int)((float)radiusY/rel); ys++)
	{
		xt = (float)(radiusX * sqrt(1-(float)(ys*ys)/(float)(radiusY*radiusY)));
		distance = (float)(xt - (int)xt);
		
		bgColor = ::GetPixel(hDC, centerX+(int)xt, centerY+ys);
		red = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f-distance)*GetRValue(clrBorderColor));
		green = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f-distance)*GetGValue(clrBorderColor));
		blue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f-distance)*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX+(int)xt, centerY+ys, RGB(red,green,blue));
		
		bgColor = ::GetPixel(hDC, centerX+(int)xt+1, centerY+ys);
		red = (UCHAR)((1.0f-distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(clrBorderColor));
		green = (UCHAR)((1.0f-distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(clrBorderColor));
		blue = (UCHAR)((1.0f-distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX+(int)xt+1, centerY+ys, RGB(red,green,blue));
		
		bgColor = ::GetPixel(hDC, centerX-(int)xt, centerY-ys);
		red = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f-distance)*GetRValue(clrBorderColor));
		green = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f-distance)*GetGValue(clrBorderColor));
		blue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f-distance)*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX-(int)xt, centerY-ys, RGB(red,green,blue));
		
		bgColor = ::GetPixel(hDC, centerX-(int)xt-1, centerY-ys);
		red = (UCHAR)((1.0f-distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(clrBorderColor));
		green = (UCHAR)((1.0f-distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(clrBorderColor));
		blue = (UCHAR)((1.0f-distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(clrBorderColor));
		::SetPixel(hDC, centerX-(int)xt-1, centerY-ys, RGB(red,green,blue));
	}
    */
}

//
// 函数：DrawBorder
// 功能：画边框
// 参数：hDC，				设备环境
//       X，				输出区域X轴坐标
//       Y，				输出区域Y轴坐标
//       nWidth，			输出区域宽度
//       nHeight，			输出区域高度
//       clrBorderColor，	边框颜色
//       ePenType，			边框类型
//       ePenStyle，		边框样式
//       nBorderWidth，		边框宽度
// 返回：无
//
VOID Win32GDI::DrawBorder(
	HDC hDC, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF crBorderColor, 
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle, 
	int nBorderWidth)
{
	HPEN	hPen;
    HPEN 	hPenOrg;
    RECT	tR;

	tR.left = X;
	tR.top = Y;
	tR.right = X + nWidth;
	tR.bottom = Y + nHeight;
    hPen = CreateAPen(ePenType, ePenStyle, crBorderColor, nBorderWidth);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
    MoveToEx(hDC, tR.right, tR.top, NULL);
    LineTo(hDC, tR.left, tR.top);
    LineTo(hDC, tR.left, tR.bottom);
    LineTo(hDC, tR.right, tR.bottom);
    LineTo(hDC, tR.right, tR.top);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//
// 函数：DrawArea
// 功能：画区域
// 参数：hDC，			设备环境
//       X，			区域左边坐标
//       X，			区域顶端坐标
//       nWidth，	    区域宽度
//       nHeight，	    区域高度
//       clrAreaColor，	区域颜色
//       eBrushType，	画刷类型
//       eBrushStyle，	画刷样式
//       eBrushHatch，	画刷阴影线样式
//       hAreaBitmap，	区域位图
// 返回：无
//
VOID Win32GDI::DrawArea(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrAreaColor, 
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch, 
	HBITMAP hAreaBitmap)
{
	RECT tR;
	HBRUSH hBrush;

	tR.left = X;
	tR.top = Y;
	tR.right = X + nWidth;
	tR.bottom = Y + nHeight;
	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrAreaColor, hAreaBitmap);
	FillRect(hDC, &tR, hBrush);
    DeleteObject(hBrush);
}
VOID Win32GDI::DrawArea(
	HDC hDC,
	RECT tR,
	COLORREF clrAreaColor,
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch,
	HBITMAP hAreaBitmap)
{
	HBRUSH hBrush;

	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrAreaColor, hAreaBitmap);
	FillRect(hDC, &tR, hBrush);
	DeleteObject(hBrush);
}

//
// 函数：CopyArea
// 功能：拷贝区域
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nWidth，	输出区域宽度
//       nHeight，	输出区域高度
// 返回：无
//
VOID Win32GDI::CopyArea(
	HDC hDestDC, 
	INT X, 
	INT Y, 
	INT nWidth, 
	INT nHeight,
	HDC hSrcDC, 
	INT SrcX, 
	INT SrcY, 
	INT nSrcWidth, 
	INT nSrcHeight)
{
	INT nPrevBltMode;
	
	nPrevBltMode = SetStretchBltMode(hDestDC, STRETCH_HALFTONE);
	StretchBlt(hDestDC, X, Y, nWidth, nHeight, hSrcDC, SrcX, SrcY, nSrcWidth, nSrcHeight, SRCCOPY);
	SetStretchBltMode(hDestDC, nPrevBltMode);
}

//
// 函数：DrawBox
// 功能：画区域及边框
// 参数：hDC，				设备环境
//       X，				X轴坐标
//       Y，				Y轴坐标
//       nWidth，			区域宽度
//       nHeight，			区域高度
//       clrFillColor，		填充颜色
//       clrBorderColor，	边框颜色
//       eBrushType，		画刷类型
//       eBrushStyle，		画刷样式
//       eBrushHatch，		画刷阴影线样式
//       ePenType，			画笔类型
//       ePenStyle，		画笔样式
//       hFillBitmap，		填充位图
//       nBorderWidth，		边框宽度
// 返回：无
//
VOID Win32GDI::DrawBox(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight, 
	COLORREF clrFillColor,
	COLORREF clrBorderColor, 
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch,
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle,
	HBITMAP hFillBitmap,
	UINT nBorderWidth)
{
	HBRUSH hBrush;
	HBRUSH hBrushOrg;
	HPEN hPen;
	HPEN hPenOrg;
	
	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrFillColor, hFillBitmap);
	hPen = CreateAPen(ePenType, ePenStyle, clrBorderColor, nBorderWidth);
	hBrushOrg = (HBRUSH)SelectObject(hDC, hBrush);
	hPenOrg = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, X, Y, X + nWidth, Y + nHeight);
	SelectObject(hDC, hBrushOrg);
	SelectObject(hDC, hPenOrg);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

//
// 函数：DrawAlphaBox
// 功能：画透明区域及边框
// 参数：hDC，				设备环境
//       X，				X轴坐标
//       Y，				Y轴坐标
//       nWidth，			区域宽度
//       nHeight，			区域高度
//       clrFillColor，		填充颜色
//       clrBorderColor，	边框颜色
//       eBrushType，		画刷类型
//       eBrushStyle，		画刷样式
//       eBrushHatch，		画刷阴影线样式
//       ePenType,			画笔类型
//       ePenStyle，		画笔样式
//       hFillBitmap，		填充位图句柄
//       nBorderWidth，		边框宽度
//       nAlpha，			透明度
// 返回：无
//
VOID Win32GDI::DrawAlphaBox(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight, 
	COLORREF clrFillColor,
	COLORREF clrBorderColor, 
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch,
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle, 
	HBITMAP hFillBitmap,
	UINT nBorderWidth, 
	BYTE nAlpha)
{
	HDC hMemDC;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	BLENDFUNCTION tBFN;
		
	HBRUSH hBrush;
	HBRUSH hBrushOrg;
	HPEN hPen;
	HPEN hPenOrg;
		
	//创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	
	//在内存DC画Box
	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrFillColor, hFillBitmap);
	hPen = CreateAPen(ePenType, ePenStyle, clrBorderColor, nBorderWidth);
	hBrushOrg = (HBRUSH)SelectObject(hMemDC, hBrush);
	hPenOrg = (HPEN)SelectObject(hMemDC, hPen);
	Rectangle(hMemDC, 0, 0, nWidth, nHeight);
	SelectObject(hMemDC, hBrushOrg);
	SelectObject(hMemDC, hPenOrg);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	
	//透明拷贝
	tBFN.BlendFlags = 0;
	tBFN.BlendOp = AC_SRC_OVER;
	tBFN.AlphaFormat = 0;
	tBFN.SourceConstantAlpha = nAlpha;
	AlphaBlend(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, nWidth, nHeight, tBFN);

	//删除内存DC
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
    DeleteDC(hMemDC);
}

//
// 函数：DrawRoundBorder
// 功能：画圆角边框
// 参数：hDC，				设备环境
//       X，				输出区域X轴坐标
//       Y，				输出区域Y轴坐标
//       nWidth，			输出区域宽度
//       nHeight，			输出区域高度
//       nRoundValue，		圆角值
//       clrBorderColor，	边框颜色
//       ePenType,			边框类型
//       ePenStyle，		边框样式
//       nBorderWidth，		边框宽度
// 返回：无
//
VOID Win32GDI::DrawRoundBorder(
	HDC hDC, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	UINT nRoundValue, 
	COLORREF crBorderColor, 
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle, 
	UINT nBorderWidth)
{
	HPEN	hPen;
    HPEN 	hPenOrg;
   
    hPen = CreateAPen(ePenType, ePenStyle, crBorderColor, nBorderWidth);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, X, Y, X + nWidth, Y + nHeight, nRoundValue, nRoundValue);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//
// 函数：DrawRoundArea
// 功能：画圆角形区域
// 参数：hDC，			设备环境
//       X，			区域左边坐标
//       X，			区域顶端坐标
//       nWidth，	    区域宽度
//       nHeight，	    区域高度
//       nRoundValue，	圆角值
//       clrAreaColor，	区域颜色
//       eBrushType，	画刷类型
//       eBrushStyle，	画刷样式
//       eBrushHatch，	画刷阴影线样式
//       hAreaBitmap，	区域位图
// 返回：无
//
VOID Win32GDI::DrawRoundArea(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	UINT nRoundValue, 
	COLORREF clrAreaColor, 
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch, 
	HBITMAP hAreaBitmap)
{
	HBRUSH hBrush;
	HBRUSH hBrushOrg;

	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrAreaColor, hAreaBitmap);
	hBrushOrg = (HBRUSH)SelectObject(hDC, hBrush);
	RoundRect(hDC, X, Y, X + nWidth, Y + nHeight, nRoundValue, nRoundValue);
    SelectObject(hDC, hBrushOrg);
	DeleteObject(hBrush);
}

//
// 函数：DrawRoundBox
// 功能：画圆角区域及边框
// 参数：hDC，				设备环境
//       X，				X轴坐标
//       Y，				Y轴坐标
//       nWidth，			区域宽度
//       nHeight，			区域高度
//       nRoundValue，		圆角值
//       clrFillColor，		填充颜色
//       clrBorderColor，	边框颜色
//       eBrushType，		画刷类型
//       eBrushStyle，		画刷样式
//       eBrushHatch，		画刷阴影线样式
//       ePenType,			画笔类型
//       ePenStyle，		画笔样式
//       hFillBitmap，		填充位图句柄
//       nBorderWidth，		边框宽度
// 返回：无
//
VOID Win32GDI::DrawRoundBox(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight, 
	UINT nRoundValue, 
	COLORREF clrFillColor,
	COLORREF clrBorderColor, 
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch,
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle, 
	HBITMAP hFillBitmap,
	UINT nBorderWidth)
{
	HBRUSH hBrush;
	HBRUSH hBrushOrg;
	HPEN hPen;
	HPEN hPenOrg;
	
	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrFillColor, hFillBitmap);
	hPen = CreateAPen(ePenType, ePenStyle, clrBorderColor, nBorderWidth);
	hBrushOrg = (HBRUSH)SelectObject(hDC, hBrush);
	hPenOrg = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, X, Y, X + nWidth, Y + nHeight, nRoundValue, nRoundValue);
	SelectObject(hDC, hBrushOrg);
	SelectObject(hDC, hPenOrg);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

//
// 函数：DrawAlphaRoundBox
// 功能：画透明圆角区域及边框
// 参数：hDC，				设备环境
//       X，				X轴坐标
//       Y，				Y轴坐标
//       nWidth，			区域宽度
//       nHeight，			区域高度
//		 nRoundValue，		圆角值
//       clrFillColor，		填充颜色
//       clrBorderColor，	边框颜色
//       eBrushType，		画刷类型
//       eBrushStyle，		画刷样式
//       eBrushHatch，		画刷阴影线样式
//       ePenType,			画笔类型
//       ePenStyle，		画笔样式
//       hFillBitmap，		填充位图句柄
//       nBorderWidth，		边框宽度
//       nAlpha，			透明度
// 返回：无
//
VOID Win32GDI::DrawAlphaRoundBox(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight, 
	UINT nRoundValue, 
	COLORREF clrFillColor,
	COLORREF clrBorderColor, 
	BrushTypeConstants eBrushType,
	BrushStyleConstants eBrushStyle,
	BrushHatchConstants eBrushHatch,
	PenTypeConstants ePenType,
	PenStyleConstants ePenStyle, 
	HBITMAP hFillBitmap,
	UINT nBorderWidth, 
	BYTE nAlpha)
{
	HDC hMemDC;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	BLENDFUNCTION uBFN;
		
	HBRUSH hBrush;
	HBRUSH hBrushOrg;
	HPEN hPen;
	HPEN hPenOrg;
		
	//创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	
	//在内存DC画Box
	hBrush = CreateABrush(eBrushType, eBrushStyle, eBrushHatch, clrFillColor, hFillBitmap);
	hPen = CreateAPen(ePenType, ePenStyle, clrBorderColor, nBorderWidth);
	hBrushOrg = (HBRUSH)SelectObject(hMemDC, hBrush);
	hPenOrg = (HPEN)SelectObject(hMemDC, hPen);
	RoundRect(hMemDC, 0, 0, nWidth, nHeight, nRoundValue, nRoundValue);
	SelectObject(hMemDC, hBrushOrg);
	SelectObject(hMemDC, hPenOrg);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	
	//透明拷贝
	uBFN.BlendFlags = 0;
	uBFN.BlendOp = AC_SRC_OVER;
	uBFN.AlphaFormat = 0;
	uBFN.SourceConstantAlpha = nAlpha;
	AlphaBlend(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, nWidth, nHeight, uBFN);
	
	//删除内存DC
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
    DeleteDC(hMemDC);
}

//
// 功能：画多边形区域
// 参数：hDC，				设备环境
//		 pPoint，			多边形点
//		 clrFillColor，		填充颜色
//		 clrBorderColor，	边框颜色
//		 nBorderWidth，		边框线宽度
// 返回：无
//
VOID Win32GDI::DrawPolyArea(
	HDC hDC, 
	POINT tPt[], 
	COLORREF clrFillColor,
	COLORREF clrBorderColor, 
	UINT nBorderWidth)
{
	HBRUSH hBrush;
	HBRUSH hBrushOrg;
	HPEN hPen;
	HPEN hPenOrg;
	
	hBrush = CreateABrush(btSolidBrush, bsSolid, hsSolid, clrFillColor);
	hPen = CreateAPen(ptSolidPen, psSolid, clrBorderColor, nBorderWidth);
	hBrushOrg = (HBRUSH)SelectObject(hDC, hBrush);
	hPenOrg = (HPEN)SelectObject(hDC, hPen);
	Polyline(hDC, tPt, sizeof(tPt));
	Polygon(hDC, tPt, sizeof(tPt));
	SelectObject(hDC, hBrushOrg);
	SelectObject(hDC, hPenOrg);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

//
// 函数：DrawFocus
// 功能：画焦点
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nWidth，	输出区域宽度
//       nHeight，	输出区域高度
// 返回：无
//
VOID Win32GDI::DrawFocus(
	HDC hDC, 
	INT X, 
	INT Y, 
	INT nWidth, 
	INT nHeight)
{
	RECT tR;

	tR.left = X;
	tR.top = Y;
	tR.right = X + nWidth;
	tR.bottom = Y + nHeight;
	DrawFocusRect(hDC, &tR);
}

//
// 函数：DrawCheck
// 功能：画复选钩
// 参数：hDC，			设备环境
//       lngX，			输出区域X轴坐标
//       lngY，			输出区域Y轴坐标
//       lngWidth，		输出区域宽度
//       lngHeight，	输出区域高度
//       clrColor，		复选钩颜色
//		 nLineWidth,	线宽
// 返回：无
//
VOID Win32GDI::DrawCheck(
	HDC hDC,
	INT X , 
	INT Y, 
	INT nWidth, 
	INT nHeight, 
	COLORREF clrColor,
	UINT nLineWidth)
{
	POINT tPt;
	HPEN hPen;
	HPEN hPenOrg;
	int i;
	int j;

	hPen = CreatePen(PS_SOLID, nLineWidth, clrColor);
	hPenOrg = (HPEN)SelectObject(hDC, hPen);

	i = X + (int)(nWidth / 2) - 3;
	for(j=0; j<=2; j++)
	{
		MoveToEx(hDC, i + j, Y + nHeight / 2 - 2 + j + 1, &tPt);
		LineTo(hDC, i + j, Y + nHeight / 2 + j + 1);
	}

	i = X + (int)(nWidth / 2) - 1;
	for(j=1; j<=4; j++)
	{
		MoveToEx(hDC, i + j, Y + nHeight / 2 - j + 1, &tPt);
		LineTo(hDC, i + j, Y + nHeight / 2 - j + 3);
	}
	
	SelectObject(hDC, hPenOrg);
	DeleteObject(hPen);
}

//
// 函数：DrawArrow
// 功能：画箭头
// 参数：hDC，			设备环境
//       X，			输出区域X轴坐标
//       Y，			输出区域Y轴坐标
//       nWidth，		输出区域宽度
//       nHeight，		输出区域高度
//       eDirection，	箭头方向
//       clrColor，		箭头颜色
// 返回：无
//
VOID Win32GDI::DrawArrow(
	HDC hDC, 
	INT X, 
	INT Y, 
	INT nWidth, 
	INT nHeight,
	ArrowDirectionConstants eDirection,
	COLORREF clrColor)
{
	POINT tPt[4];

	switch(eDirection)
	{
	case adWest: //←
		tPt[0].x = X;
		tPt[0].y = Y + nHeight / 2;
		tPt[1].x = X + nWidth;
		tPt[1].y = Y;
		tPt[2].x = X + nWidth;
		tPt[2].y = Y + nHeight;
		tPt[3].x = X;
		tPt[3].y = Y + nHeight / 2;
		break;

	case adWestNorth://↖
		tPt[0].x = X;
		tPt[0].y = Y;
		tPt[1].x = X;
		tPt[1].y = Y + nHeight;
		tPt[2].x = X + nWidth;
		tPt[2].y = Y;
		tPt[3].x = X;
		tPt[3].y = Y;
		break;

	case adNorth://↑
		tPt[0].x = X + nWidth / 2;
		tPt[0].y = Y;
		tPt[1].x = X + nWidth;
		tPt[1].y = Y + nHeight;
		tPt[2].x = X;
		tPt[2].y = Y + nHeight;
		tPt[3].x = X + nWidth / 2;
		tPt[3].y = Y;
		break;

	case adEastNorth://↗
		tPt[0].x = X;
		tPt[0].y = Y;
		tPt[1].x = X + nWidth;
		tPt[1].y = Y;
		tPt[2].x = X + nWidth;
		tPt[2].y = Y + nHeight;
		tPt[3].x = X;
        tPt[3].y = Y;
		break;

	case adEast://→
		tPt[0].x = X;
		tPt[0].y = Y;
		tPt[1].x = X + nWidth;
		tPt[1].y = Y + nHeight / 2;
		tPt[2].x = X;
		tPt[2].y = Y + nHeight;
		tPt[3].x = X;
		tPt[3].y = Y;
		break;

	case adEastSouth://↘
		tPt[0].x = X;
		tPt[0].y = Y + nHeight;
		tPt[1].x = X + nWidth;
		tPt[1].y = Y + nHeight;
		tPt[2].x = X + nWidth;
		tPt[2].y = Y;
		tPt[3].x = X;
        tPt[3].y = Y + nHeight;
		break;

	case adSouth://↓
		tPt[0].x = X;
		tPt[0].y = Y;
		tPt[1].x = X + nWidth;
		tPt[1].y = Y;
		tPt[2].x = X + nWidth / 2;
		tPt[2].y = Y + nHeight;
		tPt[3].x = X;
		tPt[3].y = Y;
		break;

	case adWestSouth://↙
		tPt[0].x = X;
		tPt[0].y = Y;
		tPt[1].x = X;
		tPt[1].y = Y + nHeight;
		tPt[2].x = X + nWidth;
		tPt[2].y = Y + nHeight;
		tPt[3].x = X;
        tPt[3].y = Y;
		break;
	}

	DrawPolyArea(hDC, tPt, clrColor, clrColor, 1);			 
}

//
// 函数：DrawClose
// 功能：画关闭
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nSize，	尺寸
//       clrColor，	颜色
//		 nLineWidth,线宽
// 返回：无
//
VOID Win32GDI::DrawClose(
	HDC hDC, 
	INT X, 
	INT Y,
	UINT nSize,
	COLORREF clrColor,
	UINT nLineWidth
	)
{
	HPEN hPen;
    HPEN hPenOrg;
    
    hPen = CreatePen(PS_SOLID, nLineWidth, clrColor);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
    MoveToEx(hDC, X, Y, NULL);
    LineTo(hDC, X + nSize, Y + nSize);
    MoveToEx(hDC, X + nSize, Y, NULL);
    LineTo(hDC, X, Y + nSize);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//
// 函数：DrawRestore
// 功能：画还原按钮
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nSize，	尺寸
//       clrColor，	颜色
//		 nLineWidth,线宽
// 返回：无
//
VOID Win32GDI::DrawRestore(
	HDC hDC, 
	INT X, 
	INT Y,
	UINT nSize,
	COLORREF clrColor,
	UINT nLineWidth)
{
    HPEN hPen;
    HPEN hPenOrg;
    INT nFactor;
    
    hPen = CreatePen(PS_SOLID, nLineWidth, clrColor);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
    nFactor = nSize / 3;
    MoveToEx(hDC, X + nFactor, Y, NULL);
    LineTo(hDC, X + nFactor, Y + nFactor);
    MoveToEx(hDC, X + nFactor, Y, NULL);
    LineTo(hDC, X + nSize, Y);
    MoveToEx(hDC, X + nFactor, Y + 1, NULL);
    LineTo(hDC, X + nSize, Y + 1);
    MoveToEx(hDC, X + nSize, Y, NULL);
    LineTo(hDC, X + nSize, Y + nSize - nFactor);
    MoveToEx(hDC, X + nSize, Y + nSize - nFactor, NULL);
    LineTo(hDC, X + nSize - nFactor, Y + nSize - nFactor);

    MoveToEx(hDC, X, Y + nFactor, NULL);
    LineTo(hDC, X + nSize - nFactor, Y + nFactor);
    LineTo(hDC, X + nSize - nFactor, Y + nSize);
    LineTo(hDC, X, Y + nSize);
    LineTo(hDC, X, Y + nFactor);
    MoveToEx(hDC, X, Y + nFactor + 1, NULL);
    LineTo(hDC, X + nSize - nFactor, Y + nFactor + 1);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//
// 函数：DrawMax
// 功能：画最大化按钮
// 参数：hDC，		设备环境
//       X，		输出区域X轴坐标
//       Y，		输出区域Y轴坐标
//       nSize，	尺寸
//       clrColor，	颜色
//		 nLineWidth,线宽
// 返回：无
//
VOID Win32GDI::DrawMax(
	HDC hDC, 
	INT X, 
	INT Y,
	UINT nSize,
	COLORREF clrColor,
	UINT nLineWidth)
{
	HPEN hPen;
    HPEN hPenOrg;
    
    hPen = CreatePen(PS_SOLID, nLineWidth, clrColor);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
    MoveToEx(hDC, X, Y, NULL);
    LineTo(hDC, X + nSize, Y);
    LineTo(hDC, X + nSize, Y + nSize);
    LineTo(hDC, X, Y + nSize);
    LineTo(hDC, X, Y);
    MoveToEx(hDC, X, Y + 1, NULL);
    LineTo(hDC, X + nSize, Y + 1);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//
// 函数：DrawMin
// 功能：画最小化按钮
// 参数：hDC，			设备环境
//       X，			输出区域X轴坐标
//       Y，			输出区域Y轴坐标
//       nSize，		尺寸
//       clrColor，		颜色
//		 nLineWidth，	线宽
// 返回：无
//
VOID Win32GDI::DrawMin(
	HDC hDC, 
	INT X, 
	INT Y,
	UINT nSize,
	COLORREF clrColor,
	UINT nLineWidth)
{
	HPEN hPen;
    HPEN hPenOrg;
    
    hPen = CreatePen(PS_SOLID, nLineWidth, clrColor);
    hPenOrg = (HPEN)SelectObject(hDC, hPen);
    MoveToEx(hDC, X, Y, NULL);
    LineTo(hDC, X + nSize, Y);
    MoveToEx(hDC, X, Y + 1, NULL);
    LineTo(hDC, X + nSize, Y + 1);
    SelectObject(hDC, hPenOrg);
    DeleteObject(hPen);
}

//
// 函数：DrawDigit
// 功能：画数字。
// 参数：hDC，	设备环境
//       X，	X位置
//       Y，	Y位置
//       nDigit，数字
//       nWidth，数字宽度
//
VOID Win32GDI::DrawDigit(
	HDC hDC,
	INT X,
	INT Y,
	UINT nDigit,
	UINT nWidth,
	COLORREF clrColor)
{
	INT idWidth;
	INT idHeight;
	HPEN hPenOrg;
	HPEN hPen;

	hPen = CreateAPen(ptSolidPen, psSolid, clrColor);
	hPenOrg = (HPEN)SelectObject(hDC, hPen);

	if (nWidth <= 4)
	{
		idWidth = (nWidth <= 3)?3:4;
		idHeight = idWidth * 2;
	}
	else
	{
		idWidth = nWidth;
		idHeight = nWidth * 2 + 3;
	}

	switch(nDigit)
	{
	case 0:
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + idWidth, Y);
		LineTo(hDC, X + idWidth, Y + idHeight);
		LineTo(hDC, X, Y + idHeight);
		LineTo(hDC, X, Y);
		break;

	case 1:
		MoveToEx(hDC, X + idWidth, Y, NULL);
		LineTo(hDC, X + idWidth, Y + idHeight + 1);
		break;

	case 2:
		MoveToEx(hDC, X, Y, 0);
		LineTo(hDC, X + idWidth, Y);
		LineTo(hDC, X + idWidth, Y + idWidth);
		LineTo(hDC, X, Y + idWidth);
		LineTo(hDC, X, Y + idHeight);
		LineTo(hDC, X + idWidth + 1, Y + idHeight);
		break;

	case 3:
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + idWidth, Y);
		LineTo(hDC, X + idWidth, Y + idWidth);
		LineTo(hDC, X - 1, Y + idWidth);
		MoveToEx(hDC, X + idWidth, Y + idWidth, NULL);
		LineTo(hDC, X + idWidth, Y + idHeight);
		LineTo(hDC, X - 1, Y + idHeight);
		break;

	case 4:
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X, Y + idWidth);
		LineTo(hDC, X + idWidth, Y + idWidth);
		MoveToEx(hDC, X + idWidth, Y, NULL);
		LineTo(hDC, X + idWidth, Y + idHeight + 1);
		break;

	case 5:
		MoveToEx(hDC, X + idWidth, Y, NULL);
		LineTo(hDC, X, Y);
		LineTo(hDC, X, Y + idWidth);
		LineTo(hDC, X + idWidth, Y + idWidth);
		LineTo(hDC, X + idWidth, Y + idHeight);
		LineTo(hDC, X - 1, Y + idHeight);
		break;

	case 6:
		MoveToEx(hDC, X + idWidth, Y, NULL);
		LineTo(hDC, X, Y);
		LineTo(hDC, X, Y + idHeight);
		LineTo(hDC, X + idWidth, Y + idHeight);
		LineTo(hDC, X + idWidth, Y + idWidth);
		LineTo(hDC, X, Y + idWidth);
		break;

	case 7:
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + idWidth, Y);
		LineTo(hDC, X + idWidth, Y + idHeight + 1);
		break;

	case 8:
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + idWidth, Y);
		LineTo(hDC, X + idWidth, Y + idHeight);
		LineTo(hDC, X, Y + idHeight);
		LineTo(hDC, X, Y);
		MoveToEx(hDC, X, Y + idWidth, NULL);
		LineTo(hDC, X + idWidth, Y + idWidth);
		break;

	case 9:
		MoveToEx(hDC, X + idWidth, Y + idWidth, NULL);
		LineTo(hDC, X, Y + idWidth);
		LineTo(hDC, X, Y);
		LineTo(hDC, X + idWidth, Y);
		LineTo(hDC, X + idWidth, Y + idHeight);
		LineTo(hDC, X - 1, Y + idHeight);
		break;

	}

	SelectObject(hDC, hPenOrg);
	DeleteObject(hPen);
}

 //
 // 函数：DrawCharacter
 // 功能：画字符
 // 参数：hDC，设备环境
 //       X，X位置
 //       Y，Y位置
 //       strLetter，字符
 //
VOID Win32GDI::DrawCharacter(
	HDC hDC,
	INT X,
	INT Y,
	LPCTSTR strLetter,
	COLORREF clrColor)
{
	HPEN hPenOrg;
	HPEN hPen;

	hPen = Win32GDI::CreateAPen(ptSolidPen, psSolid, clrColor);
	hPenOrg = (HPEN)Win32GDI::SelectAObject(hDC, hPen);

	switch(toascii(toupper(*strLetter)))
	{
	case 37://%
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + 2, Y);
		LineTo(hDC, X + 2, Y + 2);
		LineTo(hDC, X, Y + 2);
		LineTo(hDC, X, Y);

		MoveToEx(hDC, X + 4, Y + 4, NULL);
		LineTo(hDC, X + 6, Y + 4);
		LineTo(hDC, X + 6, Y + 6);
		LineTo(hDC, X + 4, Y + 6);
		LineTo(hDC, X + 4, Y + 4);

		MoveToEx(hDC, X + 6, Y, NULL);
		LineTo(hDC, X - 1, Y + 7);
		break;

	case 44://,
		MoveToEx(hDC, X + 1, Y, NULL);
		LineTo(hDC, X, Y + 2);
		break;

	case 45://-
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + 3, Y);
		break;

	case 47:///
		MoveToEx(hDC, X + 4, Y, NULL);
		LineTo(hDC, X, Y + 7);
		break;

	case 58://:
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + 1, Y);
		MoveToEx(hDC, X, Y + 3, NULL);
		LineTo(hDC, X + 1, Y + 3);
		break;

	case 72://H
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X, Y + 7);
		MoveToEx(hDC, X + 3, Y, NULL);
		LineTo(hDC, X + 3, Y + 7);
		MoveToEx(hDC, X, Y + 3, NULL);
		LineTo(hDC, X + 3, Y + 3);
		break;

	case 75://K
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X, Y + 7);
		MoveToEx(hDC, X + 4, Y, NULL);
		LineTo(hDC, X, Y + 4);
		MoveToEx(hDC, X, Y + 2, NULL);
		LineTo(hDC, X + 5, Y + 7);
		break;

	case 77://M
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X, Y + 7);
		MoveToEx(hDC, X + 1, Y + 1, NULL);
		LineTo( hDC, X + 1, Y + 3);
		MoveToEx(hDC, X + 2, Y + 3, NULL);
		LineTo(hDC, X + 2, Y + 5);
		MoveToEx(hDC, X + 3, Y + 1, NULL);
		LineTo(hDC, X + 3, Y + 3);
		MoveToEx(hDC, X + 4, Y, NULL);
		LineTo(hDC, X + 4, Y + 7);
		break;

	case 90://Z
		MoveToEx(hDC, X, Y, NULL);
		LineTo(hDC, X + 3, Y);
		LineTo(hDC, X, Y + 3);
		LineTo(hDC, X + 4, Y + 3);
		break;
	}

	SelectObject(hDC, hPenOrg);
	DeleteObject(hPen);
}

//
// 函数：DrawGradient
// 功能：画渐变
// 参数：hDC，			设备环境
//       X，			区域左边坐标
//       X，			区域顶端坐标
//       nWidth，	    区域宽度
//       nHeight，	    区域高度
//       clrStartColor，开始颜色
//       clrEndColor，	结束颜色
//       eDirection，	渐变方向
// 返回：无
//
VOID Win32GDI::DrawGradient(
	HDC hDC, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrStartColor, 
	COLORREF clrEndColor, 
	GradientDirectionConstants eDirection)
{
	TRIVERTEX tVert[2];
	GRADIENT_RECT tGR;
	COLORREF cBeginColor = TranslateColor(clrStartColor);
	COLORREF cEndColor = TranslateColor(clrEndColor);

	tVert[0].x = X;
	tVert[0].y = Y;
	tVert[0].Red = UINTToUSHORT(GetRValue(cBeginColor) * 0x100);
	tVert[0].Green = UINTToUSHORT(GetGValue(cBeginColor) * 0x100);
	tVert[0].Blue = UINTToUSHORT(GetBValue(cBeginColor) * 0x100);
	tVert[0].Alpha = 0;
	
	tVert[1].x = X + nWidth;
	tVert[1].y = Y + nHeight;
	tVert[1].Red = UINTToUSHORT(GetRValue(cEndColor) * 0x100);
	tVert[1].Green = UINTToUSHORT(GetGValue(cEndColor) * 0x100);
	tVert[1].Blue = UINTToUSHORT(GetBValue(cEndColor) * 0x100);
	tVert[1].Alpha = 0;
	
	tGR.UpperLeft = 0;
	tGR.LowerRight = 1;
	GradientFill(hDC, tVert, 2, &tGR, 1, eDirection);
}

//
// 函数：DrawAlphaGradient
// 功能：画透明渐变
// 参数：hDC，			设备环境
//       X，			区域左边坐标
//       X，			区域顶端坐标
//       nWidth，	    区域宽度
//       nHeight，	    区域高度
//       clrStartColor，开始颜色
//       clrEndColor，	结束颜色
//       eDirection，	渐变方向
//       nAlpha，		透明度，默认=100
// 返回：无
//
VOID Win32GDI::DrawAlphaGradient(
	HDC hDC, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrStartColor, 
	COLORREF clrEndColor, 
	GradientDirectionConstants eDirection,
	BYTE nAlpha)
{
	HDC hMemDC;
	HBITMAP hBitmap;
	HBITMAP hBitmapOrg;
    
	TRIVERTEX tVert[2];
	GRADIENT_RECT tGR;
	
	BLENDFUNCTION tBFN; 

	COLORREF cBeginColor = TranslateColor(clrStartColor);
	COLORREF cEndColor = TranslateColor(clrEndColor);

	//创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    hBitmapOrg = (HBITMAP)SelectObject(hMemDC, hBitmap);

	//画渐变
	tVert[0].x = 0;
	tVert[0].y = 0;
	tVert[0].Red = UINTToUSHORT(GetRValue(cBeginColor) * 0x100);
	tVert[0].Green = UINTToUSHORT(GetGValue(cBeginColor) * 0x100);
	tVert[0].Blue = UINTToUSHORT(GetBValue(cBeginColor) * 0x100);
	tVert[0].Alpha = 0;
	
	tVert[1].x = nWidth;
	tVert[1].y = nHeight;
	tVert[1].Red = UINTToUSHORT(GetRValue(cEndColor) * 0x100);
	tVert[1].Green = UINTToUSHORT(GetGValue(cEndColor) * 0x100);
	tVert[1].Blue = UINTToUSHORT(GetBValue(cEndColor) * 0x100);
	tVert[1].Alpha = 0;
	
	tGR.UpperLeft = 0;
	tGR.LowerRight = 1;
	GradientFill(hMemDC, tVert, 2, &tGR, 1, eDirection);

	//透明拷贝
	tBFN.BlendFlags = 0;
	tBFN.BlendOp = AC_SRC_OVER;
	tBFN.AlphaFormat = 0;
	tBFN.SourceConstantAlpha = nAlpha;
	AlphaBlend(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, nWidth, nHeight, tBFN);

	//删除内存DC
	SelectObject(hMemDC, hBitmapOrg);
	DeleteObject(hBitmap);
    DeleteDC(hMemDC);
}
 
//
// 函数：TransparentPaintImage
// 功能：位图透明拷贝(需要遮罩图片)
// 参数：hDestDC，	目标设备环境
//       xDest，	目标输出区域X轴坐标
//       yDest，	目标输出区域Y轴坐标
//       hImage，	源图像
//       hMask，	遮罩图像
// 返回：无
//
VOID Win32GDI::TransparentPaintImage(
	HDC hDestDC,
	INT xDest, 
	INT yDest, 
	HBITMAP hImage, 
	HBITMAP hMask)
{
	BITMAP bmp;
    HDC hSrcDC;
    HDC hMaskDC;

    hSrcDC = CreateCompatibleDC(hDestDC);
    hMaskDC = CreateCompatibleDC(hDestDC);
    SelectObject(hSrcDC, hImage);
    SelectObject(hMaskDC, hMask);
    GetObject(hImage, sizeof(bmp), &bmp);
    BitBlt(hDestDC, xDest, yDest, bmp.bmWidth, bmp.bmHeight, hMaskDC, 0, 0, MERGEPAINT);
    BitBlt(hDestDC, xDest, yDest, bmp.bmWidth, bmp.bmHeight, hSrcDC, 0, 0, SRCAND);
    DeleteDC(hMaskDC);
    DeleteDC(hSrcDC);
}

//
// 函数：TransparentPaintArea
// 功能：位图透明拷贝(利用TransparentBlt API，速度更快)
// 参数：hDestDC，				目标设备环境
//       xDest，				目标输出区域X轴坐标
//       yDest，				目标输出区域Y轴坐标
//       nDestWidth，			目标输出区域宽度
//       nDestHeight，			目标输出区域高度
//       hSrcDC，				源设备环境
//       xSrc，					源输出区域X轴坐标
//       ySrc，					源输出区域Y轴坐标
//       nSrcWidth，			源输出区域宽度
//       nSrcHeight，			源输出区域高度
//       clrTransparentColor，	透明色
// 返回：无
//
VOID Win32GDI::TransparentPaintArea( 
	HDC hDestDC, 
	INT xDest, 
	INT yDest, 
	INT nDestWidth, 
	INT nDestHeight, 
	HDC hSrcDC, 
	INT xSrc, 
	INT ySrc, 
	INT nSrcWidth, 
	INT nSrcHeight, 
	COLORREF clrTransparentColor)
{
	TransparentBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 
	hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, 
	clrTransparentColor);			   
}

//
// 函数：TransparentPaintAlpha
// 功能：位图拷贝(带Alpha通道值)
// 参数：hDestDC，		目标设备环境
//       xDest，		目标输出区域X轴坐标
//       yDest，		目标输出区域Y轴坐标
//       nDestWidth，	目标输出区域宽度
//       nDestHeight，	目标输出区域高度
//       hSrcDC，		源设备环境
//       xSrc，			源输出区域X轴坐标
//       ySrc，			源输出区域Y轴坐标
//       nSrcWidth，	源输出区域宽度
//       nSrcHeight，	源输出区域高度
//       nAlpha，		Alpha通道值
// 返回：无
//
VOID Win32GDI::TransparentPaintAlpha(
	HDC hDestDC, 
	INT xDest,
	INT yDest, 
	INT nDestWidth, 
	INT nDestHeight, 
	HDC hSrcDC, 
	INT xSrc,
	INT ySrc, 
	INT nSrcWidth, 
	INT nSrcHeight,
	BYTE nAlpha)
{
	BLENDFUNCTION tBFN;

    tBFN.BlendFlags = 0;
    tBFN.BlendOp = AC_SRC_OVER;
    tBFN.AlphaFormat = 0;//AC_SRC_ALPHA;
    tBFN.SourceConstantAlpha = nAlpha;

    AlphaBlend(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 
    hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, tBFN);
}

//
// 函数：DrawIcon
// 功能：画图标
// 参数：hDC，					设备环境
//       X，					输出区域X轴坐标
//       Y，					输出区域Y轴坐标
//       bEnabled，				图标是否可用
//       clrDisabledColor，		不可用颜色
// 返回：无
//
VOID Win32GDI::DrawIcon(
	HDC hDC,
	HGDIOBJ hIcon, 
	INT X, 
	INT Y, 
	BOOL bEnabled,
	COLORREF clrDisabledColor)
{
	INT nFlag;

	switch(GetObjectType(hIcon))
	{
	case OBJ_BITMAP:
		nFlag = DST_BITMAP;
		break;
	default:
		nFlag = DST_ICON;
		break;
	}

	if (bEnabled) 
	{
		DrawState(hDC, 0, 0, (LPARAM)hIcon, 0, X, Y, 0, 0, nFlag | DSS_NORMAL);
	}
	else
	{
		//DrawState(hDC, 0, 0, hIcon, 0, X, Y, 0, 0, nFlag | DSS_DISABLED);
		HBRUSH hBrush = CreateSolidBrush(clrDisabledColor);
		DrawState(hDC, hBrush, 0, (LPARAM)hIcon, 0, X, Y, 0, 0, nFlag | DSS_MONO);
		DeleteObject(hBrush);
	}			
}

//
// 函数：DrawImage
// 功能：画图片
// 参数：hDC，设备环境
//       hImage，图像
//       X，输出区域X轴坐标
//       Y，输出区域Y轴坐标
//       nWidth，输出区域宽度
//       nHeight，输出区域高度
//       eLayout，图像布局
// 返回：无
//
VOID Win32GDI::DrawImage(
	HDC hDC,
	HGDIOBJ hImage,
	INT X, 
	INT Y, 
	INT nWidth, 
	INT nHeight,
	ImageLayoutConstants eLayout)
{
	BITMAP bm;
	HBITMAP hBmp;
	HBITMAP hPrevBmp;
	INT nImageWidth;
	INT nImageHeight;
	HDC hMemDC;

	//创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	
	//获得图像大小
	GetObject(hImage, sizeof(bm), &bm);
	nImageWidth = bm.bmWidth;
	nImageHeight = bm.bmHeight;
	hBmp = (HBITMAP)hImage;
	
	//将图像放入内存DC
	hPrevBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

	switch(eLayout)
	{
	case ilNone:
		BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);
		break;

	case ilCenter:
		X = X + (nWidth > nImageWidth) ? (nWidth - nImageWidth) / 2 : 0;
		Y = Y + (nHeight > nImageHeight) ? (nHeight - nImageHeight) / 2: 0;
		BitBlt(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);
		break;

	case ilStretch:
		INT nPrevBltMode;
		nPrevBltMode = SetStretchBltMode(hDC, STRETCH_HALFTONE);
		StretchBlt(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, nImageWidth, nImageHeight, SRCCOPY);
		SetStretchBltMode(hDC, nPrevBltMode);
		break;

	case ilTile:
		RECT tR;
		HBRUSH hBrush;
		HBRUSH hBrushOrg;
		tR.left = X;
		tR.top = Y;
		tR.right = X + nWidth;
		tR.bottom = Y + nHeight;
		hBrush = CreatePatternBrush(hBmp);
		hBrushOrg = (HBRUSH)SelectObject(hMemDC, hBrush);
		FillRect(hDC, &tR, hBrush);
		SelectObject(hMemDC, hBrushOrg);
		DeleteObject(hBrush);
		break;

	case ilZoom:
		INT lWidth;
		INT lHeight;
	 	double nFactor;
		
		lWidth = nImageWidth;
		lHeight = nImageHeight;
		nFactor = nImageHeight / nImageWidth;

		if ((lWidth / nWidth > 0) || (lHeight / nHeight > 0)) 
		{
			while ((lWidth / nWidth > 0) || (lHeight / nHeight > 0))
			{
				lWidth = lWidth - 1;
				lHeight = (INT)(nFactor * lWidth);
			}
			X = X + (nWidth - lWidth) / 2;
			Y = Y + (nHeight - lHeight) / 2;
		}
		else if((lWidth / nWidth <= 0) || (lHeight / nHeight) <= 0)
		{
			while ((lWidth / nWidth <= 0) || (lHeight / nHeight <= 0))
			{
				lWidth = lWidth + 1;
				lHeight = (INT)(nFactor * lWidth);
			}
		}

		SetStretchBltMode(hDC, STRETCH_HALFTONE);
		StretchBlt(hDC, X, Y, lWidth, lHeight, hMemDC, 0, 0, nImageWidth, nImageHeight, SRCCOPY);
		break;
	}

	//删除内存DC
	SelectObject(hMemDC, hPrevBmp);
	DeleteDC(hMemDC);
					   
}

//
// 函数：DrawPic
// 功能：画透明位图或图标
// 参数：hDC，设备环境
//       hPic，					位图或图标对象
//       X，					输出区域X轴坐标
//       Y，					输出区域Y轴坐标
//       nWidth，				输出区域宽度
//       nHeight，				输出区域高度
//       clrMaskColor，			要屏蔽的颜色
//       clrDisabledColor，		不可用颜色
//       blnEnabled，			是否可用
// 返回：无
//
VOID Win32GDI::DrawPic(
	HDC hDC, 
	HGDIOBJ hPic,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrMaskColor, 
	COLORREF clrDisabledColor,
	BOOL bEnabled)
{
    ICONINFO ii;
    BITMAP bmp;
	BITMAPINFO bmi;
    HDC hMemDC;
	HDC hTmpDC;
    HBITMAP hBmpCurr;
	HBITMAP	hBmpOrg;
    HBRUSH hBurshCurr;
    INT nBytesPerScanLine;
	INT nPicWidth = 0;
	INT nPicHeight = 0;
    UINT i;

    switch(GetObjectType(hPic))
	{
    case OBJ_BITMAP:
		{
			HBITMAP hImage = (HBITMAP)hPic;

			//获得位图尺寸
			GetObject(hImage, sizeof(bmp), &bmp);
			nPicWidth = bmp.bmWidth;
			nPicHeight = bmp.bmHeight;

			//创建内存DC
			hMemDC = CreateCompatibleDC(hDC);
			hBmpCurr = CreateCompatibleBitmap(hDC, nPicWidth, nPicHeight);
			hBmpOrg = (HBITMAP)SelectObject(hMemDC, hBmpCurr);
			
			//将位图拷贝到内存DC
			hTmpDC = CreateCompatibleDC(hDC);
			SelectObject(hTmpDC, hImage);
			BitBlt(hMemDC, 0, 0, nPicWidth, nPicHeight, hTmpDC, 0, 0, SRCCOPY);
			DeleteDC(hTmpDC);
		}
		break;
		
	default:
        {
			HICON hIcon = (HICON)hPic;

			//获得图标尺寸
			//MSDN: GetIconInfo creates bitmaps for the hbmMask and hbmColor members of ICONINFO.The calling application must manage these bitmaps and delete them when they are no longer necessary.
			//GetIconInfo函数在获取图标信息时为会ICONINFO的成员hbmMask和hbmColor创建位图对象，不再需要时必须用DeleteObject API删除，否则会引起位图资源泄漏。
			GetIconInfo(hIcon, &ii);
			GetObject(ii.hbmColor, sizeof(bmp), &bmp);
			DeleteObject(ii.hbmColor);
			DeleteObject(ii.hbmMask);
			nPicWidth = bmp.bmWidth;
			nPicHeight = bmp.bmHeight;
			
			//创建内存DC
			hMemDC = CreateCompatibleDC(hDC);
			hBmpCurr = CreateCompatibleBitmap(hDC, nPicWidth, nPicHeight);
			hBmpOrg = (HBITMAP)SelectObject(hMemDC, hBmpCurr);
        
			//用透明色填充内存DC, 并将图标拷贝到内存DC
			RECT rc;
			rc.left = 0;
			rc.top = 0;
			rc.right = nPicWidth;
			rc.bottom = nPicHeight;
			hBurshCurr = CreateSolidBrush(clrMaskColor);
			FillRect(hMemDC, &rc, hBurshCurr); 
			DrawIconEx(hMemDC, 0, 0, hIcon, nPicWidth, nPicHeight, 0, NULL, DI_NORMAL);
			DeleteObject(hBurshCurr);
		}
		break;
    }
	
	if (!bEnabled)
	{
		/* 替换像素速度太慢
		for(i= 0; i <= iWidth; i++)
		{
			for(j = 0; j <= iHeight; j++)
			{
                if (GetPixel(hMemDC, i, j) != clrMaskColor)
				{
                    SetPixel(hMemDC, i, j, BlendColor(clrMaskColor, 
						clrDisenableColor, nAlpha));
                }
            }
        }
		*/
		
		//修改内存DC中位图的颜色
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = nPicWidth;
		bmi.bmiHeader.biHeight = -nPicHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

        nBytesPerScanLine = ((((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) + 31) / 32) * 4);
        bmi.bmiHeader.biSizeImage = nBytesPerScanLine * abs(bmi.bmiHeader.biHeight);
		BYTE *pData = new BYTE[bmi.bmiHeader.biSizeImage];

		//读取图片数据
		//函数调用后，将会把图像的每个像素点的RGB值 以16进制值写入到pData中，
		//常见的一个像素是占4个字节，第一个字节是蓝色--B，第二个字节是绿色--G，第三个字节是红色--R，第四个不知道
		GetDIBits(hMemDC, hBmpCurr, 0, nPicHeight, pData, &bmi, DIB_RGB_COLORS);

		//将非透明色设置成灰色
		BYTE r, g, b;
		COLORREF clrTransparentColor = TranslateColor(clrMaskColor);
		COLORREF clrGrayColor = TranslateColor(clrDisabledColor);
		COLORREF clrColor;
		for(i=0; i<bmi.bmiHeader.biSizeImage-2; i+=4)
		{
			r = pData[i+2];
			g = pData[i+1];
			b = pData[i+0];
			clrColor = RGB(r, g, b);
			if(clrColor != clrTransparentColor)
			{
				pData[i+2] = GetRValue(clrGrayColor);
				pData[i+1] = GetGValue(clrGrayColor);
				pData[i+0] = GetBValue(clrGrayColor);
			}
		}
		
		//重新设置图片数据
		SetDIBits(hMemDC, hBmpCurr, 0, nPicHeight, pData, &bmi, DIB_RGB_COLORS);
		
		//销毁数据
		delete[] pData;
    }
	
	//将图像从内存DC拷贝到目标DC上
	TransparentBlt(hDC, X, Y, nWidth, nHeight, hMemDC, 0, 0, nPicWidth, nPicHeight, clrMaskColor);

	//删除内存DC
	SelectObject(hMemDC, hBmpOrg);
	DeleteObject(hBmpCurr);
    DeleteDC(hMemDC);
}

//
// 颜色混合
//
COLORREF Win32GDI::BlendColor(
	COLORREF clrForeColor,
    COLORREF clrBackColor,
	BYTE nAlpha)
{
	INT nRedSrc;
    INT nGreenSrc;
    INT nBlueSrc;
    INT nRedDst;
    INT nGreenDst;
    INT nBlueDst;
    
    nRedSrc = GetRValue(clrForeColor);
    nGreenSrc = GetGValue(clrForeColor);
    nBlueSrc = GetBValue(clrForeColor);
    nRedDst = GetRValue(clrBackColor);
    nGreenDst = GetGValue(clrBackColor);
    nBlueDst = GetBValue(clrBackColor);

    return RGB(
    ((nRedSrc * nAlpha) / 0xFF) + ((nRedDst * (0xFF - nAlpha)) / 0xFF), 
    ((nGreenSrc * nAlpha) / 0xFF) + ((nGreenDst * (0xFF - nAlpha)) / 0xFF), 
    ((nBlueSrc * nAlpha) / 0xFF) + ((nBlueDst * (0xFF - nAlpha)) / 0xFF));
}

//
// 颜色透明
//
COLORREF Win32GDI::AlphaColor(
	COLORREF clrColor, 
	BYTE nAlpha)
{
    INT R;
    INT G;
    INT B;
    INT nDelta;
    
    R = GetRValue(clrColor);
    G = GetGValue(clrColor);
    B = GetBValue(clrColor);
    
    nDelta = 0xFF - nAlpha;
    B = nAlpha + B * nDelta / 0xFF;
    G = nAlpha + G * nDelta / 0xFF;
    R = nAlpha + R * nDelta / 0xFF;

    if (R > 0xFF) R = 0xFF;
    if (G > 0xFF) G = 0xFF;
    if (B > 0xFF) B = 0xFF;

    return RGB(R,G,B);
}

//
// 颜色反转
//
COLORREF Win32GDI::ReverseColor(
	COLORREF clrColor)
{
	INT R;
	INT G;
	INT B;

	R = GetRValue(clrColor);
	G = GetGValue(clrColor);
    B = GetBValue(clrColor);
	R = 0xFF - R;
	G = 0xFF - G;
	B = 0xFF - B;

	return RGB(R,G,B);
}

//
// 转换颜色
//
COLORREF Win32GDI::TranslateColor(
	COLORREF clrColor, 
	HPALETTE hPal)
{
	COLORREF clrOut = -1;
	if (OleTranslateColor(clrColor, hPal, &clrOut) == S_OK)
	{
		return clrOut;
	}
	return -1;
}

COLOR16 Win32GDI::UINTToUSHORT(UINT val)
{
    if (val < 0x8000)
	{
       return (COLOR16)val;
	}
    else
	{
       return (COLOR16)(val - 0x10000);
	}
}
