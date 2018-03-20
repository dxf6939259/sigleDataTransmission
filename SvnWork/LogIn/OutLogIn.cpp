/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�OutLogIn.cpp
* ժ    Ҫ�����������֤ģ�鵼����
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

*/

#include "stdafx.h"
#include "LogIn.h"
#include "OutLogIn.h"
#include "DbAccess.h"
#include "DlgLogIn.h"
#include "DlgUserManager.h"
#include "DbAccess.h"
#include "OutLogMgr.h"





#define  ROLE_NAME_ALL           _T("����Ա")
#define  ROLE_NAME_RECV_FILE     _T("�ļ����չ���Ա")
#define  ROLE_NAME_RECV_DATA     _T("���ݽ��չ���Ա")
#define  ROLE_NAME_RECV_MEDIA    _T("��ý����չ���Ա")
#define  ROLE_NAME_SEND_FILE     _T("�ļ��������Ա")
#define  ROLE_NAME_SEND_DATA     _T("���ݴ������Ա")
#define  ROLE_NAME_SEND_MEDIA    _T("��ý�崫�����Ա")
#define  ROLE_NAME_SEND_END      _T("�ն��豸��Ϣ����Ա")

#define MODULE_NAME_ALL        _T("ȫ��ģ��")

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
��������COutLogIn* GetInstance()

  ������������õ��������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: COutLogIn*     ���������֤���������ָ��
		
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
��������BOOL LogIn(CString strSoftName , CString &strUser)

  ����������������½�Ի�����֤�û��Ƿ���Ȩ�޵�½
  
  ������CString strSoftName ������ơ����ݷ���/���ݽ��ա� 
        CString &strUser    �û�������û���������֤ʧ�ܣ���Ϊ��
	
  ȫ�ֱ�����m_strUserName
	  
  ����ֵ: BOOL     TRUE--�û������֤�ɹ�  FALSE--�û������֤ʧ��
		
****************************************************************/
BOOL COutLogIn::LogIn(CString strSoftName , CString &strUser)
{
	//��Դ����
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CDlgLogIn dlgLogin;
	dlgLogin.SetSoftName(strSoftName);
    dlgLogin.DoModal();

	if(-1 == strSoftName.Find(_T("��")))//û��"��"��
	{
      theApp.m_enmSoftName = ENU_SOFT_SENDER;
	}
	else
	{
	  theApp.m_enmSoftName = ENU_SOFT_RECEIVER;
	}
	
	//��ȡ������û���
	strUser = theApp.m_strUserName;
	m_strLstUserName.AddTail(strUser);
    
	if(strUser.IsEmpty())
    	return FALSE;
	else
		return TRUE;

}

/***************************************************************
��������BOOL ValidateUser(CString strUser, CString strPWD)

  ������������֤������û����������Ƿ���Ȩ�޵�½
  
  ������CString strUser �û��� 
        CString strPWDr �û�������
	
  ȫ�ֱ�����
	  
  ����ֵ: BOOL     TRUE--�û���������֤�ɹ�  FALSE--�û���������֤ʧ��
		
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
		strMsg.Format(_T("%s��¼�ɹ�\r\n"), strUser);
		COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
        
	}
	else 
	{
		CString strMsg;
		strMsg.Format(_T("%s��¼ʧ��\r\n"), strUser);
		COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		bRet = FALSE;
   }

	dbAccess.CloseRecordSet();
	dbAccess.CloseDataBase();
	
	return bRet;
}

/***************************************************************
��������BOOL ValidatePermissions(CString strUser, CString strModule)

  ������������֤�û��Ƿ���Ȩ�޲����������ģ��
  
  ������CString strUser �û��� 
        CString strModule ģ����
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL     TRUE--��֤�ɹ�  FALSE--��֤ʧ��
		
****************************************************************/
BOOL COutLogIn::ValidatePermissions(CString strUser, CString strModule)
{
	 if(NULL == m_strLstUserName.Find(strUser))
		return FALSE;

	 CDbAccess dbAccess;
	 dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);
	 
	 CString strSql;
	 strSql.Format(_T("select * from DXCS_GX_YHQX where YHBH=(select YHBH from DXCS_ZB_YH where YHMC='%s') and JSBH in (select JSBH from DXCS_ZD_JS where MKMC in('ȫ��ģ��','%s'))"), 
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
�������� void UserManagement(CString strUser)

  ���������������û��������û�ά�����棬ֻ�й���Ա����ά���û���
  
  ������CString strUser �û��� 

  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
 void COutLogIn::UserManagement(CString strUser)
 {
	 if(NULL == m_strLstUserName.Find(strUser))
		return ;

	 BOOL bRet = ValidatePermissions(strUser, MODULE_NAME_ALL);

	 //��Դ����
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

     CDlgUserManager dlgUserManager;
	 dlgUserManager.DoModal();
 }
 /***************************************************************
��������BOOL VerifySoftAuthority()

  ������������֤�û��Ƿ���Ȩ��ʹ�ô����
  
  ������CString strUserName �û�����
        CString strSoftName �������
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL     TRUE-�û���Ȩ��ʹ�ô����   FALSE-�û�û��Ȩ��ʹ�ô����
		           ֻҪ�û�����ʹ�ô��������һģ���Ȩ�ޣ�����ʹ�ô����
****************************************************************/
BOOL COutLogIn::VerifySoftAuthority(CString strUserName, CString strSoftName)
{
	

	 CDbAccess dbAccess;
	 dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);

	  //��ѯ���ݿ⣬����Ȩ����֤
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

	  //��ɫ��֤���д��־
	  if(bRet)
	  {
		CString strMsg;
		strMsg.Format(_T("%s��ɫ��֤\r\n"), strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	  }
	  else
	  {
		 CString strMsg;
		 strMsg.Format(_T("%s��ɫ��֤\r\n"), strUserName);
	     COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
	  }
     
	  return bRet;
}