// Demo_SendEnd_NewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo_SendEnd_New.h"
#include "Demo_SendEnd_NewDlg.h"
#include "Shlwapi.h"
#include <string.h>


#define  LEN_DATA_SEND 1301 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CDemo_SendEnd_NewApp theApp;
#define MSG_SEND_FILE  WM_USER+1001
#define MSG_SEND_DATA  WM_USER+1002
#define MSG_SEND_MEDIA WM_USER+1003
#define MSG_TEST_XN    WM_USER+1004

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo_SendEnd_NewDlg dialog

CDemo_SendEnd_NewDlg::CDemo_SendEnd_NewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemo_SendEnd_NewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemo_SendEnd_NewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo_SendEnd_NewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemo_SendEnd_NewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemo_SendEnd_NewDlg, CDialog)
	//{{AFX_MSG_MAP(CDemo_SendEnd_NewDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BTN_SEND_FILE, OnBtnSendFile)
	ON_BN_CLICKED(IDC_BTN_CLEAR_FILE, OnBtnClearFile)
	ON_BN_CLICKED(IDC_BTN_SEND_DATA, OnBtnSendData)
	ON_BN_CLICKED(IDC_BTN_CLEAR_DATA, OnBtnClearData)
	ON_BN_CLICKED(IDC_BTN_SEND_MEDIA, OnBtnSendMedia)
	ON_BN_CLICKED(IDC_BTN_CLEAR_MEDIA, OnBtnClearMedia)
	ON_BN_CLICKED(IDC_BTN_TEST_BEGIN, OnBtnTestBegin)
	ON_BN_CLICKED(IDC_BTN_TEST_STOP, OnBtnTestStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo_SendEnd_NewDlg message handlers

BOOL CDemo_SendEnd_NewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemo_SendEnd_NewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemo_SendEnd_NewDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemo_SendEnd_NewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//ѡ��Ҫ���͵��ļ�
void CDemo_SendEnd_NewDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg( TRUE , "(*.*)|*.*","*.*" ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,"�����ļ�(*.*)");
	
	FileDlg.m_ofn.lpstrInitialDir="C:\\Users\\Administrator\\Desktop\\Send_End";
    
	if(FileDlg.DoModal()==IDOK) 
	{ 
		CString strSendFile = FileDlg.GetPathName();
		((CEdit*)GetDlgItem(IDC_EDIT_FILE_SEL))->SetWindowText(strSendFile);
	}
}

//�����ļ�
void CDemo_SendEnd_NewDlg::OnBtnSendFile() 
{
	// TODO: Add your control notification handler code here
	CString strFile;
	((CEdit*)GetDlgItem(IDC_EDIT_FILE_SEL))->GetWindowText(strFile);
	if(strFile.IsEmpty())
	{
		MessageBox(_T("������Ҫ���͵��ļ�"));
		return;
	}
	
	if(!PathFileExists(strFile))
	{
		MessageBox(_T("�ļ������ڣ�������ѡ��"));
		return;
	}
	//���ҵ������ݷ��ͳ�����
	//(�������ݷ��ͳ���Ĵ�������Ϊ"�������ݷ��ͷ���")
	CWnd* pWnd=FindWindow(NULL,_T("�������ݷ��ͷ���"));
	HANDLE hSockSrvSendMap;//�ڴ�ӳ����
	LPBYTE lpData;//�ڴ�ӳ��������
	//�����ڴ�ӳ����
	hSockSrvSendMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
	if (hSockSrvSendMap != NULL)
	{
		lpData  = (LPBYTE)MapViewOfFile(hSockSrvSendMap, 
			FILE_MAP_WRITE, 0, 0, 0);
		if (lpData == NULL)//lpData�ڴ�ӳ����������
		{
			CloseHandle(hSockSrvSendMap);
			hSockSrvSendMap = NULL;
			return ;
		}
	}
	UnmapViewOfFile(hSockSrvSendMap);
	//���ڴ�ӳ��������д������
    MoveMemory(lpData,strFile,strFile.GetLength());
	//�������ݷ��ʹ��巢��Ϣ
    pWnd->SendMessage(MSG_SEND_FILE, 0,  strFile.GetLength());
	
}

