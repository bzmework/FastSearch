#ifndef __WIN32GDI_H__
#define __WIN32GDI_H__

#include <windows.h>
#include <tchar.h>
#include <comdef.h>

#define TITLE_COLOR				GetSysColor(COLOR_CAPTIONTEXT)
#define WINDOW_COLOR			GetSysColor(COLOR_WINDOW)
#define WINDOWFRAME_COLOR       GetSysColor(COLOR_WINDOWFRAME)
#define WINDOWTEXT_COLOR		GetSysColor(COLOR_WINDOWTEXT)
#define BUTTONTEXT_COLOR		GetSysColor(COLOR_BTNTEXT)
#define BUTTONFACE_COLOR		GetSysColor(COLOR_BTNFACE)
#define BUTTONSHADOW_COLOR		GetSysColor(COLOR_BTNSHADOW)

class Win32GDI
{
protected:

	static COLOR16 UINTToUSHORT(UINT val);

public:

	//
	// 画刷
	//

	// 画刷类型
	// btDefaultBrush: 缺省画刷
	// btSystemBrush: 系统画刷
	// btSolidBrush: 实心画刷
	// btIndirectBrush:指定样式、颜色、图案的画刷
	// btHatchBrush: 指定颜色的图案画刷
	// btPatternBrush: 位图画刷
	enum BrushTypeConstants
	{
		btDefaultBrush	= 0x0,
		btSolidBrush	= 0x1,
		btIndirectBrush = 0x2,
		btHatchBrush	= 0x3,
		btPatternBrush	= 0x4
	};

	// 画刷样式
	// BS_SOLID: Solid brush.
	// BS_NULL: Same as BS_HOLLOW.
	// BS_HATCHED: Hatched brush.
	// BS_PATTERN: Pattern brush defined by a memory bitmap.
	// BS_DIBPATTERN: A pattern brush defined by a device-independent bitmap (DIB) specification. If lbStyle is BS_DIBPATTERN, the lbHatch member contains a handle to a packed DIB. For more information, see discussion in lbHatch.Windows 95: Creating brushes from bitmaps or DIBs larger than 8 by 8 pixels is not supported. If a larger bitmap is specified, only a portion of the bitmap is used.
	// BS_DIBPATTERNPT: A pattern brush defined by a device-independent bitmap (DIB) specification. If lbStyle is BS_DIBPATTERNPT, the lbHatch member contains a pointer to a packed DIB. For more information, see discussion in lbHatch.
	// BS_PATTERN8X8: Same as BS_PATTERN.
	// BS_DIBPATTERN8X8: Same as BS_DIBPATTERN.
	// BS_HOLLOW = BS_NULL: Hollow brush.
	enum BrushStyleConstants
	{
		bsSolid			= BS_SOLID,
		bsNull			= BS_NULL,
		bsHatched		= BS_HATCHED,
		bsPattern		= BS_PATTERN,
		bsIndexed		= BS_INDEXED,
		bsDibPattern	= BS_DIBPATTERN,
		bsDibPatternPt	= BS_DIBPATTERNPT,
		bsPattern8x8	= BS_PATTERN8X8,
		bsDibPattern8x8 = BS_DIBPATTERN8X8,
		bsMonoPattern	= BS_MONOPATTERN,
		bsHollow    	= BS_NULL
	};

	// 画刷图案样式
	// HS_HORIZONTAL: Horizontal hatch
	// HS_VERTICAL: Vertical hatch
	// HS_FDIAGONAL: A 45-degree downward, left-to-right hatch
	// HS_BDIAGONAL: A 45-degree upward, left-to-right hatch
	// HS_CROSS: Horizontal and vertical cross-hatch
	// HS_DIAGCROSS: 45-degree crosshatch
	enum BrushHatchConstants
	{
		hsHorizontal	= HS_HORIZONTAL,
		hsVertical		= HS_VERTICAL,
		hsFDiagonal 	= HS_FDIAGONAL,
		hsBDiagonal		= HS_BDIAGONAL,
		hsCross			= HS_CROSS,
		hsDiagCross		= HS_DIAGCROSS,
		hsSolid			= 0x8
	};

	//
	// 画笔
	//

