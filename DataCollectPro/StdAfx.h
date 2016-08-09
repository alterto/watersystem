// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__061898B5_87E9_4DA6_B7B6_8554332AB042__INCLUDED_)
#define AFX_STDAFX_H__061898B5_87E9_4DA6_B7B6_8554332AB042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxmt.h>

#include <iostream>
#include <string>
#include <stack>
using namespace std;

#include "ado.h"

#include "wcomm_dll.h"
#pragma comment(lib,"wcomm_dll.lib")
#include <mmsystem.h>  //包含头文件
#pragma comment(lib, "winmm.lib") 
#pragma message("Automatically linking with wsock.dll")

#include "GeneralFuntion.h"
#include "JSON/cJSON.h"

//机井信息
struct WellInfo
{
	WellInfo(CString strCezhanID, int iDevID)
	{
		this->strCezhanID = strCezhanID;
		this->iDevID = iDevID;
	}
	
	CString strCezhanID;
	int iDevID;
};
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__061898B5_87E9_4DA6_B7B6_8554332AB042__INCLUDED_)