//����ѷ����ļ�
void CDemo_SendEnd_NewDlg::OnBtnClearFile() 
{
	// TODO: Add your control notification handler code here
		((CEdit*)GetDlgItem(IDC_EDIT_FILE_SEL))->SetWindowText(_T(""));
}

//��������
void CDemo_SendEnd_NewDlg::OnBtnSendData() 
{
	// TODO: Add your control notification handler code here
	CString strData;
	((CEdit*)GetDlgItem(IDC_EDIT_DATA))->GetWindowText(strData);
	if(strData.IsEmpty())
	{
		MessageBox(_T("������Ҫ���͵�����"));
		return;
	}
	//���ҵ������ݷ��ʹ���
	CWnd* pWnd=FindWindow(NULL,_T("�������ݷ��ͷ���"));
	HANDLE hSockSrvRecMap;//�ڴ�ӳ����
	LPBYTE lpData;//�ڴ�ӳ��������
	
	//�����ڴ�ӳ����
	hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
	if (hSockSrvRecMap != NULL)
	{
		lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, 
			FILE_MAP_WRITE, 0, 0, 0);//lpData�ڴ�ӳ����������
		if (lpData == NULL)
		{
			CloseHandle(hSockSrvRecMap);
			hSockSrvRecMap = NULL;
			return ;
		}
	}
	//strDataΪ��Ҫ���͵���������CString��ʽ
	//���ڴ�ӳ����������д����
	memcpy(lpData, strData, strData.GetLength());
	//�������ݷ��ʹ��巢����Ϣ
    pWnd->SendMessage(MSG_SEND_DATA, 0, strData.GetLength());
}

//����ѷ�������
void CDemo_SendEnd_NewDlg::OnBtnClearData() 
{
	// TODO: Add your control notification handler code here
		((CEdit*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText(_T(""));
}

//������ý��
void CDemo_SendEnd_NewDlg::OnBtnSendMedia() 
{
	// TODO: Add your control notification handler code here
	CString strMedia;
	((CEdit*)GetDlgItem(IDC_EDIT_MEDIA))->GetWindowText(strMedia);
	if(strMedia.IsEmpty())
	{
		MessageBox(_T("������Ҫ���͵�����"));
		return;
	}
	
	//���ҵ������ݷ��ʹ���
	CWnd* pWnd=FindWindow(NULL,_T("�������ݷ��ͷ���"));
	HANDLE hSockSrvRecMap;
	LPBYTE lpData;
	//�����ڴ�ӳ����
	hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
	if (hSockSrvRecMap != NULL)
	{
		lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap,
			FILE_MAP_WRITE, 0, 0, 0);
		if (lpData == NULL)
		{
			CloseHandle(hSockSrvRecMap);
			hSockSrvRecMap = NULL;
			return ;
		}
	}
	UnmapViewOfFile(hSockSrvRecMap);
	memcpy(lpData, strMedia, strMedia.GetLength());
	//�������ݷ��ʹ��巢����Ϣ
    pWnd->SendMessage(MSG_SEND_MEDIA, 0, strMedia.GetLength());
}

//����ѷ��͵���ý������
void CDemo_SendEnd_NewDlg::OnBtnClearMedia() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_MEDIA))->SetWindowText(_T(""));
}

//��ʼ����
void CDemo_SendEnd_NewDlg::OnBtnTestBegin() 
{
	// TODO: Add your control notification handler code here
	//���ҵ������ݷ��ʹ���
	CWnd* pWnd=FindWindow(NULL,_T("�������ݷ��ͷ���"));
	int nCount=0;
    DWORD dBegin = GetTickCount();
	for(; (GetTickCount()-dBegin)<=1000; nCount++)
	{
		if(0 == nCount)
		    pWnd->SendMessage(MSG_TEST_XN, LEN_DATA_SEND, 1);//����1��ʾ��ʼ����
		else
            pWnd->SendMessage(MSG_TEST_XN, LEN_DATA_SEND, 0);
	}
	CString strCount;
	strCount.Format(_T("%d"),nCount);
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_RATE))->SetWindowText(strCount);
}

//ֹͣ����
void CDemo_SendEnd_NewDlg::OnBtnTestStop() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_RATE))->SetWindowText(_T(""));
}
