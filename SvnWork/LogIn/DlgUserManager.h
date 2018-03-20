/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgUserManager.h
* 摘    要：用户维护对话框类
* 
* 创建者：李国平
* 创建时间：2013/9/14
*/

#if !defined(AFX_DLGUSERMANAGER_H__3A930F4B_3453_4A5D_B558_097BCC5B9266__INCLUDED_)
#define AFX_DLGUSERMANAGER_H__3A930F4B_3453_4A5D_B558_097BCC5B9266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserManager.h : header file
//
#include "DbAccess.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgUserManager dialog

class CDlgUserManager : public CBCGPDialog
{
// Construction
public:
	CDlgUserManager(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUserManager)
	enum { IDD = IDD_DLG_USER_MANAGER };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUserManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
private:

	// Generated message map functions
	//{{AFX_MSG(CDlgUserManager)
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSaveUser();
	afx_msg void OnDeleteUser();
	afx_msg void OnNewUser();
	afx_msg void OnAddRole();
	afx_msg void OnRemoveRole();
	afx_msg void OnClickRole(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickUnRole(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickUser(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//当前用户没有分配到的权限集列表控件
	CBCGPListCtrl	m_lstUnRole;
	
	//当前用户已分配到的权限集列表控件
	CBCGPListCtrl	m_lstRole;
	
	//用户列表控件
	CBCGPListCtrl	m_lstUser;

	//用户主键ID
	int m_nUserID;

	//用户列表控件上，用户当前选择的用户记录的控件ID
	int m_nLstUserSel;

	//数据库访问类
	CDbAccess m_dbAccess;

	//当前选择已赋予角色列表中的ID
	int m_nRoleIDSel;

	//当前选择未赋予角色列表中的ID
	int m_nUnRoleIDSel;

    //界面初始化
	void Init();

	//验证用户名和密码参数是否合法
    BOOL ValidateParam(CString strUser, CString strPwd, CString strRemark);

    //创建新用户
    void CreateNewUser();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSERMANAGER_H__3A930F4B_3453_4A5D_B558_097BCC5B9266__INCLUDED_)
