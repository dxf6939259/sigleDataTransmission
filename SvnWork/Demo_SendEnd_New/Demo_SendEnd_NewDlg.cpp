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

//选择要发送的文件
void CDemo_SendEnd_NewDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg( TRUE , "(*.*)|*.*","*.*" ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,"所有文件(*.*)");
	
	FileDlg.m_ofn.lpstrInitialDir="C:\\Users\\Administrator\\Desktop\\Send_End";
    
	if(FileDlg.DoModal()==IDOK) 
	{ 
		CString strSendFile = FileDlg.GetPathName();
		((CEdit*)GetDlgItem(IDC_EDIT_FILE_SEL))->SetWindowText(strSendFile);
	}
}

//发送文件
void CDemo_SendEnd_NewDlg::OnBtnSendFile() 
{
	// TODO: Add your control notification handler code here
	CString strFile;
	((CEdit*)GetDlgItem(IDC_EDIT_FILE_SEL))->GetWindowText(strFile);
	if(strFile.IsEmpty())
	{
		MessageBox(_T("请输入要发送的文件"));
		return;
	}
	
	if(!PathFileExists(strFile))
	{
		MessageBox(_T("文件不存在，请重新选择"));
		return;
	}
	//查找单向数据发送程序窗体
	//(单向数据发送程序的窗体名称为"单向数据发送服务")
	CWnd* pWnd=FindWindow(NULL,_T("单向数据发送服务"));
	HANDLE hSockSrvSendMap;//内存映射句柄
	LPBYTE lpData;//内存映射数据区
	//创建内存映射句柄
	hSockSrvSendMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
	if (hSockSrvSendMap != NULL)
	{
		lpData  = (LPBYTE)MapViewOfFile(hSockSrvSendMap, 
			FILE_MAP_WRITE, 0, 0, 0);
		if (lpData == NULL)//lpData内存映射数据区域
		{
			CloseHandle(hSockSrvSendMap);
			hSockSrvSendMap = NULL;
			return ;
		}
	}
	UnmapViewOfFile(hSockSrvSendMap);
	//向内存映射数据区写入数据
    MoveMemory(lpData,strFile,strFile.GetLength());
	//向单向数据发送窗体发消息
    pWnd->SendMessage(MSG_SEND_FILE, 0,  strFile.GetLength());
	
}

//清空已发送文件
void CDemo_SendEnd_NewDlg::OnBtnClearFile() 
{
	// TODO: Add your control notification handler code here
		((CEdit*)GetDlgItem(IDC_EDIT_FILE_SEL))->SetWindowText(_T(""));
}

//发送数据
void CDemo_SendEnd_NewDlg::OnBtnSendData() 
{
	// TODO: Add your control notification handler code here
	CString strData;
	((CEdit*)GetDlgItem(IDC_EDIT_DATA))->GetWindowText(strData);
	if(strData.IsEmpty())
	{
		MessageBox(_T("请输入要发送的数据"));
		return;
	}
	//查找单向数据发送窗体
	CWnd* pWnd=FindWindow(NULL,_T("单向数据发送服务"));
	HANDLE hSockSrvRecMap;//内存映射句柄
	LPBYTE lpData;//内存映射数据区
	
	//创建内存映射句柄
	hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
	if (hSockSrvRecMap != NULL)
	{
		lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, 
			FILE_MAP_WRITE, 0, 0, 0);//lpData内存映射数据区域
		if (lpData == NULL)
		{
			CloseHandle(hSockSrvRecMap);
			hSockSrvRecMap = NULL;
			return ;
		}
	}
	//strData为将要发送的数据内容CString形式
	//向内存映射数据区域写数据
	memcpy(lpData, strData, strData.GetLength());
	//向单向数据发送窗体发送消息
    pWnd->SendMessage(MSG_SEND_DATA, 0, strData.GetLength());
}

//清空已发送数据
void CDemo_SendEnd_NewDlg::OnBtnClearData() 
{
	// TODO: Add your control notification handler code here
		((CEdit*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText(_T(""));
}

//发送流媒体
void CDemo_SendEnd_NewDlg::OnBtnSendMedia() 
{
	// TODO: Add your control notification handler code here
	CString strMedia;
	((CEdit*)GetDlgItem(IDC_EDIT_MEDIA))->GetWindowText(strMedia);
	if(strMedia.IsEmpty())
	{
		MessageBox(_T("请输入要发送的数据"));
		return;
	}
	
	//查找单向数据发送窗体
	CWnd* pWnd=FindWindow(NULL,_T("单向数据发送服务"));
	HANDLE hSockSrvRecMap;
	LPBYTE lpData;
	//创建内存映射句柄
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
	//向单向数据发送窗体发送消息
    pWnd->SendMessage(MSG_SEND_MEDIA, 0, strMedia.GetLength());
}

//清空已发送的流媒体数据
void CDemo_SendEnd_NewDlg::OnBtnClearMedia() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_MEDIA))->SetWindowText(_T(""));
}

//开始测试
void CDemo_SendEnd_NewDlg::OnBtnTestBegin() 
{
	// TODO: Add your control notification handler code here
	//查找单向数据发送窗体
	CWnd* pWnd=FindWindow(NULL,_T("单向数据发送服务"));
	int nCount=0;
    DWORD dBegin = GetTickCount();
	for(; (GetTickCount()-dBegin)<=1000; nCount++)
	{
		if(0 == nCount)
		    pWnd->SendMessage(MSG_TEST_XN, LEN_DATA_SEND, 1);//参数1表示开始测试
		else
            pWnd->SendMessage(MSG_TEST_XN, LEN_DATA_SEND, 0);
	}
	CString strCount;
	strCount.Format(_T("%d"),nCount);
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_RATE))->SetWindowText(strCount);
}

//停止测试
void CDemo_SendEnd_NewDlg::OnBtnTestStop() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_RATE))->SetWindowText(_T(""));
}
