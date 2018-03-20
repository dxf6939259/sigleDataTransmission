/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgUserManager.cpp
* 摘    要：用户维护对话框类
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

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
函数名：void Init()

  功能描述：初始化对话框界面信息
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
****************************************************************/
void CDlgUserManager::Init()
{
	//设置对话框的BCG风格
	//CBCGPVisualManager2003::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2003));
	EnableVisualManagerStyle();

	//设置用户列表控件可整行选中的属性
	m_lstUser.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); 

	//初始化用户列表控件表头信息
	CRect rectUser;
	m_lstUser.GetWindowRect(&rectUser);
	UINT nWidthUser = rectUser.Width()/6;
	m_lstUser.InsertColumn(0, "用户编号", LVCFMT_LEFT, nWidthUser);
    m_lstUser.InsertColumn(1, "用户名", LVCFMT_LEFT, 2*nWidthUser);
	m_lstUser.InsertColumn(2, "备注", LVCFMT_LEFT, 3*nWidthUser-4);

    //初始化已担任角色列表控件表头信息
	//设置角色列表控件可整行选中的属性
	m_lstRole.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); 
    CRect rectRole;
    m_lstRole.GetWindowRect(&rectRole);
	UINT nWidthRole = rectRole.Width();
    m_lstRole.InsertColumn(0, "已担任角色", LVCFMT_LEFT, nWidthRole-4);
		
	//初始化未担任角色列表控件表头信息
	//设置未担任角色列表控件可整行选中的属性
	m_lstUnRole.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	CRect rectUnRole;
    m_lstUnRole.GetWindowRect(&rectUnRole);
	UINT nWidthUnRole = rectUnRole.Width();
    m_lstUnRole.InsertColumn(0, "未担任角色", LVCFMT_LEFT, nWidthUnRole-4);

	//连接数据库
	BOOL bRet = m_dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);
	if(!bRet)
		return;

	//将用户信息显示在用户列表控件中
	UINT nIndexItem=0;
	CString strSql;
	if( ENU_SOFT_SENDER == theApp.m_enmSoftName )
	{
        strSql.Format(_T("select * from DXCS_ZB_YH where YHBH in (select YHBH from DXCS_GX_YHQX where JSBH in (select JSBH from DXCS_ZD_JS where JSMC in('管理员', '文件传输管理员','数据传输管理员','流媒体传输管理员','终端设备信息管理员')))"));
	}
	else if( ENU_SOFT_RECEIVER == theApp.m_enmSoftName )
	{
        strSql.Format(_T("select * from DXCS_ZB_YH where YHBH in (select YHBH from DXCS_GX_YHQX where JSBH in (select JSBH from DXCS_ZD_JS where JSMC in('管理员', '文件接收管理员','数据接收管理员','流媒体接收管理员')))"));
	}

	m_dbAccess.OpenQuery(strSql);
	while(!m_dbAccess.IsEOF())
	{
         int nUserID = m_dbAccess.GetIntValue("YHBH");
		 CString strUserName = m_dbAccess.GetStringValue("YHMC");
		 CString strRemark = m_dbAccess.GetStringValue("BZ");

		 //转义字符处理
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
	//释放数据库资源
	m_dbAccess.CloseRecordSet();
	m_dbAccess.CloseDataBase();
	
	return CBCGPDialog::DestroyWindow();
}

