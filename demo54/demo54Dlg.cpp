// demo54Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo54.h"
#include "demo54Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDemo54App theApp;
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
// CDemo54Dlg dialog

CDemo54Dlg::CDemo54Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemo54Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemo54Dlg)
	m_port = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bStop = FALSE;
	m_bStartSever = FALSE;

	m_port = 60021;

	m_doubleCheck = FALSE;

	m_pBufData = new byte[DATA_BUF_SIZE];
}
CDemo54Dlg::~CDemo54Dlg()
{
	SafeDelete(m_pBufData);
}

void CDemo54Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemo54Dlg)
	DDX_Control(pDX, IDC_LIST_LOG, m_lstLog);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 0, 66535);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemo54Dlg, CDialog)
	//{{AFX_MSG_MAP(CDemo54Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_SERVER, OnStartServer)
	ON_BN_CLICKED(IDC_STARTSERVER, OnStartserver)
	ON_BN_CLICKED(IDC_CLOSESOC, OnClosesoc)
	ON_BN_CLICKED(IDC_BTNCLEAR, OnBtnclear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo54Dlg message handlers

BOOL CDemo54Dlg::OnInitDialog()
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
	CRect rect;
	m_lstLog.GetWindowRect(&rect);
	m_lstLog.InsertColumn(0,_T("日志"),LVCFMT_LEFT, rect.Width()-4);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemo54Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDemo54Dlg::OnPaint() 
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
HCURSOR CDemo54Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//启动双向校验
void CDemo54Dlg::OnStartServer() 
{
	// TODO: Add your control notification handler code here
	//创建UDP接收socket
	static BOOL bRet = FALSE;
	if (bRet)
	{
		return;
	}
	bRet = m_proApply.Create(60011,SOCK_DGRAM,FD_READ);
	if (bRet)
	{
		int nCount = m_lstLog.GetItemCount();
		m_lstLog.InsertItem(nCount,_T("双向校验启动成功!"));

		m_doubleCheck = TRUE;
		 //AfxMessageBox("启动成功");
	}
	else
	{
		int a=GetLastError();
		int nCount = m_lstLog.GetItemCount();
		CString strMsg;
		strMsg.Format(_T("双向校验失败，错误码：%d"),a);
		m_lstLog.InsertItem(nCount,strMsg);
	}

}

//接数线程
UINT RecvDataThread(LPVOID lpvoid)
{
	CDemo54Dlg *pThis = (CDemo54Dlg*)lpvoid;
	
	sockaddr addFrom;
	int fromLen = sizeof(sockaddr);
	char buf[1320]={0};
	int nRecv = 0;
	
	int nPort = 0;
	char arrIp[4]={0};
	
	int checkMark = 0;
	map<int,int>::iterator it;
//	CString 
	byte byBuf[MAX_FRAME_LEN+20]={0};
	while (!pThis->m_bStop)
	{
	//	nRecv = recvfrom(pThis->m_udp_socket,(char*)pTrePack->GetBuffer(),MAX_FRAME_LEN+20,0,&addFrom,&fromLen);
		nRecv = recvfrom(pThis->m_udp_socket,buf,MAX_FRAME_LEN+20,0,&addFrom,&fromLen);

		if (nRecv<=0)
		{
			Sleep(10);
			continue;
		}
		while (!pThis->m_bStop)
		{
			//写指针不等于读指针进行写入消息数据
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//写入缓冲区
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),buf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				continue;
			}
			break;
		}
		//偏移量到达最后时,使其回归为0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_writePos+1;

	}
	
	int nCount = ((CDemo54Dlg*)theApp.GetMainWnd())->m_lstLog.GetItemCount();
	((CDemo54Dlg*)theApp.GetMainWnd())->m_lstLog.InsertItem(nCount,_T("接收线程退出！"));

	InterlockedDecrement(&pThis->m_curThreadCount);
	return 0;
}

