/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgDataSend.cpp
* 摘    要：数据发送界面类
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

*/

#include "stdafx.h"
#include "UnidirectDataTransfer.h"
#include "DlgDataSend.h"
#include "OutLogIn.h"
#include "OutLogMgr.h"
#include <shlwapi.h>
#include <winsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MSG_SEND_FILE  WM_USER+1001
#define MSG_SEND_DATA  WM_USER+1002
#define MSG_SEND_MEDIA WM_USER+1003
#define MSG_TEST_XN    WM_USER+1004

#define MODULE_NAME_FILE_SEND  _T("文件传输模块")
#define MODULE_NAME_DATA_SEND  _T("数据传输模块")
#define MODULE_NAME_MEDIA_SEND _T("流媒体传输模块")
#define MODULE_NAME_ALL        _T("全部模块")
#define MODULE_NAME_END_SEND   _T("终端设备信息模块")

#define LEN_DATA            1301

extern CUnidirectDataTransferApp theApp;


BOOL g_bMonitorFile = TRUE;
BOOL g_bSendFile = TRUE;

BOOL g_bSendData = TRUE;
BOOL g_bSendMedia = TRUE;
/***************************************************************
函数名：void CALLBACK TransCallSendFile(int nTransType, int nMesType,int nLen,unsigned char* buff)

  功能描述：回调函数 传输文件
  
	参数：int nTransType       传输类型
	int nMesType         暂无定义
	int nLen             数据长度
	unsigned char* buff  所接收的数据
	全局变量：无
	
	  返回值: 无
	  
****************************************************************/
void CALLBACK TransCallSendFile(int nTransType, int nMesType,int nLen,unsigned char* buff)
{
	
}
/***************************************************************
函数名：void CALLBACK feedBackSendFile(LONG lType,void* pValue)

  功能描述：回调函数 反映当前程序状况
  
	参数：LONG lType     参数类型
	void* pValue   参数值(指针形式)
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CALLBACK feedBackSendFile(LONG lType,void* pValue, void* pFileName)
{
	CDlgDataSend* pWnd = (CDlgDataSend*)theApp.GetMainWnd();
	
	switch(lType)
	{
	case UPDATE_PROGRESS://更新进度条
		{
			//add lgp 11-07 begin
			if(pWnd->m_bNoDelStopFileServ)
			{
			
				return;
			}
            //add lgp 11-07 end

			LPSTR strFileName = (LPSTR)pFileName;
            pWnd->m_strFileSending = strFileName;
			
			LONG* pVal = (LONG*)pValue;
			CString strValue;
			strValue.Format(_T("当前进度: 已传输%d%%"), *pVal);
			PostMessage(pWnd->GetSafeHwnd(), WM_SEND_SPEED, *pVal,0); 
			
		}
		break;
	case SAFE_CHECK_ERROR://安全检查错误
		{
			//(((CBCGPEdit*)((CDlgDataSend*)theApp.GetMainWnd())->GetSafeHwnd())->GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(_T("安全检查出错"));
		}
		break;
	case BOTHWAY_CHECK_FAIL:  //双向校验回路失败
		{
			//(((CBCGPEdit*)((CDlgDataSend*)theApp.GetMainWnd())->GetSafeHwnd())->GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(_T("双向校验回路失败"));
			pWnd->m_lockSendFileStatus.Lock();
			pWnd->m_eSendFileStatus = ENU_SEND_UN_REG;
			pWnd->m_lockSendFileStatus.Unlock();
		}
		break;
	case TRANS_COMPLETE://文件发送完毕
		{
            //PostMessage(((CDlgDataSend*)theApp.GetMainWnd())->GetSafeHwnd(), WM_SEND_OVER, 0,0); 
			pWnd->m_pTransControl_File->LogRegisNo();
			pWnd->m_lockSendFileStatus.Lock();
			pWnd->m_eSendFileStatus = ENU_SEND_OVER;
			pWnd->m_lockSendFileStatus.Unlock();
		}
		break;
	case START_SENDDATA://双向回路校验--得到注册号
		{
			pWnd->m_lockSendFileStatus.Lock();
			pWnd->m_eSendFileStatus = ENU_SEND_REG;
			pWnd->m_lockSendFileStatus.Unlock();
		}
		break;
	default:
		break;
	}
}
/***************************************************************
函数名：void CALLBACK TransCallSendData(int nTransType, int nMesType,int nLen,unsigned char* buff)

  功能描述：回调函数 传输数据
  
	参数：int nTransType       传输类型
	int nMesType         暂无定义
	int nLen             数据长度
	unsigned char* buff  所接收的数据
	全局变量：无
	
	  返回值: 无
	  
****************************************************************/
void CALLBACK TransCallSendData(int nTransType, int nMesType,int nLen,unsigned char* buff)
{
	
}

/***************************************************************
函数名：void CALLBACK feedBackSendData(LONG lType,void* pValue)

  功能描述：回调函数 反映当前程序状况
  
	参数：LONG lType     参数类型
	void* pValue   参数值(指针形式)
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CALLBACK feedBackSendData(LONG lType,void* pValue, void* pNULL)
{
	CDlgDataSend* pWnd = (CDlgDataSend*)theApp.GetMainWnd();
	switch(lType)
	{
	case SAFE_CHECK_ERROR://安全码错误
		{
			
		}
		break;
	case BOTHWAY_CHECK_FAIL://双向校验回路失败
		{
		}
		break;
	case TRANS_ERROR://TRANS_ERROR
		{
		}
		break;
	case START_SENDDATA://接收到注册码，开始发数
		{
			//if(pWnd->m_bDataInTestXn)
			//{
			pWnd->m_eSendDataStatus = ENU_SEND_REG;
			/*}
			else
			{
			pWnd->m_pTransControl_Data->SendData((byte*)pWnd->m_chData, pWnd->m_uDataLen);
			memset(pWnd->m_chData, 0 , pWnd->m_uDataLen);
			pWnd->m_pTransControl_Data->LogRegisNo();
			
			  pWnd->m_lstLogData.InsertItem(0, pWnd->GetLog(Success,_T("成功发送数据\r\n")));
			  COutLogMgr::GetInstance()->LogWriter(Success,_T("成功发送数据\r\n"));
		}*/
		}
		break;
	case TRANS_COMPLETE://数据发送完成
		{	
		}
		break;
	default:
		break;
	}
}
/***************************************************************
函数名：void CALLBACK TransCallSendMedia(int nTransType, int nMesType,int nLen,unsigned char* buff)

  功能描述：回调函数 流媒体传输
  
	参数：int nTransType       传输类型
	int nMesType         暂无定义
	int nLen             数据长度
	unsigned char* buff  所接收的数据
	全局变量：无
	
	  返回值: 无
	  
****************************************************************/
void CALLBACK TransCallSendMedia(int nTransType, int nMesType,int nLen,unsigned char* buff)
{
	
}

/***************************************************************
函数名：void CALLBACK feedBackSendMedia(LONG lType,void* pValue)

  功能描述：回调函数 反映当前程序状况
  
	参数：LONG lType     参数类型
	void* pValue   参数值(指针形式)
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CALLBACK feedBackSendMedia(LONG lType,void* pValue,void* pNULL)
{
	CDlgDataSend* pWnd = (CDlgDataSend*)theApp.GetMainWnd();
	switch(lType)
	{
	case SAFE_CHECK_ERROR://安全码错误
		{
			
		}
		break;
	case BOTHWAY_CHECK_FAIL://双向校验回路失败
		{
			
		}
		break;
	case TRANS_ERROR://TRANS_ERROR
		{
			
		}
		break;
	case START_SENDDATA://接收到注册码，开始发数
		{
		/*pWnd->m_pTransControl_Media->SendData((byte*)pWnd->m_chMedia, pWnd->m_uMediaLen);
		memset(pWnd->m_chMedia, 0 , pWnd->m_uMediaLen);
		pWnd->m_pTransControl_Media->LogRegisNo();
		pWnd->m_lstLogMedia.InsertItem(0, pWnd->GetLog(Success,_T("成功发送流媒体\r\n")));
			COutLogMgr::GetInstance()->LogWriter(Success, _T("成功发送流媒体\r\n"));*/
			pWnd->m_eSendMediaStatus = ENU_SEND_REG;
		}
		break;
	case TRANS_COMPLETE://数据发送完成
		{
		}
		break;
	default:
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CDlgDataSend dialog


CDlgDataSend::CDlgDataSend(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgDataSend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDataSend)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTransControl_File = NULL;
	m_pTransControl_Data = NULL;
	m_pTransControl_Media = NULL;
}


void CDlgDataSend::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDataSend)
	DDX_Control(pDX, IDC_LST_LOG_MEDIA, m_lstLogMedia);
	DDX_Control(pDX, IDC_LST_LOG_DATA, m_lstLogData);
	DDX_Control(pDX, IDC_LST_LOG_FILE, m_lstLogFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDataSend, CBCGPDialog)
//{{AFX_MSG_MAP(CDlgDataSend)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_RDO_DELETE_SENDED_FILE, OnRdoDeleteSendedFile)
ON_BN_CLICKED(IDC_RAO_SAVE_SENDED_FILE, OnRaoSaveSendedFile)
ON_NOTIFY(NM_RCLICK, IDC_LST_LOG_FILE, OnRclickLstLogFile)
ON_NOTIFY(NM_RCLICK, IDC_LST_LOG_DATA, OnRclickLstLogData)
ON_NOTIFY(NM_RCLICK, IDC_LST_LOG_MEDIA, OnRclickLstLogMedia)
	ON_EN_CHANGE(IDC_EDT_PORT_LOCAL_FILE, OnChangeEdtPortLocalFile)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_AIM_FILE, OnFieldchangedIpAimFile)
	ON_EN_CHANGE(IDC_EDT_PORT_RECEIVE_FILE, OnChangeEdtPortReceiveFile)
	ON_BN_DOUBLECLICKED(IDC_RDO_DELETE_SENDED_FILE, OnDoubleclickedRdoDeleteSendedFile)
	ON_BN_DOUBLECLICKED(IDC_RAO_SAVE_SENDED_FILE, OnDoubleclickedRaoSaveSendedFile)
	ON_BN_CLICKED(IDC_CHK_CHECK_FILE, OnChkCheckFile)
	ON_EN_CHANGE(IDC_EDT_PORT_LOCAL_END, OnChangeEdtPortLocalEnd)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_LOCAL_END, OnFieldchangedIpLocalEnd)
	ON_EN_CHANGE(IDC_EDT_PORT_RECEIVE_END, OnChangeEdtPortReceiveEnd)
