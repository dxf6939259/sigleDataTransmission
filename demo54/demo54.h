// demo54.h : main header file for the DEMO54 application
//

#if !defined(AFX_DEMO54_H__5610CE29_61E7_4974_ACAE_5DD278C740D0__INCLUDED_)
#define AFX_DEMO54_H__5610CE29_61E7_4974_ACAE_5DD278C740D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemo54App:
// See demo54.cpp for the implementation of this class
//

class CDemo54App : public CWinApp
{
public:
	CDemo54App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo54App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemo54App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO54_H__5610CE29_61E7_4974_ACAE_5DD278C740D0__INCLUDED_)
