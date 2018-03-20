/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgLogIn.cpp
* ժ    Ҫ����¼�Ի�����
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

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
	
	//���öԻ����BCG������
	EnableVisualManagerStyle();
    
	//�������ݿ�
	m_dbAccess.ConnectDB(DB_FILE_NAME_OFFICE_ACCESS_2003);


	//����������Ƶ�����
	m_pFontSoftName = new CFont;
	m_pFontSoftName->CreateFont(32,   //����߶�.>0:����ĸ߶�ֵ;=0:�������ȱʡֱ.<0:��ֵ�ľ���ֵΪ�߶�
		8,      //������
		0,      //�ı��е���б��
		0,      //�ַ����ߵ���б��
		FW_BOLD,    //����Ĵ�ϸFW_DONTCARE FW_THIN FW_EXTRALIGHT
		FALSE,     //�����Ƿ�Ϊб��
		FALSE,     //�����Ƿ���»���
		0,      //�����Ƿ��ɾ����
		ANSI_CHARSET,   //������ַ���ANSI_CHARSET  DEFAULT_CHARSET SYMBOL_CHARSET
		OUT_DEFAULT_PRECIS,  //�ַ����������
		CLIP_DEFAULT_PRECIS, //�ַ��ü��ľ���
		DEFAULT_QUALITY,  //�ַ����������
		DEFAULT_PITCH|FF_SWISS, //�ַ�����������(��λ˵�����,��λ˵���ַ���)
		_T("����"));   //��������
	
	GetDlgItem(IDC_STCS_SOFT)->SetFont(m_pFontSoftName);



	//���ݴ�����������m_strSoftName����ʼ����¼����ʾ���������
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
	//�ͷ����ݿ������Դ
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
	//�ͷ����ݿ������Դ
	m_dbAccess.CloseRecordSet();
	m_dbAccess.CloseDataBase();
	theApp.m_strUserName.Empty();
	OnCancel();
}


/***************************************************************
��������void SetSoftName(CString strSoftName)

  ���������������������
  
  ������CString strSoftName  ���ݴ����������ƣ���ʼ���������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
****************************************************************/
void CDlgLogIn::SetSoftName(CString strSoftName)
{
     m_strSoftName = strSoftName;
}

