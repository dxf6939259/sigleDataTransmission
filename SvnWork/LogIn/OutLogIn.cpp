/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：OutLogIn.cpp
* 摘    要：单机身份验证模块导出类
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

*/

#include "stdafx.h"
#include "LogIn.h"
#include "OutLogIn.h"
#include "DbAccess.h"
#include "DlgLogIn.h"
#include "DlgUserManager.h"
#include "DbAccess.h"
#include "OutLogMgr.h"





#define  ROLE_NAME_ALL           _T("管理员")
#define  ROLE_NAME_RECV_FILE     _T("文件接收管理员")
#define  ROLE_NAME_RECV_DATA     _T("数据接收管理员")
#define  ROLE_NAME_RECV_MEDIA    _T("流媒体接收管理员")
#define  ROLE_NAME_SEND_FILE     _T("文件传输管理员")
#define  ROLE_NAME_SEND_DATA     _T("数据传输管理员")
#define  ROLE_NAME_SEND_MEDIA    _T("流媒体传输管理员")
#define  ROLE_NAME_SEND_END      _T("终端设备信息管理员")

#define MODULE_NAME_ALL        _T("全部模块")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CLogInApp theApp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static COutLogIn* m_pCOutLogIn = NULL;

COutLogIn::COutLogIn(void)
{
   m_strLstUserName.RemoveAll();
}

COutLogIn::~COutLogIn(void)
{
	if(NULL != m_pCOutLogIn)
	{
		delete m_pCOutLogIn;
		m_pCOutLogIn = NULL;
	}
 
}


/***************************************************************
函数名：COutLogIn* GetInstance()

  功能描述：获得单例类对象
  
  参数：无
	
  全局变量：无
	  
  返回值: COutLogIn*     单机身份验证导出类对象指针
		
****************************************************************/
COutLogIn* COutLogIn::GetInstance()
{
	//static COutLogIn logIn;
	//return &logIn;
	

	if(NULL == m_pCOutLogIn)
	{
		m_pCOutLogIn = new COutLogIn();
	}
	return m_pCOutLogIn;

}

/***************************************************************
函数名：BOOL LogIn(CString strSoftName , CString &strUser)

  功能描述：弹出登陆对话框，验证用户是否有权限登陆
  
  参数：CString strSoftName 软件名称【数据发送/数据接收】 
        CString &strUser    用户输入的用户名，若验证失败，则为空
	
  全局变量：m_strUserName
	  
  返回值: BOOL     TRUE--用户身份验证成功  FALSE--用户身份验证失败
		
****************************************************************/
BOOL COutLogIn::LogIn(CString strSoftName , CString &strUser)
{
	//资源保护
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CDlgLogIn dlgLogin;
	dlgLogin.SetSoftName(strSoftName);
    dlgLogin.DoModal();

	if(-1 == strSoftName.Find(_T("接")))//没有"接"字
	{
      theApp.m_enmSoftName = ENU_SOFT_SENDER;
	}
	else
	{
	  theApp.m_enmSoftName = ENU_SOFT_RECEIVER;
	}
	
	//获取输入的用户名
	strUser = theApp.m_strUserName;
	m_strLstUserName.AddTail(strUser);
    
	if(strUser.IsEmpty())
    	return FALSE;
	else
		return TRUE;

}

/***************************************************************
函数名：BOOL ValidateUser(CString strUser, CString strPWD)

  功能描述：验证传入的用户名和密码是否有权限登陆
  
  参数：CString strUser 用户名 
        CString strPWDr 用户名密码
	
  全局变量：
	  
  返回值: BOOL     TRUE--用户名密码验证成功  FALSE--用户名密码验证失败
		
****************************************************************/
BOOL COutLogIn::ValidateUser(CString strUser, CString strPWD)
{
	CDbAccess dbAccess;
	dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);

	CString strSql;
	strSql.Format(_T("select * from DXCS_ZB_YH where YHMC='%s' and YHMM='%s'"), 
		strUser, strPWD);
	BOOL bRet = dbAccess.OpenQuery(strSql);
    int nUserID = dbAccess.GetIntValue(_T("YHBH"));
	if((TRUE == bRet)&&(DB_GET_FIELD_VALUE_ERROR != nUserID))
	{
		CString strMsg;
		strMsg.Format(_T("%s登录成功\r\n"), strUser);
		COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
        
	}
	else 
	{
		CString strMsg;
		strMsg.Format(_T("%s登录失败\r\n"), strUser);
		COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		bRet = FALSE;
   }

	dbAccess.CloseRecordSet();
	dbAccess.CloseDataBase();
	
	return bRet;
}

