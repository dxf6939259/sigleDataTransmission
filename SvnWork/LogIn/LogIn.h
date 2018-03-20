// LogIn.h : main header file for the LOGIN DLL
//

#if !defined(AFX_LOGIN_H__DB3972CD_61CD_4F09_90E6_FF73A1F03841__INCLUDED_)
#define AFX_LOGIN_H__DB3972CD_61CD_4F09_90E6_FF73A1F03841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogInApp
// See LogIn.cpp for the implementation of this class
//
//#include "BCGPWorkspace.h"

#define  DB_FILE_NAME_OFFICE_ACCESS_2003  _T("DXCS.mdb")

//软件类型枚举
typedef enum _E_SOFT_TYPE
{
	ENU_SOFT_ERROR=0,
	ENU_SOFT_SENDER=1,
	ENU_SOFT_RECEIVER,
}E_SOFT_TYPE;

class CLogInApp : public CWinApp
{
public:
	CLogInApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogInApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLogInApp)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	E_SOFT_TYPE m_enmSoftName;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__DB3972CD_61CD_4F09_90E6_FF73A1F03841__INCLUDED_)
