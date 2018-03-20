/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TransSocket.cpp
* ժ    Ҫ��socket�������ݴ���
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

    dgh       2013-09-13       Ver 1.0         ����
*/

// TransSocket.cpp: implementation of the CTransSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransControl.h"
#include "TransSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//ȫ�ֺ���,�õ�IP��ַ
CString GetLocal();
//��̬��Ա����,�õ�����IP��ַ
CString CTransSocket::m_csLocalIP = GetLocal();

//#include <afxmt.h>
//CCriticalSection g_UDPSendCs;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransSocket::CTransSocket()
{
	m_udp_socket = 0;
}

CTransSocket::~CTransSocket()
{

}

/***************************************************************
�� �� ����CreateUDPSocket(int nLocalport,int nTransType,CString sendIP)
��������������UDPͨ��
��    ����
          USHORT sdLcPort: ���Ͷ˱��ض˿ں�
		  int nLocalport: �˿ں�
          int nTransType: ��������
		  CString sendIP: IP��ַ
		  int nDesport: Ŀ�Ķ˿�
ȫ�ֱ�������
�� �� ֵ: BOOL ,�ɹ�TRUE,ʧ��FALSE
****************************************************************/
BOOL CTransSocket::CreateUDPSocket(USHORT sdLcPort,int nLocalport,int nTransType,CString sendIP,int nDesport)
{
	m_udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int nBuf = (MAX_FRAME_LEN+20)*200000;
	int nRevOpt= sizeof(nBuf);
	SOCKADDR_IN sin;
	unsigned long argp = 1;
	switch (nTransType)
	{
		//���Ͷ�
	case TYPE_SEND_FILE:
	case TYPE_SEND_SHORTMES:
	case TYPE_SEND_STREAM:
		//����UDP����socket
		m_sendAdd.sin_family = AF_INET;
		m_sendAdd.sin_port = htons(nDesport);
		m_sendAdd.sin_addr.S_un.S_addr = inet_addr(sendIP.GetBuffer(sendIP.GetLength()));
	
		//�󶨱��ض˿ں���Ϣ
		sin.sin_family = AF_INET;
		sin.sin_port = htons(sdLcPort);
		sin.sin_addr.S_un.S_addr = inet_addr(m_csLocalIP.GetBuffer(m_csLocalIP.GetLength()));
	
		//���û�������С,���Ϊ(MAX_FRAME_LEN+20)*1000;���ֽ�
		setsockopt(m_udp_socket,SOL_SOCKET,SO_SNDBUF,(char*)&nBuf,nRevOpt);
		//���÷�����ģʽ
		ioctlsocket(m_udp_socket,FIONBIO,(unsigned long*)&argp);
		break;
	case TYPE_RECEIVE_FILE:
	case TYPE_RECEIVE_SHORTMES:
	case TYPE_RECEIVE_STREAM:
		//����UDP����socket		
		sin.sin_family = AF_INET;   
		sin.sin_port = htons(nLocalport);   
		sin.sin_addr.S_un.S_addr = inet_addr(m_csLocalIP.GetBuffer(m_csLocalIP.GetLength()));

		//���û�������С,���Ϊ(MAX_FRAME_LEN+20)*1000;���ֽ�
		setsockopt(m_udp_socket,SOL_SOCKET,SO_RCVBUF,(char*)&nBuf,nRevOpt);
		break;
	}
	
	if (bind(m_udp_socket,(SOCKADDR*)&sin,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
	//	AfxMessageBox(_T("socket��ʧ��!"));
		return FALSE;
	}

	return TRUE;
}

/***************************************************************
�� �� ����SendData(byte * _byData, ULONG _ulLen)
������������������
��    ����byte * _byData: ���͵�ͷָ��
          ULONG _ulLen: ����������
ȫ�ֱ�������
�� �� ֵ: BOOL ,�ɹ�TRUE,ʧ��FALSE
****************************************************************/
BOOL CTransSocket::SendData(byte * _byData, ULONG _ulLen)
{
//	g_UDPSendCs.Lock();
	//modify lgp 2014-04-2 begin
	//int nSent = sendto(m_udp_socket,(char*)_byData,_ulLen,0,(sockaddr*)&m_sendAdd,sizeof(SOCKADDR_IN));
	int nSent = sendto(m_udp_socket,(char*)_byData,_ulLen,0,(sockaddr*)&m_sendAdd,sizeof(SOCKADDR_IN));
	while(SOCKET_ERROR == nSent)
	{
		nSent = sendto(m_udp_socket,(char*)_byData,_ulLen,0,(sockaddr*)&m_sendAdd,sizeof(SOCKADDR_IN));
	}
	//modify lgp 2014-04-2 end
//	g_UDPSendCs.Unlock();
	if (nSent != _ulLen)
	{
		int err = WSAGetLastError();
		TRACE("nSent = %d,err = %d\n",nSent,err);
		return FALSE;
	}
	return TRUE;
}

/***************************************************************
�� �� ����RecvData(byte * _byData, int _Len)
�������������ݽ���
��    ����byte* _byData: ���յ�ͷָ��
          ULONG _ulLen : ����������
ȫ�ֱ�������
�� �� ֵ: int ����ʵ�ʽ��յ������ݶȶ�
****************************************************************/
int CTransSocket::RecvData(byte * _byData, int _Len)
{
	int nLen = sizeof(SOCKADDR_IN);
	int nRecv = recvfrom(m_udp_socket,(char*)_byData,_Len,0,(sockaddr*)&m_recvfromAddr,&nLen);
	return nRecv;
}

/***************************************************************
�� �� ����Stop()
�����������ر�socket
��    ������
ȫ�ֱ�������
�� �� ֵ: ��
****************************************************************/
void CTransSocket::Stop()
{
	if(m_udp_socket)
	{
		shutdown(m_udp_socket,2);
		closesocket(m_udp_socket);
		m_udp_socket = 0;
	}
}

/***************************************************************
�� �� ����GetLocal()
�����������õ�IP��ַ
��    ������
ȫ�ֱ�������
�� �� ֵ: IP��ַ
****************************************************************/
CString GetLocal()
{
	WSADATA wsadata; 
	int err; 
	err=WSAStartup(MAKEWORD(2,2),&wsadata);
	char hostname[128]; 
	if(gethostname(hostname,128)==0)
	{ 
		//	printf("%s\n",hostname);//��������� 
	}
	struct hostent *pHost = gethostbyname(hostname);
    CString strLocalIp = inet_ntoa(*(struct in_addr*)pHost->h_addr_list[0]);
	WSACleanup();
	return strLocalIp;
}