// UnidirectDataTransfer.h : main header file for the UNIDIRECTDATATRANSFER application
//

#if !defined(AFX_UNIDIRECTDATATRANSFER_H__7E15CA95_A03A_48CF_AFAC_764D5663F9C9__INCLUDED_)
#define AFX_UNIDIRECTDATATRANSFER_H__7E15CA95_A03A_48CF_AFAC_764D5663F9C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
/////////////////////////////////////////////////////////////////////////////
// CUnidirectDataTransferApp:
// See UnidirectDataTransfer.cpp for the implementation of this class
//
//#include "OutLogIn.h"

class CUnidirectDataTransferApp : public CWinApp,public CBCGPWorkspace

{
public:
	CUnidirectDataTransferApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnidirectDataTransferApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnidirectDataTransferApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString  m_strUserName;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIDIRECTDATATRANSFER_H__7E15CA95_A03A_48CF_AFAC_764D5663F9C9__INCLUDED_)
