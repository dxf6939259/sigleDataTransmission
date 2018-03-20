// MyMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "LogIn.h"
#include "MyMessageBox.h"
#include "DlgLogIn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyMessageBox dialog
extern CLogInApp theApp;

CMyMessageBox::CMyMessageBox(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CMyMessageBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyMessageBox)
	m_strShowInfo = _T("");
	//}}AFX_DATA_INIT
}
CMyMessageBox::CMyMessageBox(CWnd* pParent,CString strShowInfo,CString strCaption)
	: CBCGPDialog(CMyMessageBox::IDD, pParent)
{
	m_strOutput = strShowInfo;
	m_strCaption = strCaption;
}

void CMyMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyMessageBox)
	DDX_Text(pDX, IDC_STATIC_SHOW, m_strShowInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyMessageBox, CBCGPDialog)
	//{{AFX_MSG_MAP(CMyMessageBox)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMessageBox message handlers

BOOL CMyMessageBox::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	theApp.m_strUserName.Empty();
	m_strShowInfo = m_strOutput;
	SetWindowText(m_strCaption);
	UpdateData(FALSE);
	//设置对话框的BCG界面风格
	EnableVisualManagerStyle();
	SetFocus();
	GetDlgItem(IDOK)->SetFocus();

// 	CDlgLogIn* pParent = (CDlgLogIn*)GetParent();
// 	pParent->m_bFlag = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
// BOOL CMyMessageBox::PreTranslateMessage(MSG* pMsg) 
// {
// if(pMsg->wParam == VK_RETURN )
// {
//  	CWnd* pwnd = GetTopLevelOwner();
//  	CString strTitle;
//  	pwnd->GetWindowText(strTitle);
//  	if(strTitle.Compare("登录") == 0){
//  	}else{
//   		OnOK();
//  	}
// }
// return CBCGPDialog::PreTranslateMessage(pMsg);
// }

void CMyMessageBox::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CBCGPDialog::OnClose();
}

void CMyMessageBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBCGPDialog::OnShowWindow(bShow, nStatus);
	CDlgLogIn* pParent = (CDlgLogIn*)GetParent();
	pParent->m_bFlag = bShow;
	// TODO: Add your message handler code here
}
