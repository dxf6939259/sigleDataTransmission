/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TreatyPack.h
* 摘    要：协议打包解包类
* 
* 创建者：dgh
* 创建时间：2013/9/13
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



//协议格式
typedef struct _PROTOCOL_PACKET_
{
	USHORT frameHead; //帧头

	//校验标识
	int checkMark;

	//目的端IP地址组
	BYTE desAddr[4];

	//目的端端口
	USHORT desPort;

	//数据类型
	//01H：文件传输
	//02H：短报文传输
	//03H：流媒体传输
	BYTE dataType;

	//数据传输序号
	int serNum;

	//数据区有效长度
	USHORT dataLen;

	//数据区
	byte dataBuf[MAX_FRAME_LEN];

	//校验码
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

//双向校验协议
typedef struct _BOTHWAY_PROTOCOL_
{
	USHORT frameHead; //帧头
	//目的端IP地址组
	BYTE desAddr[4];
	//目的端端口
	USHORT desPort;
	//传输类型
	BYTE transType;
	//操作类型
	BYTE opType;
	//校验标识
	int checkFlag;
	//校验码
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
//协议打包类
class CTreatyPack  
{
public:
	CTreatyPack();
	virtual ~CTreatyPack();
	
	//得到数据指针
	byte *GetBuffer()
	{
		return m_pDataBuf;
	}

	//得到数据长度
	int GetDataLength()
	{
		return m_dataLength;
	}
	
	//恢复默认初始值
	void ResetData();
	
	//得到校验码
	byte GetCheckCode();
	
	//打包协议
	CTreatyPack& operator << (const ProtocolPacket& rhs);
	CTreatyPack& operator << (const BothProtocol& rhs);
	//协议解包
	void operator >> (byte* pData);
	void operator >> (ProtocolPacket* pData);
	void operator >> (BothProtocol &pData);
	//解包后的数据放入此变量中
	ProtocolPacket m_protocolPack;
private:
	int m_dataLength; //数据长度
	byte *m_pDataBuf; //数据缓冲区

	CTreatyPack& operator << (const int& rhs);
	CTreatyPack& operator << (const USHORT& rhs);
	CTreatyPack& operator << (const byte& rhs);	
	
//	CTreatyPack& operator << (const byte* rhs,int dataLen);
//	void PackDataBuf(const byte* rhs,int dataLen);
};

#endif // !defined(AFX_TREATYPACK_H__C9CD3090_A2B4_4375_966E_0D60B27AFAD9__INCLUDED_)
