// Demo_ReceiveEnd_NewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo_ReceiveEnd_New.h"
#include "Demo_ReceiveEnd_NewDlg.h"

//接收到的数据或流媒体的数据包长度
#define  LEN_DATA_SEND 1301

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MSG_SOFT_REG         WM_USER+1004
#define MSG_RECEIVE_FILE     WM_USER+1005
#define MSG_RECEIVE_DATA     WM_USER+1006
#define MSG_RECEIVE_MEDIA    WM_USER+1007
#define MSG_SOFT_UN_REG      WM_USER+1008


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
// CDemo_ReceiveEnd_NewDlg dialog

CDemo_ReceiveEnd_NewDlg::CDemo_ReceiveEnd_NewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemo_ReceiveEnd_NewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemo_ReceiveEnd_NewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo_ReceiveEnd_NewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemo_ReceiveEnd_NewDlg)
	DDX_Control(pDX, IDC_LIST_MEDIA, m_lstMedia);
	DDX_Control(pDX, IDC_LIST_DATA, m_lstData);
	DDX_Control(pDX, IDC_LIST_FILE, m_lstFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemo_ReceiveEnd_NewDlg, CDialog)
	//{{AFX_MSG_MAP(CDemo_ReceiveEnd_NewDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_REG_FILE, OnButRegFile)
	ON_BN_CLICKED(IDC_BTN_UN_REG_FILE, OnBtnUnRegFile)
	ON_BN_CLICKED(IDC_BTN_CLEAR_FILE, OnBtnClearFile)
	ON_BN_CLICKED(IDC_BUT_REG_DATA, OnButRegData)
	ON_BN_CLICKED(IDC_BTN_UN_REG_DATA, OnBtnUnRegData)
	ON_BN_CLICKED(IDC_BTN_CLEAR_DATA, OnBtnClearData)
	ON_BN_CLICKED(IDC_BUT_REG_MEDIA, OnButRegMedia)
	ON_BN_CLICKED(IDC_BTN_UN_REG_MEDIA, OnBtnUnRegMedia)
	ON_BN_CLICKED(IDC_BTN_CLEAR_MEDIA, OnBtnClearMedia)
	ON_MESSAGE(MSG_RECEIVE_FILE, OnRecvFile)
	ON_MESSAGE(MSG_RECEIVE_DATA, OnRecvData)
	ON_MESSAGE(MSG_RECEIVE_MEDIA, OnRecvMedia)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo_ReceiveEnd_NewDlg message handlers

