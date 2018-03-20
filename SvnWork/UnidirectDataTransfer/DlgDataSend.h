/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgDataSend.h
* ժ    Ҫ�����ݷ��ͽ���
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/
#if !defined(AFX_DLGDATASEND_H__C04A1F95_EDEE_4CA7_8580_7B3DCD6A1A80__INCLUDED_)
#define AFX_DLGDATASEND_H__C04A1F95_EDEE_4CA7_8580_7B3DCD6A1A80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDataSend.h : header file
//
#include "TransControlEx.h"
#include "OperXml.h"
#include "OutLogMgr.h"
#include "BufZoneManager.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgDataSend dialog

typedef enum _E_STATUS_SEND
{
  ENU_SEND_UN_REG=1,
  ENU_SEND_REG_ING,
  ENU_SEND_REG,
  ENU_SEND_ING,
  ENU_SEND_OVER,
  ENU_SEND_ERROR,
}E_STATUS_SEND;


#define PORT_MAX_USE 65535
#define PORT_MIN_USE 8080




class CDlgDataSend : public CBCGPDialog
{
// Construction
public:
	CDlgDataSend(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDataSend)
	enum { IDD = IDD_DLG_DATA_SEND };
	CBCGPListCtrl	m_lstLogMedia;
	CBCGPListCtrl	m_lstLogData;
	CBCGPListCtrl	m_lstLogFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDataSend)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDataSend)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRdoDeleteSendedFile();
	afx_msg void OnRaoSaveSendedFile();
	afx_msg void OnRclickLstLogFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickLstLogData(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnRclickLstLogMedia(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEdtPortLocalFile();
	afx_msg void OnFieldchangedIpAimFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEdtPortReceiveFile();
	afx_msg void OnDoubleclickedRdoDeleteSendedFile();
	afx_msg void OnDoubleclickedRaoSaveSendedFile();
	afx_msg void OnChkCheckFile();
	afx_msg void OnChangeEdtPortLocalEnd();
	afx_msg void OnFieldchangedIpLocalEnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEdtPortReceiveEnd();
	afx_msg void OnChkCircuit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnStopFile();
private:
	afx_msg void OnFilePathMonitor(); 
	afx_msg void OnFilePath();
    afx_msg void OnStartFile();
	afx_msg void OnRestartFile();
	afx_msg void OnSaveParamFile();
	
	afx_msg void OnStartData();
	afx_msg void OnStopData();
	afx_msg void OnRestartData();
	afx_msg void OnSaveParamData();

	afx_msg void OnStartMedia();
	afx_msg void OnStopMedia();
	afx_msg void OnRestartMedia();
	afx_msg void OnSaveParamMedia();

	afx_msg void OnSaveParamEnd();

	afx_msg HRESULT SpeedSending(WPARAM wpa, LPARAM lpa);
	
    afx_msg void OnClearSendFileLog();
	afx_msg void OnClearSendDataLog();
	afx_msg void OnClearSendMediaLog();
	//�����ļ���Ϣ��Ӧ����
	afx_msg HRESULT OnSendFile_Msg(WPARAM wpa, LPARAM lpa);
	//����������Ϣ��Ӧ����
	afx_msg HRESULT OnSendData_Msg(WPARAM wpa, LPARAM lpa);
	//������ý��������Ϣ��Ӧ����
	afx_msg HRESULT OnSendMedia_Msg(WPARAM wpa, LPARAM lpa);
    //���õ����������ն˲����������
	afx_msg HRESULT OnSendData_TestXN(WPARAM wpa, LPARAM lpa);
	//��ʼ������
	void Init();

	//�������ݴ���ʱҪ��������ݺͳ��� ����ȥ��
	//void SetSendData(BYTE* byData, UINT bLen);

	//����Ҫ������ý�崫�����ý���ļ���·��
	void SetSendMediaPath(CString strMediaPath);

public:
	//Ҫ���͵��ļ��б�
	CStringList m_strLstFiles;
    //�ӵ����������ն˴������������б�
	//CList<CBufZoneManager,CBufZoneManager&> m_byteLstDatas;
	CByteArray m_arrDatas;
    //CList<CBufZoneManager,CBufZoneManager&> m_byteLstMedias;
	CByteArray m_arrMedias;
    HANDLE m_hSendDataTestXnThread;


	
	LONG m_signalCt;
	BOOL m_bInOneSecond;//�������ܹ����У��յ��������Ƿ��ǵ�һ���ڵ�����
	int  m_nDataCountTestXn;//�������ܹ����У�1�������յ�������֡��
	BOOL m_bDataInTestXn;//���յ��������Ƿ����ڽ������ܲ���
	CCriticalSection m_lockSendDataStatus;//���ݷ���״̬�ٽ���     ����m_byteLstDatas
	CCriticalSection m_lockSendMediaStatus;//��ý�巢��״̬�ٽ���  

	
	//�����ļ��������ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
    BOOL CheckUiAndXmlBeforeStartFile();
	//�������ݴ������ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
	BOOL CheckUiAndXmlBeforeStartData();
	//������ý�崫�����ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
	BOOL CheckUiAndXmlBeforeStartMedia();
	//�����ļ�ǰ���ȼ������ϵļ��·���Ƿ��������ļ��еļ��·��һ�£�����һ�£�����ʾ
	BOOL CheckMonitorPathBeforeSend();
	//�����ļ�ǰ���ȼ��ת��·���Ƿ���ڣ��������ڣ�����ʾ
	BOOL CheckSavePathBeforeSend();
	//���Ҫ���͵��ļ��б�
	BOOL GetFilesToSend();
	//�����ļ�·������չ��������Ŀ¼���ض����ļ�
	void GetFiles(CString strPath, CString strExtend);
	//�ļ����ͽ�����ɾ����ת���ѷ��͵��ļ�
	BOOL DelOrSaveAfterSended(CString strSendedFile);

	//�����ļ���·������ļ���
	inline CString GetFileNameFromPath(CString strFilePath);
private:
    //��ȡ�����ļ�����ʼ������Ԫ��ֵ
	void InitNodeValues();


	//���ݴ���ʱҪ��������� ����ȥ��
	BYTE* m_pSendData;
	//���ݴ���ʱҪ��������ݵĳ��� ����ȥ��
	UINT m_nSendData;

public:
	
	//˫���·У�飬�Ƿ��ѻ��У����
    E_STATUS_SEND  m_eSendFileStatus;
	E_STATUS_SEND  m_eSendDataStatus;
	E_STATUS_SEND  m_eSendMediaStatus;

	HANDLE m_hMonitorThread;//�ļ����ͼ���߳�
	HANDLE m_hSendFileThread;//�ļ������߳�
	HANDLE m_hSendDataThread;//���������߳�
    BOOL   m_bMonitorData;//�Ƿ��شӵ����������ն˷�����������
    HANDLE m_hSendMediaThread;//������ý���߳�

	CCriticalSection m_lockSendFileList;//�ļ������б��ٽ���
	CCriticalSection m_lockSendFileStatus;//�ļ�����״̬�ٽ���


    HANDLE m_hEventSendFile;//�����ļ��б��¼�����
    HANDLE m_hEventSendData;//���������¼�����
	HANDLE m_hEventSendMedia;//������ý���¼�����
	//HANDLE m_hEventMonitorFile;//�Ƿ�������·���е��ļ�

	//�����б���ý���б�

	

	inline CString AddDateToFileName(CString strFileName);

	CTransControlEx* m_pTransControl_File;
    CTransControlEx* m_pTransControl_Data;
	CTransControlEx* m_pTransControl_Media;

    char m_chData[1300];
    char m_chMedia[1300];
	UINT m_uDataLen;
	UINT m_uMediaLen;



	//�����߼�

	
	inline BOOL ValidatePermissions(BOOL bInitDlg, CString strModuleName);

	BOOL InitServ_File();//Init�ӿ�
	BOOL InitServ_Data();//Init�ӿ�
	BOOL InitServ_Media();//Init�ӿ�
	void ReStartAllServ();
	BOOL m_bInitDlg;//�����Ƿ��ڴ��ڳ�ʼ��������(�û������������ʱ��Ȩ����֤ʧ��ʱ���Ƿ񵯿���ʾ)

	void InitLogCtrl();
	BOOL InitMultiWorkThread();
	BOOL CheckSaveAndMonitorPath(CString strSaveAsPath, CString strMonitorPath, BOOL bSaveAs);

	//�������ʱ: ��ȡ����Ԫ�أ����кϷ��Լ�飬д�������ļ�
	BOOL ValidateParam_FileModule();
	BOOL ValidateParam_DataModule();
	BOOL ValidateParam_MediaModule();
	BOOL ValidateParam_EndModule();
	//�������ʱ����ȡ�����ļ���д���棬�Ϸ��Լ�顣
	BOOL InitParam_File();
	BOOL InitParam_Data();
	BOOL InitParam_Media();
	//CreateSocket�ӿ�
	BOOL InitParamServ_End(BOOL bStartServ, E_MODULE_TYPE eModule=ENU_MODULE_NONE);
	BOOL Validate_IP(CString strIp, CString strModuleName);
	BOOL Validate_Port(CString strPort, CString strModuleName, CString strPortName);

	BOOL StopServ_FileByUser();//����ֵ:�������Ƿ�������ļ�����ֹͣ�����Ƿ�
	BOOL StopServ_DataByUser();//����ֵ:�������Ƿ���������ݷ���ֹͣ�����Ƿ�
	BOOL StopServ_MediaByUser();//����ֵ:�������Ƿ��������ý�����ֹͣ�����Ƿ�

	CString GetLog(ENUM_LOG_TYPE enuLogType, CString strErrinfo);

	//10-29 add begin
	BOOL m_bTest;
	//10-29 add end
	//�����ļ�ǰ�����ļ����м��(3��):
	//1. ����ļ��Ƿ����          ������( �Ӽ����б���ɾ��, return 1) ������ת�벽��2
	//2. ����ļ��Ƿ��ڸ���״̬  ���ڸ���״̬(�����б��в�ɾ����return 2) ���ڸ���״̬��ת�벽��3
	//3. ��ȫ���                  ����ȫ(ɾ����ת�棬Ȼ��Ӽ����б���ɾ��,return 3)  ��ȫreturn 0  
	int CheckFileBeforeSend(CString strFileToSend);
	//11-07 add begin
	//ֹͣ�ļ�����ʱ��������ɾ����ת��
	BOOL m_bNoDelStopFileServ;
	//11-07 add end
	BOOL m_bSendFileUIChanged;
	//�ն���Ϣ
	CString m_strEndLocalPort;
	CString m_strEndAimIp;
	CString m_strEndAimPort;
	CString m_strEndIsVerfReturn;
	BOOL m_bEndUIChanged;
	BOOL InitSafeCheck();
	int SafeCheckBeforeSend(CString strFilePath);
	void GetRSingSoftPath();
	CString m_strRSingSoftPath;

	//add lgp 12-18 begin
	CString m_strFileSending;
	//add lgp 12-18 end

	//add lgp 2014-02-27 begin
	//��ر���2����Ŀ¼  2-20��������\\���������.doc  �ġ�	���·��
	CString m_strChildDirectory;
	void FindDirectory(CString strMonitorPath_File, CString strType);
	//add lgp 2014-02-27 end
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATASEND_H__C04A1F95_EDEE_4CA7_8580_7B3DCD6A1A80__INCLUDED_)
