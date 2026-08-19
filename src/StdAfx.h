// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8E13A03C_101A_11D1_90FF_444553540000__INCLUDED_)
#define AFX_STDAFX_H__8E13A03C_101A_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxmt.h>      // MFC multi-threading support
#include <afxtempl.h>   // MFC template classes

#include "mmsystem.h"
#include "vfw.h"        // Video for Windows

#include "d3drm.h"      // DirectX 3
#include "d3drmwin.h"
#include "ddraw.h"
#include "dinput.h"
#include "dplay.h"
#include "dplobby.h"
#include "dsound.h"

#pragma warning( disable : 4244 4305 )   // conversion to float, truncation

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8E13A03C_101A_11D1_90FF_444553540000__INCLUDED_)