	// 画笔类型
	// ptDefaultPen: 缺省画笔
	// ptSolidPen: 实心画笔
	// ptIndirectPen: 指定样式、宽度、颜色的画笔
	enum PenTypeConstants
	{
		ptDefaultPen	= 0x0,
		ptSolidPen		= 0x1,
		ptIndirectPen	= 0x2
	};

	// 画笔样式
	// PS_SOLID: The pen is solid.
	// PS_DASH: The pen is dashed.  -------
	// PS_DOT: The pen is dotted.  .......
	// PS_DASHDOT: he pen has alternating dashes and dots. _._._._
	// PS_DASHDOTDOT: The pen has dashes and double dots. _.._.._
	// PS_NULL: The pen is invisible.
	enum PenStyleConstants
	{
		psSolid			= PS_SOLID,
		psDash			= PS_DASH,
		psDot			= PS_DOT,
		psDashDot		= PS_DASHDOT,
		psDashDotDot	= PS_DASHDOTDOT,
		psNull			= PS_NULL,
		psInsideFrame	= PS_INSIDEFRAME
	};

	//
	// 字体
	//

	// 字体类型
	// ftDefaultFont: 缺省字体
	// ftSystemFont: 系统字体
	// ftIndirectFont: 指定字体
	enum FontTypeConstants
	{
		ftDefaultFont	= 0x0,
		ftSystemFont	= 0x1,
		ftIndirectFont	= 0x2,
		ftExistFont		= 0x3
	};

	//
	// 渐变方向
	//
	enum GradientDirectionConstants
	{
		dHorizontal		= 0x0,
		dVertical		= 0x1
	};