/***************************************************************
��������void OnButLogin()

  ������������¼��ť��Ӧ����
  
  ��������
	
  ȫ�ֱ�����m_strUserName
	  
  ����ֵ: ��
****************************************************************/
void CDlgLogIn::OnButLogin() 
{
	// TODO: Add your control notification handler code here
	
	m_nLogCount++;
	//��֤�û���¼����
	if(m_nLogCount>3)
	{
	    //14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("�Բ�����������û��������������3�Σ������ٴε�¼��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
        if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("�Բ�����������û��������������3�Σ������ٴε�¼��"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("�Բ�����������û��������������3�Σ������ٴε�¼��"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("�Բ�����������û��������������3�Σ������ٴε�¼��"));
		dlgPrompt.DoModal();*/
		//����־
		CString strMsg;
		strMsg.Format(_T("�û�%s��¼��������3�Σ����ܾ���¼"),theApp.m_strUserName);
		COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		theApp.m_strUserName.Empty();
		OnCancel();
		
		return;
	}

	//��ȡ������û���������
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_USER))->GetWindowText(theApp.m_strUserName);

    


	//��֤�û��������Ƿ�Ϸ�
	if(theApp.m_strUserName.GetLength()>20 || theApp.m_strUserName.GetLength()<4)
	{
    	//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("�Բ�����������û������Ϸ���������4-20λ����ĸ,���ֻ��֡�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
       if(m_msgDlg==NULL){

			m_msgDlg = new CMyMessageBox(this,_T("�Բ�����������û������Ϸ���������4-20λ����ĸ,���ֻ��֡�"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("�Բ�����������û������Ϸ���������4-20λ����ĸ,���ֻ��֡�"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("�Բ�����������û������Ϸ���������4-20λ����ĸ,���ֻ��֡�"));
		dlgPrompt.DoModal();*/
		return;
	}

	//��ȡ�û����������
	CString strPassword;
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_PASSWORD))->GetWindowText(strPassword);
   //��֤��������Ƿ�Ϸ�
	if(strPassword.GetLength()>20 || strPassword.GetLength()<4)
	{
		//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
       if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"));
		dlgPrompt.DoModal();*/
		//����־
		CString strMsg;
		strMsg.Format(_T("�û�%s��������벻�Ϸ�"),theApp.m_strUserName);
	    COutLogMgr::GetInstance()->LogWriter(General, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}
	for(int i=0; i < strPassword.GetLength(); ++i)
	{
		if(strPassword[i] >= 0XA0)
		{
		    //14-01-16 modify lgp from yn begin
			//BCGPMessageBox(NULL, _T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
          if(m_msgDlg==NULL){
				m_msgDlg = new CMyMessageBox(this,_T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"));
				m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
				m_msgDlg->ShowWindow(SW_SHOW);
			}else{
				m_msgDlg->SetInfo(_T("�Բ�������������벻�Ϸ���������4-20λ����ĸ�����֡�"));
				m_msgDlg->ShowWindow(SW_SHOW);
			}
				//14-01-16 modify lgp from yn end
			//����־
			CString strMsg;
			strMsg.Format(_T("�û�%s��������벻�Ϸ�"),theApp.m_strUserName);
	        COutLogMgr::GetInstance()->LogWriter(General, strMsg.GetBuffer(strMsg.GetLength()));
			return;
		}
	}

	

	//��֤�û�������
   BOOL bRet = COutLogIn::GetInstance()->ValidateUser(theApp.m_strUserName, strPassword);//VerifyUserAuthority();
	if(!bRet)
	{
		theApp.m_strUserName.Empty();
		//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("�Բ�����������û������������󣬲��ܵ�¼��"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
	    if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("�Բ�����������û������������󣬲��ܵ�¼��"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("�Բ�����������û������������󣬲��ܵ�¼��"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		/*m_bPrompt = TRUE;
		CDlgPrompt dlgPrompt;
		dlgPrompt.SetPrompt(_T("�Բ�����������û������������󣬲��ܵ�¼��"));
		dlgPrompt.DoModal();*/
		//����־
		CString strMsg;
		strMsg.Format(_T("�û�%s�����֤ʧ��"),theApp.m_strUserName);
	    COutLogMgr::GetInstance()->LogWriter(Failure, strMsg.GetBuffer(strMsg.GetLength()));
		return;
	}

	//��֤�û���ɫȨ��
	 bRet = COutLogIn::GetInstance()->VerifySoftAuthority(theApp.m_strUserName, m_strSoftName);
	if(!bRet)
	{
		theApp.m_strUserName.Empty();
		//14-01-16 modify lgp from yn begin
		//BCGPMessageBox(NULL, _T("�Բ�����û��Ȩ��ʹ�ô����������ϵ����Ա���Ȩ�ޡ�"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
	    if(m_msgDlg==NULL){
			m_msgDlg = new CMyMessageBox(this,_T("�Բ�����������û������������󣬲��ܵ�¼��"));
			m_msgDlg->Create(IDD_DIALOG_MESSAGE,this);
			m_msgDlg->ShowWindow(SW_SHOW);
		}else{
			m_msgDlg->SetInfo(_T("�Բ�����������û������������󣬲��ܵ�¼��"));
			m_msgDlg->ShowWindow(SW_SHOW);
		}
		//14-01-16 modify lgp from yn end
		//����־
		CString strMsg;
		strMsg.Format(_T("�û�%s�����֤���̺�û��Ȩ��ʹ��%s��������ܾ���¼"),theApp.m_strUserName,m_strSoftName);
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
		/*CWnd* pWnd=GetTopWindow();//FindWindow("","��ʾ��");
		CString strWindowText;
		pWnd->GetWindowText(strWindowText);
		if(this != pWnd)	
		{
			::PostMessage(pWnd->m_hWnd,WM_CLOSE,0,0);//ǿ���˳�	
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