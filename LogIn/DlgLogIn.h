/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgLogIn.h
* 摘    要：登录对话框类
* 
* 创建者：李国平
* 创建时间：2013/9/14
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
	//获得软件名称
	void SetSoftName(CString strSoftName);

	
private:

	//数据库访问对象
	CDbAccess m_dbAccess;

    //输入用户名密码的次数
    UINT m_nLogCount;

    //软件名称字体对象
	CFont* m_pFontSoftName;

	//软件名称--【数据发送/数据接收】
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
