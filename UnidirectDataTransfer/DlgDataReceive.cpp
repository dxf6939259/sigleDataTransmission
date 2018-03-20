/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgDataReceive.cpp
* ժ    Ҫ�����ݽ��ս�����
*   
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

*/

#include "stdafx.h"
#include "UnidirectDataTransfer.h"
#include "DlgDataReceive.h"
#include "TransControlEx.h"
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib") 
#include <shlwapi.h>
#include "OutLogIn.h"

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

#define LEN_DATA             1301

#define MODULE_NAME_FILE_RECV  _T("�ļ�����ģ��")
#define MODULE_NAME_DATA_RECV  _T("���ݽ���ģ��")
#define MODULE_NAME_MEDIA_RECV _T("��ý�����ģ��")
#define MODULE_NAME_ALL        _T("ȫ��ģ��")

/////////////////////////////////////////////////////////////////////////////
// CDlgDataReceive dialog

extern CUnidirectDataTransferApp theApp;
CDlgDataReceive::CDlgDataReceive(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDataReceive::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDataReceive)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTransControlEx_File = NULL;
	m_pTransControlEx_Data = NULL;
	m_pTransControlEx_Media = NULL;
	m_lstSoftReg_File.RemoveAll();
	m_lstSoftReg_Data.RemoveAll();
	m_lstSoftReg_Media.RemoveAll();
    m_strFileRecving.Empty();
}


void CDlgDataReceive::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDataReceive)
	DDX_Control(pDX, IDC_LST_MEDIA, m_lstLogMedia);
	DDX_Control(pDX, IDC_LST_FILE, m_lstLogFile);
	DDX_Control(pDX, IDC_LST_DATA, m_lstLogData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDataReceive, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgDataReceive)
	ON_BN_CLICKED(IDC_BUT_SAVE_PATH, OnFileSavePath)
	ON_BN_CLICKED(IDC_BUT_START_FILE, OnStartFile)
	ON_BN_CLICKED(IDC_BUT_STOP_FILE, OnStopFile)
	ON_BN_CLICKED(IDC_BUT_RESTART_FILE, OnRestartFile)
	ON_BN_CLICKED(IDC_BUT_SAVE_PARAN_FILE, OnSaveParamFile)
	ON_BN_CLICKED(IDC_BUT_START_DATA, OnStartData)
	ON_BN_CLICKED(IDC_BUT_STOP_DATA_RECV, OnStopData)
	ON_BN_CLICKED(IDC_BUT_RESTART_DATA, OnRestartData)
	ON_BN_CLICKED(IDC_BUT_SAVE_PARAN_DATA, OnSaveParamData)
	ON_BN_CLICKED(IDC_BUT_START_MEDIA, OnStartMedia)
	ON_BN_CLICKED(IDC_BUT_STOP_MEDIA, OnStopMedia)
	ON_BN_CLICKED(IDC_BUT_RESTART_MEDIA, OnRestartMedia)
	ON_BN_CLICKED(IDC_BUT_SAVE_PARAN_MEDIA, OnSaveParamMedia)
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_SOFT_REG, OnSoftReg)
	ON_MESSAGE(WM_SEND_SPEED, SpeedSending)
	ON_MESSAGE(MSG_SOFT_UN_REG, OnSoftUnReg)
	ON_MESSAGE(WM_RECV_ANOTHER_FILE, ShowRecvAnotherFile)
	ON_COMMAND(ID_CLEAR_RECV_FILE, OnClearRecvFileLog)
	ON_COMMAND(ID_CLEAR_RECV_DATA, OnClearRecvDataLog)
	ON_COMMAND(ID_CLEAR_RECV_MEDIA, OnClearRecvMediaLog)
	ON_NOTIFY(NM_RCLICK, IDC_LST_DATA, OnRclickLstLogData)
	ON_NOTIFY(NM_RCLICK, IDC_LST_FILE, OnRclickLstLogFile)
	ON_NOTIFY(NM_RCLICK, IDC_LST_MEDIA, OnRclickLstLogMedia)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDataReceive message handlers
/*LRESULT CDlgDataReceive::SpeedSending(WPARAM wpa, LPARAM lpa)
{
	CString strValue;
	strValue.Format(_T("�ѷ���: %d"),wpa);
    GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE)->SetWindowText(strValue);
	//((CEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->Invalidate();
	return 0;
}*/
/***************************************************************
��������void CALLBACK TransCallRecvData(int nTransType, int nMesType,int nLen,unsigned char* buff)

  �����������ص����� ��������
  
  ������int nTransType       ��������
        int nMesType         ���޶���
	    int nLen             ���ݳ���
        unsigned char* buff  �����յ�����
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
int g_nRecvCount=0;
int g_nTimeBegin=0;
BOOL g_bTest = FALSE;
void CALLBACK TransCallRecvData(int nTransType, int nMesType,int nLen,unsigned char* buff)//����
{
	//delete 14-01-16 lgp begin
	/*CDlgDataReceive* pRecvWnd = (CDlgDataReceive*)theApp.GetMainWnd();
	g_nRecvCount++; //1�����յ���֡��

	if(0==g_nTimeBegin)
		g_nTimeBegin = GetTickCount();
	if( (GetTickCount()-g_nTimeBegin)>=1000)
	{
		CString strCount;
		strCount.Format(_T("1���ӽ����� %d ֡"),g_nRecvCount);
        pRecvWnd->m_lstLogData.InsertItem(0,strCount);
		g_nRecvCount = 0;
		g_nTimeBegin = 0;
	    return;
	}
	 //add lgp 11-28 begin
	 for (int i=0; i<nLen-20; i++)
	 {
		 pRecvWnd->m_strData  += (TCHAR)buff[i];
	 }
	 //add lgp 11-28 end
   //delete lgp 11-28 begin
	if(nLen>0 && NULL!=buff )
	{
		pRecvWnd->m_lstLogData.InsertItem(0, pRecvWnd->GetLog(Success, _T("�ɹ����յ�����\r\n")));
		COutLogMgr::GetInstance()->LogWriter(Success, _T("�ɹ����յ�����\r\n"));
	}
	POSITION pos = pRecvWnd->m_lstSoftReg_Data.GetHeadPosition();
	//char pDataD[nLen-20];
	//memset(pDataD,0x00,nLen-20);
	//memcpy(pDataD,buff+19,nLen-20)
	char *pData = new char[nLen-20];
	memset(pData,0x00,nLen-20);
	memcpy(pData,buff+19,nLen-20);

	for(;NULL != pos;)
	{
	
			CString strSoftName = pRecvWnd->m_lstSoftReg_Data.GetNext(pos);
			HWND hWnd=FindWindow(NULL,strSoftName);
			if(NULL == hWnd)
				continue;

			HANDLE hSockSrvRecMap;
			LPBYTE lpData;
			
			char DataBuf[LEN_DATA];
			char chPathD[LEN_DATA];
			memset(chPathD, 0, LEN_DATA);
			
			hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
			if (hSockSrvRecMap != NULL)
			{
				lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, FILE_MAP_WRITE, 0, 0, 0);
				if (lpData == NULL)
				{
					CloseHandle(hSockSrvRecMap);
					hSockSrvRecMap = NULL;
					COutLogMgr::GetInstance()->LogWriter(Failure, _T("�������������������ն˷�������ʱ���ڴ�ӳ������\r\n"));
					pRecvWnd->m_lstLogData.InsertItem(0, pRecvWnd->GetLog(Failure, _T("�������������������ն˷�������ʱ���ڴ�ӳ�����\r\n")));
		            return ;
				}
			}
			
			for (int i=0; i<nLen-20; i++) {
				DataBuf[i] = pData[i];
				chPathD[i] = buff[i];
			}
			memcpy(lpData,DataBuf,sizeof(DataBuf));
			
			//int nLen = strlen(chPathD);
            SendMessage(hWnd, MSG_RECEIVE_DATA, (WPARAM)nLen-20, (LPARAM)DataBuf);
	}
	//delete lgp 11-28 end
	*/
	//delete 14-01-16 lgp end
}

