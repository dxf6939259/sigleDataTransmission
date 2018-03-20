/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgLogIn.h
* ժ    Ҫ����¼�Ի�����
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/

#if !defined(AFX_DLGLOGIN_H__15FE0991_CEFF_4F7E_B895_E7D94878C87A__INCLUDED_)
#define AFX_DLGLOGIN_H__15FE0991_CEFF_4F7E_B895_E7D94878C87A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLogIn.h : header file
//
#include "DlgLogIn.h"
#include "resource.h"
#include "DbAccess.h"
#include "MyMessageBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLogIn dialog

class  CDlgLogIn : public CBCGPDialog
{
// Construction
public:
	CDlgLogIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLogIn)
	enum { IDD = IDD_DLG_LOGIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLogIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
private:

	// Generated message map functions
	//{{AFX_MSG(CDlgLogIn)
	afx_msg void OnButLogin();
	afx_msg void OnButCancel();
	virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//����������
	void SetSoftName(CString strSoftName);

	
private:

	//���ݿ���ʶ���
	CDbAccess m_dbAccess;

    //�����û�������Ĵ���
    UINT m_nLogCount;

    //��������������
	CFont* m_pFontSoftName;

	//�������--�����ݷ���/���ݽ��ա�
	CString m_strSoftName;
public:
	BOOL m_bPrompt;
   //14-01-16 add lgp from yn begin
	CMyMessageBox* m_msgDlg;
	BOOL m_bFlag;
	//14-01-16 add lgp from yn end
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGIN_H__15FE0991_CEFF_4F7E_B895_E7D94878C87A__INCLUDED_)
