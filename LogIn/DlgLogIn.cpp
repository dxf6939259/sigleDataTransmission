/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgLogIn.cpp
* 摘    要：登录对话框类
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

*/

#include "stdafx.h"
#include "DlgLogIn.h"
#include "LogIn.h"
#include "OutLogIn.h"
#include "OutLogMgr.h"
//#include "DlgPrompt.h"

extern CLogInApp theApp;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgLogIn dialog


CDlgLogIn::CDlgLogIn(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgLogIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLogIn)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strSoftName.Empty();
	theApp.m_strUserName.Empty();
	m_nLogCount = 0;
	m_pFontSoftName = NULL;
	//14-01-16 add lgp from yn begin
	m_msgDlg = NULL;
	m_bFlag = FALSE;
	//14-01-16 add lgp from yn end
}


void CDlgLogIn::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLogIn)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLogIn, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgLogIn)
	ON_BN_CLICKED(ID_BUT_LOGIN, OnButLogin)
	ON_BN_CLICKED(IDC_BUT_CANCEL, OnButCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogIn message handlers
BOOL CDlgLogIn::OnInitDialog() 
{

	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//设置对话框的BCG界面风格
	EnableVisualManagerStyle();
    
	//连接数据库
	m_dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);


	//设置软件名称的字体
	m_pFontSoftName = new CFont;
	m_pFontSoftName->CreateFont(32,   //字体高度.>0:字体的高度值;=0:字体采用缺省直.<0:此值的绝对值为高度
		8,      //字体宽度
		0,      //文本行的倾斜度
		0,      //字符基线的倾斜度
		FW_BOLD,    //字体的粗细FW_DONTCARE FW_THIN FW_EXTRALIGHT
		FALSE,     //字体是否为斜体
		FALSE,     //字体是否带下划线
		0,      //字体是否带删除线
		ANSI_CHARSET,   //字体的字符集ANSI_CHARSET  DEFAULT_CHARSET SYMBOL_CHARSET
		OUT_DEFAULT_PRECIS,  //字符的输出精度
		CLIP_DEFAULT_PRECIS, //字符裁剪的精度
		DEFAULT_QUALITY,  //字符的输出质量
		DEFAULT_PITCH|FF_SWISS, //字符间距和字体族(低位说明间距,高位说明字符族)
		_T("宋体"));   //字体名称
	
	GetDlgItem(IDC_STCS_SOFT)->SetFont(m_pFontSoftName);



	//根据传入的软件名称m_strSoftName，初始化登录框显示的软件名称
	((CBCGPStatic*)GetDlgItem(IDC_STCS_SOFT))->SetWindowText(m_strSoftName);

	((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->LimitText(50);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->LimitText(50);

    m_bPrompt = FALSE;
    //14-01-16 add lgp from yn begin
	GetDlgItem(ID_BUT_LOGIN)->SetFocus();
	//14-01-16 add lgp from yn end
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CDlgLogIn::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//释放数据库访问资源
    m_dbAccess.CloseRecordSet();
	m_dbAccess.CloseDataBase();
    if(NULL != m_pFontSoftName)
	{
		delete m_pFontSoftName;
	    m_pFontSoftName = NULL;
	}
	return CBCGPDialog::DestroyWindow();
}
void CDlgLogIn::OnButCancel() 
{
	// TODO: Add your control notification handler code here
	//释放数据库访问资源
	m_dbAccess.CloseRecordSet();
	m_dbAccess.CloseDataBase();
	theApp.m_strUserName.Empty();
	OnCancel();
}


/***************************************************************
函数名：void SetSoftName(CString strSoftName)

  功能描述：设置软件名称
  
  参数：CString strSoftName  根据传入的软件名称，初始化软件界面
	
  全局变量：无
	  
  返回值: 无
****************************************************************/
void CDlgLogIn::SetSoftName(CString strSoftName)
{
     m_strSoftName = strSoftName;
}

/***************************************************************
函数名：void OnButLogin()

  功能描述：登录按钮响应函数
  
  参数：无
	
  全局变量：m_strUserName
	  
  返回值: 无
****************************************************************/
void CDlgLogIn::OnButLogin() 
{
	// TODO: Add your control notification handler code here
	
	m_nLogCount++;
	//验证用户登录次数
	if(m_nLogCount>3)
	{
	    //14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("对不起，你输入的用户名密码次数超过3次，不能再次登录。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
        if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("对不起，你输入的用户名密码次数超过3次，不能再次登录。"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("对不起，你输入的用户名密码次数超过3次，不能再次登录。"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("对不起，你输入的用户名密码次数超过3次，不能再次登录。"));
		dlgPrompt.DoModal();*/
		//记日志
		CString strMsg;
		strMsg.Format(_T("用户%s登录次数超过3次，被拒绝登录"),theApp.m_strUserName);
		COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		theApp.m_strUserName.Empty();
		OnCancel();
		
		return;
	}

	//获取输入的用户名和密码
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->GetWindowText(theApp.m_strUserName);

    


	//验证用户名参数是否合法
	if(theApp.m_strUserName.GetLength()>20 || theApp.m_strUserName.GetLength()<4)
	{
    	//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("对不起，你输入的用户名不合法，请输入4-20位的字母,数字或汉字。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
       if(m_msgDlg==NULL){

			m_msgDlg = new CMyMessageBox(this,_T("对不起，你输入的用户名不合法，请输入4-20位的字母,数字或汉字。"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("对不起，你输入的用户名不合法，请输入4-20位的字母,数字或汉字。"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("对不起，你输入的用户名不合法，请输入4-20位的字母,数字或汉字。"));
		dlgPrompt.DoModal();*/
		return;
	}

	//获取用户输入的密码
	CString strPassword;
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->GetWindowText(strPassword);
   //验证密码参数是否合法
	if(strPassword.GetLength()>20 || strPassword.GetLength()<4)
	{
		//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
       if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"));
		dlgPrompt.DoModal();*/
		//记日志
		CString strMsg;
		strMsg.Format(_T("用户%s输入的密码不合法"),theApp.m_strUserName);
	    COutLogMgr::GetInstance()->LogWriter(General, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}
	for(int i=0; i < strPassword.GetLength(); ++i)
	{
		if(strPassword[i] >= 0XA0)
		{
		    //14-01-16 modify lgp from yn begin
			//BCGPMessageBox(NULL, _T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
          if(m_msgDlg==NULL){
				m_msgDlg = new CMyMessageBox(this,_T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"));
				m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
				m_msgDlg->ShowWindow(SW_SHOW);
			}else{
				m_msgDlg->SetInfo(_T("对不起，你输入的密码不合法，请输入4-20位的字母或数字。"));
				m_msgDlg->ShowWindow(SW_SHOW);
			}
				//14-01-16 modify lgp from yn end
			//记日志
			CString strMsg;
			strMsg.Format(_T("用户%s输入的密码不合法"),theApp.m_strUserName);
	        COutLogMgr::GetInstance()->LogWriter(General, strMsg.GetBuffer(strMsg.GetLength()));
			return;
		}
	}

	

	//验证用户名密码
   BOOL bRet = COutLogIn::GetInstance()->ValidateUser(theApp.m_strUserName, strPassword);//VerifyUserAuthority();
	if(!bRet)
	{
		theApp.m_strUserName.Empty();
		//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("对不起，你输入的用户名和密码有误，不能登录。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
	    if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("对不起，你输入的用户名和密码有误，不能登录。"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("对不起，你输入的用户名和密码有误，不能登录。"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("对不起，你输入的用户名和密码有误，不能登录。"));
		dlgPrompt.DoModal();*/
		//记日志
		CString strMsg;
		strMsg.Format(_T("用户%s身份验证失败"),theApp.m_strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	//验证用户角色权限
	 bRet = COutLogIn::GetInstance()->VerifySoftAuthority(theApp.m_strUserName, m_strSoftName);
	if(!bRet)
	{
		theApp.m_strUserName.Empty();
		//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("对不起，你没有权限使用此软件，请联系管理员添加权限。"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
	    if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("对不起，你输入的用户名和密码有误，不能登录。"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("对不起，你输入的用户名和密码有误，不能登录。"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		//记日志
		CString strMsg;
		strMsg.Format(_T("用户%s身份验证工程后，没有权限使用%s软件，被拒绝登录"),theApp.m_strUserName,m_strSoftName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	m_nLogCount=0;
	OnCancel();
	return;
}

BOOL CDlgLogIn::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->wParam == VK_RETURN )
	{
		/*CWnd* pWnd=GetTopWindow();//FindWindow("","提示框");
		CString strWindowText;
		pWnd->GetWindowText(strWindowText);
		if(this != pWnd)	
		{
			::PostMessage(pWnd->m_hWnd,WM_CLOSE,0,0);//强迫退出	
			return TRUE;
		}
		else
		{*/
		//if(m_bPrompt)
		//	return TRUE;

            //14-01-16 modify lgp from yn begin
			//OnButLogin();
			if(!m_bFlag)
			{
			   OnButLogin();
			}
			else
			{
			   if(m_msgDlg!=NULL)
			   {
				  m_msgDlg->ShowWindow(SW_HIDE);
				  m_bFlag = FALSE;
			   }
			}
            //14-01-16 modify lgp from yn end
			return TRUE;
		//}


         
	}
	
	
	return CBCGPDialog::PreTranslateMessage(pMsg);
}