#include "gdip.h"

//∆Ù∂ØGDI+
BOOL Win32GDIP::Startup()
{ 
	if(!m_pGdiToken)
	{
		GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		if (GdiplusStartup(&m_pGdiToken, &input, NULL) != Ok)
		{
			m_pGdiToken = NULL;
		}
	}
	return (m_pGdiToken != NULL);
}

//πÿ±’GDI+
VOID Win32GDIP::ShutDown()
{ 
	if(m_pGdiToken)
	{
		GdiplusShutdown(m_pGdiToken); 
		m_pGdiToken = NULL;
		//MessageBox(NULL, _T("≥…π¶πÿ±’Gdiplus"), _T("±®∏Ê"), MB_OK);
	}
}

//ª≠‘≤ªÚÕ÷‘≤
VOID Win32GDIP::DrawCircle(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrFillColor, 
	COLORREF clrBorderColor,
	COLORREF clrHatchColor,
	BrushTypeConstants eBrushType,
	BrushHatchConstants eBrushHatch,
	PenStyleConstants ePenStyle,
	INT nLineWidth,
	FLOAT fAngle,
	BYTE nAlpha)
{
	Color fillColor(SetFromCOLORREF((COLORREF)clrFillColor, nAlpha));
	Color borderColor(SetFromCOLORREF((COLORREF)clrBorderColor, nAlpha));

	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);

	//ª≠±ﬂøÚ
	Pen pen(borderColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);
	graphic.DrawEllipse(&pen, X, Y, nWidth, nHeight);

	//ÃÓ≥‰
	if(eBrushType == btGradientHorizontal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeHorizontal);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
	else if(eBrushType == btGradientVertical)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeVertical);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
	else if(eBrushType == btGradientForwardDiagonal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeForwardDiagonal);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
	else if(eBrushType == btGradientBackwardDiagonal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeBackwardDiagonal);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
	else if(eBrushType == btGradientAngle)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, fAngle);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
	else if(eBrushType == btHatchFill)
	{
		Color hatchColor(SetFromCOLORREF((COLORREF)clrHatchColor, nAlpha));
		HatchBrush brush((HatchStyle)eBrushHatch, hatchColor, fillColor);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
	else
	{
		SolidBrush brush(fillColor);
		graphic.FillEllipse(&brush, X, Y, nWidth, nHeight);
	}
}

//ª≠œﬂ
VOID Win32GDIP::DrawLine(
	HDC hDC,
	INT X1,
	INT Y1,
	INT X2,
	INT Y2,
	COLORREF clrLineColor,
	PenStyleConstants ePenStyle,
	LineCapConstants eStartCap,
	LineCapConstants eEndCap,
	INT nLineWidth,
	BYTE nAlpha)
{
	Color lineColor(SetFromCOLORREF((COLORREF)clrLineColor, nAlpha));

	Pen pen(lineColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);
	pen.SetStartCap((LineCap)eStartCap);
	pen.SetEndCap((LineCap)eEndCap);

	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);
	graphic.DrawLine(&pen, X1, Y1, X2, Y2);
}

//ª≠ª°œﬂ
VOID Win32GDIP::DrawArc(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	FLOAT fStartAngle,
	FLOAT fSweepAngle,
	COLORREF clrLineColor,
	PenStyleConstants ePenStyle,
	INT nLineWidth,
	BYTE nAlpha)
{
	Color lineColor(SetFromCOLORREF((COLORREF)clrLineColor, nAlpha));

	Pen pen(lineColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);
	
	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);
	graphic.DrawArc(&pen, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
}

//ª≠±¥»˚∂˚«˙œﬂ
VOID Win32GDIP::DrawBezier(
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
	PenStyleConstants ePenStyle,
	INT nLineWidth,
	BYTE nAlpha)
{
	Color lineColor(SetFromCOLORREF((COLORREF)clrLineColor, nAlpha));

	Pen pen(lineColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);

	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);
	graphic.DrawBezier(&pen, X1, Y1, X2, Y2, X3, Y3, X4, Y4);
}

//ª≠±˝Õº
VOID Win32GDIP::DrawPie(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	FLOAT fStartAngle,
	FLOAT fSweepAngle,
	COLORREF clrFillColor, 
	COLORREF clrBorderColor,
	COLORREF clrHatchColor,
	BrushTypeConstants eBrushType,
	BrushHatchConstants eBrushHatch,
	PenStyleConstants ePenStyle,
	INT nLineWidth,
	FLOAT fAngle,
	BYTE nAlpha)
{
	Color fillColor(SetFromCOLORREF((COLORREF)clrFillColor, nAlpha));
	Color borderColor(SetFromCOLORREF((COLORREF)clrBorderColor, nAlpha));
	
	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);

	//ª≠±ﬂøÚ
	Pen pen(borderColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);
	graphic.DrawPie(&pen, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);

	//ÃÓ≥‰
	if(eBrushType == btGradientHorizontal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeHorizontal);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
	else if(eBrushType == btGradientVertical)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeVertical);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
	else if(eBrushType == btGradientForwardDiagonal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeForwardDiagonal);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
	else if(eBrushType == btGradientBackwardDiagonal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeBackwardDiagonal);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
	else if(eBrushType == btGradientAngle)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, fAngle);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
	else if(eBrushType == btHatchFill)
	{
		Color hatchColor(SetFromCOLORREF((COLORREF)clrHatchColor, nAlpha));
		HatchBrush brush((HatchStyle)eBrushHatch, hatchColor, fillColor);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
	else
	{
		SolidBrush brush(fillColor);
		graphic.FillPie(&brush, X, Y, nWidth, nHeight, fStartAngle, fSweepAngle);
	}
}

//ª≠«¯”Ú
VOID Win32GDIP::DrawArea(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrFillColor, 
	COLORREF clrHatchColor,
	BrushTypeConstants eBrushType,
	BrushHatchConstants eBrushHatch,
	BYTE nAlpha)
{
	Color fillColor(SetFromCOLORREF((COLORREF)clrFillColor, nAlpha));
	Color hatchColor(SetFromCOLORREF((COLORREF)clrHatchColor, nAlpha));

	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);
	
	if(eBrushType == btHatchFill)
	{
		HatchBrush brush((HatchStyle)eBrushHatch, hatchColor, fillColor);
		graphic.FillRectangle(&brush, X, Y, nWidth, nHeight);
	}
	else
	{
	    SolidBrush brush(fillColor);
		graphic.FillRectangle(&brush, X, Y, nWidth, nHeight);
	}
}

//ª≠±ﬂøÚ
VOID Win32GDIP::DrawBorder(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrBorderColor,
	PenStyleConstants ePenStyle,
	INT nLineWidth,
	BYTE nAlpha)
{
	Color borderColor(SetFromCOLORREF((COLORREF)clrBorderColor, nAlpha));
	
	Pen pen(borderColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);//÷ß≥÷œﬂ—˘ Ω
	
	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);
	graphic.DrawRectangle(&pen, X, Y, nWidth, nHeight);

}

