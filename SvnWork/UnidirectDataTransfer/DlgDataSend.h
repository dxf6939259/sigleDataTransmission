/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgDataSend.h
* 摘    要：数据发送界面
* 
* 创建者：李国平
* 创建时间：2013/9/14
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
	//发送文件消息响应函数
	afx_msg HRESULT OnSendFile_Msg(WPARAM wpa, LPARAM lpa);
	//发送数据消息响应函数
	afx_msg HRESULT OnSendData_Msg(WPARAM wpa, LPARAM lpa);
	//发送流媒体数据消息响应函数
	afx_msg HRESULT OnSendMedia_Msg(WPARAM wpa, LPARAM lpa);
    //利用第三方发送终端测试软件性能
	afx_msg HRESULT OnSendData_TestXN(WPARAM wpa, LPARAM lpa);
	//初始化界面
	void Init();

	//设置数据传输时要传输的数据和长度 考虑去掉
	//void SetSendData(BYTE* byData, UINT bLen);

	//设置要进行流媒体传输的流媒体文件的路径
	void SetSendMediaPath(CString strMediaPath);

public:
	//要发送的文件列表
	CStringList m_strLstFiles;
    //从第三方发送终端传过来的数据列表
	//CList<CBufZoneManager,CBufZoneManager&> m_byteLstDatas;
	CByteArray m_arrDatas;
    //CList<CBufZoneManager,CBufZoneManager&> m_byteLstMedias;
	CByteArray m_arrMedias;
    HANDLE m_hSendDataTestXnThread;


	
	LONG m_signalCt;
	BOOL m_bInOneSecond;//测试性能过程中，收到的数据是否是第一秒内的数据
	int  m_nDataCountTestXn;//测试性能过程中，1秒中内收到的数据帧数
	BOOL m_bDataInTestXn;//接收到的数据是否是在进行性能测试
	CCriticalSection m_lockSendDataStatus;//数据发送状态临界区     控制m_byteLstDatas
	CCriticalSection m_lockSendMediaStatus;//流媒体发送状态临界区  

	
	//启动文件传输服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
    BOOL CheckUiAndXmlBeforeStartFile();
	//启动数据传输服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
	BOOL CheckUiAndXmlBeforeStartData();
	//启动流媒体传输服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
	BOOL CheckUiAndXmlBeforeStartMedia();
	//发送文件前，先检查界面上的监控路径是否与配置文件中的监控路径一致，若不一致，则提示
	BOOL CheckMonitorPathBeforeSend();
	//发送文件前，先检查转存路径是否存在，若不存在，则提示
	BOOL CheckSavePathBeforeSend();
	//获得要发送的文件列表
	BOOL GetFilesToSend();
	//根据文件路径和扩展名搜索此目录下特定的文件
	void GetFiles(CString strPath, CString strExtend);
	//文件发送结束后，删除或转存已发送的文件
	BOOL DelOrSaveAfterSended(CString strSendedFile);

	//根据文件的路径获得文件名
	inline CString GetFileNameFromPath(CString strFilePath);
private:
    //读取配置文件，初始化界面元素值
	void InitNodeValues();


	//数据传输时要传输的数据 考虑去掉
	BYTE* m_pSendData;
	//数据传输时要传输的数据的长度 考虑去掉
	UINT m_nSendData;

public:
	
	//双向回路校验，是否已获得校验码
    E_STATUS_SEND  m_eSendFileStatus;
	E_STATUS_SEND  m_eSendDataStatus;
	E_STATUS_SEND  m_eSendMediaStatus;

	HANDLE m_hMonitorThread;//文件发送监控线程
	HANDLE m_hSendFileThread;//文件发送线程
	HANDLE m_hSendDataThread;//发送数据线程
    BOOL   m_bMonitorData;//是否监控从第三方调用终端发过来的数据
    HANDLE m_hSendMediaThread;//发送流媒体线程

	CCriticalSection m_lockSendFileList;//文件发送列表临界区
	CCriticalSection m_lockSendFileStatus;//文件发送状态临界区


    HANDLE m_hEventSendFile;//发送文件列表事件对象
    HANDLE m_hEventSendData;//发送数据事件对象
	HANDLE m_hEventSendMedia;//发送流媒体事件对象
	//HANDLE m_hEventMonitorFile;//是否检索监控路径中的文件

	//数据列表，流媒体列表

	

	inline CString AddDateToFileName(CString strFileName);

	CTransControlEx* m_pTransControl_File;
    CTransControlEx* m_pTransControl_Data;
	CTransControlEx* m_pTransControl_Media;

    char m_chData[1300];
    char m_chMedia[1300];
	UINT m_uDataLen;
	UINT m_uMediaLen;



	//界面逻辑

	
	inline BOOL ValidatePermissions(BOOL bInitDlg, CString strModuleName);

	BOOL InitServ_File();//Init接口
	BOOL InitServ_Data();//Init接口
	BOOL InitServ_Media();//Init接口
	void ReStartAllServ();
	BOOL m_bInitDlg;//程序是否处于窗口初始化过程中(用户启动三类服务时的权限验证失败时，是否弹框提示)

	void InitLogCtrl();
	BOOL InitMultiWorkThread();
	BOOL CheckSaveAndMonitorPath(CString strSaveAsPath, CString strMonitorPath, BOOL bSaveAs);

	//保存参数时: 读取界面元素，进行合法性检查，写入配置文件
	BOOL ValidateParam_FileModule();
	BOOL ValidateParam_DataModule();
	BOOL ValidateParam_MediaModule();
	BOOL ValidateParam_EndModule();
	//软件加载时，读取配置文件，写界面，合法性检查。
	BOOL InitParam_File();
	BOOL InitParam_Data();
	BOOL InitParam_Media();
	//CreateSocket接口
	BOOL InitParamServ_End(BOOL bStartServ, E_MODULE_TYPE eModule=ENU_MODULE_NONE);
	BOOL Validate_IP(CString strIp, CString strModuleName);
	BOOL Validate_Port(CString strPort, CString strModuleName, CString strPortName);

	BOOL StopServ_FileByUser();//返回值:函数中是否进行了文件服务停止操作是否
	BOOL StopServ_DataByUser();//返回值:函数中是否进行了数据服务停止操作是否
	BOOL StopServ_MediaByUser();//返回值:函数中是否进行了流媒体服务停止操作是否

	CString GetLog(ENUM_LOG_TYPE enuLogType, CString strErrinfo);

	//10-29 add begin
	BOOL m_bTest;
	//10-29 add end
	//发送文件前，对文件进行检查(3步):
	//1. 检查文件是否存在          不存在( 从检索列表中删除, return 1) 存在则转入步骤2
	//2. 检查文件是否处于复制状态  处于复制状态(检索列表中不删除，return 2) 不在复制状态则转入步骤3
	//3. 安全检查                  不安全(删除或转存，然后从检索列表中删除,return 3)  安全return 0  
	int CheckFileBeforeSend(CString strFileToSend);
	//11-07 add begin
	//停止文件传输时，不进行删除或转存
	BOOL m_bNoDelStopFileServ;
	//11-07 add end
	BOOL m_bSendFileUIChanged;
	//终端信息
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
	//监控遍历2层子目录  2-20测试问题\\单向传输程序.doc  四、	监控路径
	CString m_strChildDirectory;
	void FindDirectory(CString strMonitorPath_File, CString strType);
	//add lgp 2014-02-27 end
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATASEND_H__C04A1F95_EDEE_4CA7_8580_7B3DCD6A1A80__INCLUDED_)
