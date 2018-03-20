/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TreatyPack.cpp
* 摘    要：协议打包解包类
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

  dgh       2013-09-13       Ver 1.0         创建
*/

// TreatyPack.cpp: implementation of the CTreatyPack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TransControl.h"
#include "TreatyPack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreatyPack::CTreatyPack()
{
	m_dataLength = 0;
	m_pDataBuf = NULL;
	m_pDataBuf = new byte[MAX_FRAME_LEN+20];
	if (NULL == m_pDataBuf)
	{
		AfxMessageBox(_T("协议打包内存申请失败!"));
	}
}

CTreatyPack::~CTreatyPack()
{
	if (NULL!=m_pDataBuf)
	{
		delete []m_pDataBuf;
		m_pDataBuf = NULL;
	}
}

/***************************************************************
函 数 名：ResetData()
功能描述：恢复默认初始值
参    数：无
全局变量：无
返 回 值: 无
****************************************************************/
void CTreatyPack::ResetData()
{
	memset(m_pDataBuf,0x00,MAX_FRAME_LEN+20);
	m_dataLength = 0;
}

/***************************************************************
函 数 名：ResetData()
功能描述：得到校验码
参    数：无
全局变量：无
返 回 值: 无
****************************************************************/
byte CTreatyPack::GetCheckCode()
{
	byte ckCode = 0;
	ASSERT(m_pDataBuf!=NULL);

	DWORD checkCodeSum = 0;

	checkCodeSum += m_protocolPack.checkMark;
	checkCodeSum += m_protocolPack.desAddr[0];
	checkCodeSum += m_protocolPack.desAddr[1];
	checkCodeSum += m_protocolPack.desAddr[2];
	checkCodeSum += m_protocolPack.desAddr[3];
	checkCodeSum += m_protocolPack.desPort;
	checkCodeSum += m_protocolPack.dataType;
	checkCodeSum += m_protocolPack.serNum;
	checkCodeSum += m_protocolPack.dataLen;

	for (int i=0; i<m_protocolPack.dataLen; i++)
	{
		checkCodeSum += m_protocolPack.dataBuf[i];
	}

	ckCode = checkCodeSum & 0x00000000000000ff;
	return ckCode;
}

/***************************************************************
函 数 名：operator<<(const ProtocolPacket& rhs)
功能描述：打包协议
参    数：const ProtocolPacket& rhs: 协议
全局变量：无
返 回 值: CTreatyPack& 本对象的引用
****************************************************************/
CTreatyPack& CTreatyPack::operator<<(const ProtocolPacket& rhs)
{
	m_protocolPack = rhs;
	*this<<rhs.frameHead<<rhs.checkMark<<rhs.desAddr[0]<<rhs.desAddr[1]<<rhs.desAddr[2]<<rhs.desAddr[3]<<rhs.desPort\
		<<rhs.dataType<<rhs.serNum<<rhs.dataLen;

	if (rhs.dataLen!=0)
	{
		memcpy(m_pDataBuf+m_dataLength,rhs.dataBuf,rhs.dataLen);
		m_dataLength+=rhs.dataLen;
	}

	*this<<rhs.checkCode;
	return *this;
}

/***************************************************************
函 数 名：operator<<(const BothProtocol& rhs)
功能描述：打包协议
参    数：const BothProtocol& rhs: 协议
全局变量：无
返 回 值: CTreatyPack& 本对象的引用
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const BothProtocol& rhs)
{
	*this<<rhs.frameHead<<rhs.desAddr[0]<<rhs.desAddr[1]<<rhs.desAddr[2]<<rhs.desAddr[3]<<rhs.desPort\
		<<rhs.transType<<rhs.opType<<rhs.checkFlag<<rhs.checkCode;
	return *this;
}

/***************************************************************
函 数 名：operator >> (byte* pData)
功能描述：协议解包
参    数：byte* pData: 数据指针
全局变量：无
返 回 值: 无
****************************************************************/
void CTreatyPack::operator >> (byte* pData)
{
	memcpy(m_pDataBuf,pData,20+MAX_FRAME_LEN);

	memcpy(&m_protocolPack.frameHead,pData,2);
	memcpy(&m_protocolPack.checkMark,pData+2,4);
	memcpy(&m_protocolPack.desAddr[0],pData+6,1);
	memcpy(&m_protocolPack.desAddr[1],pData+7,1);
	memcpy(&m_protocolPack.desAddr[2],pData+8,1);
	memcpy(&m_protocolPack.desAddr[3],pData+9,1);
	memcpy(&m_protocolPack.desPort,pData+10,2);
	memcpy(&m_protocolPack.dataType,pData+12,1);
	memcpy(&m_protocolPack.serNum,pData+13,4);
	memcpy(&m_protocolPack.dataLen,pData+17,2);
	memcpy(&m_protocolPack.dataBuf,pData+19,MAX_FRAME_LEN);
	memcpy(&m_protocolPack.checkCode,pData+19+MAX_FRAME_LEN,1);
}