	// DT_BOTTOM: Justifies the text to the bottom of the rectangle. This value is used only with the DT_SINGLELINE value.
	// DT_CALCRECT: Determines the width and height of the rectangle. If there are multiple lines of text, DrawText uses the width of the rectangle pointed to by the lpRect parameter and extends the base of the rectangle to bound the last line of text. If the largest word is wider than the rectangle, the width is expanded. If the text is less than the width of the rectangle, the width is reduced. If there is only one line of text, DrawText modifies the right side of the rectangle so that it bounds the last character in the line. In either case, DrawText returns the height of the formatted text but does not draw the text.
	// DT_CENTER: Centers text horizontally in the rectangle.
	// DT_EDITCONTROL: Duplicates the text-displaying characteristics of a multiline edit control. Specifically, the average character width is calculated in the same manner as for an edit control, and the function does not display a partially visible last line.
	// DT_END_ELLIPSIS: For displayed text, if the end of a string does not fit in the rectangle, it is truncated and ellipses are added. If a word that is not at the end of the string goes beyond the limits of the rectangle, it is truncated without ellipses.The string is not modified unless the DT_MODIFYSTRING flag is specified.Compare with DT_PATH_ELLIPSIS and DT_WORD_ELLIPSIS.
	// DT_EXPANDTABS: Expands tab characters. The default number of characters per tab is eight. The DT_WORD_ELLIPSIS, DT_PATH_ELLIPSIS, and DT_END_ELLIPSIS values cannot be used with the DT_EXPANDTABS value.
	// DT_EXTERNALLEADING: Includes the font external leading in line height. Normally, external leading is not included in the height of a line of text.
	// DT_HIDEPREFIX: Windows 2000/XP: Ignores the ampersand (&) prefix character in the text. The letter that follows will not be underlined, but other mnemonic-prefix characters are still processed. For example:
	//   input string:    "A&bc&&d"
	//   normal:          "Abc&d"
	//   DT_HIDEPREFIX:   "Abc&d"
	//   Compare with DT_NOPREFIX and DT_PREFIXONLY.
	// DT_INTERNAL: Uses the system font to calculate text metrics.
	// DT_LEFT: Aligns text to the left.
	// DT_MODIFYSTRING: Modifies the specified string to match the displayed text. This value has no effect unless DT_END_ELLIPSIS or DT_PATH_ELLIPSIS is specified.
	// DT_NOCLIP: Draws without clipping. DrawText is somewhat faster when DT_NOCLIP is used.
	// DT_NOFULLWIDTHCHARBREAK: Windows 98/Me, Windows 2000/XP: Prevents a line break at a DBCS (double-wide character string), so that the line breaking rule is equivalent to SBCS strings. For example, this can be used in Korean windows, for more readability of icon labels. This value has no effect unless DT_WORDBREAK is specified.
	// DT_NOPREFIX: Turns off processing of prefix characters. Normally, DrawText interprets the mnemonic-prefix character & as a directive to underscore the character that follows, and the mnemonic-prefix characters && as a directive to print a single &. By specifying DT_NOPREFIX, this processing is turned off. For example,
	//   input string:   "A&bc&&d"
	//   normal:         "Abc&d"
	//   DT_NOPREFIX:    "A&bc&&d"
	//   Compare with DT_HIDEPREFIX and DT_PREFIXONLY.
	// DT_PATH_ELLIPSIS: For displayed text, replaces characters in the middle of the string with ellipses so that the result fits in the specified rectangle. If the string contains backslash (\) characters, DT_PATH_ELLIPSIS preserves as much as possible of the text after the last backslash.The string is not modified unless the DT_MODIFYSTRING flag is specified.Compare with DT_END_ELLIPSIS and DT_WORD_ELLIPSIS.
	// DT_PREFIXONLY Windows 2000/XP: Draws only an underline at the position of the character following the ampersand (&) prefix character. Does not draw any other characters in the string. For example,
	//   input string:    "A&bc&&d"
	//   normal:          "Abc&d"
	//   DT_PREFIXONLY:   " _   "
	//   Compare with DT_HIDEPREFIX and DT_NOPREFIX.
	// DT_RIGHT: Aligns text to the right.
	// DT_RTLREADING: Layout in right-to-left reading order for bi-directional text when the font selected into the hdc is a Hebrew or Arabic font. The default reading order for all text is left-to-right.
	// DT_SINGLELINE: Displays text on a single line only. Carriage returns and line feeds do not break the line.
	// DT_TABSTOP: Sets tab stops. Bits 15–8 (high-order byte of the low-order word) of the uFormat parameter specify the number of characters for each tab. The default number of characters per tab is eight. The DT_CALCRECT, DT_EXTERNALLEADING, DT_INTERNAL, DT_NOCLIP, and DT_NOPREFIX values cannot be used with the DT_TABSTOP value.
	// DT_TOP: Justifies the text to the top of the rectangle.
	// DT_VCENTER: Centers text vertically. This value is used only with the DT_SINGLELINE value.
	// DT_WORDBREAK: Breaks words. Lines are automatically broken between words if a word would extend past the edge of the rectangle specified by the lpRect parameter. A carriage return-line feed sequence also breaks the line.If this is not specified, output is on one line.
	// DT_WORD_ELLIPSIS: Truncates any word that does not fit in the rectangle and adds ellipses.Compare with DT_END_ELLIPSIS and DT_PATH_ELLIPSIS.
	enum TextFormatConstants
	{
		dtTop					= 0x00000000,
		dtLeft					= 0x00000000,
		dtCenter				= 0x00000001,
		dtRight					= 0x00000002,
		dtVCenter				= 0x00000004,
		dtBottom				= 0x00000008,
		dtWordBreak				= 0x00000010,
		dtSingleLine			= 0x00000020,
		dtExpandTabs			= 0x00000040,
		dtTabStop				= 0x00000080,
		dtNoClip				= 0x00000100,
		dtExternAlleading		= 0x00000200,
		dtCalcRect				= 0x00000400,
		dtNoPrefix				= 0x00000800,
		dtInternal				= 0x00001000,
		dtEditControl			= 0x00002000,
		dtPathEllipsis  		= 0x00004000,
		dtEndEllipsis			= 0x00008000,
		dtModifyString			= 0x00010000,
		dtRtLReading			= 0x00020000,
		dtWordEllipsis			= 0x00040000,
		dtNoFullWidthCharBreak	= 0x00080000,
		dtHidePrefix			= 0x00100000,
		dtPrefixOnly			= 0x00200000,
		dtPLotter				= 0x00000000,
		dtRasDisplay			= 0x00000001,
		dtRasPrinter			= 0x00000002,
		dtRasCamera				= 0x00000003,
		dtCharStream			= 0x00000004,
		dtMetaFile				= 0x00000005,
		dtDispFile				= 0x00000006,
		dtDefault				= 0x00000024,//DT_LEFT or DT_VCENTER or DT_SINGLELINE
		dtDefault1				= 0x00040024,//DT_LEFT or DT_VCENTER or DT_SINGLELINE or DT_WORD_ELLIPSIS
		dtDefault2				= 0x00040025 //DT_CENTER or DT_VCENTER or DT_SINGLELINE or DT_WORD_ELLIPSIS
	};

	// DFC_CAPTION: Title bar
	// DFC_MENU: Menu bar
	// DFC_SCROLL: Scroll bar
	// DFC_BUTTON: Standard button
	// DFC_POPUPMENU: Windows 98/Me, Windows 2000/XP: Popup menu item.
	enum FrameControlTypeConstants
	{
		fctCaption		= 0x1,
		fctMenu			= 0x2,
		ftcScroll		= 0x3,
		fctButton		= 0x4,
		fctPopupMenu	= 0x5
	};

	// If uType is DFC_CAPTION, uState can be one of the following values:
	// DFCS_CAPTIONCLOSE: Close button
	// DFCS_CAPTIONHELP: Help button
	// DFCS_CAPTIONMAX: Maximize button
	// DFCS_CAPTIONMIN: Minimize button
	// DFCS_CAPTIONRESTORE: Restore button
	// If uType is DFC_MENU, uState can be one of the following values:
	// DFCS_MENUARROW: Submenu arrow
	// DFCS_MENUARROWRIGHT: Submenu arrow pointing left. This is used for the right-to-left cascading menus used with right-to-left languages such as Arabic or Hebrew.
	// DFCS_MENUBULLET: Bullet
	// DFCS_MENUCHECK: Check mark
	// If uType is DFC_SCROLL, uState can be one of the following values:
	// DFCS_SCROLLCOMBOBOX: Combo box scroll bar
	// DFCS_SCROLLDOWN: Down arrow of scroll bar
	// DFCS_SCROLLLEFT: Left arrow of scroll bar
	// DFCS_SCROLLRIGHT: Right arrow of scroll bar
	// DFCS_SCROLLSIZEGRIP: Size grip in bottom-right corner of window
	// DFCS_SCROLLSIZEGRIPRIGHT: Size grip in bottom-left corner of window. This is used with right-to-left languages such as Arabic or Hebrew.
	// DFCS_SCROLLUP: Up arrow of scroll bar
	// If uType is DFC_BUTTON, uState can be one of the following values:
	// DFCS_BUTTON3STATE: Three-state button
	// DFCS_BUTTONCHECK: Check box
	// DFCS_BUTTONPUSH: Push button
	// DFCS_BUTTONRADIO: Radio button
	// DFCS_BUTTONRADIOIMAGE: Image for radio button (nonsquare needs image)
	// DFCS_BUTTONRADIOMASK: Mask for radio button (nonsquare needs mask)
	// The following style can be used to adjust the bounding rectangle of the push button:
	// DFCS_ADJUSTRECT: Bounding rectangle is adjusted to exclude the surrounding edge of the push button.
	// One or more of the following values can be used to set the state of the control to be drawn:
	// DFCS_CHECKED: Button is checked.
	// DFCS_FLAT: Button has a flat border.
	// DFCS_HOT: Windows 98/Me, Windows 2000/XP: Button is hot-tracked.
	// DFCS_INACTIVE: Button is inactive (grayed).
	// DFCS_MONO: Button has a monochrome border.
	// DFCS_PUSHED: Button is pushed.
	// DFCS_TRANSPARENT: Windows 98/Me, Windows 2000/XP: The background remains untouched.
	enum FrameControlStateConstants
	{
		fcsCaptionClose			= 0x0,
		fcsCaptionMin			= 0x1,
		fcsCaptionMax			= 0x2,
		fcsCaptionRestore		= 0x3,
		fcsCaptionHelp			= 0x4,
		fcsMenuArrow			= 0x0,
		fcsMenuCheck			= 0x1,
		fcsMenuBullet			= 0x2,
		fcsMenuArrowRight		= 0x4,
		fcsScrollUp				= 0x0,
		fcsScrollDown			= 0x1,
		fcsScrollLeft			= 0x2,
		fcsScrollRight			= 0x3,
		fcsScrollComboBox		= 0x5,
		fcsScrollSizeGrip		= 0x8,
		fcsScrollSizeGripRight	= 0x10,
		fcsButtonCheck			= 0x0,
		fcsButtonRadioImage		= 0x1,
		fcsButtonRadioMask		= 0x2,
		fcsButtonRadion			= 0x4,
		fcsButton3State			= 0x8,
		fcsButtonPush			= 0x10,
		fcsAdjustRect			= 0x2000,
		fcsInActive				= 0x100,
		fctPushed				= 0x200,
		fcsChecked				= 0x400,
		fcsTransparent			= 0x800,
		fcsHot					= 0x1000,
		fcsFlat					= 0x4000,
		fcsMono					= 0x8000
	};
   
