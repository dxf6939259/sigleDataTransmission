// Demo_SendEnd_NewDlg.h : header file
//

#if !defined(AFX_DEMO_SENDEND_NEWDLG_H__8691942A_54B6_4F97_AFA1_C3241F6D200C__INCLUDED_)
#define AFX_DEMO_SENDEND_NEWDLG_H__8691942A_54B6_4F97_AFA1_C3241F6D200C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemo_SendEnd_NewDlg dialog

class CDemo_SendEnd_NewDlg : public CDialog
{
// Construction
public:
	CDemo_SendEnd_NewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemo_SendEnd_NewDlg)
	enum { IDD = IDD_DEMO_SENDEND_NEW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo_SendEnd_NewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemo_SendEnd_NewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonBrowse();
	afx_msg void OnBtnSendFile();
	afx_msg void OnBtnClearFile();
	afx_msg void OnBtnSendData();
	afx_msg void OnBtnClearData();
	afx_msg void OnBtnSendMedia();
	afx_msg void OnBtnClearMedia();
	afx_msg void OnBtnTestBegin();
	afx_msg void OnBtnTestStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_SENDEND_NEWDLG_H__8691942A_54B6_4F97_AFA1_C3241F6D200C__INCLUDED_)
