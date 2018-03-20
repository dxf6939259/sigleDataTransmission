// TransControl.h : main header file for the TRANSCONTROL DLL
//

#if !defined(AFX_TRANSCONTROL_H__5DC639D8_49D2_4C90_A1A6_58A3221A0CDC__INCLUDED_)
#define AFX_TRANSCONTROL_H__5DC639D8_49D2_4C90_A1A6_58A3221A0CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTransControlApp
// See TransControl.cpp for the implementation of this class
//

class CTransControlApp : public CWinApp
{
public:
	CTransControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTransControlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		//add lgp 10-31 begin
	public:
		int m_nSendRate;
		//add lgp 10-31 end
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSCONTROL_H__5DC639D8_49D2_4C90_A1A6_58A3221A0CDC__INCLUDED_)
