/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgUserManager.cpp
* ժ    Ҫ���û�ά���Ի�����
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

*/

#include "stdafx.h"
#include "LogIn.h"
#include "DlgLogIn.h"
#include "DlgUserManager.h"
#include "OutLogMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLogInApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgUserManager dialog


CDlgUserManager::CDlgUserManager(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgUserManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUserManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nUserID = 0;
	m_nLstUserSel = 0;
	m_nRoleIDSel = -1;
	m_nUnRoleIDSel = -1;
}


void CDlgUserManager::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUserManager)
	DDX_Control(pDX, IDC_LST_UN_ROLE, m_lstUnRole);
	DDX_Control(pDX, IDC_LST_ROLE, m_lstRole);
	DDX_Control(pDX, IDC_LST_USER, m_lstUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUserManager, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgUserManager)
	ON_BN_CLICKED(IDC_BUT_SAVE, OnSaveUser)
	ON_BN_CLICKED(IDC_BUT_DEL, OnDeleteUser)
	ON_BN_CLICKED(IDC_BUT_NEW, OnNewUser)
	ON_BN_CLICKED(IDC_BUT_ADD, OnAddRole)
	ON_BN_CLICKED(IDC_BUT_REMOVE, OnRemoveRole)
	ON_NOTIFY(NM_CLICK, IDC_LST_UN_ROLE, OnClickUnRole)
	ON_NOTIFY(NM_CLICK, IDC_LST_ROLE, OnClickRole)
	ON_NOTIFY(NM_CLICK, IDC_LST_USER, OnClickUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUserManager message handlers

BOOL CDlgUserManager::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/***************************************************************
��������void Init()

  ������������ʼ���Ի��������Ϣ
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgUserManager::Init()
{
	//���öԻ����BCG���
	//CBCGPVisualManager2003::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2003));
	EnableVisualManagerStyle();

	//�����û��б�ؼ�������ѡ�е�����
	m_lstUser.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); 

	//��ʼ���û��б�ؼ���ͷ��Ϣ
	CRect rectUser;
	m_lstUser.GetWindowRect(&rectUser);
	UINT nWidthUser = rectUser.Width()/6;
	m_lstUser.InsertColumn(0, "�û����", LVCFMT_LEFT, nWidthUser);
    m_lstUser.InsertColumn(1, "�û���", LVCFMT_LEFT, 2*nWidthUser);
	m_lstUser.InsertColumn(2, "��ע", LVCFMT_LEFT, 3*nWidthUser-4);

    //��ʼ���ѵ��ν�ɫ�б�ؼ���ͷ��Ϣ
	//���ý�ɫ�б�ؼ�������ѡ�е�����
	m_lstRole.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); 
    CRect rectRole;
    m_lstRole.GetWindowRect(&rectRole);
	UINT nWidthRole = rectRole.Width();
    m_lstRole.InsertColumn(0, "�ѵ��ν�ɫ", LVCFMT_LEFT, nWidthRole-4);
		
	//��ʼ��δ���ν�ɫ�б�ؼ���ͷ��Ϣ
	//����δ���ν�ɫ�б�ؼ�������ѡ�е�����
	m_lstUnRole.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	CRect rectUnRole;
    m_lstUnRole.GetWindowRect(&rectUnRole);
	UINT nWidthUnRole = rectUnRole.Width();
    m_lstUnRole.InsertColumn(0, "δ���ν�ɫ", LVCFMT_LEFT, nWidthUnRole-4);

	//�������ݿ�
	BOOL bRet = m_dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);
	if(!bRet)
		return;

	//���û���Ϣ��ʾ���û��б�ؼ���
	UINT nIndexItem=0;
	CString strSql;
	if( ENU_SOFT_SENDER == theApp.m_enmSoftName )
	{
        strSql.Format(_T("select * from DXCS_ZB_YH where YHBH in (select YHBH from DXCS_GX_YHQX where JSBH in (select JSBH from DXCS_ZD_JS where JSMC in('����Ա', '�ļ��������Ա','���ݴ������Ա','��ý�崫�����Ա','�ն��豸��Ϣ����Ա')))"));
	}
	else if( ENU_SOFT_RECEIVER == theApp.m_enmSoftName )
	{
        strSql.Format(_T("select * from DXCS_ZB_YH where YHBH in (select YHBH from DXCS_GX_YHQX where JSBH in (select JSBH from DXCS_ZD_JS where JSMC in('����Ա', '�ļ����չ���Ա','���ݽ��չ���Ա','��ý����չ���Ա')))"));
	}

	m_dbAccess.OpenQuery(strSql);
	while(!m_dbAccess.IsEOF())
	{
         int nUserID = m_dbAccess.GetIntValue("YHBH");
		 CString strUserName = m_dbAccess.GetStringValue("YHMC");
		 CString strRemark = m_dbAccess.GetStringValue("BZ");

		 //ת���ַ�����
         strUserName.Replace("''", "'");
		 strRemark.Replace("''", "'");
		 CString strID;
		 strID.Format(_T("%d"),nUserID);
		 m_lstUser.InsertItem(nIndexItem, strID);
		 m_lstUser.SetItemText(nIndexItem, 1, strUserName);
		 m_lstUser.SetItemText(nIndexItem, 2, strRemark);
         nIndexItem++;
		 m_dbAccess.MoveNext();
	}

	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->LimitText(20);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->LimitText(20);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->LimitText(20);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->LimitText(200);
	

}
BOOL CDlgUserManager::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//�ͷ����ݿ���Դ
	m_dbAccess.CloseRecordSet();
	m_dbAccess.CloseDataBase();
	
	return CBCGPDialog::DestroyWindow();
}

/***************************************************************
��������void OnSaveUser()

  �������������浱ǰ����
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgUserManager::OnSaveUser()
{
	if(m_nLstUserSel<=0)
	{
		CreateNewUser();
		return;
	}

	if(0 == m_nUserID)
	{
		BCGPMessageBox(NULL, _T("��ѡ��һ���û�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}
    
	//��ȡ�û���Ϣ
	CString strUserName, strPwd, strPwdConfirm, strRemark;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->GetWindowText(strUserName);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->GetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->GetWindowText(strPwdConfirm);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->GetWindowText(strRemark);

	//��֤�Ƿ��޸����û���
	CString strUser;
	strUser = m_lstUser.GetItemText(m_nLstUserSel, 1);
	if(strUser.Compare(strUserName) != 0)
	{
		BCGPMessageBox(NULL, _T("�Բ����޸��û���Ϣ�������޸��û���"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//��֤�û�����
	BOOL bRet = ValidateParam(strUserName, strPwd, strRemark);
    if(FALSE == bRet)
	   return;


	if(strPwd.Compare(strPwdConfirm) !=0)
	{
		BCGPMessageBox(NULL, _T("����������ȷ�����벻һ�£�����"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//�޸����ݿ��е��û���Ϣ
	CString strSql;
	//�����ַ�����
	strUserName.Replace("'", "''");
	strPwd.Replace("'", "''");
	strRemark.Replace("'", "''");
	strSql.Format(_T("update DXCS_ZB_YH set YHMC='%s', YHMM='%s', BZ='%s' where YHBH=%d"), 
		strUserName,strPwd,strRemark, m_nUserID);

	bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("�޸��û���Ϣʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		//����־
		CString strMsg;
		strMsg.Format(_T("�޸�%s�û���Ϣʧ��"),strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	//���û��б�ؼ�����ʾ�Ѹ��µ��û���Ϣ
	strUserName.Replace("''","'");
    strRemark.Replace("''","'");
	m_lstUser.SetItemText(m_nLstUserSel, 1, strUserName);
	m_lstUser.SetItemText(m_nLstUserSel, 2, strRemark);

	//ɾ���û�ԭ�е����н�ɫ
	strSql.Format(_T("delete from DXCS_GX_YHQX where YHBH=%d"), m_nUserID);
    bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("ɾ���û���ɫʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//�����ѵ��ν�ɫ�б��еĽ�ɫ��Ϣ�ֱ�Ϊ��ǰ�û�������ӦȨ��
    //����Ƿ��й���ԱȨ��
	BOOL bAdmin = FALSE;
	for(int i=0; i<m_lstRole.GetItemCount(); i++)
	{
		CString strRoleName = m_lstRole.GetItemText(i,0);
		if(_T("����Ա") == strRoleName)
		{
			bAdmin = TRUE;
			break;
		}
	}
	if(bAdmin)
	{
		strSql.Format(_T("select JSBH from DXCS_ZD_JS where JSMC='����Ա'"));
		m_dbAccess.OpenQuery(strSql);
		int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
		
		strSql.Format(_T("insert into DXCS_GX_YHQX(YHBH, JSBH) values(%d, %d)"),
			m_nUserID, nRoleID);
		
		bRet = m_dbAccess.Execute(strSql);
		if(FALSE == bRet)
		{
			BCGPMessageBox(NULL, _T("����û���ɫʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	else
	{
		
		
		UINT nRoleCount = m_lstRole.GetItemCount();
		for(int i=0; i<nRoleCount; i++)
		{
			CString strRoleName = m_lstRole.GetItemText(i,0);
			strSql.Format(_T("select JSBH from DXCS_ZD_JS where JSMC='%s'"), strRoleName);
			m_dbAccess.OpenQuery(strSql);
			int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
			
			strSql.Format(_T("insert into DXCS_GX_YHQX(YHBH, JSBH) values(%d, %d)"),
				m_nUserID, nRoleID);
			
			bRet = m_dbAccess.Execute(strSql);
			if(FALSE == bRet)
			{
				BCGPMessageBox(NULL, _T("����û���ɫʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
				return;
			}
		}
	}
	//����־
	CString strMsg;
	strMsg.Format(_T("�޸��û���Ϣ%s"), strUserName);
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	BCGPMessageBox(NULL, _T("����ɹ�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
}

/***************************************************************
��������void OnDeleteUser()

  ����������ɾ����ǰ�û�
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgUserManager::OnDeleteUser()
{
	if(0 == m_nUserID)
	{
		BCGPMessageBox(NULL, _T("��ѡ��һ���û�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	CString strUserName=m_lstUser.GetItemText(m_nLstUserSel, 1);
	CString strSql;

    //�ж��Ƿ��ǹ���Ա
    CString strUserID = m_lstUser.GetItemText(m_nLstUserSel, 0);
	int nUserID = atoi(strUserID.GetBuffer(strUserID.GetLength()));
    strSql.Format(_T("select * from DXCS_GX_YHQX where YHBH=%d and JSBH=(select JSBH from DXCS_ZD_JS where JSMC='����Ա')"),nUserID);
    BOOL bRet = m_dbAccess.OpenQuery(strSql);
	int nID = m_dbAccess.GetIntValue(_T("YHBH"));
	if( DB_GET_FIELD_VALUE_ERROR != nID)
	{
		BCGPMessageBox(NULL, _T("�Բ��𣬲���ɾ������Ա�û�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		//����־
		CString strMsg;
		strMsg.Format(_T("ɾ������Ա�û�%sʧ�ܣ�����ɾ������Ա�û�"),strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	//ɾ��ǰ�����û�ȷ��
	CString strMsg;
	strMsg.Format(_T("ȷʵҪɾ�� %s �û���?"),strUserName);
	if( IDYES != BCGPMessageBox(NULL, strMsg, _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		return;

	//ɾ����ǰ�û������н�ɫ
	
	strSql.Format(_T("delete from DXCS_GX_YHQX where YHBH=%d"), m_nUserID);
     bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("ɾ���û���ɫʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//ɾ����ǰ�û�
	strSql.Format(_T("delete from DXCS_ZB_YH  where YHBH=%d"), m_nUserID);
	
	bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("ɾ���û�ʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		//����־
		CString strMsg;
		strMsg.Format(_T("ɾ���û�%sʧ��"),strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}
	m_lstUser.DeleteItem(m_nLstUserSel);
	m_nUserID = 0;
	m_nLstUserSel = 0;

	//��ս����ϵ��û���Ϣ
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->SetWindowText(_T(""));

	//��ս�ɫ�б�ؼ���Ϣ
	m_lstRole.DeleteAllItems();
	m_lstUnRole.DeleteAllItems();

	//����־
	strMsg.Format(_T("ɾ���û�%s"),strUserName);
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
}

/***************************************************************
��������void OnNewUser()

  �����������������û�
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgUserManager::OnNewUser()
{
	//����û���Ϣ�ͽ�ɫ��Ϣ
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->SetWindowText(_T(""));
	m_lstRole.DeleteAllItems();
	m_lstUnRole.DeleteAllItems();

	//modify lgp 12-03 begin
	/*CString strSqlRole;
	if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
	{
		strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('����Ա', '�ļ��������Ա','���ݴ������Ա','��ý�崫�����Ա','�ն��豸��Ϣ����Ա')"));
	}
	else if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
	{
		strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('����Ա', '�ļ����չ���Ա','���ݽ��չ���Ա','��ý����չ���Ա')"));
	}
	
	m_dbAccess.OpenQuery(strSqlRole);
	CList<UINT,UINT> lstRoleID_User;
	CString strRoleName;
	//��ʾ�û��Ѿ�ӵ�еĽ�ɫ
	int nIndex = 0;
	while(!m_dbAccess.IsEOF())
	{
		strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
		if(strRoleName.Compare(_T("����Ա")) != 0)
		{
			m_lstUnRole.InsertItem(nIndex, strRoleName);
			nIndex++;
		}
		m_dbAccess.MoveNext();
	}*/
	if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
	{
	    m_lstUnRole.InsertItem(0, _T("����Ա"));
		m_lstUnRole.InsertItem(1, _T("�ļ��������Ա"));
		m_lstUnRole.InsertItem(2, _T("���ݴ������Ա"));
		m_lstUnRole.InsertItem(3, _T("��ý�崫�����Ա"));
		m_lstUnRole.InsertItem(4, _T("�ն��豸��Ϣ����Ա"));
	}
	else if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
	{
		m_lstUnRole.InsertItem(0, _T("����Ա"));
		m_lstUnRole.InsertItem(1, _T("�ļ����չ���Ա"));
		m_lstUnRole.InsertItem(2, _T("���ݽ��չ���Ա"));
		m_lstUnRole.InsertItem(3, _T("��ý����չ���Ա"));
	}
    //modify lgp 12-03 end
	m_nLstUserSel = -1;
}
void CDlgUserManager::CreateNewUser()
{
	
	
	//��ȡ�û���Ϣ
	CString strUserName, strPwd, strPwdConfirm, strRemark;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->GetWindowText(strUserName);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->GetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->GetWindowText(strPwdConfirm);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->GetWindowText(strRemark);
	
	//��֤�û�����
	BOOL bRet = ValidateParam(strUserName, strPwd, strRemark);
    if(FALSE == bRet)
		return;
	
	if(strPwd.Compare(strPwdConfirm) !=0)
	{
		BCGPMessageBox(NULL, _T("����������ȷ�����벻һ�£�����"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	//�ж��û����Ƿ��Ѿ�����
	for(int i=0; i<m_lstUser.GetItemCount(); i++)
	{
		CString strName=m_lstUser.GetItemText(i, 1);
		if(0 == strName.Compare(strUserName))
		{
			BCGPMessageBox(NULL, _T("���û����ѱ�ռ�ã���ѡ�����û���"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	
	//�����ݿ����������û�
	CString strSql;
	if(strRemark.IsEmpty())
	{
	   strSql.Format(_T("insert into DXCS_ZB_YH(YHMC,YHMM) values('%s','%s')"), 
		  strUserName,strPwd);
	}
	else
	{
		strSql.Format(_T("insert into DXCS_ZB_YH(YHMC,YHMM,BZ) values('%s','%s','%s')"), 
		  strUserName,strPwd,strRemark);
	}

	bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("�������û�ʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		//����־
		CString strMsg;
		strMsg.Format(_T("�����û�%sʧ��"),strUserName);
		COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}
    
    //��õ�ǰ�����û���ID
	strSql.Format(_T("select * from DXCS_ZB_YH where YHMC='%s' and YHMM='%s'"), strUserName, strPwd);
	bRet = m_dbAccess.OpenQuery(strSql);
	CString strID;
	int nUserID = m_dbAccess.GetIntValue("YHBH");
	if(DB_GET_FIELD_VALUE_ERROR == nUserID)
		return;
	
	strID.Format(_T("%d"), nUserID);
    
	
	//���û��б�ؼ�����ʾ����ӵ��û���Ϣ
	UINT nUserCount = m_lstUser.GetItemCount();
	m_lstUser.InsertItem(nUserCount, strID);
	m_lstUser.SetItemText(nUserCount, 1, strUserName);
	m_lstUser.SetItemText(nUserCount, 2, strRemark);
	m_lstUser.SetItemText(nUserCount, 3, strPwd);
	
	m_nUserID = nUserID;
	m_nLstUserSel = nUserCount;
	
	//Ϊ���û�����Ȩ��
	for( i=0; i<m_lstRole.GetItemCount(); i++)
	{
		CString strRoleName = m_lstRole.GetItemText(i,0);
        strSql.Format(_T("select JSBH from DXCS_ZD_JS where JSMC='%s'"), strRoleName);
		m_dbAccess.OpenQuery(strSql);
		int nRoleID = m_dbAccess.GetIntValue("JSBH");
		if(DB_GET_FIELD_VALUE_ERROR == nRoleID)
			continue;
		
		strSql.Format(_T("insert into DXCS_GX_YHQX(YHBH, JSBH) values(%d,%d)"), m_nUserID, nRoleID);
		bRet = m_dbAccess.Execute(strSql);
		if(FALSE == bRet)
		{
			BCGPMessageBox(NULL, _T("����û���ɫʧ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	
	//����־
	CString strMsg;
	strMsg.Format(_T("����û�%s"),strUserName);
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	
	
}
/***************************************************************
��������void OnAddRole()

  �������������ӽ�ɫ
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgUserManager::OnAddRole() 
{
	// TODO: Add your control notification handler code here
	if(m_nUnRoleIDSel<0)
		return;
    
	CString strUnRoleName = m_lstUnRole.GetItemText(m_nUnRoleIDSel,0);
	
	UINT nRoleCount = m_lstRole.GetItemCount();
	m_lstRole.InsertItem(nRoleCount, strUnRoleName);
	m_lstUnRole.DeleteItem(m_nUnRoleIDSel);

	m_nUnRoleIDSel=-1;
}

/***************************************************************
��������void OnRemoveRole()

  ����������ɾ����ɫ
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgUserManager::OnRemoveRole() 
{
	// TODO: Add your control notification handler code here
    if(m_nRoleIDSel<0)
		return;
    
	CString strRoleName = m_lstRole.GetItemText(m_nRoleIDSel,0);
    if(strRoleName.Compare(_T("����Ա")) ==0 )
	{
		BCGPMessageBox(NULL, _T("�Բ��𣬹���ԱȨ�޲���ɾ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	UINT nUnRoleCount = m_lstUnRole.GetItemCount();
	m_lstUnRole.InsertItem(nUnRoleCount, strRoleName);
	m_lstRole.DeleteItem(m_nRoleIDSel);

	m_nRoleIDSel = -1;
	
}


void CDlgUserManager::OnClickUnRole(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint Point;
	::GetCursorPos(&Point);//��ȡ���λ��
	::ScreenToClient(this->m_lstUnRole.m_hWnd,&Point);

	LVHITTESTINFO hitinfo = {0};
	hitinfo.pt = Point;
	if(m_lstUnRole.SubItemHitTest(&hitinfo)<0)
		return;

	if(hitinfo.iItem<0 || hitinfo.iItem>m_lstUnRole.GetItemCount())
	{
		return;
	}
	//��¼��ǰѡ���б��IDֵ
	m_nUnRoleIDSel = hitinfo.iItem;
	
}

void CDlgUserManager::OnClickRole(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint Point;
	::GetCursorPos(&Point);//��ȡ���λ��
	::ScreenToClient(this->m_lstRole.m_hWnd,&Point);
	
	LVHITTESTINFO hitinfo = {0};
	hitinfo.pt = Point;
	if(m_lstRole.SubItemHitTest(&hitinfo)<0)
		return;
	
	if(hitinfo.iItem<0 || hitinfo.iItem>m_lstRole.GetItemCount())
		return;
	//��¼��ǰѡ���б��IDֵ
	m_nRoleIDSel = hitinfo.iItem;

}
/***************************************************************
��������void ValidateParam()

  ������������֤�û�������������Ƿ�Ϸ�
  
  ������CString strUser �û���
        CString strPwd  ����
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
BOOL CDlgUserManager::ValidateParam(CString strUserName, CString strPwd, CString strRemark)
{
	
	//��֤�û��������Ƿ�Ϸ�
	if(strUserName.GetLength()>20 || strUserName.GetLength()<4)
	{
		BCGPMessageBox(NULL, _T("�Բ�����������û������Ϸ���������4-20λ����ĸ,���ֻ��֡�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	
	//��֤��������Ƿ�Ϸ�
	if(strPwd.GetLength()>20 || strPwd.GetLength()<4)
	{
		BCGPMessageBox(NULL, _T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	for(int i=0; i < strPwd.GetLength(); ++i)
	{
		if(strPwd[i] >= 0XA0)
		{
			BCGPMessageBox(NULL, _T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}

	//�жϱ�ע�����Ƿ񳬹�200
	if(strRemark.GetLength()>200)
	{
		    BCGPMessageBox(NULL, _T("�Բ��������뱸ע�ĳ���Ӧ��200���ַ�֮�ڣ����������롣"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
			return FALSE;
	}
	//���뵣������һ����ɫ
	if(m_lstRole.GetItemCount()<1)
	{
		BCGPMessageBox(NULL, _T("�Բ����û����뵣������һ����ɫ��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	return TRUE;
}
void CDlgUserManager::OnClickUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString msg;
	NMITEMACTIVATE * m_plv=(NMITEMACTIVATE*)pNMHDR;
	long mouse_x,mouse_y;
	
	mouse_x=m_plv->ptAction.x;
	mouse_y=m_plv->ptAction.y;
	//�ж��ǵڼ���
	RECT m_rect;
	long itemrow,itemcol,index;
	for(index=m_lstUser.GetTopIndex( );index<m_lstUser.GetItemCount( );index++)
	{
		m_lstUser.GetItemRect(index,&m_rect,0);
		if((m_rect.top <=mouse_y)&&(m_rect.bottom >=mouse_y))
		{
			itemrow=index;
			break;
		}
		
	}

	//����û���Ϣ�ͽ�ɫ��Ϣ
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->SetWindowText(_T(""));
	m_lstRole.DeleteAllItems();
	m_lstUnRole.DeleteAllItems();

	if(itemrow<0 || itemrow>=m_lstUser.GetItemCount())
	{
		/*CString strSqlRole;
		strSqlRole.Format(_T("select * from DXCS_ZD_JS"));
		
		m_dbAccess.OpenQuery(strSqlRole);
		CList<UINT,UINT> lstRoleID_User;
		CString strRoleName;
		//��ʾ�û��Ѿ�ӵ�еĽ�ɫ
		int nIndex = 0;
		while(!m_dbAccess.IsEOF())
		{
			strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
			if(strRoleName.Compare(_T("����Ա")) != 0)
			{
               m_lstUnRole.InsertItem(nIndex, strRoleName);
			   nIndex++;
			}
			m_dbAccess.MoveNext();
		}*/
		return;
	}

    //��ȡ��ǰ��ѡ�е��û���Ϣ
	CString strID = m_lstUser.GetItemText(itemrow,0);
    CString strUserName = m_lstUser.GetItemText(itemrow,1);
	CString strRemark = m_lstUser.GetItemText(itemrow,2);

	//���������û���ϢΪδѡ��״̬
	for(int i=0; i<m_lstUser.GetItemCount(); i++)
	{
		m_lstUser.SetItemState(i,0,LVIS_SELECTED|LVIS_FOCUSED);
	}
	m_lstUser.SetItemState(itemrow,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	//��ѯ�û�����
	CString strPwd, strSqlUser;
	strSqlUser.Format(_T("select * from DXCS_ZB_YH where YHBH =%s"),
		strID);
    m_dbAccess.OpenQuery(strSqlUser);
	strPwd = m_dbAccess.GetStringValue(_T("YHMM"));

	//��ʾ��ǰ��ѡ�е��û���Ϣ
	strPwd.Replace("''","'");
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->SetWindowText(strUserName);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->SetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->SetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->SetWindowText(strRemark);
	
	UINT nUserID = atoi(strID);
	m_nUserID = nUserID;
	m_nLstUserSel = itemrow;
	
	//��ѯ�û���ɫ
	int nItemRole =0;
    CString strSqlRole;
    strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSBH in (select JSBH from DXCS_GX_YHQX where YHBH=%d)"),
		nUserID);
	
    m_dbAccess.OpenQuery(strSqlRole);
	CList<UINT,UINT> lstRoleID_User;
	CString strRoleName;
	//��ʾ�û��Ѿ�ӵ�еĽ�ɫ
	while(!m_dbAccess.IsEOF())
	{
		strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
		if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
		{
            if( (strRoleName == _T("�ļ����չ���Ա")) || (strRoleName == _T("���ݽ��չ���Ա")) || 
				(strRoleName == _T("��ý����չ���Ա")) )
				continue;

		}
		else if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
		{
			if( (strRoleName == _T("�ļ��������Ա")) || (strRoleName == _T("���ݴ������Ա")) || 
				(strRoleName == _T("��ý�崫�����Ա")) || (strRoleName == _T("�ն��豸��Ϣ����Ա")))
				continue;
		}

		int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
		lstRoleID_User.AddTail(nRoleID);
        m_lstRole.InsertItem(nItemRole++, strRoleName);
		
        m_dbAccess.MoveNext();
	}
	
	if(strRoleName.Compare(_T("����Ա"))==0)
		return;
	
	int nItemUnRole =0;
	if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
	{
       strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('����Ա', '�ļ����չ���Ա','���ݽ��չ���Ա','��ý����չ���Ա')"));
	}
	else if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
	{
       strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('����Ա', '�ļ��������Ա','���ݴ������Ա','��ý�崫�����Ա','�ն��豸��Ϣ����Ա')"));
	}
    m_dbAccess.OpenQuery(strSqlRole);

	//��ʾ�û��в�ӵ�еĽ�ɫ
	while(!m_dbAccess.IsEOF())
	{
		CString strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
		//delete lgp 12-03 begin
		/*if(strRoleName.Compare(_T("����Ա"))==0)
		{
			m_dbAccess.MoveNext();
			continue;
		}*/
		//delete lgp 12-03 end
		int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
		//modify lgp 12-03 begin
		//if( (NULL != lstRoleID_User.Find(nRoleID)) && (strRoleName.Compare(_T("����Ա"))!=0))
		if( (NULL != lstRoleID_User.Find(nRoleID)) /*&& (strRoleName.Compare(_T("����Ա"))!=0)*/)
		//modify lgp 12-03 end
		{
			m_dbAccess.MoveNext();
			continue;
		}
		//modify lgp 12-03 begin
		//else if( (NULL != lstRoleID_User.Find(nRoleID)) && (strRoleName.Compare(_T("����Ա"))==0))
		else if( (NULL != lstRoleID_User.Find(nRoleID)) /*&& (strRoleName.Compare(_T("����Ա"))==0)*/)
		//modify lgp 12-03 end
		{
			return;
		}
		
        m_lstUnRole.InsertItem(nItemUnRole++, strRoleName);
		
        m_dbAccess.MoveNext();
	}

}

BOOL CDlgUserManager::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->wParam == VK_RETURN )
	{
		return TRUE;
	}
	
	
	return CBCGPDialog::PreTranslateMessage(pMsg);
}