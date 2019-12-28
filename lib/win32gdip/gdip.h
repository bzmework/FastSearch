#ifndef __WIN32GDIP_H__
#define __WIN32GDIP_H__

#include <windows.h>
#include <tchar.h>

//gdiplus 
#include <comdef.h>//初始化一下com口 
#ifndef ULONG_PTR 
#define ULONG_PTR unsigned long* 
#include <gdiplus.h>
using namespace Gdiplus;
#endif 

class Win32GDIP
{

public:
	
	//平滑模式
	enum SmoothingModeConstants
	{
		smInvalid			= -1,
		smDefault			= 0,
		smHighSpeed			= 1,
		smHighQuality		= 2,
		smNone				= 3,
		smAntiAlias			= 4
	};

	//
	// 画刷
	//
	
	// 画刷类型
	enum BrushTypeConstants
	{
		btSolidColor				= 0,
		btHatchFill					= 1,
		btTextureFill				= 2,
		btPathGradient				= 3,
		btLinearGradient			= 4,

		btGradientHorizontal		= 10,
		btGradientVertical			= 11,
		btGradientForwardDiagonal	= 12,
		btGradientBackwardDiagonal	= 13,
		btGradientAngle				= 14
	};
	
	// 画刷图案样式
	enum BrushHatchConstants
	{
		hsHorizontal		= 0,
		hsVertical			= 1,
		hsForwardDiagonal	= 2,
		hsBackwardDiagonal	= 3,
		hsCross				= 4,
		hsDiagonalCross		= 5,
		hs05Percent			= 6,
		hs10Percent			= 7,
		hs20Percent			= 8,
		hs25Percent			= 9,
		hs30Percent			= 10,
		hs40Percent			= 11,
		hs50Percent			= 12,
		hs60Percent			= 13,
		hs70Percent			= 14,
		hs75Percent			= 15,
		hs80Percent			= 16,
		hs90Percent			= 17,
		hsLightDownwardDiagonal	= 18,
		hsLightUpwardDiagonal	= 19,
		hsDarkDownwardDiagonal	= 20,
		hsDarkUpwardDiagonal	= 21,
		hsWideDownwardDiagonal	= 22,
		hsWideUpwardDiagonal	= 23,
		hsLightVertical			= 24,
		hsLightHorizontal		= 25,
		hsNarrowVertical		= 26,
		hsNarrowHorizontal		= 27,
		hsDarkVertical			= 28,
		hsDarkHorizontal		= 29,
		hsDashedDownwardDiagonal= 30,
		hsDashedUpwardDiagonal	= 31,
		hsDashedHorizontal		= 32,
		hsDashedVertical		= 33,
		hsSmallConfetti			= 34,
		hsLargeConfetti			= 35,
		hsZigZag				= 36,
		hsWave					= 37,
		hsDiagonalBrick			= 38,
		hsHorizontalBrick		= 39,
		hsWeave					= 40,
		hsPlaid					= 41,
		hsDivot					= 42,
		hsDottedGrid			= 43,
		hsDottedDiamond			= 44,
		hsShingle				= 45,
		hsTrellis				= 46,
		hsSphere				= 47,
		hsSmallGrid				= 48,
		hsSmallCheckerBoard		= 49,
		hsLargeCheckerBoard		= 50,
		hsOutlinedDiamond		= 51,
		hsSolidDiamond			= 52
	};

	//
	// 画笔
	//

	// 画笔样式
	enum PenStyleConstants
	{
		psSolid			= 0,
		psDash			= 1,
		psDot			= 2,
		psDashDot		= 3,
		psDashDotDot	= 4,
		psCustom		= 5
	};

	// 线冒类型
	enum LineCapConstants
	{
		lcFlat             = 0,
		lcSquare           = 1,
		lcRound            = 2,
		lcTriangle         = 3,
		lcNoAnchor         = 0x10, // corresponds to flat cap
		lcSquareAnchor     = 0x11, // corresponds to square cap
		lcRoundAnchor      = 0x12, // corresponds to round cap
		lcDiamondAnchor    = 0x13, // corresponds to triangle cap
		lcArrowAnchor      = 0x14, // no correspondence
		lcCustom           = 0xff, // custom cap
		lcAnchorMask       = 0xf0  // mask to check for anchor or not.
	};

	//
	// 渐变方向
	//
	enum GradientDirectionConstants
	{
		dHorizontal			= 0,
		dVertical			= 1,
		dForwardDiagonal	= 2,
		dBackwardDiagonal	= 3
	};
	
	//
	// 文本格式
	//