UINT ProcDataThread(LPVOID lpvoid)
{
	CDemo54Dlg *pThis = (CDemo54Dlg*)lpvoid;
	BOOL bDCheck   = pThis->m_doubleCheck;
	long tempCount = 0;
	CTreatyPack* pTrePack    = new CTreatyPack;
	ProtocolPacket* pProPack = new ProtocolPacket;

	SOCKADDR_IN sendAddr;
	
	sendAddr.sin_family = AF_INET;   
	sendAddr.sin_port = htons(60005);   
	sendAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	int nPort = 0;
	char arrIp[4]={0};
	
	int checkMark = 0;
	
	int packLen = 0;
	map<int,int>::iterator it;
	while (!pThis->m_bStop)
	{
		tempCount = (pThis->m_readPos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_readPos+1;
		pTrePack->ResetData();
		while (!pThis->m_bStop) //循环缓冲区
		{
			if (tempCount != pThis->m_writePos)
			{
				pThis->m_readPos = tempCount;
				memcpy(pTrePack->GetBuffer(),pThis->m_pBufData+tempCount*(MAX_FRAME_LEN+20),MAX_FRAME_LEN+20);
			}
			else
			{
				Sleep(25);			
				continue;
			}
			break;
		}


		pProPack->Reset();
		*pTrePack>>pProPack;
		nPort = pProPack->desPort;
		checkMark = pProPack->checkMark;
		packLen = pProPack->dataLen+20;
 		if (bDCheck)
 		{
			if (checkMark ==-1)
			{
				continue;
			}
// 			it = pThis->m_proApply.m_mapFlag.find(checkMark);
// 			if (it == pThis->m_proApply.m_mapFlag.end())
// 			{
// 				continue;
// 			}
 		}
		arrIp[0] = pProPack->desAddr[0];
		arrIp[1] = pProPack->desAddr[1];
		arrIp[2] = pProPack->desAddr[2];
		arrIp[3] = pProPack->desAddr[3];
		
		sendAddr.sin_port = htons(nPort);
		sendAddr.sin_addr.S_un.S_un_b.s_b1 = arrIp[0];
		sendAddr.sin_addr.S_un.S_un_b.s_b2 = arrIp[1];
		sendAddr.sin_addr.S_un.S_un_b.s_b3 = arrIp[2];
		sendAddr.sin_addr.S_un.S_un_b.s_b4 = arrIp[3];
		
		sendto(pThis->m_udp_send,(char*)pTrePack->GetBuffer(),packLen,0,(sockaddr*)&sendAddr,sizeof(sockaddr));

	}
	SafeDelete(pProPack);
	SafeDelete(pTrePack);
	InterlockedDecrement(&pThis->m_curThreadCount);
	return 0;
}

//启动服务
void CDemo54Dlg::OnStartserver() 
{
	// TODO: Add your control notification handler code here
	if (m_bStartSever)
	{
		return;
	}
	m_bStop = FALSE;
	WSADATA wsd;
	int ret = WSAStartup(MAKEWORD(2,2),&wsd);
	if (0!=ret)
	{
		int nCount = m_lstLog.GetItemCount();
	    m_lstLog.InsertItem(nCount,_T("WSAStartup失败"));
	}
	m_udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	m_udp_send = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	UpdateData();
	WSADATA wsadata; 
	WORD dwVersionRequested; 
	int err; err=WSAStartup(dwVersionRequested,&wsadata);
	char hostname[128]; 
	if(gethostname(hostname,128)==0)
	{ 
		printf("%s\n",hostname);//计算机名字 
	}
	struct hostent *pHost = gethostbyname(hostname);
    CString strLocalIp = inet_ntoa(*(struct in_addr*)pHost->h_addr_list[0]);
	WSACleanup(); 
	
	//创建UDP接收socket
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;   
	sin.sin_port = htons(m_port);   
	sin.sin_addr.S_un.S_addr = inet_addr(strLocalIp);
	
	if (bind(m_udp_socket,(SOCKADDR*)&sin,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		//AfxMessageBox(_T("接收socket绑定失败!"));
		int nCount = m_lstLog.GetItemCount();
	    m_lstLog.InsertItem(nCount,_T("接收socket绑定失败!"));
		return;
	}
	else
	{
		m_curThreadCount = 0;
		m_readPos = MAX_FRAME_BUF_COUNT-1;
		m_writePos = 0;
		AfxBeginThread(RecvDataThread,this);
		AfxBeginThread(ProcDataThread,this);
		m_curThreadCount = 2;

		int nCount = m_lstLog.GetItemCount();
	    m_lstLog.InsertItem(nCount,_T("服务启动成功"));
		m_bStartSever = TRUE;
	}

	int nBuf = 1320000;
	int nRevOpt= sizeof(nBuf);
	//设置缓冲区大小,最大为(MAX_FRAME_LEN+20)*1000;个字节
	setsockopt(m_udp_socket,SOL_SOCKET,SO_RCVBUF,(char*)&nBuf,nRevOpt);
}

//关闭socket
void CDemo54Dlg::OnClosesoc() 
{
	// TODO: Add your control notification handler code here
	m_bStartSever = FALSE;

	if (0 != m_udp_socket)
	{
		WSACleanup();
		m_bStop = TRUE;

		while (1)
		{
			if (m_curThreadCount == 0)
			{
				break;
			}
			else
			{
				Sleep(10);
				continue;
			}
		}
		shutdown(m_udp_socket,2);
		closesocket(m_udp_socket);
		m_udp_socket = 0;

		shutdown(m_udp_send,2);
		closesocket(m_udp_send);
		m_udp_send = 0;
	}
		
}

//清除日志
void CDemo54Dlg::OnBtnclear() 
{
	// TODO: Add your control notification handler code here
	m_lstLog.DeleteAllItems();
}
