#if !defined(AFX_MYMESSAGEBOX_H__A17837ED_9395_42D8_BDCF_FF7D4732DD10__INCLUDED_)
#define AFX_MYMESSAGEBOX_H__A17837ED_9395_42D8_BDCF_FF7D4732DD10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyMessageBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMessageBox dialog

class CMyMessageBox : public CBCGPDialog
{
// Construction
public:
	CMyMessageBox(CWnd* pParent = NULL);   // standard constructor
	
	CMyMessageBox(CWnd* pParent = NULL,CString strShowInfo = "",CString strCaption = "ÌבÊ¾");
// Dialog Data
	//{{AFX_DATA(CMyMessageBox)
	enum { IDD = IDD_DIALOG_MESSAGE };
	CString	m_strShowInfo;
	//}}AFX_DATA

	void SetInfo(CString strInfo){m_strShowInfo = strInfo;UpdateData(FALSE);};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMessageBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyMessageBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strOutput;
	CString m_strCaption;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMESSAGEBOX_H__A17837ED_9395_42D8_BDCF_FF7D4732DD10__INCLUDED_)