/***************************************************************
函数名：void OnSaveUser()

  功能描述：保存当前操作
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
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
		BCGPMessageBox(NULL, _T("请选择一个用户"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}
    
	//获取用户信息
	CString strUserName, strPwd, strPwdConfirm, strRemark;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->GetWindowText(strUserName);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->GetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->GetWindowText(strPwdConfirm);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->GetWindowText(strRemark);

	//验证是否修改了用户名
	CString strUser;
	strUser = m_lstUser.GetItemText(m_nLstUserSel, 1);
	if(strUser.Compare(strUserName) != 0)
	{
		BCGPMessageBox(NULL, _T("对不起，修改用户信息，不能修改用户名"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//验证用户参数
	BOOL bRet = ValidateParam(strUserName, strPwd, strRemark);
    if(FALSE == bRet)
	   return;


	if(strPwd.Compare(strPwdConfirm) !=0)
	{
		BCGPMessageBox(NULL, _T("输入密码与确认密码不一致，请检查"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//修改数据库中的用户信息
	CString strSql;
	//特殊字符处理
	strUserName.Replace("'", "''");
	strPwd.Replace("'", "''");
	strRemark.Replace("'", "''");
	strSql.Format(_T("update DXCS_ZB_YH set YHMC='%s', YHMM='%s', BZ='%s' where YHBH=%d"), 
		strUserName,strPwd,strRemark, m_nUserID);

	bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("修改用户信息失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		//记日志
		CString strMsg;
		strMsg.Format(_T("修改%s用户信息失败"),strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	//在用户列表控件中显示已更新的用户信息
	strUserName.Replace("''","'");
    strRemark.Replace("''","'");
	m_lstUser.SetItemText(m_nLstUserSel, 1, strUserName);
	m_lstUser.SetItemText(m_nLstUserSel, 2, strRemark);

	//删除用户原有的所有角色
	strSql.Format(_T("delete from DXCS_GX_YHQX where YHBH=%d"), m_nUserID);
    bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("删除用户角色失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//根据已担任角色列表中的角色信息分别为当前用户增加响应权限
    //检测是否有管理员权限
	BOOL bAdmin = FALSE;
	for(int i=0; i<m_lstRole.GetItemCount(); i++)
	{
		CString strRoleName = m_lstRole.GetItemText(i,0);
		if(_T("管理员") == strRoleName)
		{
			bAdmin = TRUE;
			break;
		}
	}
	if(bAdmin)
	{
		strSql.Format(_T("select JSBH from DXCS_ZD_JS where JSMC='管理员'"));
		m_dbAccess.OpenQuery(strSql);
		int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
		
		strSql.Format(_T("insert into DXCS_GX_YHQX(YHBH, JSBH) values(%d, %d)"),
			m_nUserID, nRoleID);
		
		bRet = m_dbAccess.Execute(strSql);
		if(FALSE == bRet)
		{
			BCGPMessageBox(NULL, _T("添加用户角色失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
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
				BCGPMessageBox(NULL, _T("添加用户角色失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
				return;
			}
		}
	}
	//记日志
	CString strMsg;
	strMsg.Format(_T("修改用户信息%s"), strUserName);
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	BCGPMessageBox(NULL, _T("保存成功"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
}

/***************************************************************
函数名：void OnDeleteUser()

  功能描述：删除当前用户
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
****************************************************************/
void CDlgUserManager::OnDeleteUser()
{
	if(0 == m_nUserID)
	{
		BCGPMessageBox(NULL, _T("请选择一个用户"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	CString strUserName=m_lstUser.GetItemText(m_nLstUserSel, 1);
	CString strSql;

    //判断是否是管理员
    CString strUserID = m_lstUser.GetItemText(m_nLstUserSel, 0);
	int nUserID = atoi(strUserID.GetBuffer(strUserID.GetLength()));
    strSql.Format(_T("select * from DXCS_GX_YHQX where YHBH=%d and JSBH=(select JSBH from DXCS_ZD_JS where JSMC='管理员')"),nUserID);
    BOOL bRet = m_dbAccess.OpenQuery(strSql);
	int nID = m_dbAccess.GetIntValue(_T("YHBH"));
	if( DB_GET_FIELD_VALUE_ERROR != nID)
	{
		BCGPMessageBox(NULL, _T("对不起，不能删除管理员用户"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		//记日志
		CString strMsg;
		strMsg.Format(_T("删除管理员用户%s失败，不能删除管理员用户"),strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	//删除前请求用户确认
	CString strMsg;
	strMsg.Format(_T("确实要删除 %s 用户吗?"),strUserName);
	if( IDYES != BCGPMessageBox(NULL, strMsg, _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		return;

	//删除当前用户的所有角色
	
	strSql.Format(_T("delete from DXCS_GX_YHQX where YHBH=%d"), m_nUserID);
     bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("删除用户角色失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}

	//删除当前用户
	strSql.Format(_T("delete from DXCS_ZB_YH  where YHBH=%d"), m_nUserID);
	
	bRet = m_dbAccess.Execute(strSql);
	if(FALSE == bRet)
	{
		BCGPMessageBox(NULL, _T("删除用户失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		//记日志
		CString strMsg;
		strMsg.Format(_T("删除用户%s失败"),strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}
	m_lstUser.DeleteItem(m_nLstUserSel);
	m_nUserID = 0;
	m_nLstUserSel = 0;

	//清空界面上的用户信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->SetWindowText(_T(""));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->SetWindowText(_T(""));

	//清空角色列表控件信息
	m_lstRole.DeleteAllItems();
	m_lstUnRole.DeleteAllItems();

	//记日志
	strMsg.Format(_T("删除用户%s"),strUserName);
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
}

/***************************************************************
函数名：void OnNewUser()

  功能描述：增加新用户
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
****************************************************************/
void CDlgUserManager::OnNewUser()
{
	//清空用户信息和角色信息
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
		strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('管理员', '文件传输管理员','数据传输管理员','流媒体传输管理员','终端设备信息管理员')"));
	}
	else if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
	{
		strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('管理员', '文件接收管理员','数据接收管理员','流媒体接收管理员')"));
	}
	
	m_dbAccess.OpenQuery(strSqlRole);
	CList<UINT,UINT> lstRoleID_User;
	CString strRoleName;
	//显示用户已经拥有的角色
	int nIndex = 0;
	while(!m_dbAccess.IsEOF())
	{
		strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
		if(strRoleName.Compare(_T("管理员")) != 0)
		{
			m_lstUnRole.InsertItem(nIndex, strRoleName);
			nIndex++;
		}
		m_dbAccess.MoveNext();
	}*/
	if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
	{
	    m_lstUnRole.InsertItem(0, _T("管理员"));
		m_lstUnRole.InsertItem(1, _T("文件传输管理员"));
		m_lstUnRole.InsertItem(2, _T("数据传输管理员"));
		m_lstUnRole.InsertItem(3, _T("流媒体传输管理员"));
		m_lstUnRole.InsertItem(4, _T("终端设备信息管理员"));
	}
	else if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
	{
		m_lstUnRole.InsertItem(0, _T("管理员"));
		m_lstUnRole.InsertItem(1, _T("文件接收管理员"));
		m_lstUnRole.InsertItem(2, _T("数据接收管理员"));
		m_lstUnRole.InsertItem(3, _T("流媒体接收管理员"));
	}
    //modify lgp 12-03 end
	m_nLstUserSel = -1;
}
void CDlgUserManager::CreateNewUser()
{
	
	
	//获取用户信息
	CString strUserName, strPwd, strPwdConfirm, strRemark;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->GetWindowText(strUserName);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->GetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->GetWindowText(strPwdConfirm);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->GetWindowText(strRemark);
	
	//验证用户参数
	BOOL bRet = ValidateParam(strUserName, strPwd, strRemark);
    if(FALSE == bRet)
		return;
	
	if(strPwd.Compare(strPwdConfirm) !=0)
	{
		BCGPMessageBox(NULL, _T("输入密码与确认密码不一致，请检查"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	//判断用户名是否已经存在
	for(int i=0; i<m_lstUser.GetItemCount(); i++)
	{
		CString strName=m_lstUser.GetItemText(i, 1);
		if(0 == strName.Compare(strUserName))
		{
			BCGPMessageBox(NULL, _T("此用户名已被占用，请选择别的用户名"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	
	//向数据库中增加新用户
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
		BCGPMessageBox(NULL, _T("创建新用户失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		//记日志
		CString strMsg;
		strMsg.Format(_T("创建用户%s失败"),strUserName);
		COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}
    
    //获得当前增加用户的ID
	strSql.Format(_T("select * from DXCS_ZB_YH where YHMC='%s' and YHMM='%s'"), strUserName, strPwd);
	bRet = m_dbAccess.OpenQuery(strSql);
	CString strID;
	int nUserID = m_dbAccess.GetIntValue("YHBH");
	if(DB_GET_FIELD_VALUE_ERROR == nUserID)
		return;
	
	strID.Format(_T("%d"), nUserID);
    
	
	//在用户列表控件中显示新添加的用户信息
	UINT nUserCount = m_lstUser.GetItemCount();
	m_lstUser.InsertItem(nUserCount, strID);
	m_lstUser.SetItemText(nUserCount, 1, strUserName);
	m_lstUser.SetItemText(nUserCount, 2, strRemark);
	m_lstUser.SetItemText(nUserCount, 3, strPwd);
	
	m_nUserID = nUserID;
	m_nLstUserSel = nUserCount;
	
	//为新用户赋予权限
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
			BCGPMessageBox(NULL, _T("添加用户角色失败"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	
	//记日志
	CString strMsg;
	strMsg.Format(_T("添加用户%s"),strUserName);
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	
	
}
/***************************************************************
函数名：void OnAddRole()

  功能描述：增加角色
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
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
函数名：void OnRemoveRole()

  功能描述：删除角色
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
****************************************************************/
void CDlgUserManager::OnRemoveRole() 
{
	// TODO: Add your control notification handler code here
    if(m_nRoleIDSel<0)
		return;
    
	CString strRoleName = m_lstRole.GetItemText(m_nRoleIDSel,0);
    if(strRoleName.Compare(_T("管理员")) ==0 )
	{
		BCGPMessageBox(NULL, _T("对不起，管理员权限不能删除"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
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
	::GetCursorPos(&Point);//获取鼠标位置
	::ScreenToClient(this->m_lstUnRole.m_hWnd,&Point);

	LVHITTESTINFO hitinfo = {0};
	hitinfo.pt = Point;
	if(m_lstUnRole.SubItemHitTest(&hitinfo)<0)
		return;

	if(hitinfo.iItem<0 || hitinfo.iItem>m_lstUnRole.GetItemCount())
	{
		return;
	}
	//记录当前选中列表的ID值
	m_nUnRoleIDSel = hitinfo.iItem;
	
}

void CDlgUserManager::OnClickRole(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint Point;
	::GetCursorPos(&Point);//获取鼠标位置
	::ScreenToClient(this->m_lstRole.m_hWnd,&Point);
	
	LVHITTESTINFO hitinfo = {0};
	hitinfo.pt = Point;
	if(m_lstRole.SubItemHitTest(&hitinfo)<0)
		return;
	
	if(hitinfo.iItem<0 || hitinfo.iItem>m_lstRole.GetItemCount())
		return;
	//记录当前选中列表的ID值
	m_nRoleIDSel = hitinfo.iItem;

}
/***************************************************************
函数名：void ValidateParam()

  功能描述：验证用户名和密码参数是否合法
  
  参数：CString strUser 用户名
        CString strPwd  密码
	
  全局变量：无
	  
  返回值: 无
****************************************************************/
BOOL CDlgUserManager::ValidateParam(CString strUserName, CString strPwd, CString strRemark)
{
	
	//验证用户名参数是否合法
	if(strUserName.GetLength()>20 || strUserName.GetLength()<4)
	{
		BCGPMessageBox(NULL, _T("对不起，你输入的用户名不合法，请输入4-20位的字母,数字或汉字。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	
	//验证密码参数是否合法
	if(strPwd.GetLength()>20 || strPwd.GetLength()<4)
	{
		BCGPMessageBox(NULL, _T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	for(int i=0; i < strPwd.GetLength(); ++i)
	{
		if(strPwd[i] >= 0XA0)
		{
			BCGPMessageBox(NULL, _T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}

	//判断备注长度是否超过200
	if(strRemark.GetLength()>200)
	{
		    BCGPMessageBox(NULL, _T("对不起，你输入备注的长度应在200个字符之内，请重新输入。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
			return FALSE;
	}
	//必须担任至少一个角色
	if(m_lstRole.GetItemCount()<1)
	{
		BCGPMessageBox(NULL, _T("对不起，用户必须担任至少一个角色。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
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
	//判断是第几行
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

	//清空用户信息和角色信息
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
		//显示用户已经拥有的角色
		int nIndex = 0;
		while(!m_dbAccess.IsEOF())
		{
			strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
			if(strRoleName.Compare(_T("管理员")) != 0)
			{
               m_lstUnRole.InsertItem(nIndex, strRoleName);
			   nIndex++;
			}
			m_dbAccess.MoveNext();
		}*/
		return;
	}

    //获取当前所选中的用户信息
	CString strID = m_lstUser.GetItemText(itemrow,0);
    CString strUserName = m_lstUser.GetItemText(itemrow,1);
	CString strRemark = m_lstUser.GetItemText(itemrow,2);

	//设置所有用户信息为未选中状态
	for(int i=0; i<m_lstUser.GetItemCount(); i++)
	{
		m_lstUser.SetItemState(i,0,LVIS_SELECTED|LVIS_FOCUSED);
	}
	m_lstUser.SetItemState(itemrow,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	//查询用户密码
	CString strPwd, strSqlUser;
	strSqlUser.Format(_T("select * from DXCS_ZB_YH where YHBH =%s"),
		strID);
    m_dbAccess.OpenQuery(strSqlUser);
	strPwd = m_dbAccess.GetStringValue(_T("YHMM"));

	//显示当前所选中的用户信息
	strPwd.Replace("''","'");
	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->SetWindowText(strUserName);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->SetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_CONFIRM))->SetWindowText(strPwd);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_REMARK))->SetWindowText(strRemark);
	
	UINT nUserID = atoi(strID);
	m_nUserID = nUserID;
	m_nLstUserSel = itemrow;
	
	//查询用户角色
	int nItemRole =0;
    CString strSqlRole;
    strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSBH in (select JSBH from DXCS_GX_YHQX where YHBH=%d)"),
		nUserID);
	
    m_dbAccess.OpenQuery(strSqlRole);
	CList<UINT,UINT> lstRoleID_User;
	CString strRoleName;
	//显示用户已经拥有的角色
	while(!m_dbAccess.IsEOF())
	{
		strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
		if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
		{
            if( (strRoleName == _T("文件接收管理员")) || (strRoleName == _T("数据接收管理员")) || 
				(strRoleName == _T("流媒体接收管理员")) )
				continue;

		}
		else if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
		{
			if( (strRoleName == _T("文件传输管理员")) || (strRoleName == _T("数据传输管理员")) || 
				(strRoleName == _T("流媒体传输管理员")) || (strRoleName == _T("终端设备信息管理员")))
				continue;
		}

		int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
		lstRoleID_User.AddTail(nRoleID);
        m_lstRole.InsertItem(nItemRole++, strRoleName);
		
        m_dbAccess.MoveNext();
	}
	
	if(strRoleName.Compare(_T("管理员"))==0)
		return;
	
	int nItemUnRole =0;
	if(ENU_SOFT_RECEIVER == theApp.m_enmSoftName)
	{
       strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('管理员', '文件接收管理员','数据接收管理员','流媒体接收管理员')"));
	}
	else if(ENU_SOFT_SENDER == theApp.m_enmSoftName)
	{
       strSqlRole.Format(_T("select * from DXCS_ZD_JS where JSMC in('管理员', '文件传输管理员','数据传输管理员','流媒体传输管理员','终端设备信息管理员')"));
	}
    m_dbAccess.OpenQuery(strSqlRole);

	//显示用户尚不拥有的角色
	while(!m_dbAccess.IsEOF())
	{
		CString strRoleName = m_dbAccess.GetStringValue(_T("JSMC"));
		//delete lgp 12-03 begin
		/*if(strRoleName.Compare(_T("管理员"))==0)
		{
			m_dbAccess.MoveNext();
			continue;
		}*/
		//delete lgp 12-03 end
		int nRoleID = m_dbAccess.GetIntValue(_T("JSBH"));
		//modify lgp 12-03 begin
		//if( (NULL != lstRoleID_User.Find(nRoleID)) && (strRoleName.Compare(_T("管理员"))!=0))
		if( (NULL != lstRoleID_User.Find(nRoleID)) /*&& (strRoleName.Compare(_T("管理员"))!=0)*/)
		//modify lgp 12-03 end
		{
			m_dbAccess.MoveNext();
			continue;
		}
		//modify lgp 12-03 begin
		//else if( (NULL != lstRoleID_User.Find(nRoleID)) && (strRoleName.Compare(_T("管理员"))==0))
		else if( (NULL != lstRoleID_User.Find(nRoleID)) /*&& (strRoleName.Compare(_T("管理员"))==0)*/)
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