/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：TransControlEx.h
* 摘    要：为主程序提供导出接口(主要有:创建SOCKET,发送数据,接收数据,等)
* 
* 创建者：董国华
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

//#include "TransSocket.h"
//#include "RegistationSocket.h"
//#include "TreatyPack.h"
#include "..\TransControl\UserDefine.h"
//定义传输类型
//文件收发
#define TYPE_SEND_FILE            0
#define TYPE_RECEIVE_FILE         1
//短报文收发
#define TYPE_SEND_SHORTMES        2
#define TYPE_RECEIVE_SHORTMES     3
//流媒体收发
#define TYPE_SEND_STREAM          4
#define TYPE_RECEIVE_STREAM       5

#define MAX_FNAME_LEN             1300


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

//回调函数,反馈接收的数据给上层
// typedef  VOID (CALLBACK *TransCallBack)(int nTransType, int nMesType,int nLen,unsigned char* buff);
// 
// #define UPDATE_PROGRESS     0 //更新进度条
// #define SAFE_CHECK_ERROR    1 //安全检查错误
// #define BOTHWAY_CHECK_FAIL  2 //双向校验回路失败
// 
// //传输错误
// #define TRANS_ERROR 3 
// //传输完毕
// #define TRANS_COMPLETE 4
// //通知程序开始发数
// #define START_SENDDATA  5
// //双向校验回路重启不成功
// #define DOUBLECHECK_RESTART_ERROR 7 
// //停止接收
// #define STOP_RECV 8
// //回调函数,反馈错误信息或者文件传输时的进度
// typedef void (CALLBACK *FeedBackInfo)(LONG,void*);

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
	//初始化函数
	DLL_API BOOL Init(int _nRecvPort,LPCSTR _lpRecvIp,int _nTransType,TransCallBack _tc,FeedBackInfo _fi,USHORT _sendLocalPort=61111);
	//设置回调函数
//	DLL_API void SetFeedBackPoint(TransCallBack _tc,FeedBackInfo _fi);

	//创建Socket
	DLL_API BOOL CreateSocket(LPCSTR _lpIP,int _desPort,int _localEndPort=0);

	//输入参数0:停止  1：启动  2：重启
//	DLL_API BOOL SetState(int nState);
	
	//重启
	DLL_API BOOL ReStart(int nLocalport,int nTransType,LPCSTR _lpIP);
	//停止传输
	DLL_API void StopTrans();
	//获取当前运行状态，返回当前运行状态
	DLL_API int GetState();
	
	//发送数据
	DLL_API BOOL SendData(LPCSTR _pathDir, LPCSTR strDirectPath);

	//发送数据(发送短报文、流媒体数据)
	DLL_API BOOL SendData(byte* dataFrame,ULONG dataLen);
	//接收数据
	
	DLL_API BOOL RecvFile(LPCSTR _path);
	//add 14-01-16 lgp from yn begin
    //接收数据
	DLL_API BOOL RecvData();
	DLL_API BOOL RecvMedia();
    //add 14-01-16 lgp from yn end

	//设置是否安全检查,是否双向校验
//	DLL_API void SetCheckFlag(BOOL bcf=FALSE,BOOL bcs=FALSE);
	DLL_API void closeRegist();
	//若需双向校验，先调此函数申请注册号
	DLL_API void ApplyRegisNo();
	//注销双向校验码
	DLL_API void LogRegisNo();
	//传输状态回调
//	DLL_API	void Trans_CallBack(TransCallBack pcall);
	
	//对接收到的数据进行处理,提供CTransSocket::OnReceive中调用
//	DLL_API	void DealRevData(int nLen,unsigned char* buff);

	//UDP数据传输类
	//是否进行安全检查
	BOOL m_bCheckSafe;
};

#endif // !defined(AFX_MYTRANSCONTROL_H__9EFB8295_A3A4_4C96_80D8_B85F13E3D04A__INCLUDED_)