BOOL CDemo_ReceiveEnd_NewDlg::OnInitDialog()
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

	//设置日志列表控件可整行选中的属性
	m_lstFile.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstMedia.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//初始化日志列表
	CRect rectFile;
	m_lstFile.GetWindowRect(&rectFile);
	m_lstFile.InsertColumn(0, _T("运行日志"), LVCFMT_CENTER, rectFile.Width()-4);
	
	CRect rectData;
	m_lstData.GetWindowRect(&rectData);
	m_lstData.InsertColumn(0, _T("运行日志"), LVCFMT_CENTER, rectData.Width()-4);
	
	CRect rectMedia;
	m_lstMedia.GetWindowRect(&rectMedia);
	m_lstMedia.InsertColumn(0, _T("运行日志"), LVCFMT_CENTER, rectMedia.Width()-4);

	m_nCountDataRecv = 0;
	m_dTestTimeBegin = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemo_ReceiveEnd_NewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDemo_ReceiveEnd_NewDlg::OnPaint() 
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
HCURSOR CDemo_ReceiveEnd_NewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//文件接收服务注册
void CDemo_ReceiveEnd_NewDlg::OnButRegFile() 
{
	// TODO: Add your control notification handler code here
	StSoftInfo softInfo;
	//本demo程序的窗口名称
	strcpy(softInfo.m_chSoftName,_T("接收端第三方调用模拟程序"));
	
	softInfo.m_nRecType = 1;//注册文件
	if(Reg_UnReg(TRUE, softInfo))
	    m_lstFile.InsertItem(0, _T("文件注册成功"));
}
//文件接收服务注销
void CDemo_ReceiveEnd_NewDlg::OnBtnUnRegFile() 
{
	// TODO: Add your control notification handler code here
	StSoftInfo softInfo;
	strcpy(softInfo.m_chSoftName,_T("接收端第三方调用模拟程序"));
	
	softInfo.m_nRecType = 1;
	if(Reg_UnReg(FALSE, softInfo))
	    m_lstFile.InsertItem(0, _T("文件注销成功"));
}
//文件接收记录清空
void CDemo_ReceiveEnd_NewDlg::OnBtnClearFile() 
{
	// TODO: Add your control notification handler code here
	m_lstFile.DeleteAllItems();
}
//数据接收服务注册
void CDemo_ReceiveEnd_NewDlg::OnButRegData() 
{
	// TODO: Add your control notification handler code here
	StSoftInfo softInfo;
	strcpy(softInfo.m_chSoftName,_T("接收端第三方调用模拟程序"));
	
	softInfo.m_nRecType = 2;
	if(Reg_UnReg(TRUE, softInfo))	
	    m_lstData.InsertItem(0, _T("数据注册成功"));
}
//数据接收服务注销
void CDemo_ReceiveEnd_NewDlg::OnBtnUnRegData() 
{
	// TODO: Add your control notification handler code here
	StSoftInfo softInfo;
	strcpy(softInfo.m_chSoftName,_T("接收端第三方调用模拟程序"));
	
	softInfo.m_nRecType = 2;
	if(Reg_UnReg(FALSE, softInfo))
	   m_lstData.InsertItem(0, _T("数据注销成功"));
}
//数据接收记录清空
void CDemo_ReceiveEnd_NewDlg::OnBtnClearData() 
{
	// TODO: Add your control notification handler code here
	m_nCountDataRecv = 0;
	m_lstData.DeleteAllItems();
	((CEdit*)GetDlgItem(IDC_EDIT_RATE_DATA))->SetWindowText(_T(""));
}
//流媒体接收服务注册
void CDemo_ReceiveEnd_NewDlg::OnButRegMedia() 
{
	// TODO: Add your control notification handler code here
	StSoftInfo softInfo;
	strcpy(softInfo.m_chSoftName,_T("接收端第三方调用模拟程序"));
	
	softInfo.m_nRecType = 3;
	if(Reg_UnReg(TRUE, softInfo))
	    m_lstMedia.InsertItem(0, _T("流媒体注册成功"));
}
//流媒体接收服务注销
void CDemo_ReceiveEnd_NewDlg::OnBtnUnRegMedia() 
{
	// TODO: Add your control notification handler code here
	StSoftInfo softInfo;
	strcpy(softInfo.m_chSoftName,_T("接收端第三方调用模拟程序"));
	
	softInfo.m_nRecType = 3;
	if(Reg_UnReg(FALSE, softInfo))
	   m_lstMedia.InsertItem(0, _T("流媒体注销成功"));
}
//流媒体接收记录清空
void CDemo_ReceiveEnd_NewDlg::OnBtnClearMedia() 
{
	// TODO: Add your control notification handler code here
	m_lstMedia.DeleteAllItems();
}
/***************************************************************
  函数名：BOOL Reg_UnReg()
  功能描述：向接收端软件发起注册或注销
  参数：BOOL bReg             TRUE/注册 FALSE/注销
        StSoftInfo softInfo   要注册的数据信息(包括要注册类型和要注册的软件窗口名称)
  全局变量：无
  返回值: BOOL                TRUE/成功 FALSE/失败
****************************************************************/
BOOL CDemo_ReceiveEnd_NewDlg::Reg_UnReg(BOOL bReg, StSoftInfo softInfo)
{
	//查找单向数据接收软件的窗口句柄
	CWnd* pWnd=FindWindow(NULL,_T("单向数据接收服务"));
    if(NULL == pWnd)
	{
		  MessageBox(_T("数据接收软件未启动"));
		  return FALSE;
	}
	HANDLE hSockSrvRecMap;//内存映射句柄
	LPBYTE lpData;//内存映射数据指针
	
	//创建内存映射句柄   名称为SockSrvDataMap，
	//确保接收端软件使用此名字打开内存映射句柄
	hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
	if (hSockSrvRecMap != NULL)
	{
		lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap,
			FILE_MAP_WRITE, 0, 0, 0);
		if (lpData == NULL)
		{
			CloseHandle(hSockSrvRecMap);
			hSockSrvRecMap = NULL;
			return 0;
		}
	}
	UnmapViewOfFile(hSockSrvRecMap);
	memcpy(lpData,&softInfo,sizeof(softInfo));

	if(bReg)
	    pWnd->SendMessage(MSG_SOFT_REG,0,  0);
	else
		pWnd->SendMessage(MSG_SOFT_UN_REG,0,  0);
    
	return TRUE;
}
/***************************************************************
  函数名：HRESULT CDemo_ReceiveEnd_NewDlg::OnRecvFile(WPARAM wpa, LPARAM lpa)
  功能描述：接收从接收端软件发来的文件路径
  参数：WPARAM wpa             文件路径的字符串长度
        LPARAM lpa             暂无定义
  全局变量：无
  返回值: HRESULT              暂无定义
****************************************************************/
HRESULT CDemo_ReceiveEnd_NewDlg::OnRecvFile(WPARAM wpa, LPARAM lpa)
{
	int nFilePathLen = (int)wpa;
	//打开数据接收软件创建的内存映射对象
	HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ,
		FALSE, "SockSrvDataMap");
	if (m_hReceiveMap == NULL)
	{
		MessageBox(_T("内存映射句柄为空"));
		return 0 ;
	}
	LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,
		FILE_MAP_READ,0,0,0);
	if (m_lpbReceiveBuf == NULL)
	{
		CloseHandle(m_hReceiveMap);
		m_hReceiveMap=NULL;
		return 0;
	}
	char chPathD[LEN_DATA_SEND];
	memset(chPathD, 0, LEN_DATA_SEND);
	//读取内存映射中的数据
	MoveMemory(chPathD, m_lpbReceiveBuf, nFilePathLen);
	//显示文件路径
	m_lstFile.InsertItem(0,chPathD);
	
	return 0;
}
/***************************************************************
  函数名：HRESULT CDemo_ReceiveEnd_NewDlg::OnRecvData(WPARAM wpa, LPARAM lpa)
  功能描述：接收从接收端软件发来的数据(短报文)
  参数：WPARAM wpa             从接收端软件发来的数据(短报文)的长度
        LPARAM lpa             暂无定义
  全局变量：无
  返回值: HRESULT              暂无定义
****************************************************************/
HRESULT CDemo_ReceiveEnd_NewDlg::OnRecvData(WPARAM wpa, LPARAM lpa)
{
	//性能测试
	if(0 == m_dTestTimeBegin)
		m_dTestTimeBegin = GetTickCount();
	if( (GetTickCount()-m_dTestTimeBegin)>=1000 && m_dTestTimeBegin>0 )
	{
		CString strCount;
		strCount.Format(_T("1秒钟接收了 %d 帧"),m_nCountDataRecv);
		CString strXnTest;
		strXnTest.Format(_T("%d"),m_nCountDataRecv);	 
		((CEdit*)GetDlgItem(IDC_EDIT_RATE_DATA))->SetWindowText(strXnTest);
		m_nCountDataRecv = 0;
		m_dTestTimeBegin = 0;
	}
	m_nCountDataRecv++;
	
	int nDataLen = (int)wpa;
	//打开数据接收软件创建的内存映射对象
	HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, "SockSrvDataMap");
	if (m_hReceiveMap == NULL)
		return 0 ;
	LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,FILE_MAP_READ,0,0,0);
	if (m_lpbReceiveBuf == NULL)
	{
		CloseHandle(m_hReceiveMap);
		m_hReceiveMap=NULL;
		return 0;
	}
	//modify 14-01-16 lgp from yn begin
	/*CString strMsg;
	strMsg.Format(_T("%d"),nDataLen);
	
	char chData[LEN_DATA_SEND];
	memset(chData, 0, LEN_DATA_SEND);
	
	CString strData;
	int index=0;
	//读取内存映射中的数据
	for (; index<nDataLen; index++)
	{
		CString strTemp;
		chData[index] =  *(m_lpbReceiveBuf++);
		strData += strTemp;
	}
	chData[index] = 0;
	strData.Format(_T("%s"),chData);
	m_lstData.InsertItem(0,strData);
	*/

	char* chData = NULL;
	chData = new char[nDataLen+1];
	memset(chData,0,nDataLen+1);
	if(NULL == chData)
	{
		MessageBox(_T("申请内存失败"));
		return -1;
	}
	memcpy(chData,m_lpbReceiveBuf,nDataLen);
	CString strBuf;
	strBuf.Format("%s",chData);
	m_lstData.InsertItem(0,strBuf);
	
	delete[] chData;
    chData = NULL;
	//modify 14-01-16 lgp from yn end
	
    return 0;
}
/***************************************************************
  函数名：HRESULT CDemo_ReceiveEnd_NewDlg::OnRecvMedia(WPARAM wpa, LPARAM lpa)
  功能描述：接收从接收端软件发来的数据(短报文)
  参数：WPARAM wpa             从接收端软件发来的流媒体的长度
        LPARAM lpa             暂无定义
  全局变量：无
  返回值: HRESULT              暂无定义
****************************************************************/
HRESULT CDemo_ReceiveEnd_NewDlg::OnRecvMedia(WPARAM wpa, LPARAM lpa)
{

	int nMediaLen = (int)wpa;
	//打开数据接收软件创建的内存映射对象
	HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, "SockSrvDataMap");
	
	if (m_hReceiveMap == NULL)
		return 0 ;
	LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,FILE_MAP_READ,0,0,0);
	if (m_lpbReceiveBuf == NULL)
	{
		CloseHandle(m_hReceiveMap);
		m_hReceiveMap=NULL;
		return 0;
	}
    //modify 14-01-16 lgp from yn begin
	/*CString strMsg;
	strMsg.Format(_T("%d"),nMediaLen);
	
	char chMedia[LEN_DATA_SEND];
	memset(chMedia, 0, LEN_DATA_SEND);
	
	CString strData;
	int index=0;
	//读取内存映射中的数据
	for (; index<nMediaLen; index++)
	{
		int a = *(m_lpbReceiveBuf++);
		chMedia[index] = a;
	}
	chMedia[index] = 0;
    strData.Format(_T("%s"),chMedia);
	m_lstMedia.InsertItem(0,strData);*/
	char* chMedia = NULL;
	chMedia = new char[nMediaLen+1];
	memset(chMedia,0,nMediaLen+1);
	if(NULL == chMedia)
	{
		MessageBox(_T("申请内存失败"));
		return -1;
	}
	memcpy(chMedia,m_lpbReceiveBuf,nMediaLen);
	 CString strBuf;
	 strBuf.Format("%s",chMedia);
     m_lstMedia.InsertItem(0,strBuf);

	//modify 14-01-16 lgp from yn end
    delete[] chMedia;
    chMedia = NULL;
	
	return 0;
}
