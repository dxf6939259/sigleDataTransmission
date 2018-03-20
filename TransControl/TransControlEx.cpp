/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TransControlEx.cpp
* 摘    要：为主程序提供导出接口(主要有:创建SOCKET,发送数据,接收数据,等)
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

     dgh       2013-09-12       Ver 1.0         创建
*/



// TransControlEx.cpp: implementation of the CTransControlEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransControl.h"
#include "TransControlEx.h"
#include "RegistationSocket.h"

//add lgp 10-31 begin
extern CTransControlApp theApp;
//add lgp 10-31 end
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CTransControlEx::m_bCreate(FALSE);
int CTransControlEx::m_iLocalEndPort(0);
CTransControlEx::CTransControlEx()
{
	m_bStop = FALSE;
	m_bCheckSafe = FALSE;
//	m_bCheckFlag = FALSE;
	m_pSocket = NULL;
	m_pProPack = new ProtocolPacket;
	m_pTrePack = new CTreatyPack;
	m_pBufData = NULL;
	m_readPos = MAX_FRAME_BUF_COUNT-1;
	m_writePos = 0;
	m_state = 0;
	m_ckFlag = 0;
	m_sendLocalPort = 0;
}

CTransControlEx::~CTransControlEx()
{
	SafeDelete(m_pTrePack);
	SafeDelete(m_pProPack);
	FreeDeleteArray(m_pBufData);
}

CTransControlEx* CTransControlEx::GetInstance()
{
	static CTransControlEx tc;
	return &tc;
}

/***************************************************************
函 数 名：NewCtransCtrlEx()
功能描述：动态创建新的传输控制类
参    数：无
全局变量：无
返 回 值: 返回CTransControlEx对象指针
****************************************************************/
CTransControlEx* CTransControlEx::NewCtransCtrlEx()
{
	CTransControlEx *p = new CTransControlEx;
	return p;
}
//	文件收发
// #define TYPE_SEND_FILE            0
// #define TYPE_RECEIVE_FILE         1
// 短报文收发
// #define TYPE_SEND_SHORTMES        2
// #define TYPE_RECEIVE_SHORTMES     3
// 流媒体收发
// #define TYPE_SEND_STREAM          4
// #define TYPE_RECEIVE_STREAM       5


/***************************************************************
函 数 名：Init(int _nRecvPort,LPCSTR _lpRecvIp,int _nTransType)
功能描述：外界初始化接收端口号,IP地址,以及传输类型
参    数：int _nRecvPort   : 接收端口号
          LPCSTR _lpRecvIp : 接收IP
		  int _nTransType  : 传输类型
全局变量：无
返 回 值: 无
****************************************************************/
BOOL CTransControlEx::Init
(
	 int _nRecvPort,
	 LPCSTR _lpRecvIp,
	 int _nTransType,
	 TransCallBack _tc,
	 FeedBackInfo _fi,
	 USHORT _sendLocalPort
 )
{
	if (_nTransType>=0 && _nTransType<=5)
	{		
		m_nRecvPort = _nRecvPort;
		CString str = _lpRecvIp;
		IpToByte(str);
		m_nTransType = _nTransType;
		m_pCallBack = _tc;
		m_pFeedBack = _fi;
		m_nLocalport = _nRecvPort;
		m_sendLocalPort = _sendLocalPort;
		//如果传输类型为接收端时,为接收数据端开辟缓冲区
		if (_nTransType==TYPE_RECEIVE_FILE || _nTransType==TYPE_RECEIVE_SHORTMES || _nTransType==TYPE_RECEIVE_STREAM)
		{
			if (NULL == m_pBufData) //为空,申请数据缓冲区
			{	
				if (_nTransType == TYPE_RECEIVE_FILE)
				{
					m_pBufData = new byte[DATA_BUF_SIZE_F];
				}
				else
				m_pBufData = new byte[DATA_BUF_SIZE];
				if (NULL == m_pBufData)
				{
					AfxMessageBox(_T("socket数据缓冲区开启失败!"));
					return FALSE;
				}				
			}
			if (_nTransType == TYPE_RECEIVE_FILE)
				memset(m_pBufData,0x00,DATA_BUF_SIZE_F);
			else
				memset(m_pBufData,0x00,DATA_BUF_SIZE);
		}
	}
	else
	{
		AfxMessageBox(_T("传输类型有错,请传入0-5之间,初始化失败!"));
		return FALSE;
	}
	return TRUE;
}

/***************************************************************
	函 数 名：StopTrans()
	功能描述：停止传输
	参    数：无
	全局变量：无
	返 回 值: 无
****************************************************************/
void CTransControlEx::StopTrans()
{
	m_bStop = TRUE; //让线程退出
	m_Socket.Stop();
	WSACleanup();

	//判断线程退出
	while (1)
	{
		//发送
		if (m_nTransType==TYPE_SEND_FILE || m_nTransType==TYPE_SEND_SHORTMES || m_nTransType==TYPE_SEND_STREAM)
		{
			if (m_state == TRANS_ING)
			{
				Sleep(5);
				continue;
			}
			else
			{
				break;
			}
		}
		//接收
		else if (m_nTransType==TYPE_RECEIVE_FILE || m_nTransType==TYPE_RECEIVE_SHORTMES || m_nTransType==TYPE_RECEIVE_STREAM)
		{
			if (m_curThreadCount == 0)
			{
				break;
			}
			else
			{
				Sleep(5);
				continue;
			}
		}
		else
		{
			AfxMessageBox(_T("传输类型有错,可能初始化函数没有被调用!"));
			break;
		}
	}

}

/***************************************************************
	函 数 名：GetState()
	功能描述：获取当前运行状态，返回当前运行状态
	参    数：无
	全局变量：无
	返 回 值: 无
****************************************************************/
int CTransControlEx::GetState()
{
	return m_state;
}

