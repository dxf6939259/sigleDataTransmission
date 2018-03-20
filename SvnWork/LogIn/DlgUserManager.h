/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgUserManager.h
* ժ    Ҫ���û�ά���Ի�����
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
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
	//��ǰ�û�û�з��䵽��Ȩ�޼��б�ؼ�
	CBCGPListCtrl	m_lstUnRole;
	
	//��ǰ�û��ѷ��䵽��Ȩ�޼��б�ؼ�
	CBCGPListCtrl	m_lstRole;
	
	//�û��б�ؼ�
	CBCGPListCtrl	m_lstUser;

	//�û�����ID
	int m_nUserID;

	//�û��б�ؼ��ϣ��û���ǰѡ����û���¼�Ŀؼ�ID
	int m_nLstUserSel;

	//���ݿ������
	CDbAccess m_dbAccess;

	//��ǰѡ���Ѹ����ɫ�б��е�ID
	int m_nRoleIDSel;

	//��ǰѡ��δ�����ɫ�б��е�ID
	int m_nUnRoleIDSel;

    //�����ʼ��
	void Init();

	//��֤�û�������������Ƿ�Ϸ�
    BOOL ValidateParam(CString strUser, CString strPwd, CString strRemark);

    //�������û�
    void CreateNewUser();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSERMANAGER_H__3A930F4B_3453_4A5D_B558_097BCC5B9266__INCLUDED_)