ON_BN_CLICKED(IDC_BUT_PATH_MONITOR, OnFilePathMonitor)
ON_BN_CLICKED(IDC_BUT_FILE_PATH, OnFilePath)
ON_BN_CLICKED(IDC_BUT_START_FILE, OnStartFile)
ON_BN_CLICKED(IDC_BUT_STOP_FILE, OnStopFile)
ON_BN_CLICKED(IDC_BUT_RESTART_FILE, OnRestartFile)
ON_BN_CLICKED(IDC_BUT_SAVE_PARAM_FILE, OnSaveParamFile)
ON_BN_CLICKED(IDC_BUT_START_DATA, OnStartData)
ON_BN_CLICKED(IDC_BUT_STOP_DATA, OnStopData)
ON_BN_CLICKED(IDC_BUT_RESTART_DATA, OnRestartData)
ON_BN_CLICKED(IDC_BUT_SAVE_PARAM_DATA, OnSaveParamData)
ON_BN_CLICKED(IDC_BUT_START_MEDIA, OnStartMedia)
ON_BN_CLICKED(IDC_BUT_STOP_MEDIA, OnStopMedia)
ON_BN_CLICKED(IDC_BUT_RESTART_MEDIA, OnRestartMedia)
ON_BN_CLICKED(IDC_BUT_SAVE_PARAM_MEDIA, OnSaveParamMedia)
ON_BN_CLICKED(IDC_BUT_SAVE_PARAM_END, OnSaveParamEnd)
ON_COMMAND(ID_CLEAR_SEND_FILE, OnClearSendFileLog)
ON_COMMAND(ID_CLEAR_SEND_DATA, OnClearSendDataLog)
ON_COMMAND(ID_CLEAR_SEND_MEDIA, OnClearSendMediaLog)
ON_MESSAGE(WM_SEND_SPEED, SpeedSending)
ON_MESSAGE(MSG_SEND_FILE, OnSendFile_Msg)
ON_MESSAGE(MSG_SEND_DATA, OnSendData_Msg)
ON_MESSAGE(MSG_SEND_MEDIA, OnSendMedia_Msg)
ON_MESSAGE(MSG_TEST_XN, OnSendData_TestXN)
	ON_BN_CLICKED(IDC_CHK_CIRCUIT, OnChkCircuit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDataSend message handlers
BOOL CDlgDataSend::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    Init();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void  CDlgDataSend::OnRdoDeleteSendedFile()
{
	if(1==((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->GetCheck())
	{
		((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	
	
}
void CDlgDataSend::OnRaoSaveSendedFile()
{
	if(1==((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->GetCheck())
	{
		//((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(TRUE);
		((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
}
void CDlgDataSend::OnClearSendFileLog()
{
    ((CListCtrl*)GetDlgItem(IDC_LST_LOG_FILE))->DeleteAllItems();
}
void CDlgDataSend::OnClearSendDataLog()
{
	
	((CListCtrl*)GetDlgItem(IDC_LST_LOG_DATA))->DeleteAllItems();
	
}
void CDlgDataSend::OnClearSendMediaLog()
{
	
	((CListCtrl*)GetDlgItem(IDC_LST_LOG_MEDIA))->DeleteAllItems();
	
}
DWORD WINAPI SendMediaThread(LPVOID pPa)
{
	CDlgDataSend* pWnd = ((CDlgDataSend*)theApp.GetMainWnd());
	if(NULL == pWnd)
		return 0;
	
	BOOL bCircuitCheck_End;
	CString strCircuitCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN);
	if(_T("1") == strCircuitCheck)
		bCircuitCheck_End = TRUE;
	if(_T("0") == strCircuitCheck)
		bCircuitCheck_End = FALSE;
	
    while(g_bSendMedia)
	{
		
		while(WaitForSingleObject(pWnd->m_hEventSendMedia, INFINITE ) == WAIT_OBJECT_0
			&& 
			((CEdit*)pWnd->GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled())
		{
			
			
			//if(pWnd->m_byteLstMedias.IsEmpty())
			//	continue;
		
			//if (!pWnd->m_byteLstMedias.IsEmpty())
			//	buf = pWnd->m_byteLstMedias.GetHead();
			while(pWnd->m_arrMedias.GetSize()>0)
			{
				//int nLenCount = pWnd->m_arrMedias.GetSize();
				pWnd->m_lockSendMediaStatus.Lock();
				CBufZoneManager buf;
				if(pWnd->m_arrMedias.GetUpperBound()>=1300)
				{
				    //modify 14-01-16 lgp from yn begin
					/*byte bTemp[1300];
					memset(bTemp, 0, 1300);
                    for(int i=0; i<1300;i++)
					{
						bTemp[i] = pWnd->m_arrMedias.GetAt(i);
					}
                    buf.SetBufData(bTemp, 1300);
                    pWnd->m_arrMedias.RemoveAt(0,1300);
                    */
					byte* bTemp = new byte[pWnd->m_arrMedias.GetUpperBound()];
					memset(bTemp, 0, pWnd->m_arrMedias.GetUpperBound());
                    for(int i=0; i<pWnd->m_arrMedias.GetUpperBound();i++)
					{
						bTemp[i] = pWnd->m_arrMedias.GetAt(i);
					}
                    buf.SetBufData(bTemp, pWnd->m_arrMedias.GetUpperBound()+1);
                    pWnd->m_arrMedias.RemoveAt(0,pWnd->m_arrMedias.GetUpperBound()+1);
					delete bTemp;
					//modify 14-01-16 lgp from yn end
                    //pWnd->m_arrMedias.SetSize(pWnd->m_arrMedias.GetSize()-1300);
					//int nLen = pWnd->m_arrMedias.GetSize();
				}
				else if(pWnd->m_arrMedias.GetUpperBound()<1300 && pWnd->m_arrMedias.GetUpperBound()>=0)
				{
					byte bTemp[1300];
					memset(bTemp, 0, 1300);
					int i=0;
                    for(; i<=pWnd->m_arrMedias.GetUpperBound();i++)
					{
						bTemp[i] = pWnd->m_arrMedias.GetAt(i);
					}
					bTemp[i]=0;
                    buf.SetBufData(bTemp, pWnd->m_arrMedias.GetUpperBound()+1);
                    pWnd->m_arrMedias.RemoveAll();
				}
				pWnd->m_lockSendMediaStatus.Unlock();

			
			
			pWnd->m_eSendMediaStatus = ENU_SEND_UN_REG;
			
			
			
			//安全检查和双向校验
			if(!bCircuitCheck_End && (ENU_SEND_UN_REG == pWnd->m_eSendMediaStatus||ENU_SEND_OVER == pWnd->m_eSendMediaStatus))
			{
				//pWnd->m_pTransControl_Media->SendData((byte*)pWnd->m_chMedia, pWnd->m_uMediaLen);
				pWnd->m_pTransControl_Media->SendData((byte*)buf.m_pDataBuf, buf.m_nBufLen);
				//continue;
			}
			
			if(bCircuitCheck_End && ENU_SEND_UN_REG == pWnd->m_eSendMediaStatus )
			{
				pWnd->m_eSendMediaStatus = ENU_SEND_REG_ING;
				pWnd->m_pTransControl_Media->ApplyRegisNo();
			}
			else
			{
				Sleep(10);
			}
			
			int nWaitRegScend = 0;
			while(ENU_SEND_UN_REG != pWnd->m_eSendMediaStatus)
			{  
				if( ENU_SEND_REG == pWnd->m_eSendMediaStatus)
				{
					pWnd->m_eSendMediaStatus = ENU_SEND_ING;
                    
					
                    
					pWnd->m_pTransControl_Media->SendData((byte*)buf.m_pDataBuf, buf.m_nBufLen);
                    
					pWnd->m_pTransControl_Media->LogRegisNo();
					pWnd->m_lockSendMediaStatus.Lock();
					//if(!pWnd->m_byteLstMedias.IsEmpty())
					//{
                    //    pWnd->m_byteLstMedias.RemoveHead();
					//}
					pWnd->m_lockSendMediaStatus.Unlock();
					pWnd->m_eSendMediaStatus = ENU_SEND_OVER;
					pWnd->m_lstLogMedia.InsertItem(0, pWnd->GetLog(Success,_T(" 成功发送流媒体")));
					break;					
				}
				
				if( ENU_SEND_REG_ING == pWnd->m_eSendMediaStatus)
				{
					if(nWaitRegScend >= 5000)//超时，重新发起注册
					{
                        pWnd->m_pTransControl_Media->ApplyRegisNo();
                        nWaitRegScend = 0;
					}
					
					Sleep(5);
					nWaitRegScend += 5;
					
					continue;
				}
				
			}
			}
		}
	}
	return S_OK;
}
/*DWORD WINAPI SendFileThreadFun( LPVOID lpParam ) 
{
CDlgDataSend* pWnd = (CDlgDataSend*)lpParam;
if (NULL == pWnd)
{
return 1;
}
while(WaitForSingleObject(pWnd->m_hEventSendFile, INFINITE))
{
// 		if (pWnd->m_bSendFileThreadClose)
// 		{
// 			break;
// 		}
int iTest = 0;
/////
/////
}
return 0;
}*/



DWORD WINAPI SendDataThread(LPVOID pParam )
{
	CDlgDataSend* pWnd = ((CDlgDataSend*)theApp.GetMainWnd());
	if(NULL == pWnd)
		return 0;
	
	BOOL bCircuitCheck_End;
	CString strCircuitCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN);
	if(_T("1") == strCircuitCheck)
		bCircuitCheck_End = TRUE;
	if(_T("0") == strCircuitCheck)
		bCircuitCheck_End = FALSE;
	
    while(g_bSendData)
	{    
		while(WaitForSingleObject(pWnd->m_hEventSendData, INFINITE)==WAIT_OBJECT_0
			&& 
			((CEdit*)pWnd->GetDlgItem(IDC_BUT_STOP_DATA))->IsWindowEnabled())
		{
			//if(pWnd->m_byteLstDatas.IsEmpty())
			//	continue;
			while(pWnd->m_arrDatas.GetSize()>0)
			{
				pWnd->m_lockSendDataStatus.Lock();
				CBufZoneManager buf;
				//mofify 14-01-16 lgp from yn begin
				//modify lgp 12-16 begin
				/*if(pWnd->m_arrDatas.GetUpperBound()>=1300)
				{
					byte bTemp[1300];
					memset(bTemp, 0, 1300);
                    for(int i=0; i<1300;i++)
					{
                      bTemp[i] = pWnd->m_arrDatas.GetAt(i);
					}
                    buf.SetBufData(bTemp, 1300);
                    pWnd->m_arrDatas.RemoveAt(0,1300);
				}
				else if(pWnd->m_arrDatas.GetUpperBound()<1300 && pWnd->m_arrDatas.GetUpperBound()>=0)
				{
					byte bTemp[1300];
					memset(bTemp, 0, 1300);
					int i=0;
                    for(; i<=pWnd->m_arrDatas.GetUpperBound();i++)
					{
						bTemp[i] = pWnd->m_arrDatas.GetAt(i);
					}
					bTemp[i]=0;
                    buf.SetBufData(bTemp, pWnd->m_arrDatas.GetUpperBound()+1);
                    pWnd->m_arrDatas.RemoveAll();
				}*/
				
				/*if(pWnd->m_arrDatas.GetUpperBound()>=0)
				{
					byte* bTemp = (byte*)malloc(sizeof(byte)*(pWnd->m_arrDatas.GetUpperBound()+1));
					memset(bTemp, 0, pWnd->m_arrDatas.GetUpperBound()+1);
					
                    for(int i=0; i<=pWnd->m_arrDatas.GetUpperBound();i++)
					{
						bTemp[i] = pWnd->m_arrDatas.GetAt(i);
					}
				
                    buf.SetBufData(bTemp, pWnd->m_arrDatas.GetUpperBound()+1);
					
					if(bTemp)  
					{  
					    free(bTemp);  
					}

                    pWnd->m_arrDatas.RemoveAll();
				}
				//modify lgp 12-16 end
				else
					continue;*/
				if(pWnd->m_arrDatas.GetUpperBound()>=1300)
				{
					byte* bTemp = new byte[pWnd->m_arrDatas.GetUpperBound()];
					memset(bTemp, 0, pWnd->m_arrDatas.GetUpperBound());
                    for(int i=0; i<pWnd->m_arrDatas.GetUpperBound();i++)
					{
						bTemp[i] = pWnd->m_arrDatas.GetAt(i);
					}
                    buf.SetBufData(bTemp, pWnd->m_arrDatas.GetUpperBound()+1);
                    pWnd->m_arrDatas.RemoveAt(0,pWnd->m_arrDatas.GetUpperBound()+1);
					delete bTemp;
				}
				else if(pWnd->m_arrDatas.GetUpperBound()<1300 && pWnd->m_arrDatas.GetUpperBound()>=0)
				{
					byte bTemp[1300];
					memset(bTemp, 0, 1300);
					int i=0;
                    for(; i<=pWnd->m_arrDatas.GetUpperBound();i++)
					{
						bTemp[i] = pWnd->m_arrDatas.GetAt(i);
					}
					bTemp[i]=0;
                    buf.SetBufData(bTemp, pWnd->m_arrDatas.GetUpperBound()+1);
                    pWnd->m_arrDatas.RemoveAll();
				}
				//modify 14-01-16 lgp from yn end
                
				pWnd->m_lockSendDataStatus.Unlock();
				
				//pWnd->m_lockSendDataStatus.Lock();
				
				//if (!pWnd->m_byteLstDatas.IsEmpty())
				//	buf = pWnd->m_byteLstDatas.GetHead();
				//pWnd->m_lockSendDataStatus.Unlock();
				pWnd->m_eSendDataStatus = ENU_SEND_UN_REG;
				
				//安全检查和双向校验
				if(!bCircuitCheck_End && (ENU_SEND_UN_REG == pWnd->m_eSendDataStatus||ENU_SEND_OVER == pWnd->m_eSendDataStatus))
				{
					//pWnd->m_pTransControl_Data->SendData((byte*)pWnd->m_chData, pWnd->m_uDataLen);
					
					pWnd->m_pTransControl_Data->SendData((byte*)buf.m_pDataBuf, buf.m_nBufLen);
					//continue;
				}
				
				if(bCircuitCheck_End && ENU_SEND_UN_REG == pWnd->m_eSendDataStatus )
				{
					pWnd->m_eSendDataStatus = ENU_SEND_REG_ING;
					pWnd->m_pTransControl_Data->ApplyRegisNo();
				}
				else
				{
					Sleep(10);
				}
				
				int nWaitRegScend = 0;
				while(ENU_SEND_UN_REG != pWnd->m_eSendDataStatus)
				{  
					if( ENU_SEND_REG == pWnd->m_eSendDataStatus)
					{
						pWnd->m_eSendDataStatus = ENU_SEND_ING;
						
						pWnd->m_pTransControl_Data->SendData((byte*)buf.m_pDataBuf/*(byte*)pWnd->m_chData*/, buf.m_nBufLen/*pWnd->m_uDataLen*/);
						pWnd->m_pTransControl_Data->LogRegisNo();
						pWnd->m_lockSendDataStatus.Lock();
						//if(!pWnd->m_byteLstDatas.IsEmpty())
					//	{
					//		pWnd->m_byteLstDatas.RemoveHead();
					//	}
						pWnd->m_lockSendDataStatus.Unlock();
						pWnd->m_eSendDataStatus = ENU_SEND_OVER;
						
						pWnd->m_lstLogData.InsertItem(0, pWnd->GetLog(Success,_T(" 成功发送数据")));
						break;					
					}
					
					if( ENU_SEND_REG_ING == pWnd->m_eSendDataStatus)
					{
						if(nWaitRegScend >= 5000)//超时，重新发起注册
						{
							pWnd->m_pTransControl_Data->ApplyRegisNo();
							nWaitRegScend = 0;
						}
						
						Sleep(5);
						nWaitRegScend += 5;
						
						continue;
					}
					
				}
			}
		}
	}
	return S_OK;
}

DWORD WINAPI SendDataTestXn(LPVOID lParam)
{
	CDlgDataSend* pWnd = ((CDlgDataSend*)theApp.GetMainWnd());
    if(NULL == pWnd)
      return 0;

    //add 10-29 begin
	/*pWnd->m_nDataCountTestXn = 0;
	CString strCount;
 	DWORD dTimeBegi = 0;
	dTimeBegi = GetTickCount();
	while(pWnd->m_bTest)
	{
		if((dTimeBegi>0) && (GetTickCount()-dTimeBegi>=1000))
		{
			strCount.Format(_T("1秒钟处理了 %d 帧"),pWnd->m_nDataCountTestXn);
			pWnd->m_lstLogData.InsertItem(0, strCount);
			dTimeBegi = GetTickCount();
			pWnd->m_nDataCountTestXn = 0;
		}
		byte ch[1300];
		memset(ch, 'a', 1300);
		pWnd->m_pTransControl_Data->SendData(ch,1300);
		pWnd->m_nDataCountTestXn++;
	}
	return 0;*/
	//add 10-29 end
	
	
	pWnd->m_signalCt = 0;
	
	BOOL bCircuitCheck_End;
	CString strCircuitCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN);
	if(_T("1") == strCircuitCheck)
		bCircuitCheck_End = TRUE;
	if(_T("0") == strCircuitCheck)
		bCircuitCheck_End = FALSE;
	
    DWORD dTimeBegin = 0;
    while(g_bSendData)
	{    
		if(!pWnd->m_bInOneSecond)
			continue;
		
		if((dTimeBegin>0) && (GetTickCount()-dTimeBegin>=1000) && pWnd->m_bInOneSecond)
		{
			CString strCount;
			strCount.Format(_T("1秒钟处理了 %d 帧"),pWnd->m_nDataCountTestXn);
			pWnd->m_lstLogData.InsertItem(0, strCount);
			pWnd->m_bInOneSecond = FALSE;
			pWnd->m_nDataCountTestXn=0;
			dTimeBegin=0;
			pWnd->m_signalCt =0;
			SuspendThread(pWnd->m_hSendDataTestXnThread);
		}
		
		
		if (pWnd->m_signalCt>0 && pWnd->m_bInOneSecond)
		{
			if(0 == dTimeBegin)
			{
				dTimeBegin = GetTickCount();
			}
			
			InterlockedDecrement(&pWnd->m_signalCt);
		}
		else
		{
			Sleep(1);
			continue;
		}
		
		pWnd->m_eSendDataStatus = ENU_SEND_UN_REG;
		
		//安全检查和双向校验
		if(!bCircuitCheck_End && (ENU_SEND_UN_REG == pWnd->m_eSendDataStatus||ENU_SEND_OVER == pWnd->m_eSendDataStatus))
		{
			pWnd->m_pTransControl_Data->SendData((byte*)pWnd->m_chData, pWnd->m_uDataLen);
			//continue;
		}
		
		if(bCircuitCheck_End && ENU_SEND_UN_REG == pWnd->m_eSendDataStatus )
		{
			pWnd->m_eSendDataStatus = ENU_SEND_REG_ING;
			pWnd->m_pTransControl_Data->ApplyRegisNo();
		}
		else
		{
			Sleep(10);
		}
		
		int nWaitRegScend = 0;
		while(ENU_SEND_UN_REG != pWnd->m_eSendDataStatus)
		{  
			if( ENU_SEND_REG == pWnd->m_eSendDataStatus)
			{
				//pWnd->m_lockSendDataStatus.Lock();
				pWnd->m_eSendDataStatus = ENU_SEND_ING;
				//pWnd->m_lockSendDataStatus.Unlock();
				
				strcpy(pWnd->m_chData, _T("a"));
				pWnd->m_uDataLen = 1;
				pWnd->m_pTransControl_Data->SendData((byte*)pWnd->m_chData, pWnd->m_uDataLen);
				pWnd->m_nDataCountTestXn++;
				pWnd->m_pTransControl_Data->LogRegisNo();
				
				pWnd->m_eSendDataStatus = ENU_SEND_OVER;
				break;
				
			}
			
			if( ENU_SEND_REG_ING == pWnd->m_eSendDataStatus)
			{
				if(nWaitRegScend >= 5000)//超时，重新发起注册
				{
					pWnd->m_pTransControl_Data->ApplyRegisNo();
					nWaitRegScend = 0;
				}
				
				Sleep(5);
				nWaitRegScend += 5;
				
				continue;
			}
			
		}
	}
	
	return S_OK;
}
DWORD WINAPI SendFileThread(LPVOID pParam )
{
    while(g_bSendFile)
	{
        CDlgDataSend* pWnd = ((CDlgDataSend*)theApp.GetMainWnd());
		
		while(WaitForSingleObject(pWnd->m_hEventSendFile, INFINITE) == WAIT_OBJECT_0 &&
			((CEdit*)pWnd->GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled())
		{
            pWnd->m_lockSendFileList.Lock();
			if(pWnd->m_strLstFiles.IsEmpty())
			{
				pWnd->m_lockSendFileList.Unlock();
				continue;
			}
			pWnd->m_lockSendFileList.Unlock();
			
			
			
			CString strSendFile;
			pWnd->m_lockSendFileList.Lock();
			strSendFile = pWnd->m_strLstFiles.GetHead();
			pWnd->m_lockSendFileList.Unlock();
			
            //add 11-07 begin
			int nCheckRet = pWnd->CheckFileBeforeSend(strSendFile);
			if(1 == nCheckRet)//文件不存在
			{
				strSendFile.Empty();
				pWnd->m_lockSendFileList.Lock();
				pWnd->m_strLstFiles.RemoveHead();
				pWnd->m_lockSendFileList.Unlock();
				continue;
			}
			else if(2 == nCheckRet)//文件处于复制状态
			{
				 Sleep(1000);
                 SetEvent(pWnd->m_hEventSendFile);
				 continue;
			}

			//add 11-14 begin
			pWnd->SafeCheckBeforeSend(strSendFile);
			
			//add 11-14 end

			/*else if(3 == nCheckRet)//此文件存在病毒
			{
				//删除或转存此文件
				pWnd->DelOrSaveAfterSended(strSendFile);
				strSendFile.Empty();
				pWnd->m_lockSendFileList.Lock();
				pWnd->m_strLstFiles.RemoveHead();
				pWnd->m_lockSendFileList.Unlock();
				continue;
			}*/
			//add 11-07 end

			//delete lgp 11-13 begin
			//if(!pWnd->CheckSavePathBeforeSend())//bug:转存路径不存在，仍能发送成功(20行bug),若此bug不存在，则可删掉此代码
			//{
			//	Sleep(5000);
			//	continue;
			//}
            //delete lgp 11-13 end

			pWnd->m_lockSendFileStatus.Lock();
			pWnd->m_eSendFileStatus = ENU_SEND_UN_REG;
			pWnd->m_lockSendFileStatus.Unlock();
			
			
			//双向校验
			BOOL bCircuitCheck_End;
            CString strCircuitCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN);
			if(_T("1") == strCircuitCheck)
				bCircuitCheck_End = TRUE;
			if(_T("0") == strCircuitCheck)
				bCircuitCheck_End = FALSE;
			if(!bCircuitCheck_End && (ENU_SEND_UN_REG == pWnd->m_eSendFileStatus||ENU_SEND_OVER == pWnd->m_eSendFileStatus))
			{
				//modify 2014-02-28 begin
				//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
				//pWnd->m_pTransControl_File->SendData(strSendFile);
				if(-1 == strSendFile.Find('\\'))
				   pWnd->m_pTransControl_File->SendData(strSendFile, NULL);
				else
				{
					CString	strMonitorPath_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
					CString strRelativePath = strSendFile;
					strRelativePath.Replace(strMonitorPath_File,NULL);
				    strRelativePath.TrimLeft(_T("\\"));
					int nIndex = strRelativePath.ReverseFind('\\');
					CString strFile = strRelativePath.Right(nIndex+1);
				    strRelativePath.SetAt(nIndex+1,'\0');
                    pWnd->m_pTransControl_File->SendData(strSendFile, strRelativePath);

				}
				//modify 2014-02-28 end
				pWnd->DelOrSaveAfterSended(strSendFile);
				continue;
			}
			
			if(bCircuitCheck_End && ENU_SEND_UN_REG == pWnd->m_eSendFileStatus && !strSendFile.IsEmpty())
			{
				pWnd->m_lockSendFileStatus.Lock();
				pWnd->m_eSendFileStatus = ENU_SEND_REG_ING;
				pWnd->m_lockSendFileStatus.Unlock();
				pWnd->m_pTransControl_File->ApplyRegisNo();
				
			}
			else
			{
				Sleep(10);
			}
			
			int nWaitRegScend = 0;
			//add lgp 11-11 begin 
			int nStopSendCurrFileScend=0;
			//add lgp 11-11 end
			while(ENU_SEND_UN_REG != pWnd->m_eSendFileStatus)
			{  
				if( ENU_SEND_REG_ING == pWnd->m_eSendFileStatus)
				{
					if(nWaitRegScend >= 5000)//5秒钟没有得到校验码，则重新发起申请
					{
                        pWnd->m_pTransControl_File->ApplyRegisNo();
						nWaitRegScend = 0;
					}
					//modify lgp 11-11 begin
					//if(nWaitRegScend >= 20000)//若超过20秒还没有得到校验码，则退出此文件发送流程
					//{                         //此if流程永远都无法执行，换用nStopSendCurrFileScend进行控制
					if(nStopSendCurrFileScend >=20000)//若超过20秒还没有得到校验码，则退出此文件发送流程
					{
					//modify lgp 11-11 end
						pWnd->m_lockSendFileStatus.Lock();
						pWnd->m_eSendFileStatus = ENU_SEND_UN_REG;
						pWnd->m_lockSendFileStatus.Unlock();
						nWaitRegScend = 0;
						//add lgp 11-11 begin
                        nStopSendCurrFileScend = 0;
						//add lgp 11-11 end
						break;
					}
					Sleep(5);
					nWaitRegScend += 5;
					//add lgp 11-11 begin 
					nStopSendCurrFileScend+= 5;
			        //add lgp 11-11 end
					
					continue;
				}
				if( ENU_SEND_REG == pWnd->m_eSendFileStatus)
				{
					CString strMsg;
					strMsg.Format(_T("传输文件: %s\r\n"), pWnd->GetFileNameFromPath(strSendFile));
					pWnd->m_lstLogFile.InsertItem(0, pWnd->GetLog(Success,strMsg));
					COutLogMgr::GetInstance()->LogWriter(Success,strMsg.GetBuffer(strMsg.GetLength()));

					pWnd->m_lockSendFileStatus.Lock();
					pWnd->m_eSendFileStatus = ENU_SEND_ING;
					pWnd->m_lockSendFileStatus.Unlock();
					//modify 2014-02-28 begin
					//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
					//pWnd->m_pTransControl_File->SendData(strSendFile);
					if(-1 == strSendFile.Find('\\'))
						pWnd->m_pTransControl_File->SendData(strSendFile, NULL);
					else
					{
						CString	strMonitorPath_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
						CString strRelativePath = strSendFile;
						strRelativePath.Replace(strMonitorPath_File,NULL);
						strRelativePath.TrimLeft(_T("\\"));
						int nIndex = strRelativePath.ReverseFind('\\');
				        strRelativePath.SetAt(nIndex+1,'\0');
						pWnd->m_pTransControl_File->SendData(strSendFile, strRelativePath);
						
					}
				//modify 2014-02-28 end
					

					
				}
				
				//delete lgp 11-11 begin
				/*while( ENU_SEND_OVER != pWnd->m_eSendFileStatus)
				{
                    if(nWaitRegScend>=8000)//8秒钟还没有发送结束，则注销后，退出此发送流程 此流程需确认
					{
						pWnd->m_lockSendFileStatus.Lock();
						pWnd->m_eSendFileStatus = ENU_SEND_UN_REG;
						pWnd->m_lockSendFileStatus.Unlock();
						pWnd->m_pTransControl_File->LogRegisNo();
						break;
					}
					nWaitRegScend += 100;
					Sleep(100);
				}*/
				//delete lgp 11-11 end
				//add lgp 11-11 begin
				while( ENU_SEND_OVER != pWnd->m_eSendFileStatus)
				{
                    
					if(pWnd->m_bNoDelStopFileServ)
					{
						pWnd->m_lockSendFileList.Lock();
						pWnd->m_strLstFiles.RemoveHead();
					    pWnd->m_lockSendFileList.Unlock();
						pWnd->m_lockSendFileStatus.Lock();
						pWnd->m_eSendFileStatus = ENU_SEND_UN_REG;
						pWnd->m_lockSendFileStatus.Unlock();
						pWnd->m_pTransControl_File->LogRegisNo();
						break;
					}
					//nWaitRegScend += 100;
					Sleep(100);
				}
				//add lgp 11-11 end
				if(ENU_SEND_OVER == pWnd->m_eSendFileStatus)
				{
					//删除或转存此文件
					//midify 11-07 begin
					//pWnd->DelOrSaveAfterSended(strSendFile);
					if(!pWnd->m_bNoDelStopFileServ)
					{
                        pWnd->DelOrSaveAfterSended(strSendFile);
					}
					//modify 11-07 end
					strSendFile.Empty();
					pWnd->m_lockSendFileList.Lock();
					pWnd->m_strLstFiles.RemoveHead();
					pWnd->m_lockSendFileList.Unlock();
					
					pWnd->m_lockSendFileStatus.Lock();
					pWnd->m_eSendFileStatus = ENU_SEND_UN_REG;
					pWnd->m_lockSendFileStatus.Unlock();
				}
			}
		}
	}
	return S_OK;
}
DWORD WINAPI FileMonitorThreadFun(LPVOID pParam )
{
	CDlgDataSend* pWnd = ((CDlgDataSend*)theApp.GetMainWnd());
	if(NULL == pWnd)
		return 0;
	
	UINT nScendWait = 500;
	while(g_bMonitorFile)
	{
		
		//while(WaitForSingleObject(pWnd->m_hEventSendFile, INFINITE) == WAIT_OBJECT_0 )
		//	{
		CString strMonitorPath_File;
		strMonitorPath_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
		if(strMonitorPath_File.IsEmpty())
			return S_FALSE;
		
		BOOL bRet = pWnd->GetFilesToSend();
		if(!bRet)
			return S_FALSE;
		
		if(!pWnd->m_strLstFiles.IsEmpty())
		{
			nScendWait = 500;
		} 
		else
		{
			nScendWait = nScendWait*2;
			if(nScendWait>5000)
				nScendWait = 5000;
			Sleep(3000);
		}
		
		//等待时间++--
		//	}
	}
	return S_OK;
}
BOOL CDlgDataSend::CheckMonitorPathBeforeSend()
{
	CString strMonitorPathUI;
	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->GetWindowText(strMonitorPathUI);
	CString strMonitorPathXml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
	if(strMonitorPathXml != strMonitorPathUI)
	{
		//if( IDYES == BCGPMessageBox(NULL, _T("你输入了新的转存路径，是否按此转存路径进行转存"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		if( IDYES == MessageBox( _T("你输入了新的监控路径，是否按此监控路径进行监控"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			if(!PathFileExists(strMonitorPathUI))//新的监控路径不存在
			{
				//提示是否自动创建新的监控路径
				if( IDYES != MessageBox(_T("新的监控路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
				{
					//BCGPMessageBox(NULL, _T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
					//OnStopFile();
					MessageBox(_T("请选择监控路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
					return FALSE;
				}
                //创建新的监控路径
				if(!CreateDirectory(strMonitorPathUI, NULL))
				{
					//BCGPMessageBox(_T("新的转存路径不存在，创建失败，请重新选择转存路径"));
					//OnStopFile();
					MessageBox(_T("新的监控路径不存在，创建失败，请重新选择监控路径"),_T("提示框"));
					return FALSE;
				}
			}
			//保存配置文件
			COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH,strMonitorPathUI);
			return TRUE;
		}
		else//按原来配置文件中的监控路径进行监控
		{
			if(!PathFileExists(strMonitorPathXml))//配置文件中制定的监控路径不存在
			{
				//提示是否自动创建配置文件中制定的监控路径
				if( IDYES != MessageBox(_T("配置的监控路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
				{
					//BCGPMessageBox(NULL, _T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
					//OnStopFile();
					MessageBox(_T("请选择监控路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
					return FALSE;
				}
                //创建配置文件中制定的监控路径
				if(!CreateDirectory(strMonitorPathXml, NULL))
				{
					//BCGPMessageBox(_T("配置文件中设置的转存路径不存在，创建失败，请重新选择转存路径"));
					//OnStopFile();
					MessageBox(_T("配置文件中设置的监控路径不存在，创建失败，请重新选择监控路径"));
					return FALSE;
				}
			}
			
			((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->SetWindowText(strMonitorPathXml);
			return TRUE;
		}
	}
	
	//监控路径没有更新，但是监控目录不存在
	if(!PathFileExists(strMonitorPathXml))
	{
		
		//if( IDYES != BCGPMessageBox(NULL, _T("转存路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		if( IDYES != MessageBox(_T("监控路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			//BCGPMessageBox(NULL, _T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
			//OnStopFile();
			MessageBox(_T("请选择监控路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
		
		if(!CreateDirectory(strMonitorPathXml, NULL))
		{
			//BCGPMessageBox(NULL, _T("对不起，转存路径不存在，创建失败，请选择转存路径"), _T("提示框"), MB_OK|MB_ICONERROR);
			//OnStopFile();
			MessageBox(_T("对不起，监控路径不存在，创建失败，请选择监控路径"), _T("提示框"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}
    return TRUE;
}
//此函数需要在文件发送线程中使用，所以，不能使用BCG的MessageBox
BOOL CDlgDataSend::CheckSavePathBeforeSend()
{  


	
	CString strSaveAsPath_File= COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);
	CString strIsSaveAsSendedFile_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_RESAVE_FILE);


	CString strSavePathUI;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->GetWindowText(strSavePathUI);
	
	if(_T("1") == strIsSaveAsSendedFile_File)
	{
		if(strSaveAsPath_File != strSavePathUI)//UI与XML不同
		{
            //if( IDYES == BCGPMessageBox(NULL, _T("你输入了新的转存路径，是否按此转存路径进行转存"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
			if( IDYES == MessageBox( _T("你输入了新的转存路径，是否按此转存路径进行转存"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
			{
				if(!PathFileExists(strSavePathUI))
				{
					//提示是否自动创建新的转存路径
					if( IDYES != MessageBox(_T("新的转存路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
					{
						//BCGPMessageBox(NULL, _T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
						//OnStopFile();
						MessageBox(_T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
						
						return FALSE;
					}
					
					if(!CreateDirectory(strSavePathUI, NULL))
					{
						//BCGPMessageBox(_T("新的转存路径不存在，创建失败，请重新选择转存路径"));
						//OnStopFile();
						MessageBox(_T("新的转存路径不存在，创建失败，请重新选择转存路径"),_T("提示框"));
						
						return FALSE;
					}
				}
				//保存配置文件
				COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH,strSavePathUI);
			}
			else//按配置文件中的转存路径进行转存
			{
				if(!PathFileExists(strSaveAsPath_File))
				{
					//提示是否自动创建新的转存路径
					if( IDYES != MessageBox(_T("配置中的转存路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
					{
						//BCGPMessageBox(NULL, _T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
						//OnStopFile();
						MessageBox(_T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
					
						return FALSE;
					}
					
					if(!CreateDirectory(strSaveAsPath_File, NULL))
					{
						//BCGPMessageBox(_T("配置文件中设置的转存路径不存在，创建失败，请重新选择转存路径"));
						//OnStopFile();
						MessageBox(_T("配置文件中设置的转存路径不存在，创建失败，请重新选择转存路径"));
						
						return FALSE;
					}
				}
				
				((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->SetWindowText(strSaveAsPath_File);
			}
		}
		else//UI与XML相同
		{
			if(!PathFileExists(strSaveAsPath_File))
			{
				//if( IDYES != BCGPMessageBox(NULL, _T("转存路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
				if( IDYES != MessageBox(_T("转存路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
				{
					//BCGPMessageBox(NULL, _T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
					//OnStopFile();
					MessageBox(_T("请选择转存路径，然后才能传输文件"), _T("提示框"), MB_OK|MB_ICONERROR);
				
					return FALSE;
				}
				
				if(!CreateDirectory(strSaveAsPath_File, NULL))
				{
					//BCGPMessageBox(NULL, _T("对不起，转存路径不存在，创建失败，请选择转存路径"), _T("提示框"), MB_OK|MB_ICONERROR);
					//OnStopFile();
					MessageBox(_T("对不起，转存路径不存在，创建失败，请选择转存路径"), _T("提示框"), MB_OK|MB_ICONERROR);
					
					return FALSE;
				}
			}
		}
		
	}
	
	return TRUE;
	
}
//3. 安全检查                  不安全(删除或转存，然后从检索列表中删除,return 1)  安全return 0  安全检测条件不具备(return -1)
BOOL CDlgDataSend::SafeCheckBeforeSend(CString strFilePath)
{
	//3. 安全检查
	CString strIsSafeCheck=COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
	if(_T("0") == strIsSafeCheck)
		return 0;
	
	/////////////////
	if(!PathFileExists(_T("DXSJCS.bat")))
		return -1;
	
	//写入批处理命令
	CFile file;
	file.Open(_T("DXSJCS.bat"),CFile::modeWrite); 
	if (!file) 
		return -1; 
	if(m_strRSingSoftPath.IsEmpty() && (!m_bInitDlg))
		return 0;

	CString strBat = _T("\"") + m_strRSingSoftPath + _T("\" \"") + strFilePath +_T("\"");
	file.Write(strBat, strBat.GetLength());
	file.Close();
	
    //启动批处理命令
	if(!PathFileExists(_T("DXSJCS.vbs")))
		return -1;
	SHELLEXECUTEINFO shei;
	shei.cbSize = sizeof(shei);
	shei.fMask = SEE_MASK_INVOKEIDLIST;
	shei.lpFile = _T("DXSJCS.vbs");
	shei.lpVerb = _T("open");
	ShellExecute(NULL, shei.lpVerb, shei.lpFile, _T(""), _T(""), SW_SHOW);

	return 0;
}
//发送文件前，对文件进行检查(3步):
//1. 检查文件是否存在          不存在( 从检索列表中删除, return 1) 存在则转入步骤2
//2. 检查文件是否处于复制状态  处于复制状态(检索列表中不删除，return 2) 不在复制状态则转入步骤3

int CDlgDataSend::CheckFileBeforeSend(CString strFileToSend)
{
	

	//1. 检查文件是否存在
	if(!PathFileExists(strFileToSend))
	{
		return 1;
	}



	//2. 检查文件是否处于复制状态
	if (!SetFileAttributes(strFileToSend, FILE_ATTRIBUTE_ARCHIVE)) 
		return 2; 
	
	HANDLE hFile = ::CreateFile(strFileToSend, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL); 
	if (hFile == INVALID_HANDLE_VALUE) 
		return 2; 
	
	::CloseHandle(hFile); 
	hFile = NULL; 
	

	
    
	////////////////
	return 0;


}
void CDlgDataSend::InitLogCtrl()
{
	//设置日志列表控件可整行选中的属性
	m_lstLogFile.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstLogData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstLogMedia.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//初始化日志列表
	CRect rectFile;
	m_lstLogFile.GetWindowRect(&rectFile);
	m_lstLogFile.InsertColumn(0, _T("运行日志"), LVCFMT_CENTER, rectFile.Width()-4);
	
	CRect rectData;
	m_lstLogData.GetWindowRect(&rectData);
	m_lstLogData.InsertColumn(0, _T("运行日志"), LVCFMT_CENTER, rectData.Width()-4);
	
	CRect rectMedia;
	m_lstLogMedia.GetWindowRect(&rectMedia);
	m_lstLogMedia.InsertColumn(0, _T("运行日志"), LVCFMT_CENTER, rectMedia.Width()-4);
}
BOOL CDlgDataSend::InitMultiWorkThread()
{
	memset(m_chData,0,1300);
	memset(m_chMedia,0,1300);
	m_uDataLen = 0;
	m_uMediaLen = 0;
	m_bMonitorData = NULL;
	m_hEventSendMedia = NULL;
    m_eSendFileStatus = ENU_SEND_OVER;
	m_eSendDataStatus= ENU_SEND_OVER;
	m_eSendMediaStatus= ENU_SEND_OVER;
	
	m_hEventSendData = NULL;
	m_hEventSendFile = NULL;
	m_hSendFileThread = NULL;
	m_hMonitorThread = NULL;
	//m_hEventMonitorFile = NULL;
	m_hSendDataThread = NULL;//发送数据线程
	m_hSendMediaThread = NULL;//发送流媒体线程
	m_hSendDataTestXnThread = NULL;//测试性能线程
	
	//发送文件列表事件对象
	m_hEventSendFile = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(NULL == m_hEventSendFile)
	{
		BCGPMessageBox(_T("创建发送文件列表事件对象m_hEventSendFile失败"));
		return FALSE;
	}
    
	//是否检索监控路径中的文件
	/*m_hEventMonitorFile = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(NULL == m_hEventSendFile)
	{
	BCGPMessageBox(_T("创建是否检索监控路径中的文件事件对象m_hEventSendFile失败"));
	return FALSE;
	}*/
	
	//发送数据列表事件对象
	m_hEventSendData = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(NULL == m_hEventSendData)
	{
		BCGPMessageBox(_T("创建发送数据事件对象事件对象m_hEventSendData失败"));
		return FALSE;
	}
	
	//发送流媒体列表事件对象
	m_hEventSendMedia = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(NULL == m_hEventSendMedia)
	{
		BCGPMessageBox(_T("创建发送流媒体事件对象事件对象m_hEventSendData失败"));
		return FALSE;
	}
	
	
	//文件发送监控线程
	m_hMonitorThread = CreateThread(NULL, 0, FileMonitorThreadFun, 0,CREATE_SUSPENDED, NULL);
	if(NULL == m_hMonitorThread)
	{
		CloseHandle(m_hEventSendFile);
		m_hEventSendFile = NULL;
		BCGPMessageBox(_T("创建文件监控线程失败"));
		return FALSE;
	}
    
	//文件发送线程
	m_hSendFileThread = CreateThread(NULL, 0, SendFileThread,0,CREATE_SUSPENDED,NULL);
	if(NULL == m_hSendFileThread)
	{
		CloseHandle(m_hEventSendFile);
		m_hEventSendFile = NULL; 
		g_bMonitorFile = FALSE;
		BCGPMessageBox(_T("创建文件监控线程失败"));
		return FALSE;
	}
	//数据发送线程
	m_hSendDataThread = CreateThread(NULL, 0, SendDataThread,0,0,NULL);
	if(NULL == m_hSendDataThread)
	{
		CloseHandle(m_hEventSendFile);
		m_hEventSendFile = NULL; 
		g_bMonitorFile = FALSE;
		g_bSendFile = FALSE;
		BCGPMessageBox(_T("创建数据发送状态监控线程失败"));
		return FALSE;
	}
	
	//发送流媒体线程
	m_hSendMediaThread = CreateThread(NULL, 0, SendMediaThread,0,0,NULL);
	if(NULL == m_hSendMediaThread)
	{
		CloseHandle(m_hEventSendFile);
		m_hEventSendFile = NULL; 
		g_bMonitorFile = FALSE;
		g_bSendFile = FALSE;
		g_bSendData = FALSE;
		BCGPMessageBox(_T("创建数据发送状态监控线程失败"));
		return FALSE;
	}
	
	m_hSendDataTestXnThread = CreateThread(NULL, 0, SendDataTestXn, 0, CREATE_SUSPENDED,NULL);
	if(NULL == m_hSendDataTestXnThread)
	{
        CloseHandle(m_hSendDataTestXnThread);
		m_hSendDataTestXnThread = NULL;
	}
	
	return TRUE;
}
BOOL CDlgDataSend::InitSafeCheck()
{
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
void CDlgDataSend::GetRSingSoftPath()
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
函数名：void Init()

功能描述：初始化界面
  
参数：无
	
全局变量：无
	  
返回值: 无
		
****************************************************************/
void CDlgDataSend::Init()
{
	m_bInitDlg = TRUE;//系统开始始化主窗口界面
	m_bSendFileUIChanged = FALSE;
	m_bEndUIChanged = FALSE;
	//add lgp 2014-02-27 begin
	//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
	m_strChildDirectory.Empty();
	//add lgp 2014-02-27 end
	COutLogMgr::GetInstance()->LogWriter(General, _T("本机测试时，优先使用本机IP, 若没有本机IP可使用127.0.0.1"));
	//设置界面的BCG风格
	
	//CBCGPVisualManager2003::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2003));
	CBCGPDialog::EnableVisualManagerStyle(TRUE);
	
	//EnableVisualManagerStyle();
	

	InitLogCtrl();
    GetRSingSoftPath();
	InitSafeCheck();

	InitMultiWorkThread();
	
	m_pTransControl_File = CTransControlEx::NewCtransCtrlEx();
	InitServ_File();
	if(InitParam_File())
	{
		OnStartFile();
		ResumeThread(m_hMonitorThread);
		ResumeThread(m_hSendFileThread);
	}
	
	m_pTransControl_Data = CTransControlEx::NewCtransCtrlEx();
	InitServ_Data();
	if(InitParam_Data())
	{
		OnStartData();
	}
	
	m_pTransControl_Media = CTransControlEx::NewCtransCtrlEx();
	InitServ_Media();
	if(InitParam_Media())
	{
		OnStartMedia();
	}
	InitParamServ_End(FALSE);
	
	m_eSendDataStatus = ENU_SEND_ERROR;
	
	m_bInOneSecond = FALSE;
	m_bDataInTestXn = FALSE;
	m_nDataCountTestXn = 0;
	//m_byteLstDatas.RemoveAll();
	
	m_bInitDlg = FALSE;//系统初始化主窗口界面结束
	//10-29 add begin
	m_bTest = TRUE;
	//10-29 add end
}
		  
BOOL CDlgDataSend::InitServ_File()
{
	
	CString strLocalPort_File,strAimPort_File,strAimIp_File;
	strLocalPort_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
	strAimPort_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_PORT);
	strAimIp_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_IP);
	
	CString strSuccMsg = _T("发送文件服务初始化成功\r\n");
	CString strFailMsg = _T("发送文件服务初始化失败\r\n");
	
	int nLocalPort = atoi(strLocalPort_File);
	int nAimPort = atoi(strAimPort_File);
	//初始化服务
	BOOL bRet = m_pTransControl_File->Init(nAimPort, strAimIp_File, TYPE_SEND_FILE, TransCallSendFile,feedBackSendFile,nLocalPort);
	if(!bRet)
	{
		COutLogMgr::GetInstance()->LogWriter(Failure, strFailMsg.GetBuffer(strFailMsg.GetLength()));
		m_lstLogFile.InsertItem(0, GetLog(Failure,strFailMsg));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(strFailMsg);
		return FALSE;
	}
	
	COutLogMgr::GetInstance()->LogWriter(Success, strSuccMsg.GetBuffer(strSuccMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success,strSuccMsg));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(strSuccMsg);
	
	return TRUE;
}
BOOL CDlgDataSend::InitServ_Data()
{
	CString strLocalPort_Data,strAimPort_Data,strAimIp_Data;
	strLocalPort_Data = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);
	strAimPort_Data = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_PORT);
	strAimIp_Data = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_IP);
	
	CString strSuccMsg = _T("发送数据服务初始化成功\r\n");
	CString strFailMsg = _T("发送数据服务初始化失败\r\n");
	
	int nLocalPort = atoi(strLocalPort_Data);
	int nAimPort = atoi(strAimPort_Data);
	//初始化服务
	BOOL bRet = m_pTransControl_Data->Init(nAimPort, strAimIp_Data, TYPE_SEND_SHORTMES, TransCallSendData,feedBackSendData,nLocalPort);
	if(!bRet)
	{
		COutLogMgr::GetInstance()->LogWriter(Failure, strFailMsg.GetBuffer(strFailMsg.GetLength()));
		m_lstLogData.InsertItem(0, GetLog(Failure,strFailMsg));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(strFailMsg);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);
		return FALSE;
	}
	
	COutLogMgr::GetInstance()->LogWriter(Success, strSuccMsg.GetBuffer(strSuccMsg.GetLength()));
	m_lstLogData.InsertItem(0, GetLog(Success,strSuccMsg));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(strSuccMsg);
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);
	
	return TRUE;
}
BOOL CDlgDataSend::InitServ_Media()
{
	CString strLocalPort_Media, strAimPort_Media,strAimIp_Media;
	strLocalPort_Media = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);
	strAimPort_Media = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_PORT);
	strAimIp_Media = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_IP);
	
	CString strSuccMsg = _T("发送流媒体服务初始化成功\r\n");
	CString strFailMsg = _T("发送流媒体服务初始化失败\r\n");
	
	int nLocalPort = atoi(strLocalPort_Media);
	int nAimPort = atoi(strAimPort_Media);
	//初始化服务
	BOOL bRet = m_pTransControl_Media->Init(nAimPort, strAimIp_Media, TYPE_SEND_STREAM, TransCallSendMedia,feedBackSendMedia,nLocalPort);
	if(!bRet)
	{
		m_lstLogMedia.InsertItem(0, GetLog(Failure,strFailMsg));
		COutLogMgr::GetInstance()->LogWriter(Failure,strFailMsg.GetBuffer(strFailMsg.GetLength()));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(strFailMsg);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);
		return FALSE;
	}
	
	COutLogMgr::GetInstance()->LogWriter(Success, strSuccMsg.GetBuffer(strSuccMsg.GetLength()));
	m_lstLogMedia.InsertItem(0, GetLog(Success,strSuccMsg));
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(strSuccMsg);
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);
	
	return TRUE;
}
/***************************************************************
函数名：void InitNodeValues()

  功能描述：初始化界面元素值
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::InitNodeValues()
{
	//读取配置文件中的文件传输模块信息
	CString strFileLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
	CString strFileAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_IP);
	CString strFileAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_PORT);
	CString strFileMonitorPath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
	CString strFileMonitorType = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_TYPE);
	CString strFileIsDelFile = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_DEL_FILE);
	CString strFileIsResaveFile = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_RESAVE_FILE);
	CString strFileIsSafeCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
	CString strFileSavePath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);
	
	//显示文件传输模块信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->SetWindowText(strFileLocalPort);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_FILE))->SetWindowText(strFileAimIp);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_FILE))->SetWindowText(strFileAimPort);	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->SetWindowText(strFileMonitorPath);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_TYPE_MONITOR))->SetWindowText(strFileMonitorType);
	if(_T("1") == strFileIsDelFile)
		((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->SetCheck(FALSE);
	if(_T("1") == strFileIsResaveFile)
		((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->SetCheck(FALSE);
	if( _T("1") == strFileIsSafeCheck)
		((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->SetCheck(FALSE);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->SetWindowText(strFileSavePath);
	
	
	//读取数据传输模块信息
	CString strDataLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);
	CString strDataAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_IP);
	CString strDataAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_PORT);
	//显示数据传输模块信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->SetWindowText(strDataLocalPort);
	DWORD dwAddress = ntohl(inet_addr(strDataAimIp));
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_DATA))->SetAddress(dwAddress);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_DATA))->SetWindowText(strDataAimPort);
	
	//读取流媒体传输模块信息
	CString strMediaLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);
	CString strMediaAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_IP);
	CString strMediaAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_PORT);
	//显示流媒体传输模块信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->SetWindowText(strMediaLocalPort);
	dwAddress = ntohl(inet_addr(strMediaAimIp));
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_MEDIA))->SetAddress(dwAddress);
	((CEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_MEDIA))->SetWindowText(strMediaAimPort);
	
	//读取终端设备信息
	CString strEndLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_LOCAL_PORT);
	CString strEndAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_AIM_IP);
	CString strEndAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_AIM_PORT);
	CString strEndIsVerfReturn = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN);
	//显示终端设备信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_END))->SetWindowText(strEndLocalPort);
	//dwAddress = ntohl(inet_addr(strMediaAimIp));
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_LOCAL_END))->SetWindowText(strEndAimIp);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_END))->SetWindowText(strEndAimPort);
	if(_T("1") == strEndIsVerfReturn)
		((CBCGPButton*)GetDlgItem(IDC_CHK_CIRCUIT))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_CHK_CIRCUIT))->SetCheck(FALSE);
}
		  
		 
BOOL CDlgDataSend::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//只有管理员才能查看日志和维护用户
	
	if(VK_RETURN == pMsg->wParam ) 
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
		case VK_F3: //用户管理
			{
				BOOL bRet = COutLogIn::GetInstance()->ValidatePermissions(theApp.m_strUserName, MODULE_NAME_ALL);
				if(bRet)
					COutLogIn::GetInstance()->UserManagement(theApp.m_strUserName);
				
				break;
			}
		case VK_F4:   //日志查看
			{
				COutLogMgr::GetInstance()->DisplayLog();
				break;
			}
		}
		break;
	}	
	return CBCGPDialog::PreTranslateMessage(pMsg);
}
BOOL CDlgDataSend::InitParam_File()
{
			  
	//读取配置文件中的文件传输模块信息
	CString strFileLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
	CString strFileAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_IP);
	CString strFileAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_PORT);
	CString strFileMonitorPath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
	CString strFileMonitorType = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_TYPE);
	CString strFileIsDelFile = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_DEL_FILE);
	CString strFileIsResaveFile = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_RESAVE_FILE);
	CString strFileIsSafeCheck = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
	CString strFileSavePath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);
	
	
	//显示文件传输模块信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->SetWindowText(strFileLocalPort);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_FILE))->SetWindowText(strFileAimIp);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_FILE))->SetWindowText(strFileAimPort);	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->SetWindowText(strFileMonitorPath);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_TYPE_MONITOR))->SetWindowText(strFileMonitorType);
	if(_T("1") == strFileIsDelFile)
		((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->SetCheck(FALSE);
	if(_T("1") == strFileIsResaveFile)
		((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->SetCheck(FALSE);
	if( _T("1") == strFileIsSafeCheck)
		((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->SetCheck(TRUE);
	else
		((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->SetCheck(FALSE);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->SetWindowText(strFileSavePath);
	
	
	
	//对配置文件中的参数格式进行合法性验证
	//监控路径和转存路径不能为同一个路径
	if(strFileMonitorPath == strFileSavePath)
	{
		BCGPMessageBox(NULL, _T("配置文件中的 监控路径和转存路径为同一个路径，文件传输服务无法启动，请区分设置"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if((_T("1") != strFileIsDelFile) && (_T("0") != strFileIsDelFile))
	{
		BCGPMessageBox(NULL, _T("配置文件中的 是否删除已发送文件 设置格式有误"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if((_T("1") != strFileIsResaveFile) && (_T("0") != strFileIsResaveFile))
	{
		BCGPMessageBox(NULL, _T("配置文件中的 是否转存已发送文件 设置格式有误"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if((_T("1") != strFileIsSafeCheck) && (_T("0") != strFileIsSafeCheck))
	{
		BCGPMessageBox(NULL, _T("配置文件中的 是否对文件进行安全检查 设置格式有误"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if(strFileLocalPort.SpanIncluding(_T("0123456789")) != strFileLocalPort)
	{
		BCGPMessageBox(NULL, _T("配置文件中的 文件模块的本地端口 设置格式有误"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if(strFileAimPort.SpanIncluding(_T("0123456789")) != strFileAimPort)
	{
		BCGPMessageBox(NULL, _T("配置文件中的 文件模块的目的端口 设置格式有误"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	
	
	
	//对文件模块信息进行合法性检查
	CString strErrMsg;
	//本地端口
	if(!Validate_Port(strFileLocalPort, MODULE_NAME_FILE_SEND, _T("本地端口")))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	
	
	//目的端口
	if(!Validate_Port(strFileAimPort, MODULE_NAME_FILE_SEND, _T("目的端口")))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	
	//目的地址
	if(!Validate_IP(strFileAimIp,MODULE_NAME_FILE_SEND))
		return FALSE;
	
	//监控类型
	
	//是否可删除已发送文件, 是否转存已发送文件
	if((_T("1") ==strFileIsDelFile) && (_T("1") ==strFileIsResaveFile))
	{
		BCGPMessageBox(NULL, _T("对不起，删除-转存已发送文件，只能设置一项，请重新选择"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if((_T("1")!=strFileIsDelFile) && (_T("1") !=strFileIsResaveFile))
	{
		BCGPMessageBox(NULL, _T("对不起，删除-转存已发送文件，必须设置一项，请重新选择"), _T("提示框"), MB_OK|MB_ICONERROR);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return FALSE;
	}
	if(_T("1") ==strFileIsDelFile)
	{
		((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	if(_T("1")==strFileIsResaveFile)
	{
		//((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(TRUE);
		((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	
	//监控路径,转存路径
	if(_T("1") == strFileIsResaveFile) 
	{
		if(!CheckSaveAndMonitorPath(strFileSavePath, strFileMonitorPath,TRUE))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
			return FALSE;
		}
	}
	else if(_T("1") != strFileIsResaveFile)  
	{
		if(!CheckSaveAndMonitorPath(strFileSavePath, strFileMonitorPath,FALSE))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
			return FALSE;
		}
	}
	//安全检查待发送文件
	
	return TRUE;
			  
}
BOOL CDlgDataSend::InitParam_Data()
{
	//读取数据传输模块信息
	CString strDataLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);
	CString strDataAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_IP);
	CString strDataAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_PORT);
	//显示数据传输模块信息
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->SetWindowText(strDataLocalPort);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_DATA))->SetWindowText(strDataAimIp);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_DATA))->SetWindowText(strDataAimPort);
    //对数据模块参数进行合法性检查
	//目的地址
    if(!Validate_IP(strDataAimIp,MODULE_NAME_DATA_SEND))
		return FALSE;
    //本地端口
	//目的端口
	
	return TRUE;
}
BOOL CDlgDataSend::InitParam_Media()
{
	//读取流媒体传输模块信息
	CString strMediaLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);
	CString strMediaAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_IP);
	CString strMediaAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_PORT);
	//显示流媒体传输模块信息
	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->SetWindowText(strMediaLocalPort);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_MEDIA))->SetWindowText(strMediaAimIp);
	((CEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_MEDIA))->SetWindowText(strMediaAimPort);
	
	//对流媒体模块参数进行合法性检查
	//目的地址
    if(!Validate_IP(strMediaAimIp,MODULE_NAME_MEDIA_SEND))
		return FALSE;
    //本地端口
	//目的端口
	
	return TRUE;
	
}
BOOL CDlgDataSend::InitParamServ_End(BOOL bStartServ, E_MODULE_TYPE eModule)
{
	//读取终端设备信息
	CString strEndLocalPort, strEndAimIp, strEndAimPort, strEndIsVerfReturn;
    if( m_strEndAimIp.IsEmpty() || m_strEndAimPort.IsEmpty() || m_strEndIsVerfReturn.IsEmpty() ||
		m_strEndLocalPort.IsEmpty() || m_bEndUIChanged)
	{
		 strEndLocalPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_LOCAL_PORT);
		 strEndAimIp = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_AIM_IP);
		 strEndAimPort = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_AIM_PORT);
	     strEndIsVerfReturn = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN);
		 m_strEndLocalPort = strEndLocalPort;
		 m_strEndAimIp = strEndAimIp;
		 m_strEndAimPort = strEndAimPort;
         m_strEndIsVerfReturn = strEndIsVerfReturn;
		 m_bEndUIChanged = FALSE;
	}
	else
	{
       strEndLocalPort = m_strEndLocalPort;
       strEndAimIp = m_strEndAimIp;
       strEndAimPort = m_strEndAimPort;
       strEndIsVerfReturn = m_strEndIsVerfReturn;
	}
	//显示终端设备信息
	if(!bStartServ)
	{
		((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_END))->SetWindowText(strEndLocalPort);
		((CIPAddressCtrl*)GetDlgItem(IDC_IP_LOCAL_END))->SetWindowText(strEndAimIp);
		((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_END))->SetWindowText(strEndAimPort);
		if(_T("1") == strEndIsVerfReturn)
			((CBCGPButton*)GetDlgItem(IDC_CHK_CIRCUIT))->SetCheck(TRUE);
		else
			((CBCGPButton*)GetDlgItem(IDC_CHK_CIRCUIT))->SetCheck(FALSE);
	}
	//对终端模块参数进行合法性检查
	//目的地址
    if(!Validate_IP(strEndAimIp,MODULE_NAME_MEDIA_SEND))
		return FALSE;
    //本地端口
	//目的端口
	//是否进行双向回路校验
	if((_T("1")!=strEndIsVerfReturn) && (_T("1") !=strEndIsVerfReturn))
	{
		BCGPMessageBox(NULL, _T("是否进行双向回路校验 设置格式有误"), _T("提示框"), MB_OK|MB_ICONERROR);
		return FALSE;
	}
	
	//启动文件传输服务
	int nAimPort_End = atoi(strEndAimPort);
    int nLocalPort_End = atoi(strEndLocalPort);
	if(bStartServ && ENU_MODULE_FILE==eModule)
	{
		CString strSendReady = _T("准备开始发送数据\r\n");
		CString strSockErro = _T("文件传输服务启动失败，端口可能被占用，请检查\r\n");
        ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(_T(""));
		
		BOOL bRet = m_pTransControl_File->CreateSocket(strEndAimIp,nAimPort_End,nLocalPort_End);

		if(!bRet)
		{
			COutLogMgr::GetInstance()->LogWriter(Failure, strSockErro.GetBuffer(strSockErro.GetLength()));
			m_lstLogFile.InsertItem(0, GetLog(Failure,strSockErro));
			((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(strSockErro);
			return FALSE;
		}
		COutLogMgr::GetInstance()->LogWriter(Success, strSendReady.GetBuffer(strSendReady.GetLength()));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(strSendReady);
		m_lstLogFile.InsertItem(0, GetLog(Success,strSendReady));
		

		((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(TRUE);
		
		return TRUE;
	}
	//启动数据传输服务
	if(bStartServ && ENU_MODULE_DATA==eModule)
	{
		CString strSendReady = _T("准备开始发送数据\r\n");
		CString strSockErro = _T("数据传输服务启动失败，端口可能被占用，请检查\r\n");
        ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(_T(""));
		
		BOOL bRet = m_pTransControl_Data->CreateSocket(strEndAimIp,nAimPort_End,nLocalPort_End);
		
		if(!bRet)
		{
			COutLogMgr::GetInstance()->LogWriter(Failure, strSockErro.GetBuffer(strSockErro.GetLength()));
			m_lstLogData.InsertItem(0, GetLog(Failure,strSockErro));
			((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(strSockErro);
			return FALSE;
		}
		

		COutLogMgr::GetInstance()->LogWriter(Success, strSendReady.GetBuffer(strSendReady.GetLength()));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(strSendReady);
		m_lstLogData.InsertItem(0, GetLog(Success,strSendReady));
		
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(TRUE);
		
		return TRUE;
	}
	//启动流媒体传输服务
	if(bStartServ && ENU_MODULE_MEDIA==eModule)
	{
		CString strSendReady = _T("准备开始发送流媒体据\r\n");
        ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(_T(""));
		
		BOOL bRet = m_pTransControl_Media->CreateSocket(strEndAimIp,nAimPort_End,nLocalPort_End);
		CString strSockErro = _T("流媒体传输服务启动失败，端口可能被占用，请检查\r\n");
		if(!bRet)
		{
			COutLogMgr::GetInstance()->LogWriter(Failure, strSockErro.GetBuffer(strSockErro.GetLength()));
			m_lstLogMedia.InsertItem(0, GetLog(Failure,strSockErro));
			((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(strSockErro);
			return FALSE;
		}
		
		COutLogMgr::GetInstance()->LogWriter(Success, strSendReady.GetBuffer(strSendReady.GetLength()));
		((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(strSendReady);
		m_lstLogMedia.InsertItem(0, GetLog(Success, strSendReady));
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(TRUE);
		return TRUE;
	}
	
	return TRUE;
}
BOOL CDlgDataSend::Validate_IP(CString strIp, CString strModuleName)
{
	//MessageBox
	return TRUE;
}
BOOL CDlgDataSend::CheckSaveAndMonitorPath( CString strSaveAsPath, CString strMonitorPath, BOOL bSaveAs)
{
	//转存路径
	if(bSaveAs)
	{
		if(!PathFileExists(strSaveAsPath))
		{
			if( IDYES != BCGPMessageBox(NULL, _T("转存路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
				return FALSE;
			
			if(!CreateDirectory(strSaveAsPath, NULL))
			{
				BCGPMessageBox(NULL, _T("对不起，转存路径不存在，创建失败，请选择转存路径"), _T("提示框"), MB_OK|MB_ICONERROR);
				return FALSE;
			}
		}
	}
	//监控路径
	if(!PathFileExists(strMonitorPath))
	{
		if( IDYES != BCGPMessageBox(NULL, _T("监控路径不存在，是否自动创建"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
			return FALSE;
		
		if(!CreateDirectory(strMonitorPath, NULL))
		{
			BCGPMessageBox(NULL, _T("对不起，监控路径不存在，创建失败，请选择监控路径"), _T("提示框"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}
	
	return TRUE;
	
}
BOOL CDlgDataSend::Validate_Port(CString strPort, CString strModuleName, CString strPortName)
{
	int nPort = atoi(strPort);
	if( nPort> PORT_MAX_USE||nPort<PORT_MIN_USE)
	{
		CString strMaxPort, strMinPort;
		strMaxPort.Format(_T("%d"), PORT_MAX_USE);
		strMinPort.Format(_T("%d"), PORT_MIN_USE);
		
		CString strMsg;
        strMsg.Format(_T("对不起，你输入的%s的%s不合法，请输入%s-%s的端口号"), strModuleName, strPortName, strMinPort, strMaxPort);
		BCGPMessageBox(NULL, strMsg, _T("提示框"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}
BOOL CDlgDataSend::ValidateParam_FileModule()
{
	CString strErrMsg; 
    CString strLocalPort,strAimPort,strAimIp,strMonitorType,strIsDel,strIsSaveAs,strMonitorPath,
		strSaveAsPath,strIsSafeCheck;
	//本地端口
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->GetWindowText(strLocalPort);
	if(!Validate_Port(strLocalPort, MODULE_NAME_FILE_SEND, _T("本地端口")))
		return FALSE;
	
	//目的端口
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_FILE))->GetWindowText(strAimPort);
	if(!Validate_Port(strAimPort, MODULE_NAME_FILE_SEND, _T("目的端口")))
		return FALSE;
	
	//目的地址
    ((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_FILE))->GetWindowText(strAimIp);
    if(!Validate_IP(strAimIp, MODULE_NAME_FILE_SEND))
		return FALSE;
	
	//监控类型
	((CBCGPEdit*)GetDlgItem(IDC_EDT_TYPE_MONITOR))->GetWindowText(strMonitorType);
	
	//是否可删除已发送文件, 是否转存已发送文件
	int nDelSendedFile = ((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->GetCheck();
	int nSaveAsSendedFile = ((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->GetCheck();
	if(1 == nDelSendedFile)
		strIsDel = _T("1");
	else if(0 == nDelSendedFile)
		strIsDel = _T("0");
	if(1 == nSaveAsSendedFile)
		strIsSaveAs = _T("1");
	else if(0 == nSaveAsSendedFile)
		strIsSaveAs = _T("0");
	if(1==nDelSendedFile && 1==nSaveAsSendedFile)
	{
		BCGPMessageBox(NULL, _T("对不起，删除-转存已发送文件，只能设置一项，请重新选择"), _T("提示框"), MB_OK|MB_ICONERROR);
		return FALSE;
	}
	if(0==nDelSendedFile && 0==nSaveAsSendedFile)
	{
		BCGPMessageBox(NULL, _T("对不起，删除-转存已发送文件，必须设置一项，请重新选择"), _T("提示框"), MB_OK|MB_ICONERROR);
		return FALSE;
	}
    if(1 == nDelSendedFile)
	{
        ((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	else if(0 == nDelSendedFile)
	{
		//((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	if(1 == nSaveAsSendedFile)
	{
		//((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	else if(0 == nSaveAsSendedFile)
	{
		((CBCGPButton*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->EnableWindow(FALSE);
	}
	
	//监控路径,转存路径
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->GetWindowText(strMonitorPath);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->GetWindowText(strSaveAsPath);
	//监控路径和转存路径不能为同一个路径
    if(strMonitorPath == strSaveAsPath)
	{
		BCGPMessageBox(NULL, _T("你设置的监控路径和转存路径为同一个路径，软件无法运行，请区分这两个路径"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
        return FALSE;
	}
	if(1==nSaveAsSendedFile)
	{
		if(!CheckSaveAndMonitorPath(strSaveAsPath, strMonitorPath,TRUE))
			return FALSE;
	}
	else if(0==nSaveAsSendedFile)
	{
		if(!CheckSaveAndMonitorPath(strSaveAsPath, strMonitorPath,FALSE))
			return FALSE;
	}
	
	//安全检查待发送文件
    int nSafeCheck = ((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->GetCheck();
	if(1 == nSafeCheck)
		strIsSafeCheck = _T("1");
	else if(0 == nSafeCheck)
		strIsSafeCheck = _T("0");
	
	
	//保存文件传输模块界面元素
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT, strLocalPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_IP, strAimIp);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_PORT, strAimPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH, strMonitorPath);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_TYPE, strMonitorType);
    COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_DEL_FILE, strIsDel);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_RESAVE_FILE, strIsSaveAs);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK, strIsSafeCheck);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH, strSaveAsPath);
	
	return TRUE;
}

BOOL CDlgDataSend::ValidateParam_EndModule()
{
	CString strLocalPort,strAimIp,strAimPort, strIsCircuitCheck;
	//本地端口
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_END))->GetWindowText(strLocalPort);
	if(!Validate_Port(strLocalPort, MODULE_NAME_END_SEND, _T("本地端口")))
	{
		return FALSE;
	}
	
	//目的地址
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_LOCAL_END))->GetWindowText(strAimIp);
	if(!Validate_IP(strAimIp, MODULE_NAME_END_SEND))
		return FALSE;
	
	//目的端口
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_END))->GetWindowText(strAimPort);
	if(!Validate_Port(strAimPort, MODULE_NAME_END_SEND, _T("目的端口")))
		return FALSE;
	
	int nCircuitCheck = ((CBCGPButton*)GetDlgItem(IDC_CHK_CIRCUIT))->GetCheck();
	if(1 == nCircuitCheck)
		strIsCircuitCheck = _T("1");
	else if(0==nCircuitCheck)
        strIsCircuitCheck = _T("0");
	
	//保存终端模块界面参数
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_END, ENU_NODE_LOCAL_PORT,strLocalPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_END, ENU_NODE_AIM_IP,strAimIp);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_END, ENU_NODE_AIM_PORT,strAimPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_END, ENU_NODE_IS_VERIF_RETURN,strIsCircuitCheck);
	
	return TRUE;
}
BOOL CDlgDataSend::ValidateParam_DataModule()
{
	CString strLocalPort,strAimIp,strAimPort;
	//本地端口
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->GetWindowText(strLocalPort);
	if(!Validate_Port(strLocalPort, MODULE_NAME_DATA_SEND, _T("本地端口")))
		return FALSE;
	
	//目的地址
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_DATA))->GetWindowText(strAimIp);
	if(!Validate_IP(strAimIp, MODULE_NAME_DATA_SEND))
		return FALSE;
	
	//目的端口
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_DATA))->GetWindowText(strAimPort);
	if(!Validate_Port(strAimPort, MODULE_NAME_DATA_SEND, _T("目的端口")))
		return FALSE;
	
	//保存文件传输界面参数    
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT, strLocalPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_IP, strAimIp);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_PORT, strAimPort);
	
	
	return TRUE;
	
}
BOOL CDlgDataSend::ValidateParam_MediaModule()
{
    CString strLocalPort,strAimIp,strAimPort;
	//本地端口
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->GetWindowText(strLocalPort);
	if(!Validate_Port(strLocalPort, MODULE_NAME_MEDIA_SEND, _T("本地端口")))
		return FALSE;
	
	//目的地址
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_MEDIA))->GetWindowText(strAimIp);
	if(!Validate_IP(strAimIp, MODULE_NAME_DATA_SEND))
		return FALSE;
	
	//目的端口
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_MEDIA))->GetWindowText(strAimPort);
	if(!Validate_Port(strAimPort, MODULE_NAME_DATA_SEND, _T("目的端口")))
		return FALSE;
	
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT, strLocalPort);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_IP, strAimIp);
	COperXml::GetInstance()->SetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_PORT, strAimPort);
	
	
	return TRUE;
}

inline BOOL CDlgDataSend::ValidatePermissions(BOOL bInitDlg, CString strModuleName)
{
	//验证权限
	BOOL bRet = COutLogIn::GetInstance()->ValidatePermissions(theApp.m_strUserName,strModuleName);
	if(!bRet)
	{
		CString strMsg;
		strMsg.Format(_T("对不起，你没有%s的权限"),strModuleName);
		
		if(!bInitDlg)
			BCGPMessageBox(NULL, strMsg, _T("提示框"), MB_OK|MB_ICONINFORMATION);
		
		return FALSE;
	}
	
	return TRUE;
}
/***************************************************************
函数名：void OnFilePathMonitor()

  功能描述：选择监控路径
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::OnFilePathMonitor() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_SEND))
		return ;
	
	//选择监控路径
	TCHAR wstrFolder[MAX_PATH];
	ZeroMemory(&wstrFolder,sizeof(wstrFolder)); 
    BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(BROWSEINFO));   
	bi.hwndOwner = this->GetSafeHwnd();   
	bi.pszDisplayName = wstrFolder;   
	bi.lpszTitle = _T("选择接收目录");   
	
	LPITEMIDLIST   idl=SHBrowseForFolder(&bi);   
	if(NULL == idl)   
		return ;   
	
	SHGetPathFromIDList(idl, wstrFolder); 
	
	//显示监控路径
	CString strFolderPath;
	strFolderPath.Format(_T("%s"), wstrFolder);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->SetWindowText(strFolderPath);
	
	//记日志
	CString strLogMsg;
	strLogMsg.Format(_T("设置监控路径 %s\r\n"), strFolderPath);
	COutLogMgr::GetInstance()->LogWriter(Success, strLogMsg.GetBuffer(strLogMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success,strLogMsg));

	CheckMonitorPathBeforeSend();
}

/***************************************************************
函数名：void OnFilePath()

  功能描述：选择转储路径
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::OnFilePath() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_SEND))
		return ;
	
	//选择转储路径
	TCHAR wstrFolder[MAX_PATH];
	ZeroMemory(&wstrFolder,sizeof(wstrFolder)); 
	
    BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(BROWSEINFO));   
	bi.hwndOwner = this->GetSafeHwnd();   
	bi.pszDisplayName = wstrFolder;   
	bi.lpszTitle = _T("选择接收目录");   
	
	LPITEMIDLIST   idl=SHBrowseForFolder(&bi);   
	if(NULL == idl)   
		return ;   
	
	SHGetPathFromIDList(idl, wstrFolder); 
	
	//显示转储路径
	CString strFolderPath;
	strFolderPath.Format(_T("%s"), wstrFolder);
	((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->SetWindowText(strFolderPath);
	
	//记日志
	CString strLogMsg;
	strLogMsg.Format(_T("设置转储路径 %s\r\n"), strFolderPath);
	COutLogMgr::GetInstance()->LogWriter(Success, strLogMsg.GetBuffer(strLogMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success,strLogMsg));

	CheckSavePathBeforeSend();
}

LRESULT CDlgDataSend::SpeedSending(WPARAM wpa, LPARAM lpa)
{
	CString strValue;
	strValue.Format(_T("已发送: %s - %d%%"),m_strFileSending, wpa);
	//((CEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->Invalidate(TRUE);
    GetDlgItem(IDC_EDT_STATUS_FILE)->SetWindowText(strValue);
	((CEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->Invalidate(FALSE);
	return 0;
}

/***************************************************************
函数名：BOOL GetFilesToSend()

  功能描述：获得要发送的文件列表
  
	参数：CStringList& strLstFiles  要发送的文件列表
	
	  全局变量：无
	  
		返回值: BOOL
		
****************************************************************/
BOOL CDlgDataSend::GetFilesToSend()
{
	
	//权限验证
	//if(!ValidatePermissions(TRUE, MODULE_NAME_FILE_SEND))
	//	return FALSE;
	
	CString strMonitorType_File, strMonitorPath_File;
	strMonitorType_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_TYPE);
	strMonitorPath_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
	if(strMonitorType_File.IsEmpty())
		return FALSE;
    if(strMonitorPath_File.IsEmpty())
		return FALSE;
	//BOOL bStop = ((CEdit*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	//if(!bStop)
	//	return FALSE;
	
	if(!PathFileExists(strMonitorPath_File))
		return FALSE;
	
	//处理监控类型列表
	CStringList strLstTypeFile;
	strLstTypeFile.RemoveAll();
	for(int nIndex=0; -1!=nIndex; )
	{
		int nLastIndex = nIndex; 
		nIndex = strMonitorType_File.Find(';',nIndex+1);
		
		
		if(-1==nIndex || nIndex == nLastIndex)
		{
			CString strType;
			strType.Empty();
			for(int i=nLastIndex;i<strMonitorType_File.GetLength();i++)
			{
				if(0!=nLastIndex && i==nLastIndex)
				{
					continue;
				}
				strType += strMonitorType_File.GetAt(i);
			}
			strLstTypeFile.AddTail(strType);
			break;
		}
		else
		{
			CString strType;
			strType.Empty();
			for(int i=nLastIndex;i<nIndex;i++)
			{
				if(0!=nLastIndex && i==nLastIndex)
				{
					continue;
				}
				strType += strMonitorType_File.GetAt(i);
			}
			strLstTypeFile.AddTail(strType);
		}
	}
	
	if(strLstTypeFile.IsEmpty())
	{
		strLstTypeFile.AddTail(strMonitorType_File);
	}
    
	//根据列表中的监控类型，筛选需要发送的文件列表
	POSITION pos = strLstTypeFile.GetHeadPosition();
    for(;NULL!=pos; )
	{
		CString strType = strLstTypeFile.GetNext(pos);
		//modify 2014-02-28 begin 
		//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
        //GetFiles(strMonitorPath_File, strType);
		m_strChildDirectory.Empty();
		FindDirectory(strMonitorPath_File, strType);
		//modify 2014-02-28 end
	}
	return TRUE;
}
//add 2014-02-28 begin
//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
void CDlgDataSend::FindDirectory(CString strMonitorPath_File, CString strType)
{
	CString strFindDirectory = strMonitorPath_File;
	if (strFindDirectory.Right(3) != _T("*.*"))   
		strFindDirectory += (_T("\\*.*"));     
	CFileFind findDirectory; 
	BOOL bFind = findDirectory.FindFile(strFindDirectory);   
	while (bFind)  
	{        
		bFind = findDirectory.FindNextFile();   
		if (findDirectory.IsDots())   
		{ 
			if(m_strChildDirectory.IsEmpty())
				continue;
			else
			{
				m_strChildDirectory.TrimRight('\\');
				if(-1 == m_strChildDirectory.Find('\\'))
				{
					m_strChildDirectory.Empty();
					return;
				}
				else
				{
					int nIndex = m_strChildDirectory.ReverseFind('\\');
					m_strChildDirectory.SetAt(nIndex+1,'\0');
					return;
				}
			}
		}      
		if (findDirectory.IsDirectory())    
		{              
 			CString strNextPath = findDirectory.GetFilePath();
			//CString strTempPath = strNextPath;
			strNextPath.Replace(strMonitorPath_File,NULL);
			strNextPath.TrimLeft('\\');
			if(strNextPath == _T(".") || strNextPath == _T(".."))
				continue;
			strNextPath += _T("\\");

            //CString strTempChildDirectory = m_strChildDirectory;
			//strTempChildDirectory.TrimRight(_T("\\"));
			//if(strTempPath.Right(strTempChildDirectory.GetLength()) == strTempChildDirectory
			//	&& !m_strChildDirectory.IsEmpty())
			//{
            //  CString strNextPath = strMonitorPath_File;
			//  strNextPath.TrimRight(_T("\\"));
			//  strNextPath += _T("\\");
            //  strNextPath += strTempChildDirectory;
            //  FindDirectory(strNextPath,  strType);
			//}
			//else
			  m_strChildDirectory = strNextPath;

			  CString strPath = strMonitorPath_File;
			  if(!m_strChildDirectory.IsEmpty())
			  {
                 strPath += _T("\\");
                 strPath += m_strChildDirectory;
			  }
			  
            GetFiles(strMonitorPath_File, strType);   
		}           
		else         
		{              
			GetFiles(strMonitorPath_File, strType);    
		}         
	}
} 
//add 2014-02-28 end
/***************************************************************
函数名：void GetFiles()

  功能描述：根据文件路径和扩展名搜索此目录下特定的文件
  
	参数：CStringList& strLstFile 返回的文件列表
	CString strPath         要搜索的文件路径
	CString strExtend       文件的扩展名
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::GetFiles(CString strPath, CString strExtend)
{
	if(strPath.IsEmpty())
		return;
	if(!PathFileExists(strPath))
		return;
	
	CFileFind fileFind;
	BOOL bFound; //判断是否成功找到文件
	if( _T("\\") == strPath.Right(1))
	{
		//modify lgp 2014-02-28 begin
		//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
		//bFound=fileFind.FindFile(strPath   +   strExtend); 
        if(m_strChildDirectory.IsEmpty())
			bFound=fileFind.FindFile(strPath   +   strExtend); 
		else
			bFound=fileFind.FindFile(strPath   +   m_strChildDirectory + strExtend); 
		//modify lgp 2014-02-28 end
	}
	else
	{
		//modify 2014-02-28 begin
		//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
		//bFound=fileFind.FindFile(strPath   +   "\\" + strExtend);  
		if(m_strChildDirectory.IsEmpty())
			bFound=fileFind.FindFile(strPath   +   "\\" + strExtend);  
		else
			bFound=fileFind.FindFile(strPath   +   "\\" + m_strChildDirectory + strExtend);  
		//modify 2014-02-28 end
	}
	
	while(bFound)      //遍历所有文件
    { 
		
        bFound=fileFind.FindNextFile(); //第一次执行FindNextFile是选择到第一个文件，以后执行为选择
		
		if(fileFind.IsDots())
		{
			continue; 
		}//如果找到的是返回上层的目录 则结束本次查找
		
		if(fileFind.IsDirectory())   //找到的是文件夹，则遍历该文件夹下的文件
		{ 
			//continue;
			CString strPath = fileFind.GetFilePath();
			FindDirectory(strPath, strExtend);
		} 
		else 	
		{   
			CString strPath =fileFind.GetFilePath(); //保存文件名，包括后缀名
			//add lgp 12-18 begin
			CString strFileExt = strPath.Right(strPath.GetLength() - strPath.ReverseFind(_T('.')) -1);
			CString strExtTemp = strExtend.Right(strExtend.GetLength()-2);
            if( strExtTemp != strFileExt)
				continue;
			//add lgp 12-18 end
			m_lockSendFileList.Lock();
			//modify lgp 2014-02-28 begin
	        //监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
			/*if(NULL == m_strLstFiles.Find(strPath))
				m_strLstFiles.AddTail(strPath);*/
			if(NULL == m_strLstFiles.Find(strPath))
			{
				m_strLstFiles.AddTail(strPath);
            }
            //modify lgp 2014-02-28 end
			m_lockSendFileList.Unlock();
			SetEvent(m_hEventSendFile);
		} 
		
    } 
	m_strChildDirectory.Empty();
    fileFind.Close(); 
}


BOOL CDlgDataSend::DelOrSaveAfterSended(CString strSendedFile/*绝对路径*/)
{
	CString strSaveAsPath_File= COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_SAVE_PATH);
	CString strIsSaveAsSendedFile_File = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_RESAVE_FILE);
	CString strIsDelSendedFile_File =  COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_DEL_FILE);
	
	
	if(_T("1") == strIsSaveAsSendedFile_File)
	{
		if(strSaveAsPath_File.IsEmpty())
			return FALSE;
		if(!PathFileExists(strSaveAsPath_File))
			return FALSE;
		
		CString strFileName = GetFileNameFromPath(strSendedFile);
		CString strNewPath;
		if(_T("\\") == strSaveAsPath_File.Right(1))
		{
			CString strNewFileName =  AddDateToFileName(strFileName);
			strNewPath.Format(_T("%s%s"), strSaveAsPath_File,strNewFileName);
		}
		else
		{
			CString strNewFileName =  AddDateToFileName(strFileName);
			strNewPath.Format(_T("%s\\%s"), strSaveAsPath_File, strNewFileName);
		}
		//检查是否有子路径
		CString strMonitorPath = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_PATH);
        CString strSendedFileTemp = strSendedFile;
        strSendedFileTemp.Replace(strMonitorPath,NULL);
        strSendedFileTemp.TrimLeft(_T("\\"));
		if(strSendedFileTemp.Find(_T("\\")) == -1)
		   MoveFile(strSendedFile, strNewPath);
		else
		{
			CString strRoot = strSaveAsPath_File;
			strRoot.TrimLeft(_T("\\"));
			CString strChildDir = strSendedFileTemp;
			strChildDir.SetAt(strChildDir.ReverseFind('\\')+1,'\0');
			CString strFullPath = strRoot;
			while(strChildDir.Find(_T("\\"))!=-1 && !strChildDir.IsEmpty())
			{
				int nLen = strChildDir.Find(_T("\\"));
				CString strChild = strChildDir.Left(nLen);
				strFullPath += _T("\\");
				strFullPath += strChild;
				if (!PathFileExists(strFullPath))
				{
					BOOL bRet = CreateDirectory(strFullPath,NULL);
					strChildDir = strChildDir.Right(strChildDir.GetLength()-nLen-1);
					if(!bRet)
					{
					    break;
					}
				}
				strChildDir = strChildDir.Right(strChildDir.GetLength()-nLen-1);
			}
            strFullPath.TrimRight(_T("\\"));
			strFullPath += _T("\\");
			strFullPath += AddDateToFileName(strFileName);;
            MoveFile(strSendedFile, strFullPath);
		}
	}
	
	//删除已发送文件
	if(_T("1") == strIsDelSendedFile_File)
	{
		DeleteFile(strSendedFile);
	}
	
	return TRUE;
}
CString CDlgDataSend::AddDateToFileName(CString strFileName)
{
	CString strTime;
	CTime curTime = CTime::GetCurrentTime();
	strTime.Format(_T("_%d%02d%02d%02d%02d%02d"),curTime.GetYear(),curTime.GetMonth(),\
		curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());
	
    strFileName.Insert(strFileName.ReverseFind('.'), strTime);
	
	return strFileName;
}
/***************************************************************
函数名：void OnStartFile()

  功能描述：开始文件传输
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
**************************************************************/
void CDlgDataSend::OnStartFile() 
{
	// TODO: Add your control notification handler code here
    //权限验证
	if(!ValidatePermissions(m_bInitDlg, MODULE_NAME_FILE_SEND))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
		return;
	}
	
    
	//检查其他界面元素是否有变化，提示是否保存生效
	if(!CheckUiAndXmlBeforeStartFile())
		return;
   
    //启动文件传输服务
	if(!InitParamServ_End(TRUE, ENU_MODULE_FILE))
		return;
    
	m_bNoDelStopFileServ = FALSE;
    
	//modify lgp 11-13 begin
	//ResumeThread(m_hSendFileThread);//20131019 文件发送线程唤醒
	//ResumeThread(m_hMonitorThread);//20131019 文件监视线程唤醒
    //SetEvent(m_hEventSendFile);
	ResumeThread(m_hMonitorThread);//20131019 文件监视线程唤醒
	SetEvent(m_hEventSendFile);
	ResumeThread(m_hSendFileThread);//20131019 文件发送线程唤醒
    //modify lgp 11-13 end

	this->SetFocus();
	
}

/***************************************************************
函数名：void GetFileNameFromPath()

  功能描述：根据文件的路径获得文件名
  
	参数：CString strFilePath：文件的路径
	
	  全局变量：无
	  
		返回值: CString  文件名
		
****************************************************************/
CString CDlgDataSend::GetFileNameFromPath(CString strFilePath)
{
	int Where;
	Where = strFilePath.ReverseFind('\\');
	if (-1 == Where)
		Where = strFilePath.ReverseFind('/');
	CString FileTitle = strFilePath.Right(strFilePath.GetLength() - 1 - Where);
	
	return FileTitle;
	
}
BOOL CDlgDataSend::StopServ_FileByUser()//返回值:函数中是否进行了文件服务停止操作是否
{
	if(1 == m_pTransControl_File->GetState())//正在传输文件
	{
		if( IDYES != BCGPMessageBox(NULL, _T("正在传输文件，是否确定停止"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(FALSE);
			return FALSE;
		}
	}
	
	//停止文件发送

    //add lgp 11-07 begin
	m_bNoDelStopFileServ = TRUE;
	//add lgp 11-07 end
	m_pTransControl_File->StopTrans();

	ResetEvent(m_hEventSendFile);
	//add lgp 11-11 begin
	DWORD dSend = SuspendThread(m_hSendFileThread);
	//add lgp 11-11 end
	DWORD dMonitor = SuspendThread(m_hMonitorThread);
    CString strErr;
	strErr.Format(_T("SuspendThread %d--%d"),dSend, dMonitor);
	
    
	
	CString strStopMsg = _T("发送文件服务停止\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(_T(""));
	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_FILE))->SetWindowText(strStopMsg);
	COutLogMgr::GetInstance()->LogWriter(Success, strStopMsg.GetBuffer(strStopMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success,strStopMsg));
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_FILE))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_FILE))->EnableWindow(TRUE);
	

	return TRUE;
}
BOOL CDlgDataSend::StopServ_DataByUser()//返回值:函数中是否进行了数据服务停止操作是否
{
	if(1 == m_pTransControl_Data->GetState())//已经处于停止状态
	{
		if( IDYES != BCGPMessageBox(NULL, _T("正在传输数据，是否确定停止"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(FALSE);
			return FALSE;
		}
	}
	
	
    //停止数据发送
	m_pTransControl_Data->StopTrans();
	
	
	CString strStopMsg = _T("发送数据服务停止\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(_T(""));
	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_DATA))->SetWindowText(strStopMsg);
	COutLogMgr::GetInstance()->LogWriter(Success, strStopMsg.GetBuffer(strStopMsg.GetLength()));
	m_lstLogData.InsertItem(0, GetLog(Success,strStopMsg));
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(TRUE);
	
	return TRUE;
}
BOOL CDlgDataSend::StopServ_MediaByUser()//返回值:函数中是否进行了流媒体服务停止操作是否
{
	if(1 == m_pTransControl_Media->GetState())//已经处于停止状态
	{
		if( IDYES != BCGPMessageBox(NULL, _T("正在传输流媒体，是否确定停止"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(TRUE);
			((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(FALSE);
			return FALSE;
		}
	}
	
    //停止流媒体发送
	m_pTransControl_Media->StopTrans();
	
	CString strStopMsg = _T("发送流媒体服务停止\r\n");
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(_T(""));
	
	((CBCGPEdit*)GetDlgItem(IDC_EDT_STATUS_MEDIA))->SetWindowText(strStopMsg);
	COutLogMgr::GetInstance()->LogWriter(Success, strStopMsg.GetBuffer(strStopMsg.GetLength()));
	m_lstLogMedia.InsertItem(0, GetLog(Success,strStopMsg));
	((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);
	((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(TRUE);
	
	return TRUE;
}
/***************************************************************
函数名：void OnStopFile()

  功能描述：停止文件传输
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::OnStopFile() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_SEND))
		return;
	
	
	StopServ_FileByUser();
	
	this->SetFocus();
}

/***************************************************************
函数名：void OnRestartFile()

  功能描述：重启文件传输服务
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::OnRestartFile() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_SEND))
		return;
	
	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	if(bStop)//已经处于启动状态
	{
		if(StopServ_FileByUser())
			OnStartFile();
	}
	else
	{
        //OnStartFile();
	}
    //ResumeThread(m_hSendFileThread);
	//ResumeThread(m_hMonitorThread);
	
	this->SetFocus();
}

/***************************************************************
函数名：void OnSaveParamFile()

  功能描述：保存文件传输参数
  
	参数：无
	
	  全局变量：无
	  
		返回值: 无
		
****************************************************************/
void CDlgDataSend::OnSaveParamFile() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_SEND))
		return;
	
	CString strMsg;
	strMsg.Format(_T("保存%s界面参数\r\n"),MODULE_NAME_FILE_SEND);
	
    //参数合法性检查
	if(!ValidateParam_FileModule())
		return;
	
	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	if(bStop)//处于启动状态，则先停止
		OnStopFile();
	InitServ_File();
	if(bStop)//原始状态为启动状态，则初始化后自动启动
		OnStartFile();
	
	//记日志
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	m_lstLogFile.InsertItem(0, GetLog(Success,strMsg));
}



//启动文件传输服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
BOOL CDlgDataSend::CheckUiAndXmlBeforeStartFile()
{
	//检查监控路径与转存路径是否相同
	CString strMonitorPath, strSavePath;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_FILE_SAVE_PATH))->GetWindowText(strSavePath);
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PATH_MONITOR))->GetWindowText(strMonitorPath);
	if(strSavePath == strMonitorPath)
	{
		BCGPMessageBox(NULL, _T("你设置的监控路径和转存路径为同一个路径，文件传输服务无法启动，请区分这两个路径"), _T("提示框"), MB_OK|MB_ICONINFORMATION);
        return FALSE;
	}
    
	//检查转存路径是否有变化，提示是否保存生效
	 //BOOL bCheckSavePath= CheckSavePathBeforeSend();
	 
    //检查监控路径是否有变化，提示是否保存生效
	//BOOL bCheckMonitorPath = CheckMonitorPathBeforeSend();
	//if( (!bCheckSavePath) && (!bCheckMonitorPath))
	//if(!bCheckMonitorPath)
	//    return FALSE;


	if(!m_bSendFileUIChanged)
		return TRUE;

	//读取配置文件数据
	CString strLocalPort_Xml,strAimPort_Xml,strAimIp_Xml,strMonitorType_Xml, 
		    strIsDel_Xml, strIsSafeCheck_Xml, strIsSave_Xml;
	strLocalPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_LOCAL_PORT);
	strAimPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_PORT);
	strAimIp_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_AIM_IP);
	strMonitorType_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_MONITOR_TYPE);
	strIsDel_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_DEL_FILE);
	strIsSafeCheck_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_SAFE_CHECK);
	strIsSave_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_FILE, ENU_NODE_IS_RESAVE_FILE);

	//读取UI数据
	CString strLocalPort_Ui,strAimPort_Ui,strAimIp_Ui, strMonitorPath_Ui, strMonitorType_Ui, 
		    strIsDel_Ui, strIsSafeCheck_Ui, strIsSave_Ui, strSavePath_Ui;
     ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->GetWindowText(strLocalPort_Ui);
	 ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_FILE))->GetWindowText(strAimPort_Ui);
	 ((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_FILE))->GetWindowText(strAimIp_Ui);
	 ((CBCGPEdit*)GetDlgItem(IDC_EDT_TYPE_MONITOR))->GetWindowText(strMonitorType_Ui);
	 if(1 == ((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->GetCheck())
         strIsDel_Ui = _T("1");
	 else
         strIsDel_Ui = _T("0");
	 if(1 == ((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->GetCheck())
         strIsSave_Ui = _T("1");
	 else
         strIsSave_Ui = _T("0");
	 if(1 == ((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->GetCheck())
         strIsSafeCheck_Ui = _T("1");
	 else
         strIsSafeCheck_Ui = _T("0");

	//对比
	 if( (strLocalPort_Xml != strLocalPort_Ui) || (strAimPort_Xml!=strAimPort_Ui) || 
		 (strAimIp_Xml!=strAimIp_Ui) ||(strMonitorType_Xml!=strMonitorType_Ui)||
		 (strIsDel_Xml!=strIsDel_Ui)||(strIsSafeCheck_Xml!=strIsSafeCheck_Ui)||
		 (strIsSave_Xml!=strIsSave_Ui)
		)
	 {
		 if( IDYES != BCGPMessageBox(NULL, _T("文件传输模块界面传输参数有更新，是否保存生效"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		 {
			 ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_FILE))->SetWindowText(strLocalPort_Xml);
			 ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_FILE))->SetWindowText(strAimPort_Xml);
			 ((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_FILE))->SetWindowText(strAimIp_Xml);
	         ((CBCGPEdit*)GetDlgItem(IDC_EDT_TYPE_MONITOR))->SetWindowText(strMonitorType_Xml);
			 if(strIsDel_Xml == _T("1"))
                  ((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->SetCheck(1);
			 else
				  ((CBCGPButton*)GetDlgItem(IDC_RDO_DELETE_SENDED_FILE))->SetCheck(0);
			 if(strIsSave_Xml == _T("1"))
				 ((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->SetCheck(1);
			 else
				  ((CBCGPButton*)GetDlgItem(IDC_RAO_SAVE_SENDED_FILE))->SetCheck(0);
			 if(strIsSafeCheck_Xml == _T("1"))
				 ((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->SetCheck(1);
			 else
				  ((CBCGPButton*)GetDlgItem(IDC_CHK_CHECK_FILE))->SetCheck(0);
			 return TRUE;
		 }
		 if(!ValidateParam_FileModule())
		 {
			 m_bSendFileUIChanged = FALSE;
			 return FALSE;
		 }
	 }

    m_bSendFileUIChanged = FALSE;
    return TRUE;	
}
//启动数据传输服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
BOOL CDlgDataSend::CheckUiAndXmlBeforeStartData()
{
	//获取配置文件信息
    CString strLocalPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_LOCAL_PORT);
	CString strAimIp_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_IP);
	CString strAimPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_DATA, ENU_NODE_AIM_PORT);
	
	CString strLocalPort_Ui, strAimIp_Ui, strAimPort_Ui;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->GetWindowText(strLocalPort_Ui);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_DATA))->GetWindowText(strAimIp_Ui);
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_DATA))->GetWindowText(strAimPort_Ui);

	if( (strLocalPort_Ui!=strLocalPort_Xml) || (strAimIp_Ui!=strAimIp_Xml) ||
		(strAimPort_Ui!=strAimPort_Xml))
	{
		if( IDYES != BCGPMessageBox(NULL, _T("数据传输模块界面传输参数有更新，是否保存生效"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_DATA))->SetWindowText(strLocalPort_Xml);
			((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_DATA))->SetWindowText(strAimIp_Xml);
            ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_DATA))->SetWindowText(strAimPort_Xml);
			return TRUE;
		}
		if(!ValidateParam_DataModule())
			 return FALSE;
	}
	return TRUE;
}
//启动流媒体传输服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
BOOL CDlgDataSend::CheckUiAndXmlBeforeStartMedia()
{
	//获取配置文件信息
    CString strLocalPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_LOCAL_PORT);
	CString strAimIp_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_IP);
	CString strAimPort_Xml = COperXml::GetInstance()->GetNodeValue(ENU_MODULE_MEDIA, ENU_NODE_AIM_PORT);
	
	CString strLocalPort_Ui, strAimIp_Ui, strAimPort_Ui;
	((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->GetWindowText(strLocalPort_Ui);
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_MEDIA))->GetWindowText(strAimIp_Ui);
    ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_MEDIA))->GetWindowText(strAimPort_Ui);
	
	if( (strLocalPort_Ui!=strLocalPort_Xml) || (strAimIp_Ui!=strAimIp_Xml) ||
		(strAimPort_Ui!=strAimPort_Xml))
	{
		if( IDYES != BCGPMessageBox(NULL, _T("流媒体传输模块界面传输参数有更新，是否保存生效"), _T("提示框"), MB_YESNO|MB_ICONINFORMATION))
		{
			((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_LOCAL_MEDIA))->SetWindowText(strLocalPort_Xml);
			((CIPAddressCtrl*)GetDlgItem(IDC_IP_AIM_MEDIA))->SetWindowText(strAimIp_Xml);
            ((CBCGPEdit*)GetDlgItem(IDC_EDT_PORT_RECEIVE_MEDIA))->SetWindowText(strAimPort_Xml);
			return TRUE;
		}
		if(!ValidateParam_MediaModule())
			return FALSE;
	}
	return TRUE;
}
/***************************************************************
函数名：void OnStartData()

  功能描述：开始数据传输
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnStartData() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(m_bInitDlg, MODULE_NAME_DATA_SEND))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_DATA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->EnableWindow(FALSE);
	    ((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_DATA))->EnableWindow(FALSE);
		return;
	}

	if(!CheckUiAndXmlBeforeStartData())
		return;

	InitParamServ_End(TRUE, ENU_MODULE_DATA);
	

	this->SetFocus();
}
/***************************************************************
函数名：void OnStopData()

  功能描述：停止数据传输
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnStopData() 
{
	// TODO: Add your control notification handler code here
	//10-29 begin
	m_bTest = FALSE;
	//10-29 end
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_SEND))
		return;
	

	StopServ_DataByUser();

	this->SetFocus();
}

/***************************************************************
函数名：void OnRestartData()

  功能描述：重启数据传输服务
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnRestartData() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_SEND))
		return;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->IsWindowEnabled();
	if(bStop)//已经处于启动状态
	{
        if(StopServ_DataByUser())
	         OnStartData();
	}
	else
	{
	     //OnStartData();
	}

   //ResumeThread(m_hSendDataThread);

	this->SetFocus();
}

/***************************************************************
函数名：void OnSaveParamData()

  功能描述：保存数据传输界面参数
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnSaveParamData() 
{
	// TODO: Add your control notification handler code here

	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_SEND))
		return;

	CString strMsg;
	strMsg.Format(_T("保存%s界面参数\r\n"),MODULE_NAME_DATA_SEND);
	
	if(!ValidateParam_DataModule())
		 return;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->IsWindowEnabled();
	if(bStop)//处于启动状态，则先停止
    	OnStopData();
	InitServ_Data();
	if(bStop)//原始状态为启动状态，则初始化后自动启动
	    OnStartData();

	//记日志
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	m_lstLogData.InsertItem(0, GetLog(Success,strMsg));
}



/***************************************************************
函数名：void OnStartMedia()

  功能描述：开始流媒体传输
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnStartMedia() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(m_bInitDlg, MODULE_NAME_MEDIA_SEND))
	{
		((CBCGPButton*)GetDlgItem(IDC_BUT_START_MEDIA))->EnableWindow(TRUE);
		((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->EnableWindow(FALSE);
    	((CBCGPButton*)GetDlgItem(IDC_BUT_RESTART_MEDIA))->EnableWindow(FALSE);
		return;
	}
	
	if(!CheckUiAndXmlBeforeStartMedia())
		return;

	InitParamServ_End(TRUE, ENU_MODULE_MEDIA);

	this->SetFocus();
}

/***************************************************************
函数名：void OnStopMedia()

  功能描述：停止流媒体传输
  
  参数：无
	
  全局变量：无
  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnStopMedia() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_SEND))
		return;
	

	StopServ_MediaByUser();

	this->SetFocus();
}

/***************************************************************
函数名：void OnRestartMedia()

  功能描述：重启流媒体传输服务
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnRestartMedia() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_SEND))
		return;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled();
	if(bStop)//已经处于启动状态
	{
     	if(StopServ_MediaByUser())
	        OnStartMedia();
	}
	else
	{
          //OnStartMedia();
	}

//	ResumeThread(m_hSendMediaThread);

	this->SetFocus();
}

/***************************************************************
函数名：void OnSaveParamMedia()

  功能描述：保存流媒体传输模块界面参数
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnSaveParamMedia() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_SEND))
		return;
	CString strMsg;
	strMsg.Format(_T("保存%s界面参数\r\n"),MODULE_NAME_MEDIA_SEND);
	
	if(!ValidateParam_MediaModule())
		 return;

	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled();
	if(bStop)//处于启动状态，则先停止
	   OnStopMedia();
	InitServ_Media();
	if(bStop)//原始状态为启动状态，则初始化后自动启动
	     OnStartMedia();

	//记日志
	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	m_lstLogMedia.InsertItem(0, GetLog(Success,strMsg));
}
void CDlgDataSend::ReStartAllServ()
{
	m_pTransControl_Media->closeRegist();
	
	//重启文件发送服务
	BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	if(bStop)//处于启动状态，则先停止
		OnStopFile();
	InitServ_File();
	if(bStop)//原始状态为启动状态，则初始化后自动启动
		OnStartFile();
	
	//重启数据发送服务
	bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->IsWindowEnabled();
	if(bStop)//处于启动状态，则先停止
		OnStopData();
	InitServ_Data();
	if(bStop)//原始状态为启动状态，则初始化后自动启动
		OnStartData();
	
	
	//重启流媒体发送服务
	bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled();
	if(bStop)//处于启动状态，则先停止
		OnStopMedia();
	InitServ_Media();
	if(bStop)//原始状态为启动状态，则初始化后自动启动
		OnStartMedia();
	
}
/***************************************************************
函数名：void OnSaveParamEnd()

  功能描述：保存终端模块界面参数
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDlgDataSend::OnSaveParamEnd() 
{
	// TODO: Add your control notification handler code here
	//权限验证
	if(!ValidatePermissions(FALSE, MODULE_NAME_END_SEND))
		return;
	CString strMsg;
	strMsg.Format(_T("保存%s界面参数\r\n"),MODULE_NAME_END_SEND);
	
	if(!ValidateParam_EndModule())
		 return;

     ReStartAllServ();

	COutLogMgr::GetInstance()->LogWriter(Success, strMsg.GetBuffer(strMsg.GetLength()));
	
}

void CDlgDataSend::OnDestroy() 
{   
	//先结束发送服务，再退出发送线程

	m_pTransControl_Data->StopTrans();
	m_pTransControl_Media->StopTrans();
	//如果文件正在传输，停止传输，不删除文件
	if(!m_bNoDelStopFileServ)
	{
		m_bNoDelStopFileServ = TRUE;
		m_pTransControl_File->StopTrans();
    }
	g_bMonitorFile = FALSE;
	g_bSendFile = FALSE;
	WaitForSingleObject(m_hMonitorThread,100);
	SetEvent(m_hEventSendFile);
	WaitForSingleObject(m_hSendFileThread,100);
	if(NULL != m_hEventSendFile)
	{
		CloseHandle(m_hEventSendFile);
		m_hEventSendFile = NULL;
	}
	CloseHandle(m_hSendFileThread);
    CloseHandle(m_hMonitorThread);
	m_hSendFileThread = NULL;
	m_hMonitorThread = NULL;

	g_bSendData = FALSE;
	g_bSendMedia = FALSE;
	SetEvent(m_hEventSendData);
	WaitForSingleObject(m_hSendDataThread,100);
	CloseHandle(m_hSendDataThread);
	m_hSendDataThread = NULL;
	SetEvent(m_hEventSendMedia);
	WaitForSingleObject(m_hSendMediaThread,100);
	CloseHandle(m_hSendMediaThread);
	m_hSendMediaThread = NULL;
	m_pTransControl_File->closeRegist();
	m_pTransControl_Data->closeRegist();
	m_pTransControl_Media->closeRegist();

	
	m_pTransControl_Data = NULL;
    m_pTransControl_Media = NULL;
    m_pTransControl_File = NULL;

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
/***************************************************************
  函数名：HRESULT OnSendFile_Msg()

  功能描述：发送文件消息响应函数
  
  参数：WPARAM par 预留 为0
        LPARAM lpa 要发送文件的完整路径的长度
	
  全局变量：无
	  
  返回值: HRESULT
		
****************************************************************/
 HRESULT CDlgDataSend::OnSendFile_Msg(WPARAM wpa, LPARAM lpa)
 {
	 BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_FILE))->IsWindowEnabled();
	 if(!bStop)
		 return 0;

	 //权限验证
	 if(!ValidatePermissions(FALSE, MODULE_NAME_FILE_SEND))
		return 0;
     //获得文件全路径的长度
	 int nLen = (int)lpa;
	 HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE,
		 "SockSrvDataMap");
	 if (m_hReceiveMap == NULL)
		 return 0 ;
	 LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,
		 FILE_MAP_READ,0,0,0);//获得文件映射数据区指针
	 if (m_lpbReceiveBuf == NULL)
	 {
		 CloseHandle(m_hReceiveMap);
		 m_hReceiveMap=NULL;
		 COutLogMgr::GetInstance()->LogWriter(Failure, 
			 _T("从第三方发送终端接收到的文件路径为空\r\n"));
		 m_lstLogFile.InsertItem(0, GetLog(Failure, 
			 _T("从第三方发送终端接收到的文件路径为空\r\n")));
		 return 0;
	 }

	 char chPathD[MAX_PATH];
	 memset(chPathD, 0, MAX_PATH);
	 //读取文件映射数据区的数据内容(文件全路径)
	 MoveMemory(chPathD, m_lpbReceiveBuf, nLen);

	 UnmapViewOfFile(m_hReceiveMap);
	 CloseHandle(m_hReceiveMap);
	 m_hReceiveMap = NULL;

	 //将数据内容转换成CString
	 CString pStrFilePath ;
	 pStrFilePath.Format(_T("%s"), chPathD);
     
	 m_lockSendFileList.Lock();
     m_strLstFiles.AddTail(pStrFilePath);
	 m_lockSendFileList.Unlock();
     
	 ResumeThread(m_hMonitorThread);
	 ResumeThread(m_hSendFileThread);
	 SetEvent(m_hEventSendFile);

	 return 0;
 }
 /***************************************************************
  函数名：HRESULT OnSendData_Msg()

  功能描述：发送文件消息响应函数
  
  参数：WPARAM par 保留，为0
        LPARAM lpa 要发送数据内存块的长度
	
  全局变量：无
	  
  返回值: HRESULT
		
****************************************************************/
 HRESULT CDlgDataSend::OnSendData_Msg(WPARAM wpa, LPARAM lpa)
 {
	 m_bDataInTestXn = FALSE;

	 BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->IsWindowEnabled();
	 if(!bStop)
		 return 0;

	 //权限验证
	 if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_SEND))
		return 0;

	 //获取数据长度
	 int nFilePathLen = (int)lpa;
	 //利用内存映射名称"SockSrvDataMap"打开内存映射句柄
	 HANDLE hReceiveMap = OpenFileMapping(FILE_MAP_READ, 
		 FALSE, "SockSrvDataMap");
	 if (hReceiveMap == NULL)
		 return 0 ;
	 LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(hReceiveMap,
		 FILE_MAP_READ,0,0,0);//映射到内存映射数据区域m_lpbReceiveBuf
	 if (m_lpbReceiveBuf == NULL)
	 {
		 CloseHandle(hReceiveMap);
		 hReceiveMap=NULL;
		 COutLogMgr::GetInstance()->LogWriter(Failure,
			 _T("从第三方发送终端接收到的数据为空\r\n"));
		 m_lstLogData.InsertItem(0, GetLog(Failure, 
			 _T("从第三方发送终端接收到的数据为空\r\n")));
		 return 0;
	 }
	 m_lockSendDataStatus.Lock();
	 if(m_arrDatas.GetSize()>0)
	    m_arrDatas.SetSize(m_arrDatas.GetUpperBound()+nFilePathLen+1);
	 else
        m_arrDatas.SetSize(nFilePathLen);
	 //读取内存映射数据区数据
	 for(int i=0; i<nFilePathLen; i++)
	 {
		 byte a = *(m_lpbReceiveBuf+i);
         m_arrDatas[i] = a;
	 }
	 UnmapViewOfFile(hReceiveMap);
	 CloseHandle(hReceiveMap);
	 hReceiveMap = NULL;
	 m_lockSendDataStatus.Unlock();
	 //char chPathD[LEN_DATA];
	 //memset(chPathD, 0, LEN_DATA);
	 
	 /*for (int ii=0; ii<nFilePathLen; ii++)
	 {
		 int a = *(m_lpbReceiveBuf+ii);
		 m_chData[ii] = a;
		 //chPathD[ii] = a;

	 }
	 m_uDataLen = nFilePathLen;*/

     /*if(nFilePathLen<1300)
	 {
		 CBufZoneManager buf;
		 buf.SetBufData(m_lpbReceiveBuf, nFilePathLen);
		 
		 m_lockSendDataStatus.Lock();
		 m_byteLstDatas.AddTail(buf);
		 m_lockSendDataStatus.Unlock();
		 SetEvent(m_hEventSendData);
	}
    else
	{
        
		byte chPathD[LEN_DATA];
	    memset(chPathD, 0, LEN_DATA);
		int nStep = 0;
		for (int ii=0; ii<nFilePathLen; ii++,nStep++)
		{
			int a = *(m_lpbReceiveBuf+ii);
			chPathD[nStep] = a;
			if(nStep == (LEN_DATA-1))
			{
               CBufZoneManager buf;
			   memset(chPathD, 0, LEN_DATA);
			   buf.SetBufData(chPathD, (LEN_DATA-1));
			   m_lockSendDataStatus.Lock();
			   m_byteLstDatas.AddTail(buf);
		       m_lockSendDataStatus.Unlock();
			   //SetEvent(m_hEventSendData);
			   nStep=-1;
			}
		}
		if(nStep<(LEN_DATA-1))
		{
			CBufZoneManager buf;
			buf.SetBufData(chPathD, nStep);
			m_lockSendDataStatus.Lock();
			m_byteLstDatas.AddTail(buf);
		    m_lockSendDataStatus.Unlock();
			//SetEvent(m_hEventSendData);
		}
	}*/
	 //ResumeThread(m_hSendDataThread);

	//CBufZoneManager bufHead = m_byteLstDatas.GetHead();
	//CBufZoneManager bufTail = m_byteLstDatas.GetTail();

	 SetEvent(m_hEventSendData);

     
    return 0;
 }
 /***************************************************************
  函数名：HRESULT OnSendData_TestXN()

  功能描述：发送文件消息响应函数,测试性能
  
  参数：WPARAM par 调用软件句柄（可为0，预留待用）
        LPARAM lpa 要发送数据内存块
	
  全局变量：无
	  
  返回值: HRESULT
		
****************************************************************/
 HRESULT CDlgDataSend::OnSendData_TestXN(WPARAM wpa, LPARAM lpa)
 {
	 m_bDataInTestXn = TRUE;

	 BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_DATA))->IsWindowEnabled();
	 if(!bStop)
		 return 0;

	 //权限验证
	// if(!ValidatePermissions(FALSE, MODULE_NAME_DATA_SEND))
	//	return 0;

	 long lTest = (long)lpa;
	 if(1 == lTest)
	 {
		 m_signalCt = 0;
		 m_bInOneSecond = TRUE;
		 ResumeThread(m_hSendDataTestXnThread);
	 }
	 
	 if(!m_bInOneSecond)
		  return 0;

	
	 /*int nFilePathLen = (int)wpa;
	 HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, "SockSrvDataMap");
	 if (m_hReceiveMap == NULL)
		 return 0 ;
	 LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,FILE_MAP_READ,0,0,0);*/
	 /*if (m_lpbReceiveBuf == NULL) //10-24 delete
	 {
		 CloseHandle(m_hReceiveMap);
		 m_hReceiveMap=NULL;
	 }*/
	 /*CString strMsg;
	 strMsg.Format(_T("%d"),nFilePathLen);
	 
	 char chPathD[LEN_DATA];
	 memset(chPathD, 0, LEN_DATA);*/
	 
	 /*for (int ii=0; ii<nFilePathLen; ii++)//10-24 delete
	 {
		 int a = *(m_lpbReceiveBuf++);
		 //chPathD[ii] = a;
		 m_chData[ii] = a;
	 }*/
	 //m_uDataLen = nFilePathLen;
	 
	 InterlockedIncrement(&m_signalCt);

    return 0;
 }
 /***************************************************************
  函数名：HRESULT OnSendMedia_Msg()

  功能描述：发送文件消息响应函数
  
  参数：WPARAM par 预留，为0
        LPARAM lpa 流媒体数据的长度
	
  全局变量：无
	  
  返回值: HRESULT
		
****************************************************************/
 HRESULT CDlgDataSend::OnSendMedia_Msg(WPARAM wpa, LPARAM lpa)
 {
	 BOOL bStop = ((CBCGPButton*)GetDlgItem(IDC_BUT_STOP_MEDIA))->IsWindowEnabled();
	 if(!bStop)
		 return 0;
     
	 //权限验证
	 if(!ValidatePermissions(FALSE, MODULE_NAME_MEDIA_SEND))
		return 0;
	 
	  int nFilePathLen = (int)lpa;
	  HANDLE m_hReceiveMap = OpenFileMapping(FILE_MAP_READ, 
		  FALSE, "SockSrvDataMap");
	  if (m_hReceiveMap == NULL)
	      return 0 ;

	  LPBYTE m_lpbReceiveBuf = (LPBYTE)MapViewOfFile(m_hReceiveMap,
		  FILE_MAP_READ,0,0,0);
	  if (m_lpbReceiveBuf == NULL)
	  {
		  CloseHandle(m_hReceiveMap);
		  m_hReceiveMap=NULL;
		  COutLogMgr::GetInstance()->LogWriter(Failure, 
			  _T("从第三方发送终端接收到的流媒体数据为空\r\n"));
		  m_lstLogMedia.InsertItem(0, GetLog(Failure, 
			  _T("从第三方发送终端接收到的流媒体数据为空\r\n")));
		  return 0;
	  }
	  CString strMsg;
	  strMsg.Format(_T("%d"),nFilePathLen);

	  m_lockSendMediaStatus.Lock();
	  if(m_arrMedias.GetSize()>0)
	     m_arrMedias.SetSize(m_arrMedias.GetUpperBound()+nFilePathLen+1);
	  else
         m_arrMedias.SetSize(nFilePathLen);
	  for(int i=0; i<nFilePathLen; i++)
	  {
		  byte a = *(m_lpbReceiveBuf+i);
		  m_arrMedias[i] = a;
	  }
	  UnmapViewOfFile(m_hReceiveMap);
	  CloseHandle(m_hReceiveMap);
	  m_hReceiveMap = NULL;
	  m_lockSendMediaStatus.Unlock();
		/*char chPathD[LEN_DATA];
		memset(chPathD, 0, LEN_DATA);
		
		  for (int ii=0; ii<nFilePathLen; ii++)
		  {
		  int a = *(m_lpbReceiveBuf+ii);
		  chPathD[ii] += a;
          m_chMedia[ii] = a;
		  }
		  m_uMediaLen=nFilePathLen;

		  CBufZoneManager buf;
		  buf.SetBufData(m_lpbReceiveBuf, nFilePathLen);
		  m_lockSendMediaStatus.Lock();
		  m_byteLstMedias.AddTail(buf);
		  m_lockSendMediaStatus.Unlock();
		  
		  //ResumeThread(m_hSendDataThread);*/
    SetEvent(m_hEventSendMedia);
		  
	 return 0;
 }
 

