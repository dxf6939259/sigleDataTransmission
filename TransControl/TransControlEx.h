/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TransControlEx.h
* 摘    要：为主程序提供导出接口(主要有:创建SOCKET,发送数据,接收数据,等)
* 
* 创建者：dgh
* 创建时间：2013/9/12
*/


// MyTransControl.h: interface for the CTransControlEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTRANSCONTROL_H__9EFB8295_A3A4_4C96_80D8_B85F13E3D04A__INCLUDED_)
#define AFX_MYTRANSCONTROL_H__9EFB8295_A3A4_4C96_80D8_B85F13E3D04A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TransSocket.h"
#include "TreatyPack.h"
#include "UserDefine.h"

//提示信息（开始发送、发送进程、数据接收，接收完成等等）
#define TYPE_MES_NOTE 1    
//错误信息（丢包、网络中断等等）
#define TYPE_MES_ERROR 0
//数据接收（buff内容为数据包内容）
#define TYPE_MES_DATERECEIVE

#define DLL_EXPORT
#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


class CRegistationSocket;
//传输控制类
class CTransControlEx  
{
public:
	DLL_API CTransControlEx();
	DLL_API virtual ~CTransControlEx();

public:
	
	DLL_API static CTransControlEx* GetInstance();
	DLL_API static CTransControlEx* NewCtransCtrlEx();
	//重启设备终端
//	DLL_API static BOOL ReStartDeviceEnd(int _localEndPort,LPCSTR);
	//初始化函数
	DLL_API BOOL Init(int _nRecvPort,LPCSTR _lpRecvIp,int _nTransType,TransCallBack _tc,FeedBackInfo _fi,USHORT _sendLocalPort=61111);

	//创建Socket
	DLL_API BOOL CreateSocket(LPCSTR _lpIP,int _desPort,int _localEndPort=0);

	//停止传输
	DLL_API void StopTrans();

	//获取当前运行状态，返回当前运行状态
	DLL_API int GetState();
	
	//发送数据
	DLL_API BOOL SendData(LPCSTR _filePath,LPCSTR _dirStr="");

	//发送数据(发送短报文、流媒体数据)
	DLL_API BOOL SendData(byte* dataFrame,ULONG dataLen);
	
	//接收数据
	DLL_API BOOL RecvFile(LPCSTR _path);
	
	//add 14-01-16 lgp from yn begin
	//接收
	DLL_API BOOL RecvData();
	DLL_API BOOL RecvMedia();
	//add 14-01-16 lgp from yn end
	
	//若需双向校验，先调此函数申请注册号
	DLL_API void ApplyRegisNo();

	//注销双向校验码
	DLL_API void LogRegisNo();

	//关闭注册信息
	DLL_API void closeRegist();

	//UDP数据传输类
	CTransSocket  m_Socket;
	
	//文件或者目录路径
	CString m_strPath;

	//传输类型
	int m_nTransType;

	//设置运行状态，
	//返回设置后的状态 
	//服务运行状态 0:停止  1：运行
	int m_state;
	
	//是否停止传输
	BOOL m_bStop;

	//是否进行安全检查
	BOOL m_bCheckSafe;

	//是否进行双向校验
//	BOOL m_bCheckFlag;

	//当前IP地址
	CString m_lpIp;

	//本地端口
	int m_nLocalport;
	
	//校验标识
	int m_ckFlag;
	//目的端口号
	int m_nRecvPort;
	//目的端IP地址
	BYTE m_arrRecvIp[4];
	
	//本地IP地址
	BYTE m_arrLocalIp[4];
	static int m_iLocalEndPort;

	//回调函数，反馈状况
	FeedBackInfo m_pFeedBack;
	//回调函数句柄
	TransCallBack m_pCallBack;
	
	//数据缓冲区指针
	byte* m_pBufData;
	long m_readPos; //读缓冲区位置
	long m_writePos; //写缓冲区位置
	LONG m_curThreadCount; //当前线程数

	//add 14-01-16 lgp from yn begin
	long m_readP;
	long m_writeP;
	//add 14-01-16 lgp from yn end
public:
	bool DirExist(CString _dir); //目录是否存在
	BOOL CreateDirStruct(CString _rootDir,CString _dirStr);//创建目录结构
private:
	
	//双向回路是否初始化
	static BOOL m_bCreate;
	//数据头指针
	byte *m_pDataFrame;
	//数据长度
	ULONG m_dataLen;
	
	//双向回路校验许可
	CRegistationSocket *m_pSocket;

	//本地发送端口号
	USHORT m_sendLocalPort; 
	

	//文件是否存在
	BOOL FileExist(CString strFileName);

	//IP转换
	void IpToByte(CString _ip,int fl=0);
	
public:
	CTreatyPack* m_pTrePack;
	ProtocolPacket* m_pProPack;
	
};

#endif // !defined(AFX_MYTRANSCONTROL_H__9EFB8295_A3A4_4C96_80D8_B85F13E3D04A__INCLUDED_)