/***************************************************************
  �� �� ����void CALLBACK feedBackRecvData(LONG lType,void* pValue)
  �����������ص����� ��ӳ��ǰ����״��
  ��    ����LONG lType     ��������
        void* pValue   ����ֵ(ָ����ʽ)	
  ȫ�ֱ�������	  
  �� �� ֵ: ��		
****************************************************************/
//add 14-01-16 lgp from yn begin
CString g_strData= "";
int g_DataPos = 0;
//add 14-01-16 lgp from yn end
void CALLBACK feedBackRecvData(LONG lType,void* pValue, void* pNULL)//����״�� ������ ���д��� ��������0 
{
	//add 14-01-16 lgp from yn begin

	USHORT* len = (USHORT*)pNULL;
	int pos = 0;
    //add 14-01-16 lgp from yn end
	CDlgDataReceive* pRecvWnd = (CDlgDataReceive*)theApp.GetMainWnd()->GetSafeHwnd();
	switch(lType)
	{
		//add 14-01-16 lgp from yn begin
	case 31:
	case 32://���ݷ������
		{
			
			CString tmp = (LPCTSTR)(LPTSTR)pValue;
			
			CDlgDataReceive* pRecvWnd = (CDlgDataReceive*)theApp.GetMainWnd();
			pRecvWnd->m_lstLogData.InsertItem(0, pRecvWnd->GetLog(Success, _T("�ɹ����յ�����\r\n")));
			POSITION pos = pRecvWnd->m_lstSoftReg_Data.GetHeadPosition();
			for(;NULL != pos;)
			{
				CString strSoftName = pRecvWnd->m_lstSoftReg_Data.GetNext(pos);
				HWND hWnd=FindWindow(NULL, strSoftName);
				if(NULL == hWnd)
					continue;
				
				HANDLE hSockSrvRecMap;
				LPBYTE lpData;
				
				char DataBuf[LEN_DATA];
				char chPathD[LEN_DATA];
				memset(chPathD, 0, LEN_DATA);
				
				hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
				if (hSockSrvRecMap != NULL)
				{
					lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, FILE_MAP_WRITE, 0, 0, 0);
					if (lpData == NULL)
					{
						CloseHandle(hSockSrvRecMap);
						hSockSrvRecMap = NULL;
						COutLogMgr::GetInstance()->LogWriter(Failure, _T("�������������������ն˷������ݣ��ڴ�ӳ�����\r\n"));
						pRecvWnd->m_lstLogData.InsertItem(0, pRecvWnd->GetLog(Failure, _T("�������������������ն˷������ݣ��ڴ�ӳ�����\r\n")));
						AfxMessageBox(_T("�������������������ն˷������ݣ��ڴ�ӳ�����"));
						return ;
					}
				}
// 				memcpy(g_strData.GetBuffer(0)+g_DataPos,pValue,*len);
// 				g_DataPos+=*len;
// 				memcpy(lpData,g_strData.GetBuffer(0),g_DataPos);
// 				SendMessage(hWnd, MSG_RECEIVE_DATA, (WPARAM)g_DataPos, 0);
// 				g_DataPos = 0;
// 				g_strData="";

				memcpy(lpData,pValue,*len);
				SendMessage(hWnd, MSG_RECEIVE_DATA, (WPARAM)*len, 0);
			}
			
		}
		break;
//	case 31:
//		{
			
//			CString tmp = (LPCTSTR)(LPTSTR)pValue;
			//int len = tmp.GetLength();
			//g_strMediaData+=tmp;
//			memcpy(g_strData.GetBuffer(0)+g_DataPos,pValue,*len);
//			g_DataPos+= *len;
//		}
	//add 14-01-16 lgp from yn end
	case TRANS_COMPLETE://���ݷ������
		{
            
			/*((CBCGPEdit*)pRecvWnd->GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(_T("���ݷ������"));
			//add lgp 11-28 begin
			pRecvWnd->m_lstLogData.InsertItem(0, pRecvWnd->GetLog(Success, _T("�ɹ����յ�����\r\n")));
			COutLogMgr::GetInstance()->LogWriter(Success, _T("�ɹ����յ�����\r\n"));
			
			POSITION pos = pRecvWnd->m_lstSoftReg_Data.GetHeadPosition();
			//char pDataD[nLen-20];
			//memset(pDataD,0x00,nLen-20);
			//memcpy(pDataD,buff+19,nLen-20)
			char *pData = new char[pRecvWnd->m_strData.GetLength()];
			memset(pData,0x00,pRecvWnd->m_strData.GetLength());
			memcpy(pData,pRecvWnd->m_strData,pRecvWnd->m_strData.GetLength());
			
			for(;NULL != pos;)
			{
				
				CString strSoftName = pRecvWnd->m_lstSoftReg_Data.GetNext(pos);
				HWND hWnd=FindWindow(NULL,strSoftName);
				if(NULL == hWnd)
					continue;
				
				HANDLE hSockSrvRecMap;
				LPBYTE lpData;
				
				char DataBuf[LEN_DATA];
				char chPathD[LEN_DATA];
				memset(chPathD, 0, LEN_DATA);
				
				hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 
					
					1024, "SockSrvDataMap");
				if (hSockSrvRecMap != NULL)
				{
					lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, FILE_MAP_WRITE, 0, 0, 0);
					if (lpData == NULL)
					{
						CloseHandle(hSockSrvRecMap);
						hSockSrvRecMap = NULL;
						COutLogMgr::GetInstance()->LogWriter(Failure, _T("�������������������ն˷�������ʱ���ڴ�ӳ������\r\n"));
							pRecvWnd->m_lstLogData.InsertItem(0, pRecvWnd->GetLog(Failure, _T("�������������������ն˷�������ʱ���ڴ�ӳ�����\r\n")));
							return ;
					}
				}
				
				for (int i=0; i<pRecvWnd->m_strData.GetLength(); i++) {
					DataBuf[i] = pData[i];
				}
				memcpy(lpData,DataBuf,sizeof(DataBuf));
				
				//int nLen = strlen(chPathD);
				SendMessage(hWnd, MSG_RECEIVE_DATA, (WPARAM)pRecvWnd->m_strData.GetLength(), (LPARAM)DataBuf);
				pRecvWnd->m_strData.Empty();
				delete pData;
			}
			//add lgp 11-28 end*/
		}
		break;
	
	default:
		break;
	}
}
/***************************************************************
��������void CALLBACK TransCallRecvMedia(int nTransType, int nMesType,int nLen,unsigned char* buff)

  �����������ص����� ��������
  
  ������int nTransType       ��������
        int nMesType         ���޶���
	    int nLen             ���ݳ���
        unsigned char* buff  �����յ�����
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/

void CALLBACK TransCallRecvMedia(int nTransType, int nMesType,int nLen,unsigned char* buff)//��ý�� ����
{   //delete 14-01-16 lgp begin
	/*CDlgDataReceive* pRecvWnd = (CDlgDataReceive*)theApp.GetMainWnd();

	char *pData = new char[nLen-20];
	memset(pData,0x00,nLen-20);
	memcpy(pData,buff+19,nLen-20);
	
	if(nLen>0 && NULL!=buff)
	{
		CString strLog = _T("�ɹ����յ���ý��");
		pRecvWnd->m_lstLogMedia.InsertItem(0, pRecvWnd->GetLog(Success,strLog));
	}
	else
		return;
	
	POSITION pos = pRecvWnd->m_lstSoftReg_Media.GetHeadPosition();
	for(;NULL != pos;)
	{
		    CString strSoftName = pRecvWnd->m_lstSoftReg_Media.GetNext(pos);
			HWND hWnd=FindWindow(NULL, strSoftName);
			if(NULL == hWnd)
				continue;

			HANDLE hSockSrvRecMap;
			LPBYTE lpData;
			
			char DataBuf[LEN_DATA];
			char chPathD[LEN_DATA];
			memset(chPathD, 0, LEN_DATA);
			
			hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
			if (hSockSrvRecMap != NULL)
			{
				lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, FILE_MAP_WRITE, 0, 0, 0);
				if (lpData == NULL)
				{
					CloseHandle(hSockSrvRecMap);
					hSockSrvRecMap = NULL;
					COutLogMgr::GetInstance()->LogWriter(Failure, _T("�������������������ն˷�����ý�壬�ڴ�ӳ�����\r\n"));
					pRecvWnd->m_lstLogMedia.InsertItem(0, pRecvWnd->GetLog(Failure, _T("�������������������ն˷�����ý�壬�ڴ�ӳ�����\r\n")));
		           return ;
				}
			}
			
			for (int i=0; i<nLen-20; i++) {
				DataBuf[i] = pData[i];
			}
			memcpy(lpData,DataBuf,sizeof(DataBuf));
			
			//int nLen = strlen(chPathD);
            SendMessage(hWnd, MSG_RECEIVE_MEDIA, (WPARAM)nLen-20, (LPARAM)DataBuf);
			
		
	}*/
	//delete 14-01-16 lgp end
}

