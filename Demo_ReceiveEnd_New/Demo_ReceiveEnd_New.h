// Demo_ReceiveEnd_New.h : main header file for the DEMO_RECEIVEEND_NEW application
//

#if !defined(AFX_DEMO_RECEIVEEND_NEW_H__E4C65B4B_9CC1_4E5E_876F_8C1E1F14183C__INCLUDED_)
#define AFX_DEMO_RECEIVEEND_NEW_H__E4C65B4B_9CC1_4E5E_876F_8C1E1F14183C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemo_ReceiveEnd_NewApp:
// See Demo_ReceiveEnd_New.cpp for the implementation of this class
//

class CDemo_ReceiveEnd_NewApp : public CWinApp
{
public:
	CDemo_ReceiveEnd_NewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo_ReceiveEnd_NewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemo_ReceiveEnd_NewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_RECEIVEEND_NEW_H__E4C65B4B_9CC1_4E5E_876F_8C1E1F14183C__INCLUDED_)