	// BDR_RAISEDINNER: Raised inner edge.
	// BDR_SUNKENINNER: Sunken inner edge.
	// BDR_RAISEDOUTER: Raised outer edge.
	// BDR_SUNKENOUTER: Sunken outer edge.
	// EDGE_BUMP: Combination of BDR_RAISEDOUTER and BDR_SUNKENINNER.
	// EDGE_ETCHED: Combination of BDR_SUNKENOUTER and BDR_RAISEDINNER.
	// EDGE_RAISED: Combination of BDR_RAISEDOUTER and BDR_RAISEDINNER.
	// EDGE_SUNKEN: Combination of BDR_SUNKENOUTER and BDR_SUNKENINNER.
	enum EdgeStyleConstants
	{
		esRaiseDouter	= 0x0001,
		esSunkenOuter	= 0x0002,
		esOuter			= 0x0003,
		esRaisedInner	= 0x0004,
		esRaised		= 0x0005,
		esSunkenInner	= 0x0008,
		esSunken		= 0x000a,
		esInner			= 0x000c
	};
			
	// BF_ADJUST: If this flag is passed, shrink the rectangle pointed to by the qrc parameter to exclude the edges that were drawn.If this flag is not passed, then do not change the rectangle pointed to by the qrc parameter. .
	// BF_BOTTOM: Bottom of border rectangle.
	// BF_BOTTOMLEFT: Bottom and left side of border rectangle.
	// BF_BOTTOMRIGHT: Bottom and right side of border rectangle.
	// BF_DIAGONAL: Diagonal border.
	// BF_DIAGONAL_ENDBOTTOMLEFT: Diagonal border. The end point is the bottom-left corner of the rectangle; the origin is top-right corner.
	// BF_DIAGONAL_ENDBOTTOMRIGHT: Diagonal border. The end point is the bottom-right corner of the rectangle; the origin is top-left corner.
	// BF_DIAGONAL_ENDTOPLEFT: Diagonal border. The end point is the top-left corner of the rectangle; the origin is bottom-right corner.
	// BF_DIAGONAL_ENDTOPRIGHT: Diagonal border. The end point is the top-right corner of the rectangle; the origin is bottom-left corner.
	// BF_FLAT: Flat border.
	// BF_LEFT: Left side of border rectangle.
	// BF_MIDDLE: Interior of rectangle to be filled.
	// BF_MONO: One-dimensional border.
	// BF_RECT: Entire border rectangle.
	// BF_RIGHT: Right side of border rectangle.
	// BF_SOFT: Soft buttons instead of tiles.
	// BF_TOP: Top of border rectangle.
	// BF_TOPLEFT: Top and left side of border rectangle.
	// BF_TOPRIGHT: Top and right side of border rectangle.
	enum EdgeFlagConstants
	{
		efLeft						= 0x0001,
		efTop						= 0x0002, 
		efRight						= 0x0004, 
		efBottom					= 0x0008, 
		efDiagonal					= 0x0010, 
		efMiddle					= 0x0800, 
		efSoft						= 0x1000, 
		efAdjust					= 0x2000, 
		efFlat						= 0x4000, 
		efMono						= 0x8000, 
		efTopLeft					= 0x0003,
		efTopRight					= 0x0006,
	 	efBottomLeft				= 0x0009,
	 	efBottomRight				= 0x0012,
		efRect						= 0x0015,
		efDiagonalEndBottomLeft		= 0x0019,
		efDiagonalEndBottomRight	= 0x001c,
		efDiagonalEndTopLeft		= 0x0013,
		efDiagonalEndTopRight		= 0x0016
	};

	// 箭头方向  
	enum ArrowDirectionConstants
	{
		adWest		= 0,	//←	西方箭头 Left 
		adWestNorth = 1,	//↖	西北箭头
		adNorth		= 2,	//↑	北方箭头 Top
		adEastNorth = 3,    //↗	东北箭头
		adEast		= 4,	//→	东方箭头 Right
		adEastSouth = 5,	//↘	东南箭头
		adSouth		= 6,	//↓	南方箭头 Bottom
		adWestSouth = 7     //↙	西南方向箭头
	};
	
	// 图像布局
	enum ImageLayoutConstants
	{
		ilNone		= 0, //无布局		
		ilTile		= 1, //平铺
		ilCenter	= 2, //居中
		ilStretch	= 3, //拉伸
		ilZoom		= 4  //缩放
	};

public:

	//是否NT操作系统
	static BOOL IsWinNT();

	//将对象选入设备环境
	static HGDIOBJ SelectAObject(HDC hDC, HGDIOBJ hObject);

	//删除对象
	static BOOL DeleteAObject(HGDIOBJ hObject);

	//创建画刷
	static HBRUSH CreateABrush(
		BrushTypeConstants eBrushType, 
		BrushStyleConstants eBrushStyle, 
		BrushHatchConstants eBrushHatch, 
		COLORREF clrColor, 
		HBITMAP hBitmap = NULL);

	//创建画笔
	static HPEN CreateAPen(
		PenTypeConstants ePenType, 
		PenStyleConstants ePenStyle, 
		COLORREF clrColor, 
		UINT nLineWidth = 1);

	//创建字体
	static HFONT CreateAFont(
		HDC hDC, 
		FontTypeConstants eFontType, 
		LPCTSTR strFontName,
		UINT nFontSize = 0,
		INT  nFontRotationDegrees = 0, 
		BOOL bFontBold = FALSE, 
		BOOL bFontItalic = FALSE,
		BOOL bFontUnderline = FALSE,
		BOOL bFontStrikethru = FALSE,
		BOOL bFontHighQuality = FALSE);
 
	//计算文本区域。
	static VOID CalcTextArea(
		HDC hDC, 
		LPCTSTR strText, 
		INT* nWidth, 
		INT* nHeight,
		HFONT hFont = NULL);

	//获得文本宽度
	static LONG TextWidth(
		HDC hDC, 
		LPCTSTR strText);

	//获得文本高度
	static LONG TextHeight(
		HDC hDC,
		LPCTSTR strText);

	//画文本
	static VOID DrawAText(
		HDC hDC, 
		LPCTSTR strText,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrTextColor = BUTTONTEXT_COLOR,
		TextFormatConstants eFormat = dtDefault, 
		BOOL bEnabled = TRUE,
		BOOL bUseCustomFont = TRUE,
		FontTypeConstants eFontType = ftIndirectFont, 
		LPCTSTR strFontName = _T("Tahoma"), 
		UINT nFontSize = 9, 
		INT  nFontRotationDegrees = 0, 
		BOOL bFontBold = FALSE,
		BOOL bFontItalic = FALSE,
		BOOL bFontUnderline = FALSE,
		BOOL bFontStrikethru = FALSE,
		BOOL bFontHighQuality = FALSE);
	static VOID DrawAText(
		HDC hDC,
		LPCTSTR strText,
		RECT tR,
		COLORREF clrTextColor = BUTTONTEXT_COLOR,
		TextFormatConstants eFormat = dtDefault,
		BOOL bEnabled = TRUE,
		BOOL bUseCustomFont = TRUE,
		FontTypeConstants eFontType = ftIndirectFont,
		LPCTSTR strFontName = _T("Tahoma"),
		UINT nFontSize = 9,
		INT  nFontRotationDegrees = 0,
		BOOL bFontBold = FALSE,
		BOOL bFontItalic = FALSE,
		BOOL bFontUnderline = FALSE,
		BOOL bFontStrikethru = FALSE,
		BOOL bFontHighQuality = FALSE);

