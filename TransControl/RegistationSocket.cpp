/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：RegistationSocket.cpp
* 摘    要：为某一发送获取一个申请号
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

   dgh       2013-09-14       Ver 1.0         创建
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
函 数 名：GetInstance()
功能描述：得到双向校验回路单实例
参    数：无
全局变量：无
返 回 值: 无
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
函 数 名：ApplyRegisNo(CTransControlEx* pcontrol)
功能描述：申请注册号
参    数：CTransControlEx* pcontrol: 标示哪个对象提出申请
全局变量：无
返 回 值: 无
****************************************************************/

void CRegistationSocket::ApplyRegisNo(CTransControlEx* pcontrol)
{
	m_pControl = pcontrol;
	//当传输类型等于发送端类型时,申请发送数据注册号
	if (pcontrol->m_nTransType==0||pcontrol->m_nTransType==2||pcontrol->m_nTransType==4)
	{
		map<int,CTransControlEx*>::iterator it;
		it = m_mapControl.find(pcontrol->m_nTransType);
		if (it==m_mapControl.end()) //此类型传输对象不存在,则申请注册号
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
			bpro.opType = 1;//申请校验标识
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
		else //对象存在
		{
			LogRegisNo(pcontrol);
			pcontrol->m_pFeedBack(DOUBLECHECK_RESTART_ERROR,0,0);	
		}
	//	m_pControl->m_pFeedBack(5,0);	
	}
	else
	{
		//AfxMessageBox(_T("传输类型不正确"));
	}
}	

/***************************************************************
函 数 名：LogRegisNo(CTransControlEx* pcontrol)
功能描述：发送结束注销注册号
参    数：CTransControlEx* pcontrol: 标示哪个对象提出申请
全局变量：无
返 回 值: 无
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
			bpro.opType = 2;//通知校验标识失效
			bpro.checkFlag = pcontrol->m_ckFlag; //当前的校验码

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
			m_mapControl.erase(pcontrol->m_nTransType); //释放MAP中对应的项
		}
	
	}
	else
	{
	//	AfxMessageBox(_T("传输类型不正确"));
	}
}

/***************************************************************
函 数 名：OnReceive(int nErrorCode) 
功能描述：接收数据
参    数：int nErrorCode： 错误码
全局变量：无
返 回 值: 无
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
	//得到注册号，开始发数
	trPack>>bpro;
	map<int,int>::iterator it;
	map<int,CTransControlEx*>::iterator itTc;
	CString str;
	switch (bpro.opType)
	{
	case 1:     //处理申请注册号请求
// 		bpro.opType = 3;
// 		while (1)
// 		{
// 			//随机生成一个注册号
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
			//回调,通知界面注册已经得到,可以发数
			m_pControl->m_pFeedBack(START_SENDDATA,0,0); 
		}	
		break;
	case 2:     //接收端注销注册号
		it = m_mapCheckFlag.find(bpro.checkFlag);
		if (it!=m_mapCheckFlag.end())
		{
			m_mapCheckFlag.erase(bpro.checkFlag);
		}
		break;

	case 3: //得到注册号
		itTc = m_mapControl.find(m_pControl->m_nTransType);
		if (itTc == m_mapControl.end())
		{
			m_pControl->m_ckFlag = bpro.checkFlag;
// 			str.Format("得到的注册号是 %d",m_pControl->m_ckFlag);
// 			AfxMessageBox(str);
			m_mapControl[m_pControl->m_nTransType] = m_pControl;

			//回调,通知界面注册已经得到,可以发数
			m_pControl->m_pFeedBack(START_SENDDATA,0,0); 
		}	
		break;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
