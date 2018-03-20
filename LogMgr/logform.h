/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�LogForm.h
* ժ    Ҫ����־�������
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/


#if !defined(AFX_LogForm_H__1AAA91FD_5B1A_4867_8FDE_B97EC446A4E3__INCLUDED_)
#define AFX_LogForm_H__1AAA91FD_5B1A_4867_8FDE_B97EC446A4E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogForm.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLogForm dialog

class CLogForm : public CBCGPDialog
{
// Construction
public:
	CLogForm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogForm)
	enum { IDD = IDD_DLG_LOGMGR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogForm)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//��־�ļ������б�ؼ�
	CBCGPListCtrl	m_lstLogContent;

	//������־�ļ����е���־�ļ�����ʼ����־�ļ������б�
    void InitTree();

	//��ʾ��־�ļ�����
	afx_msg void ClickTree(NMHDR* pNMHDR, LRESULT* pResult);

	//��־�ļ��б����οؼ�
	CTreeCtrl m_treLogFile;

	CRect m_rectMaxList;
	CRect m_rectMaxTree;
	CRect m_rectRestoredList;
	CRect m_rectRestoredTree;
};  


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LogForm_H__1AAA91FD_5B1A_4867_8FDE_B97EC446A4E3__INCLUDED_)
