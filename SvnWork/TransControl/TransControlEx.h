/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�TransControlEx.h
* ժ    Ҫ��Ϊ�������ṩ�����ӿ�(��Ҫ��:����SOCKET,��������,��������,��)
* 
* �����ߣ�dgh
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

#include "TransSocket.h"
#include "TreatyPack.h"
#include "UserDefine.h"

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
	//�����豸�ն�
//	DLL_API static BOOL ReStartDeviceEnd(int _localEndPort,LPCSTR);
	//��ʼ������
	DLL_API BOOL Init(int _nRecvPort,LPCSTR _lpRecvIp,int _nTransType,TransCallBack _tc,FeedBackInfo _fi,USHORT _sendLocalPort=61111);

	//����Socket
	DLL_API BOOL CreateSocket(LPCSTR _lpIP,int _desPort,int _localEndPort=0);

	//ֹͣ����
	DLL_API void StopTrans();

	//��ȡ��ǰ����״̬�����ص�ǰ����״̬
	DLL_API int GetState();
	
	//��������
	DLL_API BOOL SendData(LPCSTR _filePath,LPCSTR _dirStr="");

	//��������(���Ͷ̱��ġ���ý������)
	DLL_API BOOL SendData(byte* dataFrame,ULONG dataLen);
	
	//��������
	DLL_API BOOL RecvFile(LPCSTR _path);
	
	//add 14-01-16 lgp from yn begin
	//����
	DLL_API BOOL RecvData();
	DLL_API BOOL RecvMedia();
	//add 14-01-16 lgp from yn end
	
	//����˫��У�飬�ȵ��˺�������ע���
	DLL_API void ApplyRegisNo();

	//ע��˫��У����
	DLL_API void LogRegisNo();

	//�ر�ע����Ϣ
	DLL_API void closeRegist();

	//UDP���ݴ�����
	CTransSocket  m_Socket;
	
	//�ļ�����Ŀ¼·��
	CString m_strPath;

	//��������
	int m_nTransType;

	//��������״̬��
	//�������ú��״̬ 
	//��������״̬ 0:ֹͣ  1������
	int m_state;
	
	//�Ƿ�ֹͣ����
	BOOL m_bStop;

	//�Ƿ���а�ȫ���
	BOOL m_bCheckSafe;

	//�Ƿ����˫��У��
//	BOOL m_bCheckFlag;

	//��ǰIP��ַ
	CString m_lpIp;

	//���ض˿�
	int m_nLocalport;
	
	//У���ʶ
	int m_ckFlag;
	//Ŀ�Ķ˿ں�
	int m_nRecvPort;
	//Ŀ�Ķ�IP��ַ
	BYTE m_arrRecvIp[4];
	
	//����IP��ַ
	BYTE m_arrLocalIp[4];
	static int m_iLocalEndPort;

	//�ص�����������״��
	FeedBackInfo m_pFeedBack;
	//�ص��������
	TransCallBack m_pCallBack;
	
	//���ݻ�����ָ��
	byte* m_pBufData;
	long m_readPos; //��������λ��
	long m_writePos; //д������λ��
	LONG m_curThreadCount; //��ǰ�߳���

	//add 14-01-16 lgp from yn begin
	long m_readP;
	long m_writeP;
	//add 14-01-16 lgp from yn end
public:
	bool DirExist(CString _dir); //Ŀ¼�Ƿ����
	BOOL CreateDirStruct(CString _rootDir,CString _dirStr);//����Ŀ¼�ṹ
private:
	
	//˫���·�Ƿ��ʼ��
	static BOOL m_bCreate;
	//����ͷָ��
	byte *m_pDataFrame;
	//���ݳ���
	ULONG m_dataLen;
	
	//˫���·У�����
	CRegistationSocket *m_pSocket;

	//���ط��Ͷ˿ں�
	USHORT m_sendLocalPort; 
	

	//�ļ��Ƿ����
	BOOL FileExist(CString strFileName);

	//IPת��
	void IpToByte(CString _ip,int fl=0);
	
public:
	CTreatyPack* m_pTrePack;
	ProtocolPacket* m_pProPack;
	
};

#endif // !defined(AFX_MYTRANSCONTROL_H__9EFB8295_A3A4_4C96_80D8_B85F13E3D04A__INCLUDED_)