	//画控件形状。例如，复选框、单选框、最大化、最小化、关闭按扭、菜单箭头等
	static VOID DrawControl(
		HDC hDC, 
		INT X,
		INT Y,
		INT nWidth, 
		INT nHeight,
		FrameControlTypeConstants eType, 
		FrameControlStateConstants eState);

	//画控件边缘特效。例如：凹下、突起等
	static VOID DrawControlEdge(
		HDC hDC, 
		INT X, 
		INT Y,
		INT nWidth, 
		INT nHeight,
		EdgeStyleConstants eStyle, 
		EdgeFlagConstants eFlags);

	//画线
	static VOID DrawLine(
		HDC hDC,
		INT X1,
		INT Y1,
		INT X2,
		INT Y2,
		COLORREF clrLineColor, 
		PenTypeConstants ePenType = ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		INT nLineWidth = 1);
	
	//画圆
	static VOID DrawCircle(
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
		INT nLineWidth = 1
		);

	//画边框
	static VOID DrawBorder(
		HDC hDC, 
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF crBorderColor, 
		PenTypeConstants ePenType = ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		int nBorderWidth = 1);

	//画区域
	static VOID DrawArea(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrAreaColor, 
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid, 
		HBITMAP hAreaBitmap = NULL);
	static VOID DrawArea(
		HDC hDC,
		RECT tR,
		COLORREF clrAreaColor,
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid,
		HBITMAP hAreaBitmap = NULL);

	//拷贝区域
	static VOID CopyArea(
		HDC hDestDC, 
		INT X, 
		INT Y, 
		INT nWidth, 
		INT nHeight,
		HDC hSrcDC, 
		INT SrcX, 
		INT SrcY, 
		INT nSrcWidth, 
		INT nSrcHeight);

	//画区域及边框
	static VOID DrawBox(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight, 
		COLORREF clrFillColor,
		COLORREF clrBorderColor, 
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid,
		PenTypeConstants ePenType =  ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		HBITMAP hFillBitmap = NULL,
		UINT nBorderWidth = 1);

	//画透明区域及边框
	static VOID DrawAlphaBox(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight, 
		COLORREF clrFillColor,
		COLORREF clrBorderColor, 
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid,
		PenTypeConstants ePenType = ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		HBITMAP hFillBitmap = NULL,
		UINT nBorderWidth = 1, 
		BYTE nAlpha = 100);

	//画圆角边框
	static VOID DrawRoundBorder(
		HDC hDC, 
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		UINT nRoundValue = 2, 
		COLORREF crBorderColor = 0x000000, 
		PenTypeConstants ePenType = ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		UINT nBorderWidth = 1);

	//画圆角形区域
	static VOID DrawRoundArea(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		UINT nRoundValue = 2, 
		COLORREF clrAreaColor = 0xFFFFFF, 
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid, 
		HBITMAP hAreaBitmap = NULL);

	//画圆角区域及边框
	static VOID DrawRoundBox(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight, 
		UINT nRoundValue = 2, 
		COLORREF clrFillColor = 0xFFFFFF,
		COLORREF clrBorderColor = 0x000000, 
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid,
		PenTypeConstants ePenType = ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		HBITMAP hFillBitmap = NULL,
		UINT nBorderWidth = 1);

	//画透明圆角区域及边框
	static VOID DrawAlphaRoundBox(
		HDC hDC,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight, 
		UINT nRoundValue = 2, 
		COLORREF clrFillColor = 0xFFFFFF,
		COLORREF clrBorderColor = 0x000000, 
		BrushTypeConstants eBrushType = btSolidBrush,
		BrushStyleConstants eBrushStyle = bsSolid,
		BrushHatchConstants eBrushHatch = hsSolid,
		PenTypeConstants ePenType = ptSolidPen,
		PenStyleConstants ePenStyle = psSolid, 
		HBITMAP hFillBitmap = NULL,
		UINT nBorderWidth = 1, 
		BYTE nAlpha = 100);

	//画多边形区域
	static VOID DrawPolyArea(
		HDC hDC, 
		POINT tPt[], 
		COLORREF clrFillColor = 0xFFFFFF,
		COLORREF clrBorderColor = 0x000000, 
		UINT nBorderWidth = 1);

