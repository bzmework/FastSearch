// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F20F1E70_12C8_4BC8_ACCD_5BD11188D186__INCLUDED_)
#define AFX_STDAFX_H__F20F1E70_12C8_4BC8_ACCD_5BD11188D186__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

#define _WIN32_WINNT   0x0500

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include <afxcmn.h>		// MFC support for Windows Common Controls
#endif

#include <winternl.h>
#include <stdio.h>

#endif // !defined(AFX_STDAFX_H__F20F1E70_12C8_4BC8_ACCD_5BD11188D186__INCLUDED_)