/***************************************************************
	函 数 名：operator >> (ProtocolPacket* pData)
	功能描述：协议解包
	参    数：ProtocolPacket* pData : 解到协议结构体中
	全局变量：无
	返 回 值: 无
****************************************************************/
void CTreatyPack::operator >> (ProtocolPacket* pData)
{
	memcpy(&pData->frameHead,m_pDataBuf,2);
	m_protocolPack.frameHead = pData->frameHead;
	memcpy(&pData->checkMark,m_pDataBuf+2,4);
	m_protocolPack.checkMark = pData->checkMark;

	memcpy(&pData->desAddr[0],m_pDataBuf+6,1);
	m_protocolPack.desAddr[0] = pData->desAddr[0];

	memcpy(&pData->desAddr[1],m_pDataBuf+7,1);
	m_protocolPack.desAddr[1] = pData->desAddr[1];
	memcpy(&pData->desAddr[2],m_pDataBuf+8,1);
	m_protocolPack.desAddr[2] = pData->desAddr[2];

	memcpy(&pData->desAddr[3],m_pDataBuf+9,1);
	m_protocolPack.desAddr[3] = pData->desAddr[3];

	memcpy(&pData->desPort,m_pDataBuf+10,2);
	m_protocolPack.desPort = pData->desPort;

	memcpy(&pData->dataType,m_pDataBuf+12,1);
	m_protocolPack.dataType = pData->dataType;

	memcpy(&pData->serNum,m_pDataBuf+13,4);
	m_protocolPack.serNum = pData->serNum;

	memcpy(&pData->dataLen,m_pDataBuf+17,2);
	m_protocolPack.dataLen = pData->dataLen;

	if (pData->dataLen!=0)
	{
		memcpy(&pData->dataBuf,m_pDataBuf+19,pData->dataLen);
		memcpy(m_protocolPack.dataBuf,m_pDataBuf+19,pData->dataLen);
	}
	
	memcpy(&pData->checkCode,m_pDataBuf+19+pData->dataLen,1);
	m_protocolPack.checkCode = pData->checkCode;
}

/***************************************************************
函 数 名：operator >> (BothProtocol pData)
功能描述：协议解包
参    数：BothProtocol pData : 协议结构体
全局变量：无
返 回 值: 无
****************************************************************/
void CTreatyPack::operator >> (BothProtocol &pData)
{
	memcpy(&pData.frameHead,m_pDataBuf,2);
	memcpy(&pData.desAddr[0],m_pDataBuf+2,1);
	memcpy(&pData.desAddr[1],m_pDataBuf+3,1);
	memcpy(&pData.desAddr[2],m_pDataBuf+4,1);
	memcpy(&pData.desAddr[3],m_pDataBuf+5,1);
	memcpy(&pData.desPort,m_pDataBuf+6,2);
	memcpy(&pData.transType,m_pDataBuf+8,1);
	memcpy(&pData.opType,m_pDataBuf+9,1);
	memcpy(&pData.checkFlag,m_pDataBuf+10,4);
	memcpy(&pData.checkCode,m_pDataBuf+14,1);
}

/***************************************************************
函 数 名：operator << (const int& rhs)
功能描述：打包int类型
参    数：const int& rhs: 要打包的INT类型数据
全局变量：无
返 回 值: CTreatyPack&,本对象的引用
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const int& rhs)
{ 
	ASSERT(sizeof(int)==4);
	memcpy(m_pDataBuf+m_dataLength,&rhs,4);
	m_dataLength+=4;
	return *this;
}

/***************************************************************
函 数 名：operator << (const USHORT& rhs)
功能描述：打包USHORT类型
参    数：const USHORT& rhs: 打包USHORT类型
全局变量：无
返 回 值: CTreatyPack&,本对象的引用
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const USHORT& rhs)
{
	ASSERT(sizeof(USHORT)==2);
	memcpy(m_pDataBuf+m_dataLength,&rhs,2);
	m_dataLength+=2;
	return *this;
}

/***************************************************************
函 数 名：operator << (const byte& rhs)
功能描述：打包byte类型
参    数：const byte& rhs: 打包byte类型
全局变量：无
返 回 值: CTreatyPack&,本对象的引用
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const byte& rhs)
{
	ASSERT(sizeof(byte)==1);
	memcpy(m_pDataBuf+m_dataLength,&rhs,1);
	m_dataLength+=1;
	return *this;
}

