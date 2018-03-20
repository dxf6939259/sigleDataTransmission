// LogMgr.h : main header file for the LOGMGR DLL
//

#if !defined(AFX_LOGMGR_H__0E8BAC17_C5DE_4BC4_9FC8_1A9CA28305FF__INCLUDED_)
#define AFX_LOGMGR_H__0E8BAC17_C5DE_4BC4_9FC8_1A9CA28305FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogMgrApp
// See LogMgr.cpp for the implementation of this class
//
#include "LogForm.h"
class CLogMgrApp : public CWinApp
{
public:
	CLogMgrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogMgrApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLogMgrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


}; 


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGMGR_H__0E8BAC17_C5DE_4BC4_9FC8_1A9CA28305FF__INCLUDED_)
