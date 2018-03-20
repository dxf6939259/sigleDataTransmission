/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：RegistationSocket.cpp
* 摘    要：为某一发送获取一个申请号
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

     dgh       2013-09-18       Ver 1.0         创建
*/

// TransControl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TransControl.h"
//lgp add 10-31 begin
#include "Markup.h"
//lgp add 10-31 end

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CTransControlApp

BEGIN_MESSAGE_MAP(CTransControlApp, CWinApp)
	//{{AFX_MSG_MAP(CTransControlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransControlApp construction

CTransControlApp::CTransControlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    //add lgp 10-31 begin
	m_nSendRate = 0;
	//add lgp 10-31 end
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTransControlApp object

CTransControlApp theApp;

BOOL CTransControlApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("MFC通信socket初始化失败!"));
	}

//	WSADATA wsd;
	int ret=0;

// 	ret = WSAStartup(MAKEWORD(2,2),&wsd);
// 	if (0!=ret)
// 	{
// 		AfxMessageBox(_T("WSAStartup失败"));
// 	}
	//add lgp 10-31 begin
	CMarkup xml;
	bool    flag; 
	
	//加载Xml文件  
	flag = xml.Load(_T("TransControl.xml"));   
	if (!flag)   
	{   
		return CWinApp::InitInstance();
	}  
	xml.ResetPos(); 
	flag = xml.FindElem("TransCon");    
	if (!flag)  
	{  
		return CWinApp::InitInstance(); 
	}  
	
	CString strSendRate; 
	xml.IntoElem();    
	if(xml.FindElem(TEXT("SendRate")))
	{
		strSendRate = xml.GetData();   
	}
	xml.OutOfElem(); 
	m_nSendRate = atoi(strSendRate.GetBuffer(strSendRate.GetLength()));
	//add lgp 10-31 end
	return CWinApp::InitInstance();
}
