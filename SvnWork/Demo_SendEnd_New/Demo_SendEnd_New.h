// Demo_SendEnd_New.h : main header file for the DEMO_SENDEND_NEW application
//

#if !defined(AFX_DEMO_SENDEND_NEW_H__C7F792AD_901E_440A_80BA_0B4D651BFCBA__INCLUDED_)
#define AFX_DEMO_SENDEND_NEW_H__C7F792AD_901E_440A_80BA_0B4D651BFCBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemo_SendEnd_NewApp:
// See Demo_SendEnd_New.cpp for the implementation of this class
//

class CDemo_SendEnd_NewApp : public CWinApp
{
public:
	CDemo_SendEnd_NewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo_SendEnd_NewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemo_SendEnd_NewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_SENDEND_NEW_H__C7F792AD_901E_440A_80BA_0B4D651BFCBA__INCLUDED_)
