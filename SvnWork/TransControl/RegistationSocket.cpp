/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�RegistationSocket.cpp
* ժ    Ҫ��Ϊĳһ���ͻ�ȡһ�������
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

   dgh       2013-09-14       Ver 1.0         ����
*/



// RegistationSocket.cpp : implementation file
//

#include "stdafx.h"
#include "TransControl.h"
#include "TransControlEx.h"
#include "RegistationSocket.h"
#include "TreatyPack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegistationSocket

CRegistationSocket::CRegistationSocket()
{
	 m_mapControl.clear();
	 m_mapCheckFlag.clear();
	 m_pControl = NULL;
}

CRegistationSocket::~CRegistationSocket()
{
	int a=0;
}

/***************************************************************
�� �� ����GetInstance()
�����������õ�˫��У���·��ʵ��
��    ������
ȫ�ֱ�������
�� �� ֵ: ��
****************************************************************/
CRegistationSocket* CRegistationSocket::GetInstance()
{
	static CRegistationSocket rs;
	return &rs;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRegistationSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CRegistationSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRegistationSocket member functions

/***************************************************************
�� �� ����ApplyRegisNo(CTransControlEx* pcontrol)
��������������ע���
��    ����CTransControlEx* pcontrol: ��ʾ�ĸ������������
ȫ�ֱ�������
�� �� ֵ: ��
****************************************************************/

void CRegistationSocket::ApplyRegisNo(CTransControlEx* pcontrol)
{
	m_pControl = pcontrol;
	//���������͵��ڷ��Ͷ�����ʱ,���뷢������ע���
	if (pcontrol->m_nTransType==0||pcontrol->m_nTransType==2||pcontrol->m_nTransType==4)
	{
		map<int,CTransControlEx*>::iterator it;
		it = m_mapControl.find(pcontrol->m_nTransType);
		if (it==m_mapControl.end()) //�����ʹ�����󲻴���,������ע���
		{
		//	CTreatyPack trPack;
		//	BothProtocol bpro;
			bpro.Reset();
			bpro.desAddr[0]=pcontrol->m_arrLocalIp[0];
			bpro.desAddr[1]=pcontrol->m_arrLocalIp[1];
			bpro.desAddr[2]=pcontrol->m_arrLocalIp[2];
			bpro.desAddr[3]=pcontrol->m_arrLocalIp[3];
			bpro.desPort = pcontrol->m_iLocalEndPort;
			bpro.transType = pcontrol->m_nTransType;
			bpro.opType = 1;//����У���ʶ
			bpro.checkCode = bpro.GetCheckCode();
			
			trPack.ResetData();
			trPack<<bpro;

			//modify lgp 2014-04-2 begin
			//SendTo(trPack.GetBuffer(),trPack.GetDataLength(),60011,pcontrol->m_lpIp,0);
			int nRet = SendTo(trPack.GetBuffer(),trPack.GetDataLength(),60011,pcontrol->m_lpIp,0);
            while(SOCKET_ERROR == nRet)
			{
				nRet = SendTo(trPack.GetBuffer(),trPack.GetDataLength(),60011,pcontrol->m_lpIp,0);
			}
			//modify lgp 2014-04-2 end
		}
		else //�������
		{
			LogRegisNo(pcontrol);
			pcontrol->m_pFeedBack(DOUBLECHECK_RESTART_ERROR,0,0);	
		}
	//	m_pControl->m_pFeedBack(5,0);	
	}
	else
	{
		//AfxMessageBox(_T("�������Ͳ���ȷ"));
	}
}	

/***************************************************************
�� �� ����LogRegisNo(CTransControlEx* pcontrol)
�������������ͽ���ע��ע���
��    ����CTransControlEx* pcontrol: ��ʾ�ĸ������������
ȫ�ֱ�������
�� �� ֵ: ��
****************************************************************/
void CRegistationSocket::LogRegisNo(CTransControlEx* pcontrol)
{
	m_pControl = pcontrol;
	if (pcontrol->m_nTransType==0||pcontrol->m_nTransType==2||pcontrol->m_nTransType==4)
	{	
		map<int,CTransControlEx*>::iterator it;
		it = m_mapControl.find(pcontrol->m_nTransType);
		if (it!=m_mapControl.end())
		{
		//	CTreatyPack trPack;
		//	BothProtocol bpro;
			bpro.Reset();
			bpro.desAddr[0]=it->second->m_arrLocalIp[0];
			bpro.desAddr[1]=it->second->m_arrLocalIp[1];
			bpro.desAddr[2]=it->second->m_arrLocalIp[2];
			bpro.desAddr[3]=it->second->m_arrLocalIp[3];
			bpro.desPort = pcontrol->m_iLocalEndPort;
			bpro.transType = pcontrol->m_nTransType;
			bpro.opType = 2;//֪ͨУ���ʶʧЧ
			bpro.checkFlag = pcontrol->m_ckFlag; //��ǰ��У����

			bpro.checkCode = bpro.GetCheckCode();
			
			trPack.ResetData();
			trPack<<bpro; 
			//modify lgp 2014-04-2 begin
			//SendTo(trPack.GetBuffer(),trPack.GetDataLength(),60011,pcontrol->m_lpIp,0);
			int nRet = SendTo(trPack.GetBuffer(),trPack.GetDataLength(),60011,pcontrol->m_lpIp,0);
            while(SOCKET_ERROR == nRet)
			{
				nRet = SendTo(trPack.GetBuffer(),trPack.GetDataLength(),60011,pcontrol->m_lpIp,0);
			}
			//modify lgp 2014-04-2 end
			m_mapControl.erase(pcontrol->m_nTransType); //�ͷ�MAP�ж�Ӧ����
		}
	
	}
	else
	{
	//	AfxMessageBox(_T("�������Ͳ���ȷ"));
	}
}

/***************************************************************
�� �� ����OnReceive(int nErrorCode) 
������������������
��    ����int nErrorCode�� ������
ȫ�ֱ�������
�� �� ֵ: ��
****************************************************************/
void CRegistationSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pControl == 0)
	{
		return;
	}
//	CTreatyPack trPack;
//	BothProtocol bpro;
	bpro.Reset();
	CString serIP;
	unsigned int serPort;
    int nRecv =	ReceiveFrom((void*)trPack.GetBuffer(),MAX_FRAME_LEN+20,serIP,serPort,0);
	
	if (SOCKET_ERROR == nRecv)
	{
		return;
	}
	//�õ�ע��ţ���ʼ����
	trPack>>bpro;
	map<int,int>::iterator it;
	map<int,CTransControlEx*>::iterator itTc;
	CString str;
	switch (bpro.opType)
	{
	case 1:     //��������ע�������
// 		bpro.opType = 3;
// 		while (1)
// 		{
// 			//�������һ��ע���
// 			bpro.checkFlag = rand()%10000;
// 			it = m_mapCheckFlag.find(bpro.checkFlag);
// 			if (it==m_mapCheckFlag.end())
// 			{
// 				m_mapCheckFlag[bpro.checkFlag] = bpro.checkFlag;
// 				break;
// 			}
// 		}
// 	
// 		trPack<<bpro;
// 		SendTo(trPack.GetBuffer(),trPack.GetDataLength(),serPort,serIP.GetBuffer(serIP.GetLength()),0);
// 		break;

		itTc = m_mapControl.find(m_pControl->m_nTransType);
		if (itTc == m_mapControl.end())
		{
			m_pControl->m_ckFlag = bpro.checkFlag;		
			m_mapControl[m_pControl->m_nTransType] = m_pControl;			
			//�ص�,֪ͨ����ע���Ѿ��õ�,���Է���
			m_pControl->m_pFeedBack(START_SENDDATA,0,0); 
		}	
		break;
	case 2:     //���ն�ע��ע���
		it = m_mapCheckFlag.find(bpro.checkFlag);
		if (it!=m_mapCheckFlag.end())
		{
			m_mapCheckFlag.erase(bpro.checkFlag);
		}
		break;

	case 3: //�õ�ע���
		itTc = m_mapControl.find(m_pControl->m_nTransType);
		if (itTc == m_mapControl.end())
		{
			m_pControl->m_ckFlag = bpro.checkFlag;
// 			str.Format("�õ���ע����� %d",m_pControl->m_ckFlag);
// 			AfxMessageBox(str);
			m_mapControl[m_pControl->m_nTransType] = m_pControl;

			//�ص�,֪ͨ����ע���Ѿ��õ�,���Է���
			m_pControl->m_pFeedBack(START_SENDDATA,0,0); 
		}	
		break;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