void CDlgDataSend::OnRclickLstLogFile(NMHDR* pNMHDR, LRESULT* pResult) 
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
	obMenu.LoadMenu(IDR_MENU_SEND_FILE); 
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

void CDlgDataSend::OnRclickLstLogData(NMHDR* pNMHDR, LRESULT* pResult) 
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
	obMenu.LoadMenu(IDR_MENU_SEND_DATA); 
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
void CDlgDataSend::OnRclickLstLogMedia(NMHDR* pNMHDR, LRESULT* pResult) 
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
	obMenu.LoadMenu(IDR_MENU_SEND_MEDIA); 
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
CString CDlgDataSend::GetLog(ENUM_LOG_TYPE enuLogType, CString strErrinfo)
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

	//整理日志内容格式
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

void CDlgDataSend::OnChangeEdtPortLocalFile() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bSendFileUIChanged = TRUE;
}

void CDlgDataSend::OnFieldchangedIpAimFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_bSendFileUIChanged = TRUE;
	*pResult = 0;
}

void CDlgDataSend::OnChangeEdtPortReceiveFile() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bSendFileUIChanged = TRUE;
}

void CDlgDataSend::OnDoubleclickedRdoDeleteSendedFile() 
{
	// TODO: Add your control notification handler code here
	m_bSendFileUIChanged = TRUE;
}

void CDlgDataSend::OnDoubleclickedRaoSaveSendedFile() 
{
	// TODO: Add your control notification handler code here
	m_bSendFileUIChanged = TRUE;
}

void CDlgDataSend::OnChkCheckFile() 
{
	// TODO: Add your control notification handler code here
	m_bSendFileUIChanged = TRUE;
}

void CDlgDataSend::OnChangeEdtPortLocalEnd() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bEndUIChanged = TRUE;
}

void CDlgDataSend::OnFieldchangedIpLocalEnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_bEndUIChanged = TRUE;
	*pResult = 0;
}

void CDlgDataSend::OnChangeEdtPortReceiveEnd() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bEndUIChanged = TRUE;
}

void CDlgDataSend::OnChkCircuit() 
{
	// TODO: Add your control notification handler code here
	m_bEndUIChanged = TRUE;
}
