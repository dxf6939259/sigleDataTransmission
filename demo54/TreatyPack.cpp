/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TreatyPack.cpp
* ժ    Ҫ��Э���������
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

  dgh       2013-09-13       Ver 1.0         ����
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
		AfxMessageBox(_T("Э�����ڴ�����ʧ��!"));
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
�� �� ����ResetData()
�����������ָ�Ĭ�ϳ�ʼֵ
��    ������
ȫ�ֱ�������
�� �� ֵ: ��
****************************************************************/
void CTreatyPack::ResetData()
{
	memset(m_pDataBuf,0x00,MAX_FRAME_LEN+20);
	m_dataLength = 0;
}

/***************************************************************
�� �� ����ResetData()
�����������õ�У����
��    ������
ȫ�ֱ�������
�� �� ֵ: ��
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
�� �� ����operator<<(const ProtocolPacket& rhs)
�������������Э��
��    ����const ProtocolPacket& rhs: Э��
ȫ�ֱ�������
�� �� ֵ: CTreatyPack& �����������
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
�� �� ����operator<<(const BothProtocol& rhs)
�������������Э��
��    ����const BothProtocol& rhs: Э��
ȫ�ֱ�������
�� �� ֵ: CTreatyPack& �����������
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const BothProtocol& rhs)
{
	*this<<rhs.frameHead<<rhs.desAddr[0]<<rhs.desAddr[1]<<rhs.desAddr[2]<<rhs.desAddr[3]<<rhs.desPort\
		<<rhs.transType<<rhs.opType<<rhs.checkFlag<<rhs.checkCode;
	return *this;
}

/***************************************************************
�� �� ����operator >> (byte* pData)
����������Э����
��    ����byte* pData: ����ָ��
ȫ�ֱ�������
�� �� ֵ: ��
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
	�� �� ����operator >> (ProtocolPacket* pData)
	����������Э����
	��    ����ProtocolPacket* pData : �⵽Э��ṹ����
	ȫ�ֱ�������
	�� �� ֵ: ��
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
�� �� ����operator >> (BothProtocol pData)
����������Э����
��    ����BothProtocol pData : Э��ṹ��
ȫ�ֱ�������
�� �� ֵ: ��
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
�� �� ����operator << (const int& rhs)
�������������int����
��    ����const int& rhs: Ҫ�����INT��������
ȫ�ֱ�������
�� �� ֵ: CTreatyPack&,�����������
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const int& rhs)
{ 
	ASSERT(sizeof(int)==4);
	memcpy(m_pDataBuf+m_dataLength,&rhs,4);
	m_dataLength+=4;
	return *this;
}

/***************************************************************
�� �� ����operator << (const USHORT& rhs)
�������������USHORT����
��    ����const USHORT& rhs: ���USHORT����
ȫ�ֱ�������
�� �� ֵ: CTreatyPack&,�����������
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const USHORT& rhs)
{
	ASSERT(sizeof(USHORT)==2);
	memcpy(m_pDataBuf+m_dataLength,&rhs,2);
	m_dataLength+=2;
	return *this;
}

/***************************************************************
�� �� ����operator << (const byte& rhs)
�������������byte����
��    ����const byte& rhs: ���byte����
ȫ�ֱ�������
�� �� ֵ: CTreatyPack&,�����������
****************************************************************/
CTreatyPack& CTreatyPack::operator << (const byte& rhs)
{
	ASSERT(sizeof(byte)==1);
	memcpy(m_pDataBuf+m_dataLength,&rhs,1);
	m_dataLength+=1;
	return *this;
}