	//  DirectionRightToLeft          - For horizontal text, the reading order is
	//                                  right to left. This value is called
	//                                  the base embedding level by the Unicode
	//                                  bidirectional engine.
	//                                  For vertical text, columns are read from
	//                                  right to left.
	//                                  By default, horizontal or vertical text is
	//                                  read from left to right.
	//
	//  DirectionVertical             - Individual lines of text are vertical. In
	//                                  each line, characters progress from top to
	//                                  bottom.
	//                                  By default, lines of text are horizontal,
	//                                  each new line below the previous line.
	//
	//  NoFitBlackBox                 - Allows parts of glyphs to overhang the
	//                                  bounding rectangle.
	//                                  By default glyphs are first aligned
	//                                  inside the margines, then any glyphs which
	//                                  still overhang the bounding box are
	//                                  repositioned to avoid any overhang.
	//                                  For example when an italic
	//                                  lower case letter f in a font such as
	//                                  Garamond is aligned at the far left of a
	//                                  rectangle, the lower part of the f will
	//                                  reach slightly further left than the left
	//                                  edge of the rectangle. Setting this flag
	//                                  will ensure the character aligns visually
	//                                  with the lines above and below, but may
	//                                  cause some pixels outside the formatting
	//                                  rectangle to be clipped or painted.
	//
	//  NumberContextArabic           - Causes any initial numeric in the string to
	//                                  be analysed for bidirection layout as if
	//                                  it was preceeded by Arabic text.
	//
	//  DisableKashidaJustification   - Arabic text will not be justified by the
	//                                  insertion of kashidas (i.e. extending the
	//                                  joining line between characters). Instead
	//                                  Arabic script will be justified by the
	//                                  widening of the whitespace between words.
	//
	//  DisplayFormatControl          - Causes control characters such as the
	//                                  left-to-right mark to be shown in the
	//                                  output with a representative glyph.
	//
	//  DisableKerning                - Disables Truetype and OpenType kerning.
	//
	//  DisableLigatures              - Disables Truetype and OpenType ligatures.
	//
	//  LayoutLegacyBidi              - Causes the bidirection algorithm to use
	//                                  slightly different classifications for
	//                                  '+', '-' and '/' that make their layout
	//                                  much closer to that expected by files
	//                                  generated in Windows or by Windows
	//                                  applications.
	//
	//  NoChanges                     - A text imager created with this flag set
	//                                  does not support those APIs that change
	//                                  it's contents or formatting, but for most
	//                                  simple text will be significantly faster in
	//                                  performing measurement and drawing
	//                                  functions.
	//
	//  NoFontFallback                - Disables fallback to alternate fonts for
	//                                  characters not supported in the requested
	//                                  font. Any missing characters will be
	//                                  be displayed with the fonts missing glyph,
	//                                  usually an open square.
	//
	//  NoWrap                        - Disables wrapping of text between lines
	//                                  when formatting within a rectangle.
	//                                  NoWrap is implied when a point is passed
	//                                  instead of a rectangle, or when the
	//                                  specified rectangle has a zero line length.
	//
	//  NoClip                        - By default text is clipped to the
	//                                  formatting rectangle. Setting NoClip
	//                                  allows overhanging pixels to affect the
	//                                  device outside the formatting rectangle.
	//                                  Pixels at the end of the line may be
	//                                  affected if the glyphs overhang their
	//                                  cells, and either the NoFitBlackBox flag
	//                                  has been set, or the glyph extends to far
	//                                  to be fitted.
	//                                  Pixels above/before the first line or
	//                                  below/after the last line may be affected
	//                                  if the glyphs extend beyond their cell
	//                                  ascent / descent. This can occur rarely
	//                                  with unusual diacritic mark combinations.
	enum TextFormatConstants
	{
		tfDefault						= 0x00000000,
		tfDirectionRightToLeft			= 0x00000001,
		tfDirectionVertical				= 0x00000002,
		tfNoFitBlackBox					= 0x00000004,
		tfNumberContextArabic			= 0x00000008,
		tfDisableKashidaJustification	= 0x00000010,
		tfDisplayFormatControl			= 0x00000020,
		tfDisableKerning				= 0x00000040,
		tfDisableLigatures				= 0x00000080,
		tfLayoutLegacyBidi				= 0x00000100,
		tfFlagsNoChanges				= 0x00000200,
		tfNoFontFallback				= 0x00000400,
		tfMeasureTrailingSpaces			= 0x00000800,
		tfFlagsNoWrap					= 0x00001000,
		tfLineLimit						= 0x00002000,
		tfNoClip						= 0x00004000
	};

	//
	// 文本水平对齐方式
	//
	enum TextHAlignmentConstants
	{
		// Left edge for left-to-right text,
		// right for right-to-left text,
		// and top for vertical
		thaLeft		= 0,
		thaCenter	= 1,
		thaRight	= 2
	};

	//
	// 文本垂直对齐方式
	//
	enum TextVAlignmentConstants
	{
		// Left edge for left-to-right text,
		// right for right-to-left text,
		// and top for vertical
		tvaTop		= 0,
		tvaCenter	= 1,
		tvaBottom	= 2
	};

public:
	ULONG_PTR m_pGdiToken; //GDI+令牌
	SmoothingMode m_eSmoothingMode; //平滑模式

