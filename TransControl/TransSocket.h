/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TransSocket.h
* 摘    要：socket用于数据传输
* 
* 创建者：dgh
* 创建时间：2013/9/13
*/


// TransSocket.h: interface for the CTransSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSSOCKET_H__A4C8A5BC_D0F1_48B5_AE99_6A0792D5989B__INCLUDED_)
#define AFX_TRANSSOCKET_H__A4C8A5BC_D0F1_48B5_AE99_6A0792D5989B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib,"ws2_32")
#include "UserDefine.h"

//数据传输类
class CTransSocket  
{
public:
	CTransSocket();
	virtual ~CTransSocket();

	BOOL CreateUDPSocket(USHORT sdLcPort,int nLocalport,int nTransType,CString sendIP="127.0.0.1",int nDesport = 0);
	
	//发送数据
	BOOL SendData(byte * _byData, ULONG _ulLen);

	//数据接收
	int RecvData(byte * _byData, int _Len);

	//重启socket
	BOOL ReStart(int nLocalport,int nTransType,CString sendIP);
	//关闭socket
	void Stop();

	static CString m_csLocalIP;
private:

	//UDP Socket
	SOCKET m_udp_socket;
	SOCKADDR_IN m_sendAdd;

	SOCKADDR_IN m_recvfromAddr;
};

#endif // !defined(AFX_TRANSSOCKET_H__A4C8A5BC_D0F1_48B5_AE99_6A0792D5989B__INCLUDED_)