//ª≠«¯”Úº∞±ﬂøÚ
VOID Win32GDIP::DrawBox(
	HDC hDC,
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrFillColor, 
	COLORREF clrBorderColor,
	COLORREF clrHatchColor,
	BrushTypeConstants eBrushType,
	BrushHatchConstants eBrushHatch,
	PenStyleConstants ePenStyle,
	INT nLineWidth,
	FLOAT fAngle,
	BYTE nAlpha)
{
	Color fillColor(SetFromCOLORREF((COLORREF)clrFillColor, nAlpha));
	Color borderColor(SetFromCOLORREF((COLORREF)clrBorderColor, nAlpha));
	  
	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);

	//ª≠±ﬂøÚ
	Pen pen(borderColor, (float)nLineWidth);
	pen.SetDashStyle((DashStyle)ePenStyle);
	graphic.DrawRectangle(&pen, X, Y, nWidth, nHeight);

	//ÃÓ≥‰
	if(eBrushType == btGradientHorizontal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeHorizontal);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
	else if(eBrushType == btGradientVertical)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeVertical);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
	else if(eBrushType == btGradientForwardDiagonal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeForwardDiagonal);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
	else if(eBrushType == btGradientBackwardDiagonal)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, LinearGradientModeBackwardDiagonal);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
	else if(eBrushType == btGradientAngle)
	{
		Rect tR(X, Y, nWidth, nHeight);
		LinearGradientBrush brush(tR, fillColor, borderColor, fAngle);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
	else if(eBrushType == btHatchFill)
	{
		Color hatchColor(SetFromCOLORREF((COLORREF)clrHatchColor, nAlpha));
		HatchBrush brush((HatchStyle)eBrushHatch, hatchColor, fillColor);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
	else
	{
		SolidBrush brush(fillColor);
		graphic.FillRectangle(&brush, X + 1, Y + 1, nWidth - 1, nHeight - 1);
	}
}

//ª≠Ω•±‰
VOID Win32GDIP::DrawGradient(
	HDC hDC, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrStartColor, 
	COLORREF clrEndColor, 
	GradientDirectionConstants eDirection,
	FLOAT fAngle, 
	BYTE nAlpha)
{
	Color startColor(SetFromCOLORREF((COLORREF)clrStartColor, nAlpha));
	Color endColor(SetFromCOLORREF((COLORREF)clrEndColor, nAlpha));
	
	Graphics graphic((HDC)hDC);
	graphic.SetSmoothingMode(m_eSmoothingMode);
	
	Rect tR(X, Y, nWidth, nHeight);
	if(fAngle == 0.0f)
	{
		LinearGradientBrush brush(tR, startColor, endColor, (LinearGradientMode)eDirection);
		graphic.FillRectangle(&brush, X, Y, nWidth, nHeight);
	}
	else
	{
		LinearGradientBrush brush(tR, startColor, endColor, fAngle);
		graphic.FillRectangle(&brush, X, Y, nWidth, nHeight);
	}
}

//ª≠Œƒ±æ
VOID Win32GDIP::DrawText(
	HDC hDC, 
	LPCWSTR strText, 
	INT X,
	INT Y,
	INT nWidth,
	INT nHeight,
	COLORREF clrTextColor, 
	TextFormatConstants eFormat,
    TextHAlignmentConstants eHAlignment, 
	TextVAlignmentConstants eVAlignment,
	LPCWSTR strFontName, 
	UINT nFontSize, 
	INT  nFontRotationDegrees, 
	BOOL bFontBold,
	BOOL bFontItalic,
	BOOL bFontUnderline,
	BOOL bFontStrikethru,
	BOOL bFontHighQuality,
	BYTE nAlpha)
{
	RectF rectF((REAL)X, (REAL)Y, (REAL)nWidth, (REAL)nHeight);

	Color textColor(SetFromCOLORREF((COLORREF)clrTextColor, nAlpha));
	SolidBrush brush(textColor);

	INT nFontStyle = 0;
	if(bFontBold)
		nFontStyle |= FontStyleBold;
	else if(bFontItalic)
		nFontStyle |= FontStyleItalic;
	else if(bFontUnderline)
		nFontStyle |= FontStyleUnderline;
	else if(bFontStrikethru)
		nFontStyle |= FontStyleBoldItalic;
	Gdiplus::FontFamily fontFamily(strFontName);
	Gdiplus::Font font(&fontFamily, (REAL)nFontSize, nFontStyle, UnitPoint);
	
	StringFormat format;
	format.SetAlignment((StringAlignment)eHAlignment); //ÀÆ∆Ω∂‘∆Î
	format.SetLineAlignment((StringAlignment)eVAlignment);//¥π÷±∂‘∆Î
	if(eFormat > tfDefault)
	{
		format.SetFormatFlags(eFormat); //…Ë÷√∏Ò Ω
	}
	
	Graphics graphic((HDC)hDC);
	if(bFontHighQuality)
	{
		graphic.SetSmoothingMode(SmoothingModeAntiAlias);
	}
	else
	{
		graphic.SetSmoothingMode(SmoothingModeDefault);
	}
	graphic.DrawString(strText, -1, &font, rectF, &format, &brush);
}