	Win32GDIP()
	{
		m_pGdiToken = NULL;
		m_eSmoothingMode = SmoothingModeAntiAlias;
	}

	~Win32GDIP()
	{
		this->ShutDown();
	}

public:

	//启动GDI+
	BOOL Startup();

	//关闭GDI+
	VOID ShutDown();

	//画圆或椭圆
	VOID DrawCircle(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrFillColor, 
		COLORREF clrBorderColor,
		COLORREF clrHatchColor = 0x000000,
		BrushTypeConstants eBrushType = btSolidColor,
		BrushHatchConstants eBrushHatch = hsHorizontal,
		PenStyleConstants ePenStyle = psSolid,
		INT nLineWidth = 1,
		FLOAT fAngle = 0.0f,
	    BYTE nAlpha = 255);

	//画线
	VOID DrawLine(
		HDC hDC,
		INT X1,
		INT Y1,
		INT X2,
		INT Y2,
		COLORREF clrLineColor,
		PenStyleConstants ePenStyle = psSolid,
		LineCapConstants eStartCap = lcFlat,
		LineCapConstants eEndCap = lcFlat,
		INT nLineWidth = 1,
		BYTE nAlpha = 255);

	//画弧线
	VOID DrawArc(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		FLOAT fStartAngle,
		FLOAT fSweepAngle,
		COLORREF clrLineColor,
		PenStyleConstants ePenStyle = psSolid,
		INT nLineWidth = 1,
		BYTE nAlpha = 255);

	//画贝塞尔曲线
	VOID DrawBezier(
		HDC hDC,
		INT X1,
		INT Y1,
		INT X2,
		INT Y2,
		INT X3,
		INT Y3,
		INT X4,
		INT Y4,
		COLORREF clrLineColor,
		PenStyleConstants ePenStyle = psSolid,
		INT nLineWidth = 1,
		BYTE nAlpha = 255);
	
	//画饼图
	VOID DrawPie(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		FLOAT fStartAngle,
		FLOAT fSweepAngle,
		COLORREF clrFillColor, 
		COLORREF clrBorderColor,
		COLORREF clrHatchColor = 0x000000,
		BrushTypeConstants eBrushType = btSolidColor,
		BrushHatchConstants eBrushHatch = hsHorizontal,
		PenStyleConstants ePenStyle = psSolid,
		INT nLineWidth = 1,
		FLOAT fAngle = 0.0f,
		BYTE nAlpha = 255);

	//画区域
	VOID DrawArea(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrFillColor, 
		COLORREF clrHatchColor = 0x000000,
		BrushTypeConstants eBrushType = btSolidColor,
		BrushHatchConstants eBrushHatch = hsHorizontal,
		BYTE nAlpha = 255);

	//画边框
	VOID DrawBorder(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrBorderColor,
		PenStyleConstants ePenStyle = psSolid,
		INT nLineWidth = 1,
		BYTE nAlpha = 255);

	//画区域及边框
	VOID DrawBox(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrFillColor, 
		COLORREF clrBorderColor,
		COLORREF clrHatchColor = 0x000000,
		BrushTypeConstants eBrushType = btSolidColor,
		BrushHatchConstants eBrushHatch = hsHorizontal,
		PenStyleConstants ePenStyle = psSolid,
		INT nLineWidth = 1,
		FLOAT fAngle = 0.0f,
		BYTE nAlpha = 255);

	//画渐变
	VOID DrawGradient(
		HDC hDC, 
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrStartColor, 
		COLORREF clrEndColor, 
		GradientDirectionConstants eDirection = dHorizontal,
		FLOAT fAngle = 0.0f, 
		BYTE nAlpha = 255);

	//画文本
	VOID DrawText(
		HDC hDC, 
		LPCWSTR strText, 
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrTextColor, 
		TextFormatConstants eFormat = tfDefault,
		TextHAlignmentConstants eHAlignment = thaLeft, 
		TextVAlignmentConstants eVAlignment = tvaCenter,
		LPCWSTR strFontName = L"Arial", 
		UINT nFontSize = 8, 
		INT  nFontRotationDegrees = 0, 
		BOOL bFontBold = FALSE,
		BOOL bFontItalic = FALSE,
		BOOL bFontUnderline = FALSE,
		BOOL bFontStrikethru = FALSE,
		BOOL bFontHighQuality = FALSE,
		BYTE nAlpha = 255);


	//COLORREF To ARGB
	static ARGB SetFromCOLORREF(COLORREF rgb, BYTE nAlpha)
    {
		BYTE r = GetRValue(rgb);
		BYTE g = GetGValue(rgb);
		BYTE b = GetBValue(rgb);

        return (((ARGB) (b) << 0) |	((ARGB) (g) << 8) |((ARGB) (r) << 16) | ((ARGB) (nAlpha) << 24));
    }
};

#endif //__WIN32GDIP_H__