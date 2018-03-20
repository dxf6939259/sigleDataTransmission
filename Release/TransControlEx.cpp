/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TransControlEx.cpp
* ժ    Ҫ��Ϊ�������ṩ�����ӿ�(��Ҫ��:����SOCKET,��������,��������,��)
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

     dgh       2013-09-12       Ver 1.0         ����
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
�� �� ����NewCtransCtrlEx()
������������̬�����µĴ��������
��    ������
ȫ�ֱ�������
�� �� ֵ: ����CTransControlEx����ָ��
****************************************************************/
CTransControlEx* CTransControlEx::NewCtransCtrlEx()
{
	CTransControlEx *p = new CTransControlEx;
	return p;
}
//	�ļ��շ�
// #define TYPE_SEND_FILE            0
// #define TYPE_RECEIVE_FILE         1
// �̱����շ�
// #define TYPE_SEND_SHORTMES        2
// #define TYPE_RECEIVE_SHORTMES     3
// ��ý���շ�
// #define TYPE_SEND_STREAM          4
// #define TYPE_RECEIVE_STREAM       5


/***************************************************************
�� �� ����Init(int _nRecvPort,LPCSTR _lpRecvIp,int _nTransType)
��������������ʼ�����ն˿ں�,IP��ַ,�Լ���������
��    ����int _nRecvPort   : ���ն˿ں�
          LPCSTR _lpRecvIp : ����IP
		  int _nTransType  : ��������
ȫ�ֱ�������
�� �� ֵ: ��
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
		//�����������Ϊ���ն�ʱ,Ϊ�������ݶ˿��ٻ�����
		if (_nTransType==TYPE_RECEIVE_FILE || _nTransType==TYPE_RECEIVE_SHORTMES || _nTransType==TYPE_RECEIVE_STREAM)
		{
			if (NULL == m_pBufData) //Ϊ��,�������ݻ�����
			{	
				if (_nTransType == TYPE_RECEIVE_FILE)
				{
					m_pBufData = new byte[DATA_BUF_SIZE_F];
				}
				else
				m_pBufData = new byte[DATA_BUF_SIZE];
				if (NULL == m_pBufData)
				{
					AfxMessageBox(_T("socket���ݻ���������ʧ��!"));
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
		AfxMessageBox(_T("���������д�,�봫��0-5֮��,��ʼ��ʧ��!"));
		return FALSE;
	}
	return TRUE;
}

/***************************************************************
	�� �� ����StopTrans()
	����������ֹͣ����
	��    ������
	ȫ�ֱ�������
	�� �� ֵ: ��
****************************************************************/
void CTransControlEx::StopTrans()
{
	m_bStop = TRUE; //���߳��˳�
	m_Socket.Stop();
	WSACleanup();

	//�ж��߳��˳�
	while (1)
	{
		//����
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
		//����
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
			AfxMessageBox(_T("���������д�,���ܳ�ʼ������û�б�����!"));
			break;
		}
	}

}

/***************************************************************
	�� �� ����GetState()
	������������ȡ��ǰ����״̬�����ص�ǰ����״̬
	��    ������
	ȫ�ֱ�������
	�� �� ֵ: ��
****************************************************************/
int CTransControlEx::GetState()
{
	return m_state;
}

/***************************************************************
�� �� ����SendData(LPCSTR _filePath)
������������������
��    ����_pathDir :Ŀ¼����·�������ݴ������Ͷ�
		  _dirStr  :Ŀ¼�ṹ(����:"a\b\")
ȫ�ֱ�������
�� �� ֵ: BOOL ,TRUE:�����߳���������;FALSE:����ʧ��
****************************************************************/
BOOL CTransControlEx::SendData(LPCSTR _filePath,LPCSTR _dirStr)
{
	ASSERT(m_pFeedBack!=NULL && m_pCallBack!=NULL);
	m_bStop = FALSE;
	m_state = TRANS_ING; //���ڴ���
	CString strPath = _filePath ;
	CString dirPathFile = _dirStr;
 	//����IP��ַ
	byte byIp[4]={0};
	byIp[0] = m_arrRecvIp[0];
	byIp[1] = m_arrRecvIp[1];
	byIp[2] = m_arrRecvIp[2];
	byIp[3] = m_arrRecvIp[3];

	if (m_nTransType == TYPE_SEND_FILE)
	{
		//�����ļ�

		CFile ifile;
		CString strFileName;
		int nCt = 0;
		char chBuf[MAX_FRAME_LEN]={0};

		int serNum = 0;
	
		LONG lEveTimes=0;
		static LONG lCurTimes;
		lCurTimes = 0;
		LONG lCurTmpTs = 0; //��ʱ�洢��������ֵ�����Ƚ�

		DWORD dwFileLen=0;

		//�Ѿ����͵��ܳ���
		DWORD dwSendSum = 0;
		
		int seriNum = 0;
 			BOOL bRet = ifile.Open(strPath,CFile::modeRead);
 			if (!bRet)
 			{//�ļ���ȡʧ��
				return FALSE;
 			}
		
			//��ȡ�ļ����з���
			dwFileLen = ifile.GetLength();

			seriNum = 0;
			strFileName = ifile.GetFileName();
			//add dgh 2014-2-27,��Ŀ¼�ṹ
			strFileName = dirPathFile+strFileName;
			//end
			int fileLen = strFileName.GetLength();
			
			m_pTrePack->ResetData();
			m_pProPack->Reset();
			
			//������ն˿ں�
			m_pProPack->desPort = m_nRecvPort;
			m_pProPack->desAddr[0] = byIp[0];
			m_pProPack->desAddr[1] = byIp[1];
			m_pProPack->desAddr[2] = byIp[2];
			m_pProPack->desAddr[3] = byIp[3];
			m_pProPack->dataType = TRANS_FILE;
			m_pProPack->checkMark = m_ckFlag;
			m_pProPack->dataLen = fileLen;
			m_pProPack->serNum = seriNum; //�����
			seriNum++;
			
			memcpy(m_pProPack->dataBuf,strFileName.GetBuffer(fileLen),fileLen); 

			*m_pTrePack<<*m_pProPack;

			//����ļ��ܳ���
			m_pTrePack->PackFileLen(dwFileLen,fileLen);

			//�����ļ���Ϣ
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
				
				//������ն˿ں�
				m_pProPack->desPort = m_nRecvPort;
				m_pProPack->desAddr[0] = byIp[0];
				m_pProPack->desAddr[1] = byIp[1];
				m_pProPack->desAddr[2] = byIp[2];
				m_pProPack->desAddr[3] = byIp[3];


				m_pProPack->checkMark = m_ckFlag;
				m_pProPack->dataLen = nRead;
				m_pProPack->serNum = seriNum; //�����
				seriNum++;

				*m_pTrePack<<*m_pProPack;
				
				m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
				//add 10-30 begin ������������
				for(int i=0;i<theApp.m_nSendRate;i++)
				for(int i=0;i<theApp.m_nSendRate;i++) //35M/s
						;
			    //add 10-30 end
				dwSendSum += nRead;			
				//�����������ֵ
				lCurTimes = (1.0*dwSendSum/dwFileLen)*100;
				if (lCurTimes>lCurTmpTs)
				{
					//modify lgp 12-18 begin
					//���ͽ�������ֵ
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
			//������ն˿ں�
			m_pProPack->desPort = m_nRecvPort;
			m_pProPack->desAddr[0] = byIp[0];
			m_pProPack->desAddr[1] = byIp[1];
			m_pProPack->desAddr[2] = byIp[2];
			m_pProPack->desAddr[3] = byIp[3];

			m_pProPack->checkMark = m_ckFlag;
			m_pProPack->dataLen = 0;
			m_pProPack->serNum = seriNum; //�����
			seriNum++;

			*m_pTrePack<<*m_pProPack;

			//������ɰ�,���ݳ���Ϊ��
			m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());

			lCurTimes = 100;
			m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,0); //��ɽ���
			m_pFeedBack(TRANS_COMPLETE,0,0);//�������
		}
	m_state = TRANS_END;
	return TRUE;
}

/***************************************************************
�� �� ����SendData(byte* dataFrame,ULONG dataLen)
������������������(�������ݴ�������ý�崫��)
��    ����dataFrame :����ͷָ��
	      dataLen   :���ݵĳ���	
ȫ�ֱ�������
�� �� ֵ: BOOL ,TRUE:���ݷ��ͳɹ�;FALSE:����ʧ��
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
		AfxMessageBox(_T("�������Ͳ���ȷ!"));
		return FALSE;
	}
		

	//������Ч�Լ��
	if (m_nTransType == TYPE_SEND_SHORTMES)
	{
		//delete lgp 12-16 begin
		/*if (dataLen>MAX_FRAME_LEN)
		{
			AfxMessageBox(_T("�̱������ݹ���!"));
			return FALSE;
		}*/
		//delete lgp 12-16 end
	}
		
		//����IP��ַ
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
		
		//������ն˿ں�
		m_pProPack->desPort = m_nRecvPort;
		m_pProPack->desAddr[0] = byIp[0];
		m_pProPack->desAddr[1] = byIp[1];
		m_pProPack->desAddr[2] = byIp[2];
		m_pProPack->desAddr[3] = byIp[3];


		//���зְ�����
		if (dataLen >MAX_FRAME_LEN)
		{
			// modify lgp 12-16 begin
			/*
			//����ְ���
			nPack = (dataLen+MAX_FRAME_LEN-1)/MAX_FRAME_LEN;
			for (int i=0; i<nPack; i++)
			{		
				if (m_bStop) //ֹͣ����
				{
					break;
				}
				m_pProPack->serNum = seriNum; //�����
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
				*m_pTrePack<<*m_pProPack;  //���
				m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
				TRACE("�����Ѿ����ͣ�\n");
			}
			*/
			//����ְ���
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
						if (m_bStop) //ֹͣ����
						{
							break;
						}
						m_pProPack->serNum = seriNum; //�����
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
						*m_pTrePack<<*m_pProPack;  //���
						m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
						TRACE("�����Ѿ����ͣ�\n");
					//delete 14-01-16 lgp from yn begin	
					//}
					//delete 14-01-16 lgp from yn end
				}
				else
				{
					m_pProPack->serNum = seriNum; //�����
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
					*m_pTrePack<<*m_pProPack;  //���
					m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
				}
				
			}
			//modify lgp 12-16 end
		}
		else
		{
			m_pProPack->serNum = seriNum; //�����
			seriNum++;
			m_pProPack->dataLen = dataLen;
            //modify 14-01-16 lgp from yn begin
			m_pProPack->dataType = 32;
			//modify 14-01-16 lgp from yn end
			memcpy(m_pProPack->dataBuf,dataFrame,dataLen);						
			*m_pTrePack<<*m_pProPack;  //���
			m_Socket.SendData(m_pTrePack->GetBuffer(),m_pTrePack->GetDataLength());
		//	AfxMessageBox("�����Ѿ����ͣ�\n");
		}
		m_state = TRANS_ERROR;
		return TRUE;
}

/***************************************************************
�� �� ����RecvDataThread(LPVOID lpvoid)
�������������ݽ����߳�
��    ����LPVOID lpvoid:�߳�ָ��(���봫�����)
ȫ�ֱ�������
�� �� ֵ: 0�����˳�
****************************************************************/
UINT RecvFileThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	byte byBuf[MAX_FRAME_LEN+20]={0};
	int nRecv = 0;
	while (!pThis->m_bStop)
	{
		nRecv = pThis->m_Socket.RecvData(byBuf,MAX_FRAME_LEN+20);
		if (SOCKET_ERROR == nRecv) //���մ���,socket�ر�
		{
			pThis->m_pFeedBack(TRANS_ERROR,0,0); 
			break;
		}
		if (0 == nRecv) //�ӵ�0�����ݲ�����,��������
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
			//дָ�벻���ڶ�ָ�����д����Ϣ����
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//д�뻺����
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),byBuf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				pThis->m_pFeedBack(DATABUF_IS_FULL,0,0); //���ͻ���������
				continue;
			}
			break;
		}
		//ƫ�����������ʱ,ʹ��ع�Ϊ0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT_F-1)?0:pThis->m_writePos+1;
	}
	InterlockedDecrement(&pThis->m_curThreadCount);

	//�����մ����߳���Ϊ0ʱ,����״̬��־Ϊ����
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
		if (SOCKET_ERROR == nRecv) //���մ���,socket�ر�
		{
			pThis->m_pFeedBack(TRANS_ERROR,0,0); 
			break;
		}
		if (0 == nRecv) //�ӵ�0�����ݲ�����,��������
		{
			continue;
		}
		while (!pThis->m_bStop)
		{
			//add lgp 12-02 end
			//дָ�벻���ڶ�ָ�����д����Ϣ����
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//д�뻺����
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),byBuf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				pThis->m_pFeedBack(DATABUF_IS_FULL,0,0); //���ͻ���������
				continue;
			}
			break;
		}
		//ƫ�����������ʱ,ʹ��ع�Ϊ0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_writePos+1;
	}
	InterlockedDecrement(&pThis->m_curThreadCount);
	
	//�����մ����߳���Ϊ0ʱ,����״̬��־Ϊ����
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
		if (SOCKET_ERROR == nRecv) //���մ���,socket�ر�
		{
			pThis->m_pFeedBack(TRANS_ERROR,0,0); 
			break;
		}
		if (0 == nRecv) //�ӵ�0�����ݲ�����,��������
		{
			continue;
		}
		while (!pThis->m_bStop)
		{
			//add lgp 12-02 end
			//дָ�벻���ڶ�ָ�����д����Ϣ����
			if (pThis->m_writePos != pThis->m_readPos) 
			{
				//д�뻺����
				memcpy(pThis->m_pBufData+pThis->m_writePos*(MAX_FRAME_LEN+20),byBuf,(MAX_FRAME_LEN+20));
			}
			else
			{
				Sleep(10);
				pThis->m_pFeedBack(DATABUF_IS_FULL,0,0); //���ͻ���������
				continue;
			}
			break;
		}
		//ƫ�����������ʱ,ʹ��ع�Ϊ0;
		pThis->m_writePos = (pThis->m_writePos == MAX_FRAME_BUF_COUNT-1)?0:pThis->m_writePos+1;
	}
	InterlockedDecrement(&pThis->m_curThreadCount);
	
	//�����մ����߳���Ϊ0ʱ,����״̬��־Ϊ����
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
/***************************************************************
�� �� ����ProcessDataThread(LPVOID lpvoid)
�������������ݴ����߳�
��    ����LPVOID lpvoid:�߳�ָ��(���봫�����)
ȫ�ֱ�������
�� �� ֵ: 0�����˳�
****************************************************************/
UINT ProcessFileThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	CString csPath = pThis->m_strPath; //����·��
	pThis->m_state = TRANS_ING;  //���ڽ�������

	long tempCount = 0;

	BOOL bCallBack = FALSE; //�Ƿ�ص����ϲ�

	//�жϴ�������(���ݴ��������ý�崫��ʱ�ص����ϲ�)
	if (pThis->m_nTransType == TYPE_RECEIVE_SHORTMES || pThis->m_nTransType == TYPE_RECEIVE_STREAM )
	{
		//�ص����ϲ�
		bCallBack = TRUE;		
	}

	RecvWriteFile* pRwFile = NULL;
	map<int,RecvWriteFile*>::iterator itRwf;
	map<int,RecvWriteFile*> mapRecvWtFile;
	mapRecvWtFile.clear();

	CString fileName; //�ļ�������
	int serNum = 0;  
	int curSerNum = 0;  //��ǰ���
	CString strFileName;  //�ļ�ȫ·��
	CFileStatus status;
	byte byData = 0;  //���ݴ�������
	byte arrFileName[90]={0};
	DWORD dwFileLen = 0;
	CTime curTime; //ʱ��
	CString strTime;   //�ӵ����ݵ�ʱ��
	LONG64 curRecv = 0;  //��ǰ���յ�������������
	BOOL bOpenRet = FALSE; //���ļ��Ƿ�ɹ�
	LONG lCurTimes = 0; //��ǰ���ȴ���
	int ckFlag = 0;
	int iTimeOut = 0;
	USHORT dataLen = 0;//����������(������Ϊ��ʱ,�ļ��������)
	int aTest = 0;

	CString strTimeFileRecv;
	//add lgp 12-02 begin
	BOOL bRecvPathExist = TRUE;
	//add lgp 12-02 end
	while (!pThis->m_bStop)
	{
			tempCount = (pThis->m_readPos == MAX_FRAME_BUF_COUNT_F-1)?0:pThis->m_readPos+1;
			pThis->m_pTrePack->ResetData();
			while (!pThis->m_bStop) //ѭ��������
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
					if (iTimeOut >= 3000) //�ӳ�����ر�IO
					{
						iTimeOut = 0;
						for (map<int,RecvWriteFile*>::iterator it=mapRecvWtFile.begin(); it!=mapRecvWtFile.end(); it++)
						{
							if (NULL!=it->second->pOfile)
							{
								
								CString strNewName = csPath + "\\" +  it->second->strReFileName;
								CString strOldName = strNewName + _T(".temp");
								
								//�ļ����滻
								int nRet = rename(strOldName.GetBuffer(strOldName.GetLength()), strNewName.GetBuffer(strNewName.GetLength()));

								it->second->pOfile->Close();
								//�������,��������ļ���
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
			//�����ֹͣ���˳����һ��ѭ��
			if (pThis->m_bStop) 
			{
				break;
			}
			pThis->m_pProPack->Reset();
			*(pThis->m_pTrePack)>>pThis->m_pProPack;
			
			//�ж�֡ͷ��Ϣ,���У����
			if (pThis->m_pProPack->frameHead != 0x16ee || pThis->m_pProPack->checkCode != pThis->m_pTrePack->GetCheckCode())
			{
				pThis->m_pFeedBack(FRAME_ERROR,0,0);
				continue;
			}
			byData = pThis->m_pProPack->dataType; 
			dataLen = pThis->m_pProPack->dataLen;

			//�ӵ���֡���������Ͳ�ƥ��
			if (((byData==1)&&(TYPE_RECEIVE_FILE != pThis->m_nTransType))
				||(byData==20&&TYPE_RECEIVE_SHORTMES != pThis->m_nTransType)
				||(byData==30&&TYPE_RECEIVE_STREAM != pThis->m_nTransType))
			{
				pThis->m_pFeedBack(TRANSTYPE_ERROR,0,0);
				continue;
			}
			

			////////////////////////////////////////////////
			if (bCallBack) //�ص����ϲ�
			{
				pThis->m_pCallBack(pThis->m_nTransType,0,pThis->m_pProPack->dataLen+20,pThis->m_pTrePack->GetBuffer());
				TRACE("�ص����ϲ㣡%d\n",aTest++);
				continue;
			}
			
			
			//˫��У����
			ckFlag = pThis->m_pProPack->checkMark;
			
			//��ǰ����֡���
			curSerNum = pThis->m_pProPack->serNum;
			int i  = 0;
			
			itRwf = mapRecvWtFile.find(ckFlag);
			if (itRwf != mapRecvWtFile.end())
			{
				//�ж��Ƿ�֡
				if (itRwf->second->curSerNum < curSerNum)
				{
					//��д��
					if (itRwf->second->pOfile->m_hFile!=-1)
					{
						byte ltBuf[MAX_FRAME_LEN]={0};
						for (int i=0; i<curSerNum-itRwf->second->curSerNum; i++)
						{
							itRwf->second->pOfile->Write(ltBuf,MAX_FRAME_LEN);
							itRwf->second->curRecvLen += pThis->m_pProPack->dataLen;
						}
						
						//���������
						lCurTimes = (1.0*itRwf->second->curRecvLen/itRwf->second->fileLen)*100;
						if (lCurTimes >itRwf->second->lCurTimes)
						{
							pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,0);
							itRwf->second->lCurTimes = lCurTimes;
						}
						
					}
					else
					{//�޷�д���ļ�,�ļ��������
						pThis->m_pFeedBack(WRITE_FILE_ERROR,0,0);
					}
					
					//�ܹ���֡����
					itRwf->second->lostPacketCt += (curSerNum-itRwf->second->curSerNum);
					pThis->m_pFeedBack(TRANS_LOSS_PACKET,0,0); //���ֶ�֡
				}
				//���ô��ļ��´�Ӧ���յ�֡���
				itRwf->second->curSerNum = curSerNum+1;  
			}
		
		
			//��������
			if (curSerNum !=0 && dataLen !=0)
			{
				if (itRwf != mapRecvWtFile.end())
				{
					if (itRwf->second->pOfile->m_hFile!=-1)
					{
						itRwf->second->pOfile->Write(pThis->m_pProPack->dataBuf,pThis->m_pProPack->dataLen);
						itRwf->second->curRecvLen += pThis->m_pProPack->dataLen;
						
						//���������
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
					//������Ϣ,�˱�ʶ֡�ļ�ĩ������,���ܶ�ʧ��Ϣͷ
					pThis->m_pFeedBack(FILE_NOCREATE,0,0);
				}
			}
			
			//�ļ���
			else if (curSerNum == 0 && dataLen != 0)
			{
				//add lgp 12-02 begin
				if((TRANS_ING == pThis->m_state)&& (1 == pThis->m_nTransType))
				{	//�洢·��������,�˳�����!
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
				
				for (i=0; i<6; i++) //�ļ��ܳ���(�ļ�����6���ֽ�)
				{
					dwFileLen<<=8;
					dwFileLen |= *(pThis->m_pProPack->dataBuf+95-i);
				}
				if (dwFileLen==0)
				{
					//�ļ��ܳ�������
					continue;
				}
				
				memset(arrFileName,0x00,90);
				memcpy(arrFileName,pThis->m_pProPack->dataBuf,90);
				
				//���ͽ����ļ���
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
				//����Ŀ¼�ṹ
				if (!pThis->CreateDirStruct(csPath,fileName))
				{
					//Ŀ¼�ṹ����ʧ��
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
				if(!bOpenRet) //�ļ���ʧ��!
				{
					continue;
				}
				curRecv = 0;
			}

		 //�������ݽ������	
			else if (curSerNum!=0 && dataLen==0)
			{
				
				if (itRwf != mapRecvWtFile.end())
				{			
					//		if (itRwf->second->pOfile->GetStatus(strFileName,status) && itRwf->second->pOfile->m_hFile!=-1)
					//		{
					itRwf->second->pOfile->Close();

					CString strNewName = csPath + "\\" +  itRwf->second->strReFileName;
					CString strOldName = strNewName + _T(".temp");

					//�ļ����滻
					int nRet = rename(strOldName.GetBuffer(strOldName.GetLength()), strNewName.GetBuffer(strNewName.GetLength()));
					
					strTimeFileRecv = itRwf->second->strReFileName;					
					//		}
					//�������,��������ļ���
					pThis->m_pFeedBack(TRANS_COMPLETE,(void*)itRwf->second->strReFileName.GetBuffer(itRwf->second->strReFileName.GetLength()),0);
					
					lCurTimes = 100;
					pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
					SafeDelete(itRwf->second);
					mapRecvWtFile.erase(itRwf);	
				}		
	
			}			
			else
			{
				//������Ϣ,֡��ʽ����
				pThis->m_pFeedBack(FRAME_ERROR,0,0); 
			}			
		//add lgp 12-02 begin		
//		bRecvPathExist = TRUE;

		//add lgp 12-13 begin
		//�����ֹͣ���˳����һ��ѭ��
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
	
	//�ͷ�map����
	for (map<int,RecvWriteFile*>::iterator it=mapRecvWtFile.begin(); it!=mapRecvWtFile.end(); it++)
	{
		if (NULL!=it->second->pOfile)
		{
			it->second->pOfile->Close();
		}
		SafeDelete(it->second);
	}
	mapRecvWtFile.clear();

	//�ص�ֹͣ����
	pThis->m_pFeedBack(STOP_RECV,0,0);
	InterlockedDecrement(&pThis->m_curThreadCount);

	//�����մ����߳���Ϊ0ʱ,����״̬��־Ϊ����
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn begin
/***************************************************************
�� �� ����ProcessMediaThread(LPVOID lpvoid)
�������������ݴ����߳�
��    ����LPVOID lpvoid:�߳�ָ��(���봫�����)
ȫ�ֱ�������
�� �� ֵ: 0�����˳�
****************************************************************/

UINT ProcessMediaThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	//CString csPath = pThis->m_strPath; //����·��
	pThis->m_state = TRANS_ING;  //���ڽ�������

	long tempCount = 0;
	RecvWriteFile* pRwFile = NULL;
 

	CString fileName; //�ļ�������
	int serNum = 0;  
	int curSerNum = 0;  //��ǰ���
	CString strFileName;  //�ļ�ȫ·��
//	CFileStatus status;
	byte byData = 0;
	byte arrFileName[90]={0};
	DWORD dwFileLen = 0;
	CTime curTime; //ʱ��
	CString strTime;   //�ӵ����ݵ�ʱ��
	LONG64 curRecv = 0;  //��ǰ���յ�������������
//	BOOL bOpenRet = FALSE; //���ļ��Ƿ�ɹ�
	LONG lCurTimes = 0; //��ǰ���ȴ���
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
			while (!pThis->m_bStop) //ѭ��������
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
					if (iTimeOut >= 3000) //�ӳ�����ر�IO
					{
						iTimeOut = 0;
					}
					continue;
				}
				break;
			}
			//�����ֹͣ���˳����һ��ѭ��
			if (pThis->m_bStop) 
			{
				break;
			}
			pThis->m_pProPack->Reset();
			*(pThis->m_pTrePack)>>pThis->m_pProPack;
			
			//�ж�֡ͷ��Ϣ,���У����
			if (pThis->m_pProPack->frameHead != 0x16ee || pThis->m_pProPack->checkCode != pThis->m_pTrePack->GetCheckCode())
			{
				pThis->m_pFeedBack(FRAME_ERROR,0,0);
				continue;
			}
			byData = pThis->m_pProPack->dataType; 
			
			//�ӵ���֡���������Ͳ�ƥ��
			if (TYPE_RECEIVE_STREAM != pThis->m_nTransType)
			{
				pThis->m_pFeedBack(TRANSTYPE_ERROR,0,0);
				continue;
			}			
			//˫��У����
			ckFlag = pThis->m_pProPack->checkMark;
			
			//��ǰ����֡���
			curSerNum = pThis->m_pProPack->serNum;
			int i  = 0;
			switch (byData)
			{
			case 31: //��������
				//add lgp 12-02 begin
				pThis->m_pFeedBack(31,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes =100;
				break;
			case 32: //�������ݽ������		
				pThis->m_pFeedBack(32,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes = 100;
// 			case 30:
// 				pThis->m_pFeedBack(30,pThis->m_pProPack->dataBuf,(void*)&pThiFs->m_pProPack->dataLen);
// 				lCurTimes = 100;				
			//	pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
				break;
			default:
				//������Ϣ,֡��ʽ����
				pThis->m_pFeedBack(FRAME_ERROR,0,0); 
				break;
			}
		}
	 
	}
	//�ص�ֹͣ����
	pThis->m_pFeedBack(STOP_RECV,0,0);
	InterlockedDecrement(&pThis->m_curThreadCount);

	//�����մ����߳���Ϊ0ʱ,����״̬��־Ϊ����
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
//add 14-01-16 lgp from yn begin
/***************************************************************
�� �� ����ProcessMediaThread(LPVOID lpvoid)
�������������ݴ����߳�
��    ����LPVOID lpvoid:�߳�ָ��(���봫�����)
ȫ�ֱ�������
�� �� ֵ: 0�����˳�
****************************************************************/

UINT ProcessDataThread(LPVOID lpvoid)
{
	CTransControlEx* pThis = (CTransControlEx*)lpvoid;
	//CString csPath = pThis->m_strPath; //����·��
	pThis->m_state = TRANS_ING;  //���ڽ�������

	long tempCount = 0;
	RecvWriteFile* pRwFile = NULL;
 

	CString fileName; //�ļ�������
	int serNum = 0;  
	int curSerNum = 0;  //��ǰ���
	CString strFileName;  //�ļ�ȫ·��
//	CFileStatus status;
	byte byData = 0;
	byte arrFileName[90]={0};
	DWORD dwFileLen = 0;
	CTime curTime; //ʱ��
	CString strTime;   //�ӵ����ݵ�ʱ��
	LONG64 curRecv = 0;  //��ǰ���յ�������������
//	BOOL bOpenRet = FALSE; //���ļ��Ƿ�ɹ�
	LONG lCurTimes = 0; //��ǰ���ȴ���
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
			while (!pThis->m_bStop) //ѭ��������
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
					if (iTimeOut >= 3000) //�ӳ�����ر�IO
					{
						iTimeOut = 0;
					}
					continue;
				}
				break;
			}
			//�����ֹͣ���˳����һ��ѭ��
			if (pThis->m_bStop) 
			{
				break;
			}
			pThis->m_pProPack->Reset();
			*(pThis->m_pTrePack)>>pThis->m_pProPack;
			
			//�ж�֡ͷ��Ϣ,���У����
			if (pThis->m_pProPack->frameHead != 0x16ee || pThis->m_pProPack->checkCode != pThis->m_pTrePack->GetCheckCode())
			{
				pThis->m_pFeedBack(FRAME_ERROR,0,0);
				continue;
			}
			byData = pThis->m_pProPack->dataType; 
			
			//�ӵ���֡���������Ͳ�ƥ��
			if (TYPE_RECEIVE_SHORTMES != pThis->m_nTransType)
			{
				pThis->m_pFeedBack(TRANSTYPE_ERROR,0,0);
				continue;
			}			
			//˫��У����
			ckFlag = pThis->m_pProPack->checkMark;
			
			//��ǰ����֡���
			curSerNum = pThis->m_pProPack->serNum;
			int i  = 0;
			switch (byData)
			{
			case 31: //��������
				//add lgp 12-02 begin
				pThis->m_pFeedBack(31,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes =100;
				break;
			case 32: //�������ݽ������		
				pThis->m_pFeedBack(32,pThis->m_pProPack->dataBuf,(void*)&pThis->m_pProPack->dataLen);
				lCurTimes = 100;
// 			case 30:
// 				pThis->m_pFeedBack(30,pThis->m_pProPack->dataBuf,(void*)&pThiFs->m_pProPack->dataLen);
// 				lCurTimes = 100;				
			//	pThis->m_pFeedBack(UPDATE_PROGRESS,(void*)&lCurTimes,(void*)strTimeFileRecv.GetBuffer(strTimeFileRecv.GetLength()));
				break;
			default:
				//������Ϣ,֡��ʽ����
				pThis->m_pFeedBack(FRAME_ERROR,0,0); 
				break;
			}
		}
	 
	}
	//�ص�ֹͣ����
	pThis->m_pFeedBack(STOP_RECV,0,0);
	InterlockedDecrement(&pThis->m_curThreadCount);

	//�����մ����߳���Ϊ0ʱ,����״̬��־Ϊ����
	if (0 == pThis->m_curThreadCount)
	{
		pThis->m_state = TRANS_END; 
	}
	return 0;
}
//add 14-01-16 lgp from yn end
/***************************************************************
��������RecvData(LPCSTR _path)
������������������
������LPCSTR _path: ����·��
ȫ�ֱ�������
����ֵ: BOOL ,TRUE:�����߳������ɹ�,���ڽ�������; FALSE:����ʧ��
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
			AfxMessageBox(_T("�ļ���Ŀ¼������!"));
			return FALSE;
		}
	}
 
	//�������ݽ����߳�
	AfxBeginThread(RecvFileThread,(LPVOID)this);

	//�������ݴ����߳�
	AfxBeginThread(ProcessFileThread,(LPVOID)this);

	m_curThreadCount = 2;  //��ǰ�����߳�
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
			AfxMessageBox(_T("�ļ���Ŀ¼������!"));
			return FALSE;
		}
	}
	
	//�������ݽ����߳�
	AfxBeginThread(RecvDataThread,(LPVOID)this);
	
	//�������ݴ����߳�
	AfxBeginThread(ProcessDataThread,(LPVOID)this);
	
	m_curThreadCount = 2;  //��ǰ�����߳�
	
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
			AfxMessageBox(_T("�ļ���Ŀ¼������!"));
			return FALSE;
		}
	}
	
	//�������ݽ����߳�
	AfxBeginThread(RecvMediaThread,(LPVOID)this);
	
	//�������ݴ����߳�
	AfxBeginThread(ProcessMediaThread,(LPVOID)this);
	
	m_curThreadCount = 2;  //��ǰ�����߳�
	
	return TRUE;
}
//add 14-01-16 lgp from yn end
/***************************************************************
�� �� ����CreateSocket(LPCSTR _lpIP)
��������������SOCKET�׽���
��    ����_lpIP: IP��ַ
ȫ�ֱ�������
�� �� ֵ: BOOL ,TRUE:socket�����ɹ�; FALSE:socket����ʧ��
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
		AfxMessageBox(_T("WSAStartupʧ��"));
		return FALSE;
	}

	//���Ƿ��Ͷ�ʱ,����˫��У��socket
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
�� �� ����DirExist(CString _dir)
����������Ŀ¼�Ƿ����
��    ����CString _dir: Ŀ¼
ȫ�ֱ�������
�� �� ֵ: bool TRUE : Ŀ¼����; 
               FALSE: Ŀ¼������;
****************************************************************/
bool CTransControlEx::DirExist(CString _dir)
{
	DWORD   dwAttr   =   GetFileAttributes(_dir); 
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)//Ŀ¼������ 
	{
		return false;
	}
	return true;
}

/***************************************************************
�� �� ����CreateDirStruct(CString _dirStr)
��������������Ŀ¼�ṹ
��    ����CString _rootDir:��Ŀ¼
		  CString _dirStr: Ŀ¼�ṹ
ȫ�ֱ�������
�� �� ֵ: BOOL TRUE : Ŀ¼�����ɹ�; 
               FALSE: Ŀ¼����ʧ��;
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
�� �� ����FileExist(CString strFileName)
�����������ж��ļ��Ƿ����
��    ����CString strFileName :�ļ���
ȫ�ֱ�������
�� �� ֵ: BOOL TRUE : �ļ�����; 
               FALSE: �ļ�������;
****************************************************************/
BOOL CTransControlEx::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName);
}

/***************************************************************
	�� �� ����IpToByte(CString _ip)
	����������IPת��
	��    ����CString _ip�ַ���IP
			: int fl
	ȫ�ֱ�������
	�� �� ֵ: ��
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
	�� �� ����closeRegist()
	�����������ر�˫��У��socket
	��    ������
	ȫ�ֱ�������
	�� �� ֵ: ��
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
	�� �� ����ApplyRegisNo()
	��������������ע���
	��    ������
	ȫ�ֱ�������
	�� �� ֵ: ��
****************************************************************/
void CTransControlEx::ApplyRegisNo()
{
	if (m_bCreate)
	{
		m_pSocket->ApplyRegisNo(this);
	}
	else
	{
		AfxMessageBox(_T("ע��˿�û�г�ʼ��"));
	}
}

/***************************************************************
	�� �� ����LogRegisNo()
	����������ע�������
	��    ������
	ȫ�ֱ�������
	�� �� ֵ: ��
****************************************************************/
void CTransControlEx::LogRegisNo()
{
	if (m_bCreate)
	{
		m_pSocket->LogRegisNo(this);
	}
	else
	{
		AfxMessageBox(_T("ע��˿�û�г�ʼ��"));
	}
}
