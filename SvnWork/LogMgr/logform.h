/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：LogForm.h
* 摘    要：日志管理界面
* 
* 创建者：李国平
* 创建时间：2013/9/14
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
	//日志文件内容列表控件
	CBCGPListCtrl	m_lstLogContent;

	//遍历日志文件夹中的日志文件，初始化日志文件树形列表
    void InitTree();

	//显示日志文件内容
	afx_msg void ClickTree(NMHDR* pNMHDR, LRESULT* pResult);

	//日志文件列表树形控件
	CTreeCtrl m_treLogFile;

	CRect m_rectMaxList;
	CRect m_rectMaxTree;
	CRect m_rectRestoredList;
	CRect m_rectRestoredTree;
};  


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LogForm_H__1AAA91FD_5B1A_4867_8FDE_B97EC446A4E3__INCLUDED_)
