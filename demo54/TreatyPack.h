/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TreatyPack.h
* ժ    Ҫ��Э���������
* 
* �����ߣ�dgh
* ����ʱ�䣺2013/9/13
*/

// TreatyPack.h: interface for the CTreatyPack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREATYPACK_H__C9CD3090_A2B4_4375_966E_0D60B27AFAD9__INCLUDED_)
#define AFX_TREATYPACK_H__C9CD3090_A2B4_4375_966E_0D60B27AFAD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef MAX_FRAME_LEN
#define MAX_FRAME_LEN 1300
#endif



//Э���ʽ
typedef struct _PROTOCOL_PACKET_
{
	USHORT frameHead; //֡ͷ

	//У���ʶ
	int checkMark;

	//Ŀ�Ķ�IP��ַ��
	BYTE desAddr[4];

	//Ŀ�Ķ˶˿�
	USHORT desPort;

	//��������
	//01H���ļ�����
	//02H���̱��Ĵ���
	//03H����ý�崫��
	BYTE dataType;

	//���ݴ������
	int serNum;

	//��������Ч����
	USHORT dataLen;

	//������
	byte dataBuf[MAX_FRAME_LEN];

	//У����
	byte checkCode;
	_PROTOCOL_PACKET_()
	{
		frameHead = 0xee16;
		checkMark = 0;
		memset(desAddr,0x00,4);
		desPort  = 0;
		dataType = 0;
		serNum = 0;
		dataLen= 0;
		memset(dataBuf,0x00,MAX_FRAME_LEN);
		checkCode=0;
	}
	void Reset()
	{
		frameHead = 0xee16;
		checkMark = 0;
		memset(desAddr,0x00,4);
		desPort = 0;
		dataType = 0;
		serNum=0;
		dataLen=0;
		memset(dataBuf,0x00,MAX_FRAME_LEN);
		checkCode=0;
	}
}ProtocolPacket;

//˫��У��Э��
typedef struct _BOTHWAY_PROTOCOL_
{
	USHORT frameHead; //֡ͷ
	//Ŀ�Ķ�IP��ַ��
	BYTE desAddr[4];
	//Ŀ�Ķ˶˿�
	USHORT desPort;
	//��������
	BYTE transType;
	//��������
	BYTE opType;
	//У���ʶ
	int checkFlag;
	//У����
	byte checkCode;

	_BOTHWAY_PROTOCOL_()
	{
		frameHead = 0xee18;
		memset(desAddr,0x00,4);
		desPort=0;
		transType=0;
		opType=0;
		checkFlag=0;
		checkCode=0;
	}
	void Reset()
	{
		frameHead = 0xee18;
		memset(desAddr,0x00,4);
		desPort=0;
		transType=0;
		opType=0;
		checkFlag=0;
		checkCode=0;
	}
	
	byte GetCheckCode()
	{
		DWORD dwCheckSum=0;
		dwCheckSum+=desAddr[0];
		dwCheckSum+=desAddr[1];
		dwCheckSum+=desAddr[2];
		dwCheckSum+=desAddr[3];
		dwCheckSum+=desPort;
		dwCheckSum+=transType;
		dwCheckSum+=opType;
		dwCheckSum+=checkFlag;

		byte ckCode = dwCheckSum & 0x00000000000000ff;
		return ckCode;
	}
}BothProtocol;

/////////////////////////////////////////////////////
//Э������
class CTreatyPack  
{
public:
	CTreatyPack();
	virtual ~CTreatyPack();
	
	//�õ�����ָ��
	byte *GetBuffer()
	{
		return m_pDataBuf;
	}

	//�õ����ݳ���
	int GetDataLength()
	{
		return m_dataLength;
	}
	
	//�ָ�Ĭ�ϳ�ʼֵ
	void ResetData();
	
	//�õ�У����
	byte GetCheckCode();
	
	//���Э��
	CTreatyPack& operator << (const ProtocolPacket& rhs);
	CTreatyPack& operator << (const BothProtocol& rhs);
	//Э����
	void operator >> (byte* pData);
	void operator >> (ProtocolPacket* pData);
	void operator >> (BothProtocol &pData);
	//���������ݷ���˱�����
	ProtocolPacket m_protocolPack;
private:
	int m_dataLength; //���ݳ���
	byte *m_pDataBuf; //���ݻ�����

	CTreatyPack& operator << (const int& rhs);
	CTreatyPack& operator << (const USHORT& rhs);
	CTreatyPack& operator << (const byte& rhs);	
	
//	CTreatyPack& operator << (const byte* rhs,int dataLen);
//	void PackDataBuf(const byte* rhs,int dataLen);
};

#endif // !defined(AFX_TREATYPACK_H__C9CD3090_A2B4_4375_966E_0D60B27AFAD9__INCLUDED_)
