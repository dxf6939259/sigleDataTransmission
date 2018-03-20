/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TransControlEx.h
* ժ    Ҫ��Ϊ�������ṩ�����ӿ�(��Ҫ��:����SOCKET,��������,��������,��)
* 
* �����ߣ�������
* ����ʱ�䣺2013/9/12
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
//���崫������
//�ļ��շ�
#define TYPE_SEND_FILE            0
#define TYPE_RECEIVE_FILE         1
//�̱����շ�
#define TYPE_SEND_SHORTMES        2
#define TYPE_RECEIVE_SHORTMES     3
//��ý���շ�
#define TYPE_SEND_STREAM          4
#define TYPE_RECEIVE_STREAM       5

#define MAX_FNAME_LEN             1300


//��ʾ��Ϣ����ʼ���͡����ͽ��̡����ݽ��գ�������ɵȵȣ�
#define TYPE_MES_NOTE 1    
//������Ϣ�������������жϵȵȣ�
#define TYPE_MES_ERROR 0
//���ݽ��գ�buff����Ϊ���ݰ����ݣ�
#define TYPE_MES_DATERECEIVE


#define DLL_EXPORT
#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

//�ص�����,�������յ����ݸ��ϲ�
// typedef  VOID (CALLBACK *TransCallBack)(int nTransType, int nMesType,int nLen,unsigned char* buff);
// 
// #define UPDATE_PROGRESS     0 //���½�����
// #define SAFE_CHECK_ERROR    1 //��ȫ������
// #define BOTHWAY_CHECK_FAIL  2 //˫��У���·ʧ��
// 
// //�������
// #define TRANS_ERROR 3 
// //�������
// #define TRANS_COMPLETE 4
// //֪ͨ����ʼ����
// #define START_SENDDATA  5
// //˫��У���·�������ɹ�
// #define DOUBLECHECK_RESTART_ERROR 7 
// //ֹͣ����
// #define STOP_RECV 8
// //�ص�����,����������Ϣ�����ļ�����ʱ�Ľ���
// typedef void (CALLBACK *FeedBackInfo)(LONG,void*);

class CRegistationSocket;
//���������
class CTransControlEx  
{
public:
	DLL_API CTransControlEx();
	DLL_API virtual ~CTransControlEx();

public:
	
	DLL_API static CTransControlEx* GetInstance();
	DLL_API static CTransControlEx* NewCtransCtrlEx();
	//��ʼ������
	DLL_API BOOL Init(int _nRecvPort,LPCSTR _lpRecvIp,int _nTransType,TransCallBack _tc,FeedBackInfo _fi,USHORT _sendLocalPort=61111);
	//���ûص�����
//	DLL_API void SetFeedBackPoint(TransCallBack _tc,FeedBackInfo _fi);

	//����Socket
	DLL_API BOOL CreateSocket(LPCSTR _lpIP,int _desPort,int _localEndPort=0);

	//�������0:ֹͣ  1������  2������
//	DLL_API BOOL SetState(int nState);
	
	//����
	DLL_API BOOL ReStart(int nLocalport,int nTransType,LPCSTR _lpIP);
	//ֹͣ����
	DLL_API void StopTrans();
	//��ȡ��ǰ����״̬�����ص�ǰ����״̬
	DLL_API int GetState();
	
	//��������
	DLL_API BOOL SendData(LPCSTR _pathDir, LPCSTR strDirectPath);

	//��������(���Ͷ̱��ġ���ý������)
	DLL_API BOOL SendData(byte* dataFrame,ULONG dataLen);
	//��������
	
	DLL_API BOOL RecvFile(LPCSTR _path);
	//add 14-01-16 lgp from yn begin
    //��������
	DLL_API BOOL RecvData();
	DLL_API BOOL RecvMedia();
    //add 14-01-16 lgp from yn end

	//�����Ƿ�ȫ���,�Ƿ�˫��У��
//	DLL_API void SetCheckFlag(BOOL bcf=FALSE,BOOL bcs=FALSE);
	DLL_API void closeRegist();
	//����˫��У�飬�ȵ��˺�������ע���
	DLL_API void ApplyRegisNo();
	//ע��˫��У����
	DLL_API void LogRegisNo();
	//����״̬�ص�
//	DLL_API	void Trans_CallBack(TransCallBack pcall);
	
	//�Խ��յ������ݽ��д���,�ṩCTransSocket::OnReceive�е���
//	DLL_API	void DealRevData(int nLen,unsigned char* buff);

	//UDP���ݴ�����
	//�Ƿ���а�ȫ���
	BOOL m_bCheckSafe;
};

#endif // !defined(AFX_MYTRANSCONTROL_H__9EFB8295_A3A4_4C96_80D8_B85F13E3D04A__INCLUDED_)