/***************************************************************
��������void CALLBACK feedBackRecvMedia(LONG lType,void* pValue)

  �����������ص����� ��ӳ��ǰ����״��
  
  ������LONG lType     ��������
        void* pValue   ����ֵ(ָ����ʽ)
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
//add 14-01-16 lgp from yn begin
CString g_strMediaData= "";
int g_pos = 0;
//add 14-01-16 lgp from yn end
void CALLBACK feedBackRecvMedia(LONG lType,void* pValue, void* pNULL)//����״�� ������ ���д��� ��������0 
{
    //add 14-01-16 lgp from yn begin
	USHORT* len = (USHORT*)pNULL;
	int pos = 0;
    //add 14-01-16 lgp from yn end
	switch(lType)
	{
	case 4://���ݷ������
		{
			(((CBCGPEdit*)((CDlgDataReceive*)theApp.GetMainWnd())->GetSafeHwnd())->GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(_T("���ݷ������"));
		}
		break;
    //add 14-01-16 lgp from yn begin
		case 32://���ݷ������
		{
			CString tmp = (LPCTSTR)(LPTSTR)pValue;
		    
			CDlgDataReceive* pRecvWnd = (CDlgDataReceive*)theApp.GetMainWnd();
			pRecvWnd->m_lstLogMedia.InsertItem(0, pRecvWnd->GetLog(Success, _T("�ɹ����յ���ý��\r\n")));
			POSITION pos = pRecvWnd->m_lstSoftReg_Media.GetHeadPosition();
			for(;NULL != pos;)
			{
				CString strSoftName = pRecvWnd->m_lstSoftReg_Media.GetNext(pos);
				HWND hWnd=FindWindow(NULL, strSoftName);
				if(NULL == hWnd)
					continue;
				
				HANDLE hSockSrvRecMap;
				LPBYTE lpData;
				
				char DataBuf[LEN_DATA];
				char chPathD[LEN_DATA];
				memset(chPathD, 0, LEN_DATA);
				
				hSockSrvRecMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024, "SockSrvDataMap");
				if (hSockSrvRecMap != NULL)
				{
					lpData  = (LPBYTE)MapViewOfFile(hSockSrvRecMap, FILE_MAP_WRITE, 0, 0, 0);
					if (lpData == NULL)
					{
						CloseHandle(hSockSrvRecMap);
						hSockSrvRecMap = NULL;
						COutLogMgr::GetInstance()->LogWriter(Failure, _T("�������������������ն˷�����ý�壬�ڴ�ӳ�����\r\n"));
						pRecvWnd->m_lstLogMedia.InsertItem(0, pRecvWnd->GetLog(Failure, _T("�������������������ն˷�����ý�壬�ڴ�ӳ�����\r\n")));
						return ;
					}
				}
				memcpy(g_strMediaData.GetBuffer(0)+g_pos,pValue,*len);
				g_pos+=*len;
				memcpy(lpData,g_strMediaData.GetBuffer(0),g_pos);
				SendMessage(hWnd, MSG_RECEIVE_MEDIA, (WPARAM)g_pos, 0);
				g_pos = 0;
				
			}
			//(((CBCGPEdit*)((CDlgDataReceive*)theApp.GetMainWnd())->GetSafeHwnd())->GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(_T("���ݷ������"));
			g_strMediaData="";
		}
		break;
	case 31:
		{
			
			CString tmp = (LPCTSTR)(LPTSTR)pValue;
			//int len = tmp.GetLength();
			//g_strMediaData+=tmp;
			memcpy(g_strMediaData.GetBuffer(0)+g_pos,pValue,*len);
			g_pos+= *len;
		}
	//add 14-01-16 lgp from yn end
	default:
		break;
	}
}
BOOL CDlgDataReceive::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgDataReceive::InitSafeCheck()
{
	GetRSingSoftPath();

	if(!PathFileExists(_T("DXSJCS.bat")))
	{
		CFile file;
		file.Open(_T("DXSJCS.bat"),CFile::modeRead|CFile::modeWrite|CFile::modeCreate); 
        if (!file) 
			return FALSE; 
		
		file.Close(); 
	}
	if(!PathFileExists(_T("DXSJCS.vbs")))
	{
		CFile file;
		file.Open(_T("DXSJCS.vbs"),CFile::modeRead|CFile::modeWrite|CFile::modeCreate); 
        if (!file) 
			return FALSE; 
		CString strVbs =_T("Set ws = CreateObject(\"Wscript.Shell\")\r\nws.run \"cmd /c DXSJCS.bat\",vbhide ");
		file.Write(strVbs, strVbs.GetLength());
		file.Close(); 
	}
	return TRUE;
}
void CDlgDataReceive::GetRSingSoftPath()
{
	TCHAR buffer[MAX_PATH];
	TCHAR szValueName[] = _T("installpath");
	TCHAR SubKeyName[] = _T("SOFTWARE\\rising\\RAV");
	TCHAR* installPath;
	HKEY hKey;
	DWORD dwBufLen;
	LONG lret;
	
	HRESULT hr = E_FAIL;
	DWORD* reservedNULL = NULL;
	DWORD reservedZero = 0;
	
	lret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		SubKeyName,
		reservedZero,
		KEY_READ,
		&hKey);
	if (lret == ERROR_SUCCESS)
	{
		dwBufLen = MAX_PATH;
		lret = RegQueryValueEx(hKey,
			szValueName,
			reservedNULL,
			NULL,
			(BYTE*)buffer,
			&dwBufLen);
		if (lret == ERROR_SUCCESS)
		{
			m_strRSingSoftPath = buffer;
			m_strRSingSoftPath += _T("\\ravlite.exe");
			hr = S_OK;
		} // end if successfull value read
		RegCloseKey(hKey);
	} // end if successfule key open
	
	return;
}
/***************************************************************
��������void Init()

  ������������ʼ������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::Init()
{
	m_bInitDlg = TRUE;//ϵͳ��ʼʼ�������ڽ���

	//CBCGPVisualManager2003::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2003));//
	//EnableVisualManagerStyle();
	CBCGPDialog::EnableVisualManagerStyle(TRUE);
    InitLogCtrl();
    
	InitSafeCheck();

	m_pTransControlEx_File = CTransControlEx::NewCtransCtrlEx();
	InitServ_File();

	if(InitParam_File())
	{
		OnStartFile();
	}
    
	m_pTransControlEx_Data = CTransControlEx::NewCtransCtrlEx();
	InitServ_Data();

	if(InitParam_Data())
	{
		OnStartData();
	}
	
	 m_pTransControlEx_Media = CTransControlEx::NewCtransCtrlEx();
	 InitServ_Media();

	if(InitParam_Media())
	{
		OnStartMedia();
	}

	m_bInitDlg = FALSE;//ϵͳ��ʼ�������ڽ������

}
void CDlgDataReceive::InitLogCtrl()
{
	//������־�б�ؼ�������ѡ�е�����
	m_lstLogFile.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstLogData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstLogMedia.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//��ʼ����־�б�
	CRect rectFile;
	m_lstLogFile.GetWindowRect(&rectFile);
	m_lstLogFile.InsertColumn(0, _T("������־"), LVCFMT_CENTER, rectFile.Width()-4);
	
	CRect rectData;
	m_lstLogData.GetWindowRect(&rectData);
	m_lstLogData.InsertColumn(0, _T("������־"), LVCFMT_CENTER, rectData.Width()-4);
	
	CRect rectMedia;
	m_lstLogMedia.GetWindowRect(&rectMedia);
	m_lstLogMedia.InsertColumn(0, _T("������־"), LVCFMT_CENTER, rectMedia.Width()-4);
}
/***************************************************************
��������void InitNodeValues()

  ������������ʼ������Ԫ��ֵ
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::InitNodeValues()
{
   //��ȡ�����ļ��е��ļ�����ģ����Ϣ
   CString strFileLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
   CString strFileIsSafeCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
   CString strFileSavePath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);

   //��ȡ���ݽ���ģ����Ϣ
   CString strDataLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);

   //��ȡ��ý�����ģ����Ϣ
   CString strMediaLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);

   //��ʾ�ļ�����ģ����Ϣ
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->SetWindowText(strFileLocalPort);
   if(_T("1") == strFileIsSafeCheck)
	   ((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->SetCheck(TRUE);
   else
	   ((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->SetCheck(FALSE);
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_SAVE_PATH))->SetWindowText(strFileSavePath);

   //��ʾ���ݽ���ģ����Ϣ
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->SetWindowText(strDataLocalPort);

   //��ʾ��ý�����ģ����Ϣ
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->SetWindowText(strMediaLocalPort);

}

LRESULT CDlgDataReceive::SpeedSending(WPARAM wpa, LPARAM lpa)
{
	//CString strFile = (CString)lpa;
	//AfxMessageBox(strFile);
	CString strValue;
	strValue.Format(_T("�ѽ���: %s - %d%%"),m_strFileRecving, wpa);

	//((CEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->Invalidate(TRUE);
    GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE)->SetWindowText(strValue);
	((CEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->Invalidate(FALSE);
	
	return 0;
}


LRESULT CDlgDataReceive::ShowRecvAnotherFile(WPARAM wpa, LPARAM lpa)
{
	CString* pStrFile = (CString*)wpa;
	int nCount = ((CDlgDataReceive*)theApp.GetMainWnd())->m_lstLogFile.GetItemCount();
	CString strMsg;
	strMsg.Format(_T("���ڽ���: %s\r\n"), *pStrFile);
	m_lstLogFile.InsertItem(nCount, GetLog(General, strMsg));
	COutLogMgr::GetInstance()->LogWriter(General,strMsg.GetBuffer(strMsg.GetLength()));

	return 0;
}


BOOL CDlgDataReceive::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//ֻ�й���Ա���ܲ鿴��־��ά���û�
	
	if( VK_RETURN == pMsg->wParam )
	{
		return TRUE;
	}

	if(VK_ESCAPE == pMsg->wParam ) 
	{
		return TRUE;
	}

    switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_F3:  //�û�����
			{

				BOOL bRet = COutLogIn::GetInstance()->ValidatePermissions(theApp.m_strUserName, MODULE_NAME_ALL);
				if(bRet)
	                COutLogIn::GetInstance()->UserManagement(theApp.m_strUserName);
				
				break;
			}
			case VK_F4:    //��־�鿴
				{
					COutLogMgr::GetInstance()->DisplayLog();
					break;
				}
		}
		break;
	}	
	return CBCGPDialog::PreTranslateMessage(pMsg);
}
/***************************************************************
��������void OnFileSavePath()

  ����������ѡ���ļ�·��
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnFileSavePath() 
{
	// TODO: Add your control notification handler code here
	
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_RECV))
		return;

	TCHAR wstrFolder[MAX_PATH];
	ZeroMemory(&wstrFolder,sizeof(wstrFolder)); 
	
    BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(BROWSEINFO));   
	bi.hwndOwner = this->GetSafeHwnd();   
	bi.pszDisplayName = wstrFolder;   
	bi.lpszTitle = _T("ѡ�����Ŀ¼");   
	
	LPITEMIDLIST   idl=SHBrowseForFolder(&bi);   
	if(NULL == idl)   
		return ;   
	
	SHGetPathFromIDList(idl, wstrFolder); 
	
	
	CString strFolderPath;
	strFolderPath.Format(_T("%s"), wstrFolder);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_SAVE_PATH))->SetWindowText(strFolderPath);

	CString strLogMsg;
	strLogMsg.Format(_T("ѡ���ļ�����·�� %s\r\n"), strFolderPath);
	COutLogMgr::GetInstance()->LogWriter(General, strLogMsg.GetBuffer(strLogMsg.GetLength()));
	m_lstLogFile.InsertItem(0, 	GetLog(General, strLogMsg));

	CheckReceivePathBeforeSend();
}

/***************************************************************
  ��������TransCallRecvFile(int nTransType, int nMesType,int nLen,unsigned char* buff)
  �����������ص����� �������� 
  ��    ����int nTransType       ��������
            int nMesType         ���޶���
	        int nLen             ���ݳ���
            unsigned char* buff  �����յ�����
  ȫ�ֱ�������	  
  �� �� ֵ: ��		
****************************************************************/
void CALLBACK TransCallRecvFile(int nTransType, int nMesType,int nLen,unsigned char* buff)//�ļ�
{
}
BOOL CDlgDataReceive::SafeCheck(CString strFile)
{
	CString strRecvPath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);
	CString strRecvFile;
	if( _T('\\') == strRecvPath.GetAt(strRecvPath.GetLength()-1))
         strRecvFile = strRecvPath + strFile;
	else
         strRecvFile = strRecvPath + _T("\\")  + strFile;

	if(!PathFileExists(strRecvFile))
		return TRUE;

	CString strIsSafeCheck=COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
	if(_T("0") == strIsSafeCheck)
		return TRUE;
	
	//3. ��ȫ���
	/////////////////
	if(!PathFileExists(_T("DXSJCS.bat")))
		return FALSE;
	
	//д������������
	CFile file;
	file.Open(_T("DXSJCS.bat"),CFile::modeRead|CFile::modeWrite|CFile::modeCreate); 
	if (!file) 
		return FALSE; 
	CString strSavePath;
	((CBCGPEdit*)((CDlgDataReceive*)theApp.GetMainWnd())->GetDlgItem(IDC_EDT_SAVE_PATH))->GetWindowText(strSavePath);
				strSavePath += _T("\\");
				strSavePath += strFile;

	CString strBat = _T("\"") + m_strRSingSoftPath + _T("\" \"") + strSavePath +_T("\"");
	file.Write(strBat, strBat.GetLength());
	file.Close;
	
    //��������������
	if(!PathFileExists(_T("DXSJCS.vbs")))
		return FALSE;

	SHELLEXECUTEINFO shei;
	shei.cbSize = sizeof(shei);
	shei.fMask = SEE_MASK_INVOKEIDLIST;
	shei.lpFile = _T("DXSJCS.vbs");
	shei.lpVerb = _T("open");
	ShellExecute(NULL, shei.lpVerb, shei.lpFile, _T(""), _T(""), SW_SHOW);

	return TRUE;
}
/***************************************************************
  �� �� ����void CALLBACK feedBackRecvData(LONG lType,void* pValue)
  �����������ص����� ��ӳ��ǰ����״�� 
  ��    ����LONG lType     ��������
            void* pValue   ����ֵ(ָ����ʽ)	
  ȫ�ֱ�������	  
  �� �� ֵ: ��		
****************************************************************/
void CALLBACK feedBackRecvFile(LONG lType,void* pValue, void* pFileName)//����״�� ������ ���д��� ��������0 
{ 
	CDlgDataReceive* pRecvWnd = (CDlgDataReceive*)theApp.GetMainWnd();
	switch(lType)
	{
	case UPDATE_PROGRESS://���½�����
		{
			LONG* pVal = (LONG*)pValue; 

			LPSTR strFileName = (LPSTR)pFileName;
			pRecvWnd->m_strFileRecving = strFileName;
			
		
			/*CString strValue;
			strValue.Format(_T("��ǰ����: �ѽ���%d%%"), *pVal);*/
			PostMessage(pRecvWnd->GetSafeHwnd(), WM_SEND_SPEED, *pVal,0);
		}
		break;
	case RECV_FILE_NAME:
		{
			LPSTR strFileName = (LPSTR)pValue;
            CString strRecvFileMsg;

			//�������ļ����Ƿ����
			CString strRecvPath;
			((CBCGPEdit*)pRecvWnd->GetDlgItem(IDC_EDT_SAVE_PATH))->GetWindowText(strRecvPath);
            
			if(!PathFileExists(strRecvPath))
			{
                strRecvFileMsg.Format(_T("����·�������ڣ������ļ�: %s����\r\n"), strFileName);
				pRecvWnd->m_lstLogFile.InsertItem(0, pRecvWnd->GetLog(Success, strRecvFileMsg));
		        COutLogMgr::GetInstance()->LogWriter(Success,strRecvFileMsg.GetBuffer(strRecvFileMsg.GetLength()));

				MessageBox(NULL, strRecvFileMsg, _T("��ʾ��"), MB_OK|MB_ICONERROR);
				
				/*//ֹͣ�ļ�����
				pRecvWnd->m_pTransControlEx_File->StopTrans();
				
				CString strStopMsg = _T("�����ļ�����ֹͣ\r\n");
				((CBCGPEdit*)pRecvWnd->GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(_T(""));
				
				
				((CBCGPEdit*)pRecvWnd->GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strStopMsg);
				COutLogMgr::GetInstance()->LogWriter(Success, strStopMsg.GetBuffer(strStopMsg.GetLength()));
				pRecvWnd->m_lstLogFile.InsertItem(0, pRecvWnd->GetLog(Success, strStopMsg));
				((CBCGPButton*)pRecvWnd->GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
				((CBCGPButton*)pRecvWnd->GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
	            ((CBCGPButton*)pRecvWnd->GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);
                */

				
				break;
			}
			else
			{
				strRecvFileMsg.Format(_T("�����ļ�: %s\r\n"), strFileName);
				pRecvWnd->m_lstLogFile.InsertItem(0, pRecvWnd->GetLog(Success, strRecvFileMsg));
		        COutLogMgr::GetInstance()->LogWriter(Success,strRecvFileMsg.GetBuffer(strRecvFileMsg.GetLength()));
			}
			
		}
		break;
	case SAFE_CHECK_ERROR://��ȫ������
		{
           //(((CBCGPEdit*)((CDlgDataReceive*)theApp.GetMainWnd())->GetSafeHwnd())->GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(_T("��ȫ������")); 
		}
		break;
	case BOTHWAY_CHECK_FAIL://˫���·У��ʧ��
		{
	       //(((CBCGPEdit*)((CDlgDataReceive*)theApp.GetMainWnd())->GetSafeHwnd())->GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(_T("˫��У���·ʧ��"));
		}
		break;
	case RECV_PATH_NOT_EXIST:
		{
			AfxMessageBox(_T("�Ѿ����ļ���ʼ���ͣ����ļ�����·�������ڣ�����ʧ�ܣ��뾡�����ý���·��"));
			break;
		}
	case TRANS_ERROR://�������
		{

		}
		break;
	case DATABUF_IS_FULL://��������
		{
			pRecvWnd->m_lstLogFile.InsertItem(0, pRecvWnd->GetLog(Failure, _T("��������\r\n")));
			COutLogMgr::GetInstance()->LogWriter(Failure,_T("��������\r\n"));
		}
		break;
	case TRANS_LOSS_PACKET://���䶪��
		{
			pRecvWnd->m_lstLogFile.InsertItem(0, pRecvWnd->GetLog(Failure, _T("���䶪��\r\n")));
			COutLogMgr::GetInstance()->LogWriter(Failure,_T("���䶪��\r\n"));
		}
		break;

	case TRANS_COMPLETE://���ݴ������
		{	
			//�������ļ����Ƿ����
			CString strRecvPath;
			((CBCGPEdit*)pRecvWnd->GetDlgItem(IDC_EDT_SAVE_PATH))->GetWindowText(strRecvPath);
	
			if(!PathFileExists(strRecvPath))
			{
				AfxMessageBox(_T("����·��������"));
				return;
			}

			LPSTR strFileName = (LPSTR)pValue;
			pRecvWnd->SafeCheck(strFileName);
			CString strRecvFileMsg;
			strRecvFileMsg.Format(_T("�����ļ�: %s\r\n"), strFileName);
			POSITION pos = pRecvWnd->m_lstSoftReg_File.GetHeadPosition();
			for(;NULL != pos;)
			{
				CString strSoftName = pRecvWnd->m_lstSoftReg_File.GetNext(pos);
				LPSTR strFile = (LPSTR)pValue;
				HWND hWnd=FindWindow(NULL,strSoftName);
				if(NULL == hWnd)
					continue;
				
				HANDLE hSockSrvRecMap;
				LPBYTE lpData;
				CString strSavePath;
				((CBCGPEdit*)((CDlgDataReceive*)theApp.GetMainWnd())->GetDlgItem(IDC_EDT_SAVE_PATH))->GetWindowText(strSavePath);
				strSavePath += _T("\\");
				strSavePath += strFile;
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
						COutLogMgr::GetInstance()->LogWriter(Failure, 
							_T("�������������������ն˷����ļ�����ʱ���ڴ�ӳ�����\r\n"));
						pRecvWnd->m_lstLogFile.InsertItem(0, pRecvWnd->GetLog(Failure, 
							_T("�������������������ն˷����ļ�����ʱ���ڴ�ӳ�����\r\n")));
		                return;
					}
				}
				MoveMemory(lpData, strSavePath, strSavePath.GetLength());
				SendMessage(hWnd, MSG_RECEIVE_FILE, 
					(WPARAM)strSavePath.GetLength(), 0);
			}
		}
		break;
	case STOP_RECV://ֹͣ����
        break;
	default:
		break;
	}
}
/***************************************************************
��������void GetFileNameFromPath()

  ���������������ļ���·������ļ���
  
  ������CString strFilePath���ļ���·��
	
  ȫ�ֱ�������
	  
  ����ֵ: CString  �ļ���
		
****************************************************************/
CString CDlgDataReceive::GetFileNameFromPath(CString strFilePath)
{
	int Where;
	Where = strFilePath.ReverseFind('\\');
	if (-1 == Where)
		Where = strFilePath.ReverseFind('/');
	CString FileTitle = strFilePath.Right(strFilePath.GetLength() - 1 - Where);
	
	return FileTitle;

}
BOOL CDlgDataReceive::ValidatePermissions(BOOL bInitDlg, CString strModuleName)
{
	//��֤Ȩ��
	BOOL bRet = COutLogIn::GetInstance()->ValidatePermissions(theApp.m_strUserName,strModuleName);
	if(!bRet)
	{
		CString strMsg;
		strMsg.Format(_T("�Բ�����û��%s��Ȩ��"),strModuleName);

		if(!bInitDlg)
			BCGPMessageBox(NULL, strMsg, _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);

		return FALSE;
	}

	return TRUE;
}
//�����ļ����շ���ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
BOOL CDlgDataReceive::CheckUiAndXmlBeforeStartFile()
{
	//��ȡ�����ļ���Ϣ
	CString strLocalPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE,ENU_NODE_LOCAL_PORT);
	CString strIsSafeCheck_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
	
	//��ȡ����Ԫ��ֵ
	CString strLocalPort_Ui, strIsSafeCheck_Ui;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->GetWindowText(strLocalPort_Ui);
	if(1 == ((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->GetCheck())
		strIsSafeCheck_Ui = _T("1");
	else 
		strIsSafeCheck_Ui = _T("0");

	if( (strLocalPort_Ui != strLocalPort_Xml) || (strIsSafeCheck_Xml != strIsSafeCheck_Ui))
	{
		if( IDYES == BCGPMessageBox(NULL, _T("�ļ�����ģ���������ļ���������и��£��Ƿ񱣴���Ч"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			if(!Validate_Port(strLocalPort_Ui, MODULE_NAME_FILE_RECV, _T("���ض˿�")))
				return FALSE;

                //��������Ϣд�������ļ�
			COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT, strLocalPort_Ui);
			COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK, strIsSafeCheck_Ui);
			return TRUE;
		}
		else
		{
               //�������ļ���Ϣд������
			((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->SetWindowText(strLocalPort_Xml);
			if(_T("1") == strIsSafeCheck_Xml)
		     	((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->SetCheck(1);
			else 
				((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->SetCheck(0);
			return TRUE;
		}
	}
	return TRUE;
}
//�������ݽ��շ���ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
BOOL CDlgDataReceive::CheckUiAndXmlBeforeStartData()
{
	//��ȡ�����ļ���Ϣ
	CString strLocalPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA,ENU_NODE_LOCAL_PORT);
	
	//��ȡ����Ԫ��ֵ
	CString strLocalPort_Ui;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->GetWindowText(strLocalPort_Ui);
	
	if( (strLocalPort_Ui != strLocalPort_Xml))
	{
		if( IDYES == BCGPMessageBox(NULL, _T("���ݽ���ģ�����������ݷ�������и��£��Ƿ񱣴���Ч"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			//��������Ϣд�������ļ�
			COperXml::GetInstance()->SetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT, strLocalPort_Ui);
			return TRUE;
		}
		else
		{
			//�������ļ���Ϣд������
			((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->SetWindowText(strLocalPort_Xml);
			return TRUE;
		}
	}
	return TRUE;
}
//������ý����շ���ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
BOOL CDlgDataReceive::CheckUiAndXmlBeforeStartMedia()
{
	//��ȡ�����ļ���Ϣ
	CString strLocalPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA,ENU_NODE_LOCAL_PORT);
	
	//��ȡ����Ԫ��ֵ
	CString strLocalPort_Ui;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->GetWindowText(strLocalPort_Ui);
	
	if( (strLocalPort_Ui != strLocalPort_Xml))
	{
		if( IDYES == BCGPMessageBox(NULL, _T("��ý�����ģ����������ý���������и��£��Ƿ񱣴���Ч"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			//��������Ϣд�������ļ�
			COperXml::GetInstance()->SetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT, strLocalPort_Ui);
			return TRUE;
		}
		else
		{
			//�������ļ���Ϣд������
			((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->SetWindowText(strLocalPort_Xml);
			return TRUE;
		}
	}
	return TRUE;
}
//�����ļ�ǰ���ȼ������ϵĽ���·���Ƿ��������ļ��еĽ���·��һ�£�����һ�£�����ʾ
BOOL CDlgDataReceive::CheckReceivePathBeforeSend()
{
	CString strReceivePath_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);
	
	CString strReceivePath_Ui;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_SAVE_PATH))->GetWindowText(strReceivePath_Ui);
	
	
	if(strReceivePath_Xml != strReceivePath_Ui)//UI��XML��ͬ
	{
		if( IDYES == BCGPMessageBox(NULL, _T("���������µ��ļ�����·�����Ƿ񰴴�·�������ļ�"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		//if( IDYES == MessageBox( _T("���������µ��ļ�����·�����Ƿ񰴴�·�������ļ�"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			if(!PathFileExists(strReceivePath_Ui))
			{
				if(!CreateDirectory(strReceivePath_Ui, NULL))
				{
					OnStopFile();
					//MessageBox(_T("�µ��ļ�����·�������ڣ�����ʧ�ܣ�������ѡ���ļ�����·��"),_T("��ʾ��"));
					BCGPMessageBox(_T("�µ��ļ�����·�������ڣ�����ʧ�ܣ�������ѡ���ļ�����·��"));
					return FALSE;
				}
			}
			//���������ļ�
			COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH,strReceivePath_Ui);
		}
		else
		{
			if(!PathFileExists(strReceivePath_Xml))
			{
				if(!CreateDirectory(strReceivePath_Xml, NULL))
				{
					OnStopFile();
					//MessageBox(_T("�����ļ������õ��ļ�����·�������ڣ�����ʧ�ܣ�������ѡ���ļ�����·��"));
					BCGPMessageBox(_T("�����ļ������õ��ļ�����·�������ڣ�����ʧ�ܣ�������ѡ���ļ�����·��"));
					return FALSE;
				}
			}
			
			((CBCGPEdit*)GetDlgItem(IDC_EDT_SAVE_PATH))->SetWindowText(strReceivePath_Xml);
		}
	}
	else
	{
		if(!PathFileExists(strReceivePath_Xml))
		{
			if( IDYES != BCGPMessageBox(NULL, _T("�ļ�����·�������ڣ��Ƿ��Զ�����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
			//if( IDYES != MessageBox(_T("�ļ�����·�������ڣ��Ƿ��Զ�����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
			{
				//OnStopFile();
				//MessageBox(_T("��ѡ���ļ�����·����Ȼ����ܽ����ļ�"), _T("��ʾ��"), MB_OK|MB_ICONERROR);
				BCGPMessageBox(NULL, _T("��ѡ���ļ�����·����Ȼ����ܽ����ļ�"), _T("��ʾ��"), MB_OK|MB_ICONERROR);
				return FALSE;
			}
			
			if(!CreateDirectory(strReceivePath_Xml, NULL))
			{
				//OnStopFile();
				//MessageBox(_T("�Բ����ļ�����·�������ڣ�����ʧ�ܣ���ѡ���ļ�����·��"), _T("��ʾ��"), MB_OK|MB_ICONERROR);
				BCGPMessageBox(NULL, _T("�Բ����ļ�����·�������ڣ�����ʧ�ܣ���ѡ���ļ�����·��"), _T("��ʾ��"), MB_OK|MB_ICONERROR);
				return FALSE;
			}
		}
	}
	
	return TRUE;

}
BOOL CDlgDataReceive::Validate_Port(CString strPort, CString strModuleName, CString strPortName)
{
	int nPort = atoi(strPort);
	if( nPort> PORT_MAX_USE||nPort<PORT_MIN_USE)
	{
		CString strMaxPort, strMinPort;
		strMaxPort.Format(_T("%d"), PORT_MAX_USE);
		strMinPort.Format(_T("%d"), PORT_MIN_USE);
		
		CString strMsg;
        strMsg.Format(_T("�Բ����������%s��%s���Ϸ���������%s-%s�Ķ˿ں�"), strModuleName, strPortName, strMinPort, strMaxPort);
		BCGPMessageBox(NULL, strMsg, _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}
BOOL CDlgDataReceive::ValidateParam_FileModule()
{
	CString strLocalPath,strLocalPort,strSafeCheck;
	//�洢·��
	((CBCGPEdit*)GetDlgItem(IDC_EDT_SAVE_PATH))->GetWindowText(strLocalPath);
	if(!PathFileExists(strLocalPath))
	{
		if( IDYES != BCGPMessageBox(NULL, _T("�洢·�������ڣ��Ƿ��Զ�����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			return FALSE;
		}
		if(!CreateDirectory(strLocalPath, NULL))
		{
			BCGPMessageBox(NULL, _T("�Բ����ļ��洢·�������ڣ�����ʧ�ܣ���ѡ���ļ��洢·��"), _T("��ʾ��"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	
	//�Ƿ�ȫ��飬���ض˿�
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->GetWindowText(strLocalPort);
	int nCheckFile = ((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->GetCheck();
    if(1 == nCheckFile)
		strSafeCheck = _T("1");
	else if(0 == nCheckFile)
		strSafeCheck = _T("0");

	if(!Validate_Port(strLocalPort, MODULE_NAME_FILE_RECV, _T("���ض˿�")))
		return FALSE;

	//�����ļ�����ģ�����Ԫ��
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT, strLocalPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH, strLocalPath);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK, strSafeCheck);

	return TRUE;
}

BOOL CDlgDataReceive::ValidateParam_DataModule()
{	
	CString strLocalPort;
	//���ض˿�
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->GetWindowText(strLocalPort);
    int nLocalPort = atoi(strLocalPort);
	if(!Validate_Port(strLocalPort, MODULE_NAME_DATA_RECV, _T("���ض˿�")))
		return FALSE;

	//�������ݽ���ģ�����Ԫ��
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT, strLocalPort);

	return TRUE;

}
BOOL CDlgDataReceive::ValidateParam_MediaModule()
{
	//���ض˿�
	CString strLocalPort;
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->GetWindowText(strLocalPort);
    
	if(!Validate_Port(strLocalPort, MODULE_NAME_MEDIA_RECV, _T("���ض˿�")))
		return FALSE;

	//������ý�����ģ�����Ԫ��
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT, strLocalPort);

	return TRUE;
}
BOOL CDlgDataReceive::InitServ_File()
{
	CString strLocalPort_File;
	strLocalPort_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
	if(strLocalPort_File.IsEmpty())
	    return FALSE;
	CString strSuccMsg = _T("�����ļ������ʼ���ɹ�\r\n");
	CString strFailMsg = _T("�����ļ������ʼ��ʧ��\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(_T(""));

	int nLocalPort = atoi(strLocalPort_File);
	BOOL bRet = m_pTransControlEx_File->Init(nLocalPort, _T(""), TYPE_RECEIVE_FILE, TransCallRecvFile,feedBackRecvFile);
	
	if(!bRet)
    {
		COutLogMgr::GetInstance()->LogWriter(Failure, strFailMsg.GetBuffer(strFailMsg.GetLength()));
		m_lstLogFile.InsertItem(0, GetLog(Failure, strFailMsg));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strFailMsg);
		return FALSE;
	}
	
	COutLogMgr::GetInstance()->LogWriter(Success, strSuccMsg.GetBuffer(strSuccMsg.GetLength()));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strSuccMsg);
	m_lstLogFile.InsertItem(0, GetLog(Success, strSuccMsg));

	return TRUE;
}


BOOL CDlgDataReceive::InitServ_Data()
{
	CString strLocalPort_Data;
	strLocalPort_Data = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);
	if(strLocalPort_Data.IsEmpty())
		return FALSE;
	CString strSuccMsg = _T("�������ݷ����ʼ���ɹ�\r\n");
	CString strFailMsg = _T("�������ݷ����ʼ��ʧ��\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(_T(""));

	int nLocalPort = atoi(strLocalPort_Data);
	CString strLocalIp;
	BOOL bRet = m_pTransControlEx_Data->Init(nLocalPort, _T(""), TYPE_RECEIVE_SHORTMES,TransCallRecvData,feedBackRecvData);
	if(!bRet)
	{
		COutLogMgr::GetInstance()->LogWriter(Failure, strFailMsg.GetBuffer(strFailMsg.GetLength()));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strFailMsg);
		m_lstLogData.InsertItem(0, GetLog(Failure, strFailMsg));
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);
		return FALSE;
	}
	
	COutLogMgr::GetInstance()->LogWriter(Success, strSuccMsg.GetBuffer(strSuccMsg.GetLength()));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strSuccMsg);
	m_lstLogData.InsertItem(0, GetLog(Success, strSuccMsg));

	return TRUE;
}

BOOL CDlgDataReceive::InitServ_Media()
{
	CString strLocalPort_Media;
	strLocalPort_Media = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);

	if(strLocalPort_Media.IsEmpty())
		return FALSE;
	CString strSuccMsg = _T("������ý������ʼ���ɹ�\r\n");
	CString strFailMsg = _T("������ý������ʼ��ʧ��\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(_T(""));
	
	int nLocalPort = atoi(strLocalPort_Media);
	BOOL bRet = m_pTransControlEx_Media->Init(nLocalPort, _T(""), TYPE_RECEIVE_STREAM, TransCallRecvMedia,feedBackRecvMedia);
	if(!bRet)
	{
		COutLogMgr::GetInstance()->LogWriter(Failure, strFailMsg.GetBuffer(strFailMsg.GetLength()));
		m_lstLogMedia.InsertItem(0, GetLog(Failure, strFailMsg));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strFailMsg);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);
		return FALSE;
	}
	
	m_lstLogMedia.InsertItem(0, GetLog(Success, strSuccMsg));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strSuccMsg);
	COutLogMgr::GetInstance()->LogWriter(Success,strSuccMsg.GetBuffer(strSuccMsg.GetLength()));
	
	return TRUE;
}
BOOL CDlgDataReceive::InitParam_File()
{
	//��ȡ�����ļ��е��ļ�����ģ����Ϣ
	CString strFileLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
	CString strFileIsSafeCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
    CString strFileSavePath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);

	//��ʾ�ļ�����ģ����Ϣ
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->SetWindowText(strFileLocalPort);
	if(_T("1") == strFileIsSafeCheck)
		((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->SetCheck(TRUE);
	else if(_T("0") == strFileIsSafeCheck)
		((CBCGPButton*)GetDlgItem(IDC_CHECK_RECEIVEED_FILE))->SetCheck(FALSE);
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_SAVE_PATH))->SetWindowText(strFileSavePath);

   //�������ĺϷ���
   //��ȫУ��
   if((_T("0") != strFileIsSafeCheck) && (_T("1") != strFileIsSafeCheck))
   {
	   BCGPMessageBox(NULL, _T("�����ļ��е� �Ƿ���а�ȫУ�� ���ø�ʽ����"), _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
	   ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);	
	   ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
	   return FALSE;
   }
   //�洢·��
   if(!PathFileExists(strFileSavePath))
   {
	   if( IDYES != BCGPMessageBox(NULL, _T("�洢·�������ڣ��Ƿ��Զ�����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
	   {
		   ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		   ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		   return FALSE;
	   }
	   if(!CreateDirectory(strFileSavePath, NULL))
	   {
		   ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		    ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		   BCGPMessageBox(NULL, _T("�Բ����ļ��洢·�������ڣ�����ʧ�ܣ���ѡ���ļ��洢·��"), _T("��ʾ��"), MB_OK|MB_ICONERROR);
		   return FALSE;
	   }
   }
   //���ض˿�
   if(!Validate_Port(strFileLocalPort, MODULE_NAME_FILE_RECV, _T("���ض˿�")))
		return FALSE;

   return TRUE;
}
BOOL CDlgDataReceive::InitParam_Data()
{
	//��ȡ���ݽ���ģ����Ϣ
	CString strDataLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);
	//��ʾ���ݽ���ģ����Ϣ
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->SetWindowText(strDataLocalPort);
	//�Ϸ��Լ��
	if(!Validate_Port(strDataLocalPort, MODULE_NAME_DATA_RECV, _T("���ض˿�")))
		return FALSE;

	return TRUE;
}
BOOL CDlgDataReceive::InitParam_Media()
{
	//��ȡ��ý�����ģ����Ϣ
	CString strMediaLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);
	//��ʾ��ý�����ģ����Ϣ
   ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->SetWindowText(strMediaLocalPort);
   //�Ϸ���У��
   if(!Validate_Port(strMediaLocalPort, MODULE_NAME_MEDIA_RECV, _T("���ض˿�")))
	   return FALSE;

   return TRUE;
}

BOOL CDlgDataReceive::Validate_IP(CString strIp, CString strModuleName)
{
   return TRUE;
}
/***************************************************************
  �� �� ����void OnStartFile()
  ������������ʼ�ļ����� 
  ��    ������	
  ȫ�ֱ�������	  
  �� �� ֵ: ��		
****************************************************************/
void CDlgDataReceive::OnStartFile() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
    if(!ValidatePermissions(m_bInitDlg, MODULE_NAME_FILE_RECV))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
	    ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);

		return ;
	}

    if(!CheckReceivePathBeforeSend())
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		
		return ;
	}

	if(!CheckUiAndXmlBeforeStartFile())
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		
		return ;
	}


	CString strSerOpen = _T("�����ļ�����������\r\n");
	CString strRecvErro = _T("�����ļ�RecvData�ӿڳ���\r\n");
	CString strSockErro = _T("�ļ����շ�������ʧ��,�����Ƕ˿ڱ�ռ��,����\r\n");
	CString strRecvReady = _T("׼����ʼ�����ļ�\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(_T(""));
    CString strSavePath_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);

	//���ڴ��䣬�����ٴ�����
	if (1 == m_pTransControlEx_File->GetState())
	{
		m_lstLogFile.InsertItem(0, GetLog(General, strSerOpen));
		COutLogMgr::GetInstance()->LogWriter(General, strSerOpen.GetBuffer(strSerOpen.GetLength()));
        ((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(TRUE);
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strRecvReady);
		return;
	}
    
	BOOL bRet = m_pTransControlEx_File->CreateSocket(_T(""),0);
	if(!bRet)
    {
		COutLogMgr::GetInstance()->LogWriter(Failure, strSockErro.GetBuffer(strSockErro.GetLength()));
		m_lstLogFile.InsertItem(0, GetLog(Failure, strSockErro));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strSockErro);
		return ;
	}

     bRet = m_pTransControlEx_File->RecvFile(strSavePath_File);
	if(!bRet)
    {
		COutLogMgr::GetInstance()->LogWriter(Failure, strRecvErro.GetBuffer(strRecvErro.GetLength()));
	    m_lstLogFile.InsertItem(0, GetLog(Failure, strRecvErro));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strRecvErro);
		//((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(FALSE);
		//((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(TRUE);
	    //((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(TRUE);
		BCGPMessageBox(NULL, strRecvErro, _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		OnStopFile();
		return;
	}
    
	COutLogMgr::GetInstance()->LogWriter(Success, strRecvReady.GetBuffer(strRecvReady.GetLength()));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strRecvReady);
	m_lstLogFile.InsertItem(0, GetLog(Success, strRecvReady));

	 ((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(FALSE);
	 ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(TRUE);
	 ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(TRUE);

	 this->SetFocus();
}

BOOL CDlgDataReceive::StopServ_FileByUser()//����ֵ:�������Ƿ�������ļ�����ֹͣ�����Ƿ�
{
	if(1 == m_pTransControlEx_File->GetState())//�Ѿ�����ֹͣ״̬
	{
		if( IDYES != BCGPMessageBox(NULL, _T("����׼�������ļ����Ƿ�ȷ��ֹͣ����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(TRUE);
        	((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(FALSE);

			return FALSE;
		}
	}
    //ֹͣ�ļ�����
	m_pTransControlEx_File->StopTrans();

	CString strStopMsg = _T("�����ļ�����ֹͣ\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(_T(""));
	
	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_FILE))->SetWindowText(strStopMsg);
	COutLogMgr::GetInstance()->LogWriter(Success, strStopMsg.GetBuffer(strStopMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success, strStopMsg));
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);


	return TRUE;
}
BOOL CDlgDataReceive::StopServ_DataByUser()//����ֵ:�������Ƿ���������ݷ���ֹͣ�����Ƿ�
{
	if(1 == m_pTransControlEx_Data->GetState())//���ڽ���
	{
		if( IDYES != BCGPMessageBox(NULL, _T("����׼���������ݣ��Ƿ�ȷ��ֹͣ����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(TRUE);
        	((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(FALSE);

			return FALSE;
		}
	}
	
	//ֹͣ����
	m_pTransControlEx_Data->StopTrans();
    

	CString strStopServ = _T("ֹͣ���ݽ��շ���\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(_T(""));
	
	
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(TRUE);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strStopServ);
	COutLogMgr::GetInstance()->LogWriter(Success, strStopServ.GetBuffer(strStopServ.GetLength()));
	m_lstLogData.InsertItem(0, GetLog(Success,strStopServ));


	return TRUE;
}
BOOL CDlgDataReceive::StopServ_MediaByUser()//����ֵ:�������Ƿ��������ý�����ֹͣ�����Ƿ�
{
	if(1 == m_pTransControlEx_Media->GetState())//�Ѿ�����ֹͣ״̬
	{
		if( IDYES != BCGPMessageBox(NULL, _T("����׼��������ý�壬�Ƿ�ȷ��ֹͣ����"), _T("��ʾ��"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(TRUE);
	        ((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(FALSE);

			return FALSE;
		}
	}

	//ֹͣ����
	m_pTransControlEx_Media->StopTrans();

	CString strStopServ = _T("ֹͣ��ý����շ���\r\n");
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(_T(""));
	
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(TRUE);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strStopServ);
	COutLogMgr::GetInstance()->LogWriter(Success, strStopServ.GetBuffer(strStopServ.GetLength()));
	m_lstLogMedia.InsertItem(0, GetLog(Success,strStopServ));

	return TRUE;
}
/***************************************************************
  �� �� ����void OnStopFile()
  ����������ֹͣ�ļ�����
  ��    ������	
  ȫ�ֱ�������	  
  �� �� ֵ: ��		
****************************************************************/
void CDlgDataReceive::OnStopFile() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_RECV))
		return ;
	
    StopServ_FileByUser();

	this->SetFocus();
}

/***************************************************************
�� �� ����void OnRestartFile()
���������������ļ����շ��� 
  ��    ������	
  ȫ�ֱ�������	  
  �� �� ֵ: ��		
****************************************************************/
void CDlgDataReceive::OnRestartFile() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
    if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_RECV))
		return ;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	if(bStop)//�Ѿ���������״̬
	{
		if(StopServ_FileByUser())
			OnStartFile();
	}
	else//����ֹͣ״̬
	{
    	//OnStartFile();
		//OnStopFile();
	}

	this->SetFocus();
}

/***************************************************************
��������void OnSaveParamFile()

  ���������������ļ����ղ���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnSaveParamFile() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_RECV))
		return;
     CString strMsg;
	 strMsg.Format(_T("����%s�������\r\n"),MODULE_NAME_FILE_RECV);

	//�����Ϸ��Լ��:��ȡ����Ԫ�أ��Ϸ��Լ�飬д�����ļ�
	if(!ValidateParam_FileModule())
		return;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	if(bStop)//��������״̬,��ֹͣ
	   m_pTransControlEx_File->StopTrans();
	InitServ_File();
	if(bStop)//ԭʼ״̬Ϊ����״̬����ʼ����Ҳ�Զ�����
	    OnStartFile();

	//����־
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success,strMsg));
}


/***************************************************************
��������void OnStartData()

  ������������ʼ���ݽ���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnStartData() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
    if(!ValidatePermissions(m_bInitDlg, MODULE_NAME_DATA_RECV))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->EnableWindow(FALSE);
	    ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);

		return ;
	}

    if(!CheckUiAndXmlBeforeStartData())
		return ;

	CString strSerOpen = _T("�������ݷ���������\r\n");
	CString strSocketErro = _T("�������ݷ�������ʧ�ܣ��˿ڿ��ܱ�ռ�ã�����\r\n");
	CString strRecvErro = _T("��������RecvData�ӿڳ���\r\n");
	CString strRecvReady = _T("׼����ʼ��������\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(_T(""));
 

	//���ڴ��䣬�����ٴ�����
	if (1 == m_pTransControlEx_Data->GetState())
	{
		m_lstLogData.InsertItem(0, GetLog(General,strSerOpen));
		COutLogMgr::GetInstance()->LogWriter(General, strSerOpen.GetBuffer(strSerOpen.GetLength()));
        ((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(TRUE);
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strRecvReady);
		return;
	}
    
	BOOL bRet = m_pTransControlEx_Data->CreateSocket(_T(""),0);
	if (!bRet)
	{
		COutLogMgr::GetInstance()->LogWriter(Failure, strSocketErro.GetBuffer(strSocketErro.GetLength()));
		m_lstLogData.InsertItem(0, GetLog(Failure,strSocketErro));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strSocketErro);
		return ;
	}
	//modify 14-01-16 lgp from yn begin
	//bRet = m_pTransControlEx_Data->RecvData(_T(""));
	bRet = m_pTransControlEx_Data->RecvData();
	 //modify 14-01-16 lgp from yn end
	if(!bRet)
    {
		COutLogMgr::GetInstance()->LogWriter(Failure, strRecvErro.GetBuffer(strRecvErro.GetLength()));
	    m_lstLogData.InsertItem(0, GetLog(Failure,strRecvErro));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strRecvErro);
		BCGPMessageBox(NULL, strRecvErro, _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		OnStopData();
		return;
	}
    

	COutLogMgr::GetInstance()->LogWriter(Success, strRecvReady.GetBuffer(strRecvReady.GetLength()));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_DATA))->SetWindowText(strRecvReady);
	m_lstLogData.InsertItem(0, GetLog(Success,strRecvReady));

	 ((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(FALSE);
	 ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->EnableWindow(TRUE);
	 ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(TRUE);

	 this->SetFocus();
}

/***************************************************************
��������void OnStopData()

  ����������ֹͣ���ݽ���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnStopData() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
	if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_RECV))
		return;
    
    StopServ_DataByUser();

    this->SetFocus();

}

/***************************************************************
��������void OnRestartData()

  �����������������ݽ��շ���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnRestartData() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
    if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_RECV))
		return ;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->IsWindowEnabled();
	if(bStop)//�Ѿ���������״̬
	{
	   if(StopServ_DataByUser())
		   OnStartData();
	}
	else
	{
		//OnStartData();
		//OnStopData();
	}

    this->SetFocus();
	
}

/***************************************************************
��������void OnSaveParamData()

  �����������������ݽ��ղ���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnSaveParamData() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
	if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_RECV))
		return;
    CString strMsg;
	strMsg.Format(_T("����%s�������\r\n"),MODULE_NAME_DATA_RECV);

	//�����Ϸ��Լ��
	if(!ValidateParam_DataModule())
		return;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA_RECV))->IsWindowEnabled();
	if(bStop)//��������״̬����ֹͣ
	    m_pTransControlEx_Data->StopTrans();
	InitServ_Data();
	if(bStop)//ԭʼ״̬Ϊ����״̬�����ʼ�����Զ�����
	    OnStartData();
	
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	m_lstLogData.InsertItem(0, GetLog(Success,strMsg));

}
/***************************************************************
��������void OnStartMedia()

  ������������ʼ������ý������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnStartMedia() 
{
	// TODO: Add your control notification handler code here

	//Ȩ����֤
    if(!ValidatePermissions(m_bInitDlg, MODULE_NAME_MEDIA_RECV))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);

		return ;
	}

	if(!CheckUiAndXmlBeforeStartMedia())
		return ;

	CString strSerOpen = _T("������ý�����ݷ���������\r\n");
	CString strSocketErro = _T("������ý���������ʧ�ܣ��˿ڿ��ܱ�ռ�ã�����\r\n");
	CString strRecvErro = _T("������ý������RecvData�ӿڳ���\r\n");
	CString strRecvReady = _T("׼����ʼ������ý������\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(_T(""));
	
	//���ڴ��䣬�����ٴ�����
	if (1 == m_pTransControlEx_Media->GetState())
	{
		m_lstLogMedia.InsertItem(0, GetLog(General,strSerOpen));
		COutLogMgr::GetInstance()->LogWriter(General, strSerOpen.GetBuffer(strSerOpen.GetLength()));
        ((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(TRUE);
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strRecvReady);
		return;
	}
    
    
	BOOL bRet = m_pTransControlEx_Media->CreateSocket(_T(""),0);
	if (!bRet)
	{
		COutLogMgr::GetInstance()->LogWriter(Failure, strSocketErro.GetBuffer(strSocketErro.GetLength()));
		m_lstLogMedia.InsertItem(0, GetLog(Failure,strSocketErro));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strSocketErro);
		return ;
	}

     //modify 14-01-16 lgp from yn begin
     //bRet = m_pTransControlEx_Media->RecvData(_T(""));
	 bRet = m_pTransControlEx_Media->RecvMedia();
	 //modify 14-01-16 lgp from yn end
	if(!bRet)
    {
		COutLogMgr::GetInstance()->LogWriter(Failure, strRecvErro.GetBuffer(strRecvErro.GetLength()));
		m_lstLogMedia.InsertItem(0, GetLog(Failure,strRecvErro));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strRecvErro);
		BCGPMessageBox(NULL, strRecvErro, _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
		OnStopMedia();
		return;
	}
    
	COutLogMgr::GetInstance()->LogWriter(Success, strRecvReady.GetBuffer(strRecvReady.GetLength()));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_CURRENT_MEDIA))->SetWindowText(strRecvReady);
	m_lstLogMedia.InsertItem(0, GetLog(Success,strRecvReady));
	
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(TRUE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(TRUE);

	this->SetFocus();
}

/***************************************************************
��������void OnStopMedia()

  ����������ֹͣ������ý������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnStopMedia() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
	if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_RECV))
		return;
    
    StopServ_MediaByUser();
	
	this->SetFocus();
}

/***************************************************************
��������void OnRestartMedia()

  ��������������������ý�����ݷ���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnRestartMedia() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
    if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_RECV))
		return ;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled();
	if(bStop)//�Ѿ���������״̬
	{
		if(StopServ_MediaByUser())
			OnStartMedia();
	}
	else
	{
		//OnStartMedia();
		//OnStopMedia();
	}

	this->SetFocus();
}

/***************************************************************
��������void OnSaveParamMedia()

  ����������������ý����ս������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CDlgDataReceive::OnSaveParamMedia() 
{
	// TODO: Add your control notification handler code here
	//Ȩ����֤
	if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_RECV))
		return;
    CString strMsg;
	strMsg.Format(_T("����%s�������\r\n"),MODULE_NAME_MEDIA_RECV);
	
	//�����Ϸ��Լ��
	if(!ValidateParam_MediaModule())
		return;
	
	
	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled();
	if(bStop)//��������״̬����ֹͣ
	    m_pTransControlEx_Media->StopTrans();
	InitServ_Media();
	if(bStop)//ԭʼ״̬Ϊ����״̬�����ʼ�����Զ�����
	    OnStartMedia();
	
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	m_lstLogMedia.InsertItem(0, GetLog(Success,strMsg));
}



void CDlgDataReceive::OnDestroy() 
{
	if(NULL != m_pTransControlEx_File)
	{
		m_pTransControlEx_File->closeRegist();
		m_pTransControlEx_File->StopTrans();
		delete m_pTransControlEx_File;
		m_pTransControlEx_File = NULL;
	}
	if(NULL != m_pTransControlEx_Data)
	{
	    m_pTransControlEx_Data->closeRegist();
		m_pTransControlEx_Data->StopTrans();
 		delete m_pTransControlEx_Data;
 		m_pTransControlEx_Data = NULL;
	}
	if(NULL != m_pTransControlEx_Media)
	{
		m_pTransControlEx_Media->closeRegist();
		m_pTransControlEx_Media->StopTrans();
		delete m_pTransControlEx_Media;
		m_pTransControlEx_Media = NULL;
	}
    CTransControlEx::GetInstance()->closeRegist();
	CTransControlEx::GetInstance()->StopTrans();

	if(PathFileExists(_T("DXSJCS.bat")))
	{
		DeleteFile(_T("DXSJCS.bat"));
	}

	if(PathFileExists(_T("DXSJCS.vbs")))
	{
		DeleteFile(_T("DXSJCS.vbs"));
	}

	CBCGPDialog::OnDestroy();
	
	
	// TODO: Add your message handler code here
	
}

void CDlgDataReceive::SoftReg(CStringList& strLstSoft, CString strSoftInfo)
{
	POSITION pos = strLstSoft.Find(strSoftInfo);
	if(NULL == pos)
		strLstSoft.AddTail(strSoftInfo);

   int nCount = strLstSoft.GetCount();
}
void CDlgDataReceive::SoftUnReg(CStringList& strLstSoft,CString strSoftInfo)
{
	for(int n=0; n<strLstSoft.GetCount();n++)
	{
		POSITION pos = strLstSoft.Find(strSoftInfo);
		if(NULL != pos)
		{
			strLstSoft.RemoveAt(pos);
		}
	}
	int nCount = strLstSoft.GetCount();
}
/***************************************************************
��������HRESULT OnSoftReg()

  �������������ע����Ϣ��Ӧ����
  
  ������WPARAM wpa  ����������
        
        LPARAM lpa  StSoftInfo���󣬴洢�����������Ϣ��
		            ������Ӧ֧�ֶ����ע�ᣬ
					ͳһ�������ע����Զ��滻��������
	
  ȫ�ֱ�������
	  
  ����ֵ: HRESULT
		
****************************************************************/
 HRESULT CDlgDataReceive::OnSoftReg(WPARAM wpa, LPARAM lpa)
 {
	 //HWND hWnd = (HWND)wpa;

	 HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, "SockSrvDataMap");
	 if (m_hReceiveMap == NULL)
		 return 0 ;
	 LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,FILE_MAP_READ,0,0,0);
	 if (m_lpbReceiveBuf == NULL)
	 {
		 CloseHandle(m_hReceiveMap);
		 m_hReceiveMap=NULL;
		 COutLogMgr::GetInstance()->LogWriter(Failure, _T("���յ����������ն�ע������ʱ����ȡ�ڴ�ӳ�����ݳ���\r\n"));
		 m_lstLogMedia.InsertItem(0, GetLog(Failure, _T("���յ����������ն�ע������ʱ����ȡ�ڴ�ӳ�����ݳ���\r\n")));
		 return 0;
	 }


	 char chPathD[MAX_PATH];
	 memset(chPathD, 0, MAX_PATH);
	 for (int ii=0; ii<sizeof(StSoftInfo); ii++)
	 {
		 int a = *(m_lpbReceiveBuf+ii);
		 chPathD[ii] = a;
	 }


	 StSoftInfo* pSoftInfo = (StSoftInfo*)chPathD;

     CString strSoftName;
	 strSoftName.Format(_T("%s"), pSoftInfo->m_chSoftName);

	 switch(pSoftInfo->m_nRecType)
	 {
	 case 1://�ļ�
		 {
			 SoftReg(m_lstSoftReg_File, strSoftName);
			 CString strMsg;
			 strMsg.Format(_T("������ %s ��ע��\r\n"),strSoftName);
			 
			 POSITION pos = m_lstSoftReg_File.Find(strSoftName);
			 if(NULL != pos)
			 {
			    m_lstLogFile.InsertItem(0, GetLog(Success,strMsg));
				COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));
			 }
		 }
		  return 0;
	 case 2://����
		 {
             SoftReg(m_lstSoftReg_Data, strSoftName);
			 CString strMsg;
			 strMsg.Format(_T("������ %s ��ע��\r\n"),strSoftName);

			 POSITION pos = m_lstSoftReg_Data.Find(strSoftName);
			 if(NULL != pos)
			 {
			    m_lstLogData.InsertItem(0, GetLog(Success,strMsg));
				COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));
			 }
		 }
		  return 0;
	 case 3://��ý��
		 {
			 SoftReg(m_lstSoftReg_Media, strSoftName);
			 CString strMsg;
			 strMsg.Format(_T("������ %s ��ע��\r\n"),strSoftName);

			 POSITION pos = m_lstSoftReg_Media.Find(strSoftName);
			 if(NULL != pos)
			 {
			     m_lstLogMedia.InsertItem(0, GetLog(Success,strMsg));
				 COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));
			 }
		 }
		 return 0;
	 default:
		 return 0;
	 }
	 return 0;
 }
 /***************************************************************
��������HRESULT OnSoftUnReg()

  �������������ע����Ϣ��Ӧ����
  
  ������WPARAM wpa  ����������
        
        LPARAM lpa  StSoftInfo���󣬴洢�����������Ϣ��
	
  ȫ�ֱ�������
	  
  ����ֵ: HRESULT
		
****************************************************************/
 HRESULT CDlgDataReceive::OnSoftUnReg(WPARAM wpa, LPARAM lpa)
 {
	 HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, "SockSrvDataMap");
	 if (m_hReceiveMap == NULL)
		 return 0 ;
	 LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,FILE_MAP_READ,0,0,0);
	 if (m_lpbReceiveBuf == NULL)
	 {
		 CloseHandle(m_hReceiveMap);
		 m_hReceiveMap=NULL;
		 COutLogMgr::GetInstance()->LogWriter(Failure, _T("���յ����������ն�ע������ʱ����ȡ�ڴ�ӳ�����ݳ���\r\n"));
		 m_lstLogMedia.InsertItem(0, GetLog(Failure, _T("���յ����������ն�ע������ʱ����ȡ�ڴ�ӳ�����ݳ���\r\n")));
		 return 0;
	 }
	 
	 
	 char chPathD[MAX_PATH];
	 memset(chPathD, 0, MAX_PATH);
	 for (int ii=0; ii<sizeof(StSoftInfo); ii++)
	 {
		 int a = *(m_lpbReceiveBuf+ii);
		 chPathD[ii] = a;
	 }
	 
	 StSoftInfo* pSoftInfo = (StSoftInfo*)chPathD;
	 
     CString strSoftName;
	 strSoftName.Format(_T("%s"), pSoftInfo->m_chSoftName);
	 
	 switch(pSoftInfo->m_nRecType)
	 {
	 case 1://�ļ�
		 {
			 SoftUnReg(m_lstSoftReg_File, strSoftName);
			 CString strMsg;
			 strMsg.Format(_T("������ %s ��ע��\r\n"), strSoftName);
			 m_lstLogFile.InsertItem(0, GetLog(Success, strMsg));
			 COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));
		 }
		 return 0;
	 case 2://����
		 {
             SoftUnReg(m_lstSoftReg_Data, strSoftName);
			 CString strMsg;
			 strMsg.Format(_T("������ %s ��ע��\r\n"),strSoftName);
			 m_lstLogData.InsertItem(0, GetLog(Success,strMsg));
			 COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));
		 }
		 return 0;
	 case 3://��ý��
		 {
			 SoftUnReg(m_lstSoftReg_Media, strSoftName);
			 CString strMsg;
			 strMsg.Format(_T("������ %s ��ע��\r\n"),strSoftName);
			 m_lstLogMedia.InsertItem(0, GetLog(Success, strMsg));
			 COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));
		 }
		 return 0;
	 default:
		 return 0;
	 }
 }
 void CDlgDataReceive::OnClearRecvFileLog()
 {
	 
	 ((CListCtrl*)GetDlgItem(IDC_LST_FILE))->DeleteAllItems();

 }
 void CDlgDataReceive::OnClearRecvDataLog()
 {
	 
	 ((CListCtrl*)GetDlgItem(IDC_LST_DATA))->DeleteAllItems();
	 
 }
 void CDlgDataReceive::OnClearRecvMediaLog()
 {
	 
	 ((CListCtrl*)GetDlgItem(IDC_LST_MEDIA))->DeleteAllItems();

 }
 void CDlgDataReceive::OnRclickLstLogFile(NMHDR* pNMHDR, LRESULT* pResult) 
 {
	 // TODO: Add your control notification handler code here
	 POINT pt;
	 GetCursorPos(&pt);
	 m_lstLogFile.ScreenToClient(&pt);
	 LVHITTESTINFO info;
	 info.pt=pt;
	 info.flags=LVHT_ONITEMLABEL;
	 if(m_lstLogFile.SubItemHitTest(&info)<0)
	 {
		 return;
	 }
	 CMenu obMenu;
	 obMenu.LoadMenu(IDR_MENU_RECV_FILE); 
	 CMenu* pPopupMenu = obMenu.GetSubMenu(0);
	 ASSERT(pPopupMenu); 
	 // Get the cursor position
	 CPoint obCursorPoint = (0, 0);
	 GetCursorPos(&obCursorPoint);
	 // Track the popup menu
	 pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, obCursorPoint.x, 
		 obCursorPoint.y, this);
	 *pResult = 0;
 }
 
 void CDlgDataReceive::OnRclickLstLogData(NMHDR* pNMHDR, LRESULT* pResult) 
 {
	 // TODO: Add your control notification handler code here
	 POINT pt;
	 GetCursorPos(&pt);
	 m_lstLogData.ScreenToClient(&pt);
	 LVHITTESTINFO info;
	 info.pt=pt;
	 info.flags=LVHT_ONITEMLABEL;
	 if(m_lstLogData.SubItemHitTest(&info)<0)
	 {
		 return;
	 }
	 CMenu obMenu;
	 obMenu.LoadMenu(IDR_MENU_RECV_DATA); 
	 CMenu* pPopupMenu = obMenu.GetSubMenu(0);
	 ASSERT(pPopupMenu); 
	 // Get the cursor position
	 CPoint obCursorPoint = (0, 0);
	 GetCursorPos(&obCursorPoint);
	 // Track the popup menu
	 pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, obCursorPoint.x, 
		 obCursorPoint.y, this);
	 *pResult = 0;
 }
 void CDlgDataReceive::OnRclickLstLogMedia(NMHDR* pNMHDR, LRESULT* pResult) 
 {
	 // TODO: Add your control notification handler code here
	 POINT pt;
	 GetCursorPos(&pt);
	 m_lstLogMedia.ScreenToClient(&pt);
	 LVHITTESTINFO info;
	 info.pt=pt;
	 info.flags=LVHT_ONITEMLABEL;
	 if(m_lstLogMedia.SubItemHitTest(&info)<0)
	 {
		 return;
	 }
	 CMenu obMenu;
	 obMenu.LoadMenu(IDR_MENU_RECV_MEDIA); 
	 CMenu* pPopupMenu = obMenu.GetSubMenu(0);
	 ASSERT(pPopupMenu); 
	 // Get the cursor position
	 CPoint obCursorPoint = (0, 0);
	 GetCursorPos(&obCursorPoint);
	 // Track the popup menu
	 pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, obCursorPoint.x, 
		 obCursorPoint.y, this);
	 *pResult = 0;
}
 CString CDlgDataReceive::GetLog(ENUM_LOG_TYPE enuLogType, CString strErrinfo)
 {
	 
	 CString strLogRet;
	 switch(enuLogType)
	 {
	 case Success:
		 strLogRet.Format(_T("[Success]"));
		 break;
	 case Failure:
		 strLogRet.Format(_T("[Failure]")); 
		 break;
	 case General:
		 strLogRet.Format(_T("[General]"));  
		 break;
	 }
	 
	 //������־���ݸ�ʽ
	 CTime time = CTime::GetCurrentTime();
	 CString strTime;
	 strTime.Format(_T("%d-%d-%d %d:%d:%d"),
		 time.GetYear(),time.GetMonth(),
		 time.GetDay(),time.GetHour(),
		 time.GetMinute(),time.GetSecond()); 
	 
	 CString strMsg;
	 strMsg.Format(_T("%s %s  %s"),strLogRet,strTime,strErrinfo );
	 return  strMsg;
}