/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TransSocket.cpp
* 摘    要：socket用于数据传输
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

    dgh       2013-09-13       Ver 1.0         创建
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

//全局函数,得到IP地址
CString GetLocal();
//静态成员变量,得到本地IP地址
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
函 数 名：CreateUDPSocket(int nLocalport,int nTransType,CString sendIP)
功能描述：创建UDP通信
参    数：
          USHORT sdLcPort: 发送端本地端口号
		  int nLocalport: 端口号
          int nTransType: 传输类型
		  CString sendIP: IP地址
		  int nDesport: 目的端口
全局变量：无
返 回 值: BOOL ,成功TRUE,失败FALSE
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
		//发送端
	case TYPE_SEND_FILE:
	case TYPE_SEND_SHORTMES:
	case TYPE_SEND_STREAM:
		//创建UDP发送socket
		m_sendAdd.sin_family = AF_INET;
		m_sendAdd.sin_port = htons(nDesport);
		m_sendAdd.sin_addr.S_un.S_addr = inet_addr(sendIP.GetBuffer(sendIP.GetLength()));
	
		//绑定本地端口号信息
		sin.sin_family = AF_INET;
		sin.sin_port = htons(sdLcPort);
		sin.sin_addr.S_un.S_addr = inet_addr(m_csLocalIP.GetBuffer(m_csLocalIP.GetLength()));
	
		//设置缓冲区大小,最大为(MAX_FRAME_LEN+20)*1000;个字节
		setsockopt(m_udp_socket,SOL_SOCKET,SO_SNDBUF,(char*)&nBuf,nRevOpt);
		//设置非阻塞模式
		ioctlsocket(m_udp_socket,FIONBIO,(unsigned long*)&argp);
		break;
	case TYPE_RECEIVE_FILE:
	case TYPE_RECEIVE_SHORTMES:
	case TYPE_RECEIVE_STREAM:
		//创建UDP接收socket		
		sin.sin_family = AF_INET;   
		sin.sin_port = htons(nLocalport);   
		sin.sin_addr.S_un.S_addr = inet_addr(m_csLocalIP.GetBuffer(m_csLocalIP.GetLength()));

		//设置缓冲区大小,最大为(MAX_FRAME_LEN+20)*1000;个字节
		setsockopt(m_udp_socket,SOL_SOCKET,SO_RCVBUF,(char*)&nBuf,nRevOpt);
		break;
	}
	
	if (bind(m_udp_socket,(SOCKADDR*)&sin,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
	//	AfxMessageBox(_T("socket绑定失败!"));
		return FALSE;
	}

	return TRUE;
}

/***************************************************************
函 数 名：SendData(byte * _byData, ULONG _ulLen)
功能描述：发送数据
参    数：byte * _byData: 发送的头指针
          ULONG _ulLen: 数据区长度
全局变量：无
返 回 值: BOOL ,成功TRUE,失败FALSE
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
函 数 名：RecvData(byte * _byData, int _Len)
功能描述：数据接收
参    数：byte* _byData: 接收的头指针
          ULONG _ulLen : 数据区长度
全局变量：无
返 回 值: int 返回实际接收到的数据度度
****************************************************************/
int CTransSocket::RecvData(byte * _byData, int _Len)
{
	int nLen = sizeof(SOCKADDR_IN);
	int nRecv = recvfrom(m_udp_socket,(char*)_byData,_Len,0,(sockaddr*)&m_recvfromAddr,&nLen);
	return nRecv;
}

/***************************************************************
函 数 名：Stop()
功能描述：关闭socket
参    数：无
全局变量：无
返 回 值: 无
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
函 数 名：GetLocal()
功能描述：得到IP地址
参    数：无
全局变量：无
返 回 值: IP地址
****************************************************************/
CString GetLocal()
{
	WSADATA wsadata; 
	int err; 
	err=WSAStartup(MAKEWORD(2,2),&wsadata);
	char hostname[128]; 
	if(gethostname(hostname,128)==0)
	{ 
		//	printf("%s\n",hostname);//计算机名字 
	}
	struct hostent *pHost = gethostbyname(hostname);
    CString strLocalIp = inet_ntoa(*(struct in_addr*)pHost->h_addr_list[0]);
	WSACleanup();
	return strLocalIp;
}