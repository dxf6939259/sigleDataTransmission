/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TransSocket.h
* ժ    Ҫ��socket�������ݴ���
* 
* �����ߣ�dgh
* ����ʱ�䣺2013/9/13
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

//���ݴ�����
class CTransSocket  
{
public:
	CTransSocket();
	virtual ~CTransSocket();

	BOOL CreateUDPSocket(USHORT sdLcPort,int nLocalport,int nTransType,CString sendIP="127.0.0.1",int nDesport = 0);
	
	//��������
	BOOL SendData(byte * _byData, ULONG _ulLen);

	//���ݽ���
	int RecvData(byte * _byData, int _Len);

	//����socket
	BOOL ReStart(int nLocalport,int nTransType,CString sendIP);
	//�ر�socket
	void Stop();

	static CString m_csLocalIP;
private:

	//UDP Socket
	SOCKET m_udp_socket;
	SOCKADDR_IN m_sendAdd;

	SOCKADDR_IN m_recvfromAddr;
};

#endif // !defined(AFX_TRANSSOCKET_H__A4C8A5BC_D0F1_48B5_AE99_6A0792D5989B__INCLUDED_)