/***************************************************************
函数名：BOOL ValidatePermissions(CString strUser, CString strModule)

  功能描述：验证用户是否有权限操作所传入的模块
  
  参数：CString strUser 用户名 
        CString strModule 模块名
	
  全局变量：无
	  
  返回值: BOOL     TRUE--验证成功  FALSE--验证失败
		
****************************************************************/
BOOL COutLogIn::ValidatePermissions(CString strUser, CString strModule)
{
	 if(NULL == m_strLstUserName.Find(strUser))
		return FALSE;

	 CDbAccess dbAccess;
	 dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);
	 
	 CString strSql;
	 strSql.Format(_T("select * from DXCS_GX_YHQX where YHBH=(select YHBH from DXCS_ZB_YH where YHMC='%s') and JSBH in (select JSBH from DXCS_ZD_JS where MKMC in('全部模块','%s'))"), 
		 strUser, strModule);
	 BOOL bRet = dbAccess.OpenQuery(strSql);
	 int nID = dbAccess.GetIntValue(_T("YHBH"));
	 if(DB_GET_FIELD_VALUE_ERROR != nID)
         bRet = TRUE;
	 else
         bRet = FALSE;
	 dbAccess.CloseRecordSet();
	 dbAccess.CloseDataBase();
	 
	return bRet;
 }
/***************************************************************
函数名： void UserManagement(CString strUser)

  功能描述：根据用户名弹出用户维护界面，只有管理员才能维护用户。
  
  参数：CString strUser 用户名 

  全局变量：无
	  
  返回值: 无
		
****************************************************************/
 void COutLogIn::UserManagement(CString strUser)
 {
	 if(NULL == m_strLstUserName.Find(strUser))
		return ;

	 BOOL bRet = ValidatePermissions(strUser, MODULE_NAME_ALL);

	 //资源保护
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

     CDlgUserManager dlgUserManager;
	 dlgUserManager.DoModal();
 }
 /***************************************************************
函数名：BOOL VerifySoftAuthority()

  功能描述：验证用户是否有权限使用此软件
  
  参数：CString strUserName 用户名称
        CString strSoftName 软件名称
	
  全局变量：无
	  
  返回值: BOOL     TRUE-用户有权限使用此软件   FALSE-用户没有权限使用此软件
		           只要用户具有使用此软件的任一模块的权限，即可使用此软件
****************************************************************/
BOOL COutLogIn::VerifySoftAuthority(CString strUserName, CString strSoftName)
{
	

	 CDbAccess dbAccess;
	 dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);

	  //查询数据库，进行权限验证
      CString strSql;
	  if(strSoftName.Compare(SOFT_NAME_XML_SEND)==0)
	  {
         strSql.Format(_T("select * from DXCS_GX_YHQX where YHBH=(select YHBH from DXCS_ZB_YH where YHMC='%s') and JSBH in (select JSBH from DXCS_ZD_JS where JSMC in('%s','%s','%s','%s','%s'))"), 
			 strUserName, ROLE_NAME_ALL, ROLE_NAME_SEND_FILE, ROLE_NAME_SEND_DATA, ROLE_NAME_SEND_MEDIA, ROLE_NAME_SEND_END);
	  }
	  else if(strSoftName.Compare(SOFT_NAME_XML_RECV)==0)
	  {
         strSql.Format(_T("select * from DXCS_GX_YHQX where YHBH=(select YHBH from DXCS_ZB_YH where YHMC='%s') and JSBH in (select JSBH from DXCS_ZD_JS where JSMC in('%s','%s','%s','%s'))"), 
			 strUserName, ROLE_NAME_ALL, ROLE_NAME_RECV_FILE, ROLE_NAME_RECV_DATA, ROLE_NAME_RECV_MEDIA);
	  }
	  else
	  {
		  dbAccess.CloseRecordSet();
	      dbAccess.CloseDataBase();
		  return FALSE;
	  }

	  BOOL bRet = dbAccess.OpenQuery(strSql);
	  int nID = dbAccess.GetIntValue(_T("YHBH"));
	  if(DB_GET_FIELD_VALUE_ERROR != nID)
		  bRet = TRUE;
	  else
         bRet = FALSE;

	  dbAccess.CloseRecordSet();
	  dbAccess.CloseDataBase();

	  //角色验证结果写日志
	  if(bRet)
	  {
		CString strMsg;
		strMsg.Format(_T("%s角色验证\r\n"), strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	  }
	  else
	  {
		 CString strMsg;
		 strMsg.Format(_T("%s角色验证\r\n"), strUserName);
	     COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
	  }
     
	  return bRet;
}