/***************************************************************
函 数 名：SendData(LPCSTR _filePath)
功能描述：发送数据
参    数：_pathDir :目录或者路径，根据传输类型定
		  _dirStr  :目录结构(形如:"a\b\")
全局变量：无
返 回 值: BOOL ,TRUE:发送线程启动发数;FALSE:发送失败
****************************************************************/
BOOL CTransControlEx::SendData(LPCSTR _filePath,LPCSTR _dirStr)
{
	ASSERT(m_pFeedBack!=NULL && m_pCallBack!=NULL);
	m_bStop = FALSE;
	m_state = TRANS_ING; //正在传输
	CString strPath = _filePath ;
	CString dirPathFile = _dirStr;
 	//接收IP地址
	byte byIp[4]={0};
	byIp[0] = m_arrRecvIp[0];
	byIp[1] = m_arrRecvIp[1];
	byIp[2] = m_arrRecvIp[2];
	byIp[3] = m_arrRecvIp[3];

	if (m_nTransType == TYPE_SEND_FILE)
	{
		//发送文件

		CFile ifile;
		CString strFileName;
		int nCt = 0;
		char chBuf[MAX_FRAME_LEN]={0};

		int serNum = 0;
	
		LONG lEveTimes=0;
		static LONG lCurTimes;
		lCurTimes = 0;
		LONG lCurTmpTs = 0; //临时存储进度条的值，做比较

		DWORD dwFileLen=0;

		//已经发送的总长度
		DWORD dwSendSum = 0;
		
		int seriNum = 0;
 			BOOL bRet = ifile.Open(strPath,CFile::modeRead);
 			if (!bRet)
 			{//文件读取失败
				return FALSE;
 			}
		
			//读取文件进行发送
			dwFileLen = ifile.GetLength();

			seriNum = 0;
			strFileName = ifile.GetFileName();
			//add dgh 2014-2-27,带目录结构
			strFileName = dirPathFile+strFileName;
			//end
			int fileLen = strFileName.GetLength();
			
			m_pTrePack->ResetData();
			m_pProPack->Reset();
			
			//打包接收端口号
			m_pProPack->desPort = m_nRecvPort;
			m_pProPack->desAddr[0] = byIp[0];
			m_pProPack->desAddr[1] = byIp[1];
			m_pProPack->desAddr[2] = byIp[2];
			m_pProPack->desAddr[3] = byIp[3];
			m_pProPack->dataType = TRANS_FILE;
			m_pProPack->checkMark = m_ckFlag;
			m_pProPack->dataLen = fileLen;
			m_pProPack->serNum = seriNum; //包序号
			seriNum++;
			
			memcpy(m_pProPack->dataBuf,strFileName.GetBuffer(fileLen),fileLen); 

			*m_pTrePack<<*m_pProPack;

			//打包文件总长度
			m_pTrePack->PackFileLen(dwFileLen,fileLen);

			//发送文件信息
			m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
            
			while(!m_bStop)
			{
				m_pTrePack->ResetData();
				m_pProPack->Reset();
				
				UINT nRead = ifile.Read((char*)m_pProPack->dataBuf,MAX_FRAME_LEN);
				
				if (nRead <= 0)
				{
					break;
				}
				m_pProPack->dataType = TRANS_FILE;
				
				//打包接收端口号
				m_pProPack->desPort = m_nRecvPort;
				m_pProPack->desAddr[0] = byIp[0];
				m_pProPack->desAddr[1] = byIp[1];
				m_pProPack->desAddr[2] = byIp[2];
				m_pProPack->desAddr[3] = byIp[3];


				m_pProPack->checkMark = m_ckFlag;
				m_pProPack->dataLen = nRead;
				m_pProPack->serNum = seriNum; //包序号
				seriNum++;

				*m_pTrePack<<*m_pProPack;
				
				m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
				//add 10-30 begin 减缓发送速率
				for(int i=0;i<theApp.m_nSendRate;i++)
				for(int i=0;i<theApp.m_nSendRate;i++) //35M/s
						;
			    //add 10-30 end
				dwSendSum += nRead;			
				//计算进度条的值
				lCurTimes = (1.0*dwSendSum/dwFileLen)*100;
				if (lCurTimes>lCurTmpTs)
				{
					//modify lgp 12-18 begin
					//发送进度条的值
					//m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes);

					//CString strFileName = ifile.GetFileTitle();
					//AfxMessageBox(strFileName);

                    m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes, (void*)strFileName.GetBuffer(strFileName.GetLength()));
					//modify lgp 12-18 end
					lCurTmpTs = lCurTimes;
				}
				

		//		Sleep(1);	
				if (nRead<MAX_FRAME_LEN)
				{
					break;
				}
				
			}
			ifile.Close();
		
			m_pTrePack->ResetData();
			m_pProPack->Reset();
			m_pProPack->dataType = TRANS_FILE;
			//打包接收端口号
			m_pProPack->desPort = m_nRecvPort;
			m_pProPack->desAddr[0] = byIp[0];
			m_pProPack->desAddr[1] = byIp[1];
			m_pProPack->desAddr[2] = byIp[2];
			m_pProPack->desAddr[3] = byIp[3];

			m_pProPack->checkMark = m_ckFlag;
			m_pProPack->dataLen = 0;
			m_pProPack->serNum = seriNum; //包序号
			seriNum++;

			*m_pTrePack<<*m_pProPack;

			//发送完成包,数据长度为零
			m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());

			lCurTimes = 100;
			m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,0); //完成进度
			m_pFeedBack(TRANS_COMPLETE,0,0);//传输完成
		}
	m_state = TRANS_END;
	return TRUE;
}

/***************************************************************
函 数 名：SendData(byte* dataFrame,ULONG dataLen)
功能描述：发送数据(用于数据传输与流媒体传输)
参    数：dataFrame :数据头指针
	      dataLen   :数据的长度	
全局变量：无
返 回 值: BOOL ,TRUE:数据发送成功;FALSE:发送失败
****************************************************************/
BOOL CTransControlEx::SendData(byte* dataFrame,ULONG dataLen)
{

	ASSERT(NULL != dataFrame && dataLen!=0);
	m_state = TRANS_ING;
	int nTransType = 0;
	if (TYPE_SEND_SHORTMES == m_nTransType)
	{
		nTransType = 20;
	}
	else if(TYPE_SEND_STREAM == m_nTransType)
	{
		nTransType = 30;
	}
	else
	{
		AfxMessageBox(_T("传输类型不正确!"));
		return FALSE;
	}
		

	//数据有效性检查
	if (m_nTransType == TYPE_SEND_SHORTMES)
	{
		//delete lgp 12-16 begin
		/*if (dataLen>MAX_FRAME_LEN)
		{
			AfxMessageBox(_T("短报文数据过长!"));
			return FALSE;
		}*/
		//delete lgp 12-16 end
	}
		
		//接收IP地址
		byte byIp[4]={0};
		byIp[0] = m_arrRecvIp[0];
		byIp[1] = m_arrRecvIp[1];
		byIp[2] = m_arrRecvIp[2];
		byIp[3] = m_arrRecvIp[3];
		int seriNum = 0;
		
		int nPack = 0;

		m_pTrePack->ResetData();
		m_pProPack->Reset();

		m_pProPack->checkMark = m_ckFlag;
		m_pProPack->dataType = nTransType;
		
		//打包接收端口号
		m_pProPack->desPort = m_nRecvPort;
		m_pProPack->desAddr[0] = byIp[0];
		m_pProPack->desAddr[1] = byIp[1];
		m_pProPack->desAddr[2] = byIp[2];
		m_pProPack->desAddr[3] = byIp[3];


		//进行分包发送
		if (dataLen >MAX_FRAME_LEN)
		{
			// modify lgp 12-16 begin
			/*
			//计算分包数
			nPack = (dataLen+MAX_FRAME_LEN-1)/MAX_FRAME_LEN;
			for (int i=0; i<nPack; i++)
			{		
				if (m_bStop) //停止发送
				{
					break;
				}
				m_pProPack->serNum = seriNum; //包序号
				seriNum++;
				if (i == nPack-1)
				{
					m_pProPack->dataLen = dataLen-(i*MAX_FRAME_LEN);
				}
				else
				{
					m_pProPack->dataLen = MAX_FRAME_LEN;
				}
				
				
				memcpy(m_pProPack->dataBuf,dataFrame,dataLen);						
				*m_pTrePack<<*m_pProPack;  //打包
				m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
				TRACE("数据已经发送！\n");
			}
			*/
			//计算分包数
			int dataLenTemp = dataLen;
			int dataBegin = 0;
			for(int k=0; (dataLenTemp-k*MAX_FRAME_LEN)>0 ;k++)
			{
				if((dataLenTemp-k*MAX_FRAME_LEN)>=MAX_FRAME_LEN)
				{
				    dataLen=MAX_FRAME_LEN;
					dataBegin = k*MAX_FRAME_LEN;
				}
				else if( (dataLenTemp-k*MAX_FRAME_LEN)>0 && (dataLenTemp-k*MAX_FRAME_LEN)<MAX_FRAME_LEN)
				{
					dataLen = dataLenTemp-k*MAX_FRAME_LEN;
					dataBegin = k*MAX_FRAME_LEN;
					
				}
				

				nPack = (dataLen+2*MAX_FRAME_LEN-1)/MAX_FRAME_LEN;
				if(dataLen == MAX_FRAME_LEN)
				{
				    //delete 14-01-16 lgp from yn begin
					//for (int i=0; i<nPack; i++)
					//{		
					//delete 14-01-16 lgp from yn end
						if (m_bStop) //停止发送
						{
							break;
						}
						m_pProPack->serNum = seriNum; //包序号
						seriNum++;
						//delete 14-01-16 lgp from yn begin 
						/*if (i == nPack-1)
						{
							m_pProPack->dataLen = dataLen-(i*MAX_FRAME_LEN);
						}
						else
						{
							m_pProPack->dataLen = MAX_FRAME_LEN;
						}*/
						//delete 14-01-16 lgp from yn end
						
						memcpy(m_pProPack->dataBuf,dataFrame+dataBegin,dataLen);
                        //add 14-01-16 lgp from yn begin
						m_pProPack->dataLen = dataLen;
				        m_pProPack->dataType = 31;
						//add 14-01-16 lgp from yn end
						m_pTrePack->ResetData();
						*m_pTrePack<<*m_pProPack;  //打包
						m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
						TRACE("数据已经发送！\n");
					//delete 14-01-16 lgp from yn begin	
					//}
					//delete 14-01-16 lgp from yn end
				}
				else
				{
					m_pProPack->serNum = seriNum; //包序号
					seriNum++;
					m_pProPack->dataLen = dataLen;
                    //add 14-01-16 lgp from yn begin
					m_pProPack->dataType = 32;
					//add 14-01-16 lgp from yn end
					//modify 14-01-16 from yn begin
					//memcpy(m_pProPack->dataBuf,dataFrame,dataLen);
					memcpy(m_pProPack->dataBuf,dataFrame+dataBegin,dataLen);
					//modify 14-01-16 from yn end
					m_pTrePack->ResetData();
					*m_pTrePack<<*m_pProPack;  //打包
					m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
				}
				
			}
			//modify lgp 12-16 end
		}
		else
		{
			m_pProPack->serNum = seriNum; //包序号
			seriNum++;
			m_pProPack->dataLen = dataLen;
            //modify 14-01-16 lgp from yn begin
			m_pProPack->dataType = 32;
			//modify 14-01-16 lgp from yn end
			memcpy(m_pProPack->dataBuf,dataFrame,dataLen);						
			*m_pTrePack<<*m_pProPack;  //打包
			m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
		//	AfxMessageBox("数据已经发送！\n");
		}
		m_state = TRANS_ERROR;
		return TRUE;
}

/***************************************************************
函 数 名：RecvDataThread(LPVOID lpvoid)
功能描述：数据接收线程
参    数：LPVOID lpvoid:线程指针(传入传输控制)
全局变量：无
返 回 值: 0正常退出
****************************************************************/
UINT RecvFileThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	byte byBuf[MAX_FRAME_LEN+20]={0};
	int nRecv = 0;
	while (!pThis->m_bStop)
	{
		nRecv = pThis->m_Socket.RecvData(byBuf,MAX_FRAME_LEN+20);
		if (SOCKET_ERROR == nRecv) //接收错误,socket关闭
		{
			pThis->m_pFeedBack(TRANS_ERROR,0,0); 
			break;
		}
		if (0 == nRecv) //接到0个数据不处理,继续接数
		{
			continue;
		}
		while (!pThis->m_bStop)
		{
			//add lgp 12-02 begin
// 			if(TRANS_ING == pThis->m_state && (1 == pThis->m_nTransType))
// 			{
// 				if(!pThis->DirExist(pThis->m_strPath) )
// 					break;
// 			}
		    //add lgp 12-02 end
			//写指针不等于读指针进行写入消息数据
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//写入缓冲区
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),byBuf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				pThis->m_pFeedBack(DATABUF_IS_FULL,0,0); //推送缓冲区已满
				continue;
			}
			break;
		}
		//偏移量到达最后时,使其回归为0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT_F-1)?0:pThis->m_writePos+1;
	}
	InterlockedDecrement(&pThis->m_curThreadCount);

	//当接收处理线程数为0时,设置状态标志为空闲
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn begin
UINT RecvMediaThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	byte byBuf[MAX_FRAME_LEN+20]={0};
	int nRecv = 0;
	while (!pThis->m_bStop)
	{
		nRecv = pThis->m_Socket.RecvData(byBuf,MAX_FRAME_LEN+20);
		if (SOCKET_ERROR == nRecv) //接收错误,socket关闭
		{
			pThis->m_pFeedBack(TRANS_ERROR,0,0); 
			break;
		}
		if (0 == nRecv) //接到0个数据不处理,继续接数
		{
			continue;
		}
		while (!pThis->m_bStop)
		{
			//add lgp 12-02 end
			//写指针不等于读指针进行写入消息数据
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//写入缓冲区
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),byBuf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				pThis->m_pFeedBack(DATABUF_IS_FULL,0,0); //推送缓冲区已满
				continue;
			}
			break;
		}
		//偏移量到达最后时,使其回归为0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_writePos+1;
	}
	InterlockedDecrement(&pThis->m_curThreadCount);
	
	//当接收处理线程数为0时,设置状态标志为空闲
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
//add 14-01-16 lgp from yn begin
UINT RecvDataThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	byte byBuf[MAX_FRAME_LEN+20]={0};
	int nRecv = 0;
	while (!pThis->m_bStop)
	{
		nRecv = pThis->m_Socket.RecvData(byBuf,MAX_FRAME_LEN+20);
		if (SOCKET_ERROR == nRecv) //接收错误,socket关闭
		{
			pThis->m_pFeedBack(TRANS_ERROR,0,0); 
			break;
		}
		if (0 == nRecv) //接到0个数据不处理,继续接数
		{
			continue;
		}
		while (!pThis->m_bStop)
		{
			//add lgp 12-02 end
			//写指针不等于读指针进行写入消息数据
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//写入缓冲区
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),byBuf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				pThis->m_pFeedBack(DATABUF_IS_FULL,0,0); //推送缓冲区已满
				continue;
			}
			break;
		}
		//偏移量到达最后时,使其回归为0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_writePos+1;
	}
	InterlockedDecrement(&pThis->m_curThreadCount);
	
	//当接收处理线程数为0时,设置状态标志为空闲
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
/***************************************************************
函 数 名：ProcessDataThread(LPVOID lpvoid)
功能描述：数据处理线程
参    数：LPVOID lpvoid:线程指针(传入传输控制)
全局变量：无
返 回 值: 0正常退出
****************************************************************/
UINT ProcessFileThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	CString csPath = pThis->m_strPath; //传输路径
	pThis->m_state = TRANS_ING;  //正在接收数据

	long tempCount = 0;

	BOOL bCallBack = FALSE; //是否回调给上层

	//判断传输类型(数据传输或者流媒体传输时回调给上层)
	if (pThis->m_nTransType == TYPE_RECEIVE_SHORTMES || pThis->m_nTransType == TYPE_RECEIVE_STREAM )
	{
		//回调给上层
		bCallBack = TRUE;		
	}

	RecvWriteFile* pRwFile = NULL;
	map<int,RecvWriteFile*>::iterator itRwf;
	map<int,RecvWriteFile*> mapRecvWtFile;
	mapRecvWtFile.clear();

	CString fileName; //文件的名称
	int serNum = 0;  
	int curSerNum = 0;  //当前序号
	CString strFileName;  //文件全路径
	CFileStatus status;
	byte byData = 0;  //数据传输类型
	byte arrFileName[90]={0};
	DWORD dwFileLen = 0;
	CTime curTime; //时间
	CString strTime;   //接到数据的时间
	LONG64 curRecv = 0;  //当前接收到的数据总数和
	BOOL bOpenRet = FALSE; //打开文件是否成功
	LONG lCurTimes = 0; //当前进度次数
	int ckFlag = 0;
	int iTimeOut = 0;
	USHORT dataLen = 0;//数据区长度(当长度为零时,文件传输结束)
	int aTest = 0;

	CString strTimeFileRecv;
	//add lgp 12-02 begin
	BOOL bRecvPathExist = TRUE;
	//add lgp 12-02 end
	while (!pThis->m_bStop)
	{
			tempCount = (pThis->m_readPos == MAX_FRAME_BUF_COUNT_F-1)?0:pThis->m_readPos+1;
			pThis->m_pTrePack->ResetData();
			while (!pThis->m_bStop) //循环缓冲区
			{

				if (tempCount != pThis->m_writePos)
				{
					pThis->m_readPos = tempCount;
					memcpy(pThis->m_pTrePack->GetBuffer(),pThis->m_pBufData+tempCount*(MAX_FRAME_LEN+20),MAX_FRAME_LEN+20);
					iTimeOut = 0;
				}
				else
				{
					Sleep(25);
					iTimeOut+=25;
					if (iTimeOut >= 3000) //延迟三秒关闭IO
					{
						iTimeOut = 0;
						for (map<int,RecvWriteFile*>::iterator it=mapRecvWtFile.begin(); it!=mapRecvWtFile.end(); it++)
						{
							if (NULL!=it->second->pOfile)
							{
								
								CString strNewName = csPath + "\\" +  it->second->strReFileName;
								CString strOldName = strNewName + _T(".temp");
								
								//文件名替换
								int nRet = rename(strOldName.GetBuffer(strOldName.GetLength()), strNewName.GetBuffer(strNewName.GetLength()));

								it->second->pOfile->Close();
								//接收完成,推送完成文件名
								pThis->m_pFeedBack(TRANS_COMPLETE,(void*)it->second->strReFileName.GetBuffer(it->second->strReFileName.GetLength()),0);
							}
							SafeDelete(it->second);
						}
						mapRecvWtFile.clear();
					}
					continue;
				}
				break;
			}
			//如果是停止，退出最后一层循环
			if (pThis->m_bStop) 
			{
				break;
			}
			pThis->m_pProPack->Reset();
			*(pThis->m_pTrePack)>>pThis->m_pProPack;
			
			//判断帧头信息,检查校验码
			if (pThis->m_pProPack->frameHead != 0x16ee || pThis->m_pProPack->checkCode != pThis->m_pTrePack->GetCheckCode())
			{
				pThis->m_pFeedBack(FRAME_ERROR,0,0);
				continue;
			}
			byData = pThis->m_pProPack->dataType; 
			dataLen = pThis->m_pProPack->dataLen;

			//接到的帧，数据类型不匹配
			if (((byData==1)&&(TYPE_RECEIVE_FILE != pThis->m_nTransType))
				||(byData==20&&TYPE_RECEIVE_SHORTMES != pThis->m_nTransType)
				||(byData==30&&TYPE_RECEIVE_STREAM != pThis->m_nTransType))
			{
				pThis->m_pFeedBack(TRANSTYPE_ERROR,0,0);
				continue;
			}
			

			////////////////////////////////////////////////
			if (bCallBack) //回调给上层
			{
				pThis->m_pCallBack(pThis->m_nTransType,0,pThis->m_pProPack->dataLen+20,pThis->m_pTrePack->GetBuffer());
				TRACE("回调给上层！%d\n",aTest++);
				continue;
			}
			
			
			//双向校验码
			ckFlag = pThis->m_pProPack->checkMark;
			
			//当前解析帧序号
			curSerNum = pThis->m_pProPack->serNum;
			int i  = 0;
			
			itRwf = mapRecvWtFile.find(ckFlag);
			if (itRwf != mapRecvWtFile.end())
			{
				//判断是否丢帧
				if (itRwf->second->curSerNum < curSerNum)
				{
					//补写零
					if (itRwf->second->pOfile->m_hFile!=-1)
					{
						byte ltBuf[MAX_FRAME_LEN]={0};
						for (int i=0; i<curSerNum-itRwf->second->curSerNum; i++)
						{
							itRwf->second->pOfile->Write(ltBuf,MAX_FRAME_LEN);
							itRwf->second->curRecvLen += pThis->m_pProPack->dataLen;
						}
						
						//计算进度条
						lCurTimes = (1.0*itRwf->second->curRecvLen/itRwf->second->fileLen)*100;
						if (lCurTimes >itRwf->second->lCurTimes)
						{
							pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,0);
							itRwf->second->lCurTimes = lCurTimes;
						}
						
					}
					else
					{//无法写入文件,文件句柄错误
						pThis->m_pFeedBack(WRITE_FILE_ERROR,0,0);
					}
					
					//总共丢帧个数
					itRwf->second->lostPacketCt += (curSerNum-itRwf->second->curSerNum);
					pThis->m_pFeedBack(TRANS_LOSS_PACKET,0,0); //出现丢帧
				}
				//设置此文件下次应接收的帧序号
				itRwf->second->curSerNum = curSerNum+1;  
			}
		
		
			//正常数据
			if (curSerNum !=0 && dataLen !=0)
			{
				if (itRwf != mapRecvWtFile.end())
				{
					if (itRwf->second->pOfile->m_hFile!=-1)
					{
						itRwf->second->pOfile->Write(pThis->m_pProPack->dataBuf,pThis->m_pProPack->dataLen);
						itRwf->second->curRecvLen += pThis->m_pProPack->dataLen;
						
						//计算进度条
						lCurTimes = (1.0*itRwf->second->curRecvLen/itRwf->second->fileLen)*100;
						if (lCurTimes>itRwf->second->lCurTimes)
						{					
							pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
							itRwf->second->lCurTimes = lCurTimes;
						}			
					}
				}
				else
				{
					//推送消息,此标识帧文件末被创建,可能丢失信息头
					pThis->m_pFeedBack(FILE_NOCREATE,0,0);
				}
			}
			
			//文件名
			else if (curSerNum == 0 && dataLen != 0)
			{
				//add lgp 12-02 begin
				if((TRANS_ING == pThis->m_state)&& (1 == pThis->m_nTransType))
				{	//存储路径不存在,退出接收!
					if(!pThis->DirExist(csPath))
					{
						
						// 						curTime = CTime::GetCurrentTime();
						// 						strTime.Format(_T("%d%02d%02d%02d%02d%02d"),curTime.GetYear(),curTime.GetMonth(),\
						// 							curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());			
						// 						strTime = _T("_")+strTime;
						// 						fileName = arrFileName;
						// 						fileName.Insert(fileName.ReverseFind(_T('.')),strTime);
						// 						strFileName = csPath + "\\" + fileName;
						// 						pThis->m_pFeedBack(RECV_FILE_NAME,(void*)strFileName.GetBuffer(strFileName.GetLength()),0);
						pThis->m_pFeedBack(RECV_PATH_NOT_EXIST,NULL,0);
						break;
					}
				}
				//add lgp 12-02 end
				
				for (i=0; i<6; i++) //文件总长度(文件长度6个字节)
				{
					dwFileLen<<=8;
					dwFileLen |= *(pThis->m_pProPack->dataBuf+95-i);
				}
				if (dwFileLen==0)
				{
					//文件总长度有误
					continue;
				}
				
				memset(arrFileName,0x00,90);
				memcpy(arrFileName,pThis->m_pProPack->dataBuf,90);
				
				//推送接收文件名
				//		pThis->m_pFeedBack(RECV_FILE_NAME,(void*)arrFileName);
				serNum = 0;
				
				curTime = CTime::GetCurrentTime();
				strTime.Format(_T("%d%02d%02d%02d%02d%02d"),curTime.GetYear(),curTime.GetMonth(),\
					curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());			
				strTime = _T("_")+strTime;
				fileName = arrFileName;
				int index = 0;
				int fNameLen = fileName.GetLength();
				index = fileName.Find(_T(":"));
				if (index != -1)
				{
					fileName = fileName.Right(fNameLen-index-2);
				}
				//创建目录结构
				if (!pThis->CreateDirStruct(csPath,fileName))
				{
					//目录结构创建失败
					pThis->m_pFeedBack(CREATE_DIR_ERROR,NULL,0);
					break;
				}
				fileName.Insert(fileName.ReverseFind(_T('.')),strTime);
				
				strTimeFileRecv = fileName;
				strFileName = csPath + "\\" + fileName;
				
				//10-30 add lgp begin
				pThis->m_pFeedBack(RECV_FILE_NAME,(void*)strFileName.GetBuffer(strFileName.GetLength()),0);
				//10-30 add lgp end
				
				pRwFile = new RecvWriteFile;
				pRwFile->ckFlag = ckFlag;
				pRwFile->fileLen = dwFileLen;
				pRwFile->strReFileName = fileName;
				memcpy(pRwFile->fileName,arrFileName,90);
				
				if (itRwf != mapRecvWtFile.end())
				{
					itRwf->second->pOfile->Close();
					SafeDelete(itRwf->second);
					mapRecvWtFile.erase(itRwf);
				}	
				
				mapRecvWtFile[ckFlag] = pRwFile;
				strFileName += _T(".temp");
				bOpenRet = mapRecvWtFile[ckFlag]->pOfile->Open(strFileName,CFile::modeWrite|CFile::modeCreate|CFile::shareDenyWrite);
				if(!bOpenRet) //文件打开失败!
				{
					continue;
				}
				curRecv = 0;
			}

		 //所有数据接收完成	
			else if (curSerNum!=0 && dataLen==0)
			{
				
				if (itRwf != mapRecvWtFile.end())
				{			
					//		if (itRwf->second->pOfile->GetStatus(strFileName,status) && itRwf->second->pOfile->m_hFile!=-1)
					//		{
					itRwf->second->pOfile->Close();

					CString strNewName = csPath + "\\" +  itRwf->second->strReFileName;
					CString strOldName = strNewName + _T(".temp");

					//文件名替换
					int nRet = rename(strOldName.GetBuffer(strOldName.GetLength()), strNewName.GetBuffer(strNewName.GetLength()));
					
					strTimeFileRecv = itRwf->second->strReFileName;					
					//		}
					//接收完成,推送完成文件名
					pThis->m_pFeedBack(TRANS_COMPLETE,(void*)itRwf->second->strReFileName.GetBuffer(itRwf->second->strReFileName.GetLength()),0);
					
					lCurTimes = 100;
					pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
					SafeDelete(itRwf->second);
					mapRecvWtFile.erase(itRwf);	
				}		
	
			}			
			else
			{
				//推送消息,帧格式错误
				pThis->m_pFeedBack(FRAME_ERROR,0,0); 
			}			
		//add lgp 12-02 begin		
//		bRecvPathExist = TRUE;

		//add lgp 12-13 begin
		//如果是停止，退出最后一层循环
// 		if (pThis->m_bStop) 
// 		{
// 			break;
// 		}
		//add lgp 12-13 end 
// 		if(TRANS_ING == pThis->m_state && bRecvPathExist)
// 		{
// 			
// 			bRecvPathExist = FALSE;
// 		}
//		if(TRANS_ING == pThis->m_state && !bRecvPathExist)
			//modify lgp 12-13 begin
			//Sleep(100);
//			break;
		    //modify lgp 12-13 end
        //add lgp 12-02 end	
	}//while (!pThis->m_bStop)
	
	//释放map内容
	for (map<int,RecvWriteFile*>::iterator it=mapRecvWtFile.begin(); it!=mapRecvWtFile.end(); it++)
	{
		if (NULL!=it->second->pOfile)
		{
			it->second->pOfile->Close();
		}
		SafeDelete(it->second);
	}
	mapRecvWtFile.clear();

	//回调停止接收
	pThis->m_pFeedBack(STOP_RECV,0,0);
	InterlockedDecrement(&pThis->m_curThreadCount);

	//当接收处理线程数为0时,设置状态标志为空闲
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn begin
/***************************************************************
函 数 名：ProcessMediaThread(LPVOID lpvoid)
功能描述：数据处理线程
参    数：LPVOID lpvoid:线程指针(传入传输控制)
全局变量：无
返 回 值: 0正常退出
****************************************************************/

UINT ProcessMediaThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	//CString csPath = pThis->m_strPath; //传输路径
	pThis->m_state = TRANS_ING;  //正在接收数据

	long tempCount = 0;
	RecvWriteFile* pRwFile = NULL;
 

	CString fileName; //文件的名称
	int serNum = 0;  
	int curSerNum = 0;  //当前序号
	CString strFileName;  //文件全路径
//	CFileStatus status;
	byte byData = 0;
	byte arrFileName[90]={0};
	DWORD dwFileLen = 0;
	CTime curTime; //时间
	CString strTime;   //接到数据的时间
	LONG64 curRecv = 0;  //当前接收到的数据总数和
//	BOOL bOpenRet = FALSE; //打开文件是否成功
	LONG lCurTimes = 0; //当前进度次数
	int ckFlag = 0;
	int iTimeOut = 0;
	int aTest = 0;
	CString strTimeFileRecv;
	
	while (!pThis->m_bStop)
	{
		//add lgp 12-02 begin
		while((TYPE_RECEIVE_STREAM == pThis->m_nTransType&&!pThis->m_bStop))
		{
			tempCount = (pThis->m_readPos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_readPos+1;
			pThis->m_pTrePack->ResetData();
			while (!pThis->m_bStop) //循环缓冲区
			{
				
				if (tempCount != pThis->m_writePos)
				{
					pThis->m_readPos = tempCount;
					memcpy(pThis->m_pTrePack->GetBuffer(),pThis->m_pBufData+tempCount*(MAX_FRAME_LEN+20),MAX_FRAME_LEN+20);
					iTimeOut = 0;
				}
				else
				{
					Sleep(25);
					iTimeOut+=25;
					if (iTimeOut >= 3000) //延迟三秒关闭IO
					{
						iTimeOut = 0;
					}
					continue;
				}
				break;
			}
			//如果是停止，退出最后一层循环
			if (pThis->m_bStop) 
			{
				break;
			}
			pThis->m_pProPack->Reset();
			*(pThis->m_pTrePack)>>pThis->m_pProPack;
			
			//判断帧头信息,检查校验码
			if (pThis->m_pProPack->frameHead != 0x16ee || pThis->m_pProPack->checkCode != pThis->m_pTrePack->GetCheckCode())
			{
				pThis->m_pFeedBack(FRAME_ERROR,0,0);
				continue;
			}
			byData = pThis->m_pProPack->dataType; 
			
			//接到的帧，数据类型不匹配
			if (TYPE_RECEIVE_STREAM != pThis->m_nTransType)
			{
				pThis->m_pFeedBack(TRANSTYPE_ERROR,0,0);
				continue;
			}			
			//双向校验码
			ckFlag = pThis->m_pProPack->checkMark;
			
			//当前解析帧序号
			curSerNum = pThis->m_pProPack->serNum;
			int i  = 0;
			switch (byData)
			{
			case 31: //正常数据
				//add lgp 12-02 begin
				pThis->m_pFeedBack(31,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes =100;
				break;
			case 32: //所有数据接收完成		
				pThis->m_pFeedBack(32,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes = 100;
// 			case 30:
// 				pThis->m_pFeedBack(30,pThis->m_pProPack->dataBuf,(void*)&pThiFs->m_pProPack->dataLen);
// 				lCurTimes = 100;				
			//	pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
				break;
			default:
				//推送消息,帧格式错误
				pThis->m_pFeedBack(FRAME_ERROR,0,0); 
				break;
			}
		}
	 
	}
	//回调停止接收
	pThis->m_pFeedBack(STOP_RECV,0,0);
	InterlockedDecrement(&pThis->m_curThreadCount);

	//当接收处理线程数为0时,设置状态标志为空闲
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
//add 14-01-16 lgp from yn begin
/***************************************************************
函 数 名：ProcessMediaThread(LPVOID lpvoid)
功能描述：数据处理线程
参    数：LPVOID lpvoid:线程指针(传入传输控制)
全局变量：无
返 回 值: 0正常退出
****************************************************************/

UINT ProcessDataThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	//CString csPath = pThis->m_strPath; //传输路径
	pThis->m_state = TRANS_ING;  //正在接收数据

	long tempCount = 0;
	RecvWriteFile* pRwFile = NULL;
 

	CString fileName; //文件的名称
	int serNum = 0;  
	int curSerNum = 0;  //当前序号
	CString strFileName;  //文件全路径
//	CFileStatus status;
	byte byData = 0;
	byte arrFileName[90]={0};
	DWORD dwFileLen = 0;
	CTime curTime; //时间
	CString strTime;   //接到数据的时间
	LONG64 curRecv = 0;  //当前接收到的数据总数和
//	BOOL bOpenRet = FALSE; //打开文件是否成功
	LONG lCurTimes = 0; //当前进度次数
	int ckFlag = 0;
	int iTimeOut = 0;
	int aTest = 0;
	CString strTimeFileRecv;
	
	while (!pThis->m_bStop)
	{
		//add lgp 12-02 begin
		while((TYPE_RECEIVE_SHORTMES == pThis->m_nTransType&&!pThis->m_bStop))
		{
			tempCount = (pThis->m_readPos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_readPos+1;
			pThis->m_pTrePack->ResetData();
			while (!pThis->m_bStop) //循环缓冲区
			{
				
				if (tempCount != pThis->m_writePos)
				{
					pThis->m_readPos = tempCount;
					memcpy(pThis->m_pTrePack->GetBuffer(),pThis->m_pBufData+tempCount*(MAX_FRAME_LEN+20),MAX_FRAME_LEN+20);
					iTimeOut = 0;
				}
				else
				{
					Sleep(25);
					iTimeOut+=25;
					if (iTimeOut >= 3000) //延迟三秒关闭IO
					{
						iTimeOut = 0;
					}
					continue;
				}
				break;
			}
			//如果是停止，退出最后一层循环
			if (pThis->m_bStop) 
			{
				break;
			}
			pThis->m_pProPack->Reset();
			*(pThis->m_pTrePack)>>pThis->m_pProPack;
			
			//判断帧头信息,检查校验码
			if (pThis->m_pProPack->frameHead != 0x16ee || pThis->m_pProPack->checkCode != pThis->m_pTrePack->GetCheckCode())
			{
				pThis->m_pFeedBack(FRAME_ERROR,0,0);
				continue;
			}
			byData = pThis->m_pProPack->dataType; 
			
			//接到的帧，数据类型不匹配
			if (TYPE_RECEIVE_SHORTMES != pThis->m_nTransType)
			{
				pThis->m_pFeedBack(TRANSTYPE_ERROR,0,0);
				continue;
			}			
			//双向校验码
			ckFlag = pThis->m_pProPack->checkMark;
			
			//当前解析帧序号
			curSerNum = pThis->m_pProPack->serNum;
			int i  = 0;
			switch (byData)
			{
			case 31: //正常数据
				//add lgp 12-02 begin
				pThis->m_pFeedBack(31,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes =100;
				break;
			case 32: //所有数据接收完成		
				pThis->m_pFeedBack(32,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes = 100;
// 			case 30:
// 				pThis->m_pFeedBack(30,pThis->m_pProPack->dataBuf,(void*)&pThiFs->m_pProPack->dataLen);
// 				lCurTimes = 100;				
			//	pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
				break;
			default:
				//推送消息,帧格式错误
				pThis->m_pFeedBack(FRAME_ERROR,0,0); 
				break;
			}
		}
	 
	}
	//回调停止接收
	pThis->m_pFeedBack(STOP_RECV,0,0);
	InterlockedDecrement(&pThis->m_curThreadCount);

	//当接收处理线程数为0时,设置状态标志为空闲
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
/***************************************************************
函数名：RecvData(LPCSTR _path)
功能描述：接收数据
参数：LPCSTR _path: 接收路径
全局变量：无
返回值: BOOL ,TRUE:接收线程启动成功,正在接收数据; FALSE:发送失败
****************************************************************/
BOOL CTransControlEx::RecvFile(LPCSTR _path)
{
	
	ASSERT(m_pFeedBack!=NULL&&m_pCallBack!=NULL);
	m_strPath = _path;
	m_state   = TRANS_ING;
	m_bStop   = FALSE;
	
	m_readPos      = MAX_FRAME_BUF_COUNT-1; 
	m_writePos     = 0;

	m_curThreadCount = 0; 
	if (1 == m_nTransType)
	{
		if (!DirExist(m_strPath))
		{
			AfxMessageBox(_T("文件或目录不存在!"));
			return FALSE;
		}
	}
 
	//启动数据接收线程
	AfxBeginThread(RecvFileThread,(LPVOID)this);

	//启动数据处理线程
	AfxBeginThread(ProcessFileThread,(LPVOID)this);

	m_curThreadCount = 2;  //当前接收线程
	return TRUE;
}
//add 14-01-16 lgp from yn begin
BOOL CTransControlEx::RecvData(){
	ASSERT(m_pFeedBack!=NULL&&m_pCallBack!=NULL);
	m_state   = TRANS_ING;
	m_bStop   = FALSE;
	
	m_readPos      = MAX_FRAME_BUF_COUNT-1; 
	m_writePos     = 0;
	
	m_curThreadCount = 0; 
	if (1 == m_nTransType)
	{
		if (!DirExist(m_strPath))
		{
			AfxMessageBox(_T("文件或目录不存在!"));
			return FALSE;
		}
	}
	
	//启动数据接收线程
	AfxBeginThread(RecvDataThread,(LPVOID)this);
	
	//启动数据处理线程
	AfxBeginThread(ProcessDataThread,(LPVOID)this);
	
	m_curThreadCount = 2;  //当前接收线程
	
	return TRUE;
}
//add 14-01-16 lgp from yn end
//add 14-01-16 lgp from yn begin
BOOL CTransControlEx::RecvMedia(){
	ASSERT(m_pFeedBack!=NULL&&m_pCallBack!=NULL);
	m_state   = TRANS_ING;
	m_bStop   = FALSE;
	
	m_readPos      = MAX_FRAME_BUF_COUNT-1; 
	m_writePos     = 0;
	
	m_curThreadCount = 0; 
	if (1 == m_nTransType)
	{
		if (!DirExist(m_strPath))
		{
			AfxMessageBox(_T("文件或目录不存在!"));
			return FALSE;
		}
	}
	
	//启动数据接收线程
	AfxBeginThread(RecvMediaThread,(LPVOID)this);
	
	//启动数据处理线程
	AfxBeginThread(ProcessMediaThread,(LPVOID)this);
	
	m_curThreadCount = 2;  //当前接收线程
	
	return TRUE;
}
//add 14-01-16 lgp from yn end
/***************************************************************
函 数 名：CreateSocket(LPCSTR _lpIP)
功能描述：创建SOCKET套接字
参    数：_lpIP: IP地址
全局变量：无
返 回 值: BOOL ,TRUE:socket创建成功; FALSE:socket创建失败
****************************************************************/
BOOL CTransControlEx::CreateSocket(LPCSTR _lpIP,int _desPort,int _localEndPort)
{
	CString str = _lpIP;
	m_lpIp = _lpIP;
	BOOL bRet = FALSE;
	
	WSADATA wsd;
	int ret = WSAStartup(MAKEWORD(2,2),&wsd);
	if (0!=ret)
	{
		AfxMessageBox(_T("WSAStartup失败"));
		return FALSE;
	}

	//当是发送端时,创建双向校验socket
	if (m_nTransType==TYPE_SEND_FILE || m_nTransType==TYPE_SEND_SHORTMES || m_nTransType==TYPE_SEND_STREAM)
	{
		CString strLocalIp = m_Socket.m_csLocalIP;
		m_iLocalEndPort = _localEndPort;
		IpToByte(strLocalIp,1);

		m_pSocket = CRegistationSocket::GetInstance();
		if (!m_bCreate && NULL != m_pSocket)
		{
			m_bCreate = m_pSocket->Create(m_iLocalEndPort,SOCK_DGRAM);
		}
	}
	return m_Socket.CreateUDPSocket(m_sendLocalPort,m_nLocalport,m_nTransType,str,_desPort);
}

/***************************************************************
函 数 名：DirExist(CString _dir)
功能描述：目录是否存在
参    数：CString _dir: 目录
全局变量：无
返 回 值: bool TRUE : 目录存在; 
               FALSE: 目录不存在;
****************************************************************/
bool CTransControlEx::DirExist(CString _dir)
{
	DWORD   dwAttr   =   GetFileAttributes(_dir); 
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)//目录不存在 
	{
		return false;
	}
	return true;
}

/***************************************************************
函 数 名：CreateDirStruct(CString _dirStr)
功能描述：创建目录结构
参    数：CString _rootDir:根目录
		  CString _dirStr: 目录结构
全局变量：无
返 回 值: BOOL TRUE : 目录创建成功; 
               FALSE: 目录创建失败;
****************************************************************/
BOOL CTransControlEx::CreateDirStruct(CString _rootDir,CString _dirStr)
{
	if (!DirExist(_rootDir))
	{
		return FALSE;
	}
	BOOL bRet = TRUE;

	CString strRoot = _rootDir;
	CString strChildDir =_dirStr;
	CString strFullPath = strRoot;
	while(strChildDir.Find(_T("\\"))!=-1)
	{
		int nLen = strChildDir.Find(_T("\\"));
		CString strChild = strChildDir.Left(nLen);
        strFullPath += _T("\\") ;
		strFullPath += strChild;
		if (!DirExist(strFullPath))
		{
		   bRet = CreateDirectory(strFullPath,NULL);
		   if(!bRet)
			return FALSE;
		}
		strChildDir = strChildDir.Right(strChildDir.GetLength()-nLen-1);
		
	}
	/*CString strDir,str1,str2;
	strDir = _rootDir;
	str2 = _dirStr;
	int index = 0;
	int fLen = str2.GetLength();
	BOOL bRet = TRUE;
	while (index = str2.Find(_T("\\")) != -1)
	{
		str1 = str2.Left(index);
		str2 = str2.Right(fLen-index-1);
		fLen = str2.GetLength();
		strDir = strDir+_T("\\")+str1;
		if (!DirExist(strDir))
		{
			bRet = CreateDirectory(strDir,NULL);
		}
	}*/
	return bRet;	
}

/***************************************************************
函 数 名：FileExist(CString strFileName)
功能描述：判断文件是否存在
参    数：CString strFileName :文件名
全局变量：无
返 回 值: BOOL TRUE : 文件存在; 
               FALSE: 文件不存在;
****************************************************************/
BOOL CTransControlEx::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName);
}

/***************************************************************
	函 数 名：IpToByte(CString _ip)
	功能描述：IP转换
	参    数：CString _ip字符串IP
			: int fl
	全局变量：无
	返 回 值: 无
****************************************************************/
void CTransControlEx::IpToByte(CString _ip,int fl)
{
	CString str,str1;
	str = _ip;
	int index = 0;
	int ipLen = 0;
	ipLen = str.GetLength();

	int i=0;

	while (1)
	{
		index = str.Find(".");
		if (index==-1)
		{
			break;
		}
		str1 = str.Left(index);
		if (fl == 0)
		{
			m_arrRecvIp[i++] = atoi(str1);
		}
		else
		{
			m_arrLocalIp[i++] = atoi(str1);
		}
		str = str.Right(ipLen-index-1);
		ipLen = str.GetLength();
	}
	if (fl==0)
	{
		m_arrRecvIp[3] = atoi(str);
	}
	else
	{
		m_arrLocalIp[3] = atoi(str);
	}
}

/***************************************************************
	函 数 名：closeRegist()
	功能描述：关闭双向校验socket
	参    数：无
	全局变量：无
	返 回 值: 无
****************************************************************/
void CTransControlEx::closeRegist()
{
	if (m_bCreate)
	{
		if (NULL == m_pSocket)
		{
			return;
		}
		m_pSocket->ShutDown(2);
		m_pSocket->Close();
		m_pSocket = NULL;
		m_bCreate = FALSE;
	}
	
}

/***************************************************************
	函 数 名：ApplyRegisNo()
	功能描述：申请注册号
	参    数：无
	全局变量：无
	返 回 值: 无
****************************************************************/
void CTransControlEx::ApplyRegisNo()
{
	if (m_bCreate)
	{
		m_pSocket->ApplyRegisNo(this);
	}
	else
	{
		AfxMessageBox(_T("注册端口没有初始化"));
	}
}

/***************************************************************
	函 数 名：LogRegisNo()
	功能描述：注销申请号
	参    数：无
	全局变量：无
	返 回 值: 无
****************************************************************/
void CTransControlEx::LogRegisNo()
{
	if (m_bCreate)
	{
		m_pSocket->LogRegisNo(this);
	}
	else
	{
		AfxMessageBox(_T("注册端口没有初始化"));
	}
}