	//画焦点
	static VOID DrawFocus(
		HDC hDC, 
		INT X, 
		INT Y, 
		INT nWidth, 
		INT nHeight);

	//画复选钩
	static VOID DrawCheck(
		HDC hDC,
		INT X , 
		INT Y, 
		INT nWidth, 
		INT nHeight, 
		COLORREF clrColor = 0x000000,
		UINT nLineWidth = 1);

	//画箭头
	static VOID DrawArrow(
		HDC hDC, 
		INT X, 
		INT Y, 
		INT nWidth, 
		INT nHeight,
		ArrowDirectionConstants eDirection = adEast,
		COLORREF clrColor = 0x000000);

	//画关闭
	static VOID DrawClose(
		HDC hDC, 
		INT X, 
		INT Y,
		UINT nSize = 8,
		COLORREF clrColor = 0x000000,
		UINT nLineWidth = 2);

	//画还原按钮
	static VOID DrawRestore(
		HDC hDC, 
		INT X, 
		INT Y,
		UINT nSize = 10,
		COLORREF clrColor = 0x000000,
		UINT nLineWidth = 1);

	//画最大化按钮
	static VOID DrawMax(
		HDC hDC, 
		INT X, 
		INT Y,
		UINT nSize = 8,
		COLORREF clrColor = 0x000000,
		UINT nLineWidth = 1);

	//画最小化按钮
	static VOID DrawMin(
		HDC hDC, 
		INT X, 
		INT Y,
		UINT nSize = 8,
		COLORREF clrColor = 0x000000,
		UINT nLineWidth = 1);

	//画数字
	static VOID DrawDigit(
		HDC hDC,
		INT X,
		INT Y,
		UINT nDigit,
		UINT nWidth = 3,
		COLORREF clrColor = 0x0000FF);

	//画字符
	static VOID DrawCharacter(
		HDC hDC,
		INT X,
		INT Y,
		LPCTSTR strLetter,
		COLORREF clrColor = 0x000000);

	//画渐变
	static VOID DrawGradient(
		HDC hDC, 
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrStartColor, 
		COLORREF clrEndColor, 
		GradientDirectionConstants eDirection);

	//画透明渐变
	static VOID DrawAlphaGradient(
		HDC hDC, 
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrStartColor, 
		COLORREF clrEndColor, 
		GradientDirectionConstants eDirection,
		BYTE nAlpha = 100);
 
	//位图透明拷贝(需要遮罩图片)
	static VOID TransparentPaintImage(
		HDC hDestDC,
		INT xDest, 
		INT yDest, 
		HBITMAP hImage, 
		HBITMAP hMask);

	//位图透明拷贝(利用TransparentBlt API，速度更快)
	static VOID TransparentPaintArea( 
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
		COLORREF clrTransparentColor);

	//位图拷贝(带Alpha通道值)
	static VOID TransparentPaintAlpha(
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
		BYTE nAlpha = 100);

	//画图标
	static VOID DrawIcon(
		HDC hDC,
		HGDIOBJ hIcon, 
		INT X, 
		INT Y, 
		BOOL bEnabled,
		COLORREF clrDisabledColor);

	//画图片
	static VOID DrawImage(
		HDC hDC,
		HGDIOBJ hImage,
		INT X, 
		INT Y, 
		INT nWidth, 
		INT nHeight,
		ImageLayoutConstants eLayout);

	//画透明位图或图标
	static VOID DrawPic(
		HDC hDC, 
		HGDIOBJ hPic,
		INT X,
		INT Y,
		INT nWidth,
		INT nHeight,
		COLORREF clrMaskColor, 
		COLORREF clrDisabledColor,
		BOOL bEnabled);

	//颜色混合
	static COLORREF BlendColor(
		COLORREF clrForeColor,
		COLORREF clrBackColor,
		BYTE nAlpha);


	//颜色透明
	static COLORREF AlphaColor(
		COLORREF clrColor, 
		BYTE nAlpha);

	//颜色反转
	static COLORREF ReverseColor(
		COLORREF clrColor);

	//转换颜色
	static COLORREF TranslateColor(
		COLORREF clrColor, 
		HPALETTE hPal = NULL);

};

#endif //__WIN32GDI_H__