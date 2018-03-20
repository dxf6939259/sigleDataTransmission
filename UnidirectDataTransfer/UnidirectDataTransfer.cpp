// UnidirectDataTransfer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UnidirectDataTransfer.h"
#include "DlgDataReceive.h"
#include "DlgDataSend.h"
#include "OperXml.h"
#include "OutLogIn.h"
#include <shlwapi.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnidirectDataTransferApp

BEGIN_MESSAGE_MAP(CUnidirectDataTransferApp, CWinApp)
	//{{AFX_MSG_MAP(CUnidirectDataTransferApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnidirectDataTransferApp construction

CUnidirectDataTransferApp::CUnidirectDataTransferApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_strUserName.Empty();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUnidirectDataTransferApp object

CUnidirectDataTransferApp theApp;
HANDLE hMutex;
/////////////////////////////////////////////////////////////////////////////
// CUnidirectDataTransferApp initialization

BOOL CUnidirectDataTransferApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//CUnidirectDataTransferDlg dlg;
   CBCGPVisualManager2007::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2007));//



	/*CString StrName="UnidirectDataTransfer";//程序(进程)名
	hMutex=OpenMutex(MUTEX_ALL_ACCESS,FALSE,StrName);
	if(NULL == hMutex)
	{
		hMutex=::CreateMutex(NULL,NULL,StrName);
		ResumeThread();
	}
	else
	{
		BCGPMessageBox(NULL, _T("单向数据传输软件已启动，不能再次启动"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return false;
	}*/

     if(!PathFileExists(_T("DXSJCS.xml")))
	 {
		 BCGPMessageBox(_T("应用程序目录缺少配置文件DXSJCS.xml，程序启动失败"));
		 return FALSE;
	 }

	 if(!PathFileExists(_T("DXCS.mdb")))
	 {
		 BCGPMessageBox(_T("应用程序目录缺少数据库文件DXCS.mdb，程序启动失败"));
		 return FALSE;
	 }

	CString strSoftName = COperXml::GetInstance()->GetSoftType();
	if(strSoftName.IsEmpty())
		 return FALSE;

	 int nResponse ;
     if(ENU_SOFT_SENDER == COperXml::g_softType)
	 {
		 CString strUserName;
		 COutLogIn::GetInstance()->LogIn(SOFT_NAME_XML_SEND, strUserName);
		 if(strUserName.IsEmpty())
			 return FALSE;

		 m_strUserName = strUserName;
		 CDlgDataSend dlgSend;
		 m_pMainWnd = &dlgSend;
		 nResponse = dlgSend.DoModal();
	 }
	 
	if(ENU_SOFT_RECEIVER == COperXml::g_softType)
	{
		CString strUserName;
		COutLogIn::GetInstance()->LogIn(SOFT_NAME_XML_RECV, strUserName);
		if(strUserName.IsEmpty())
			 return FALSE;

		m_strUserName = strUserName;
		CDlgDataReceive dlgReceive;
		m_pMainWnd = &dlgReceive;
	    nResponse = dlgReceive.DoModal();
	}

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
		return FALSE;
	}
	

	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CUnidirectDataTransferApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(hMutex!=NULL)
	{
		ReleaseMutex(hMutex);
	}
	//CleanState();
    //BCGCBProCleanUp();
	return CWinApp::ExitInstance();
}


