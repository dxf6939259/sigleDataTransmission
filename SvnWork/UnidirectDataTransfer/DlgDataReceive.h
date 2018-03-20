/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DlgDataReceive.h
* 摘    要：数据接收界面
* 
* 创建者：李国平
* 创建时间：2013/9/14
*/
#if !defined(AFX_DLGDATARECEIVE_H__BF470A09_2602_4635_8201_358A08287B8F__INCLUDED_)
#define AFX_DLGDATARECEIVE_H__BF470A09_2602_4635_8201_358A08287B8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDataReceive.h : header file
//
#include "OutLogIn.h"
#include "OperXml.h"
#include "OutLogMgr.h"
#include "TransControlEx.h"
#include <afxtempl.h>


#define  WM_RECV_ANOTHER_FILE  WM_USER+100

#define PORT_MAX_USE 65535
#define PORT_MIN_USE 8080
  
/////////////////////////////////////////////////////////////////////////////
// CDlgDataReceive dialog
typedef struct _StSoftInfo
{
  //软件名称
  char m_chSoftName[50];
  //接收类型
  //1：文件接收 2：数据接收，3：流媒体接收 4：其他
  int m_nRecType;
  _StSoftInfo()
  {
	  memset(m_chSoftName, '\0', sizeof(m_chSoftName));
	  m_nRecType = -1;
  }
}StSoftInfo;


class CDlgDataReceive : public CBCGPDialog
{
// Construction
public:
	CDlgDataReceive(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDataReceive)
	enum { IDD = IDD_DLG_DATA_RECEIVE };
	CBCGPListCtrl	m_lstLogMedia;
	CBCGPListCtrl	m_lstLogFile;
	CBCGPListCtrl	m_lstLogData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDataReceive)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDataReceive)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickLstLogFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickLstLogData(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnRclickLstLogMedia(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//初始化界面
	void Init();
public:
	afx_msg void OnStopFile();
private:
	afx_msg void OnClearRecvFileLog();
	afx_msg void OnClearRecvDataLog();
	afx_msg void OnClearRecvMediaLog();
	afx_msg void OnFileSavePath();
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

	//软件注册消息响应函数
	afx_msg HRESULT OnSoftReg(WPARAM wpa, LPARAM lpa);
	//软件注销消息响应函数
	afx_msg HRESULT OnSoftUnReg(WPARAM wpa, LPARAM lpa);

	afx_msg HRESULT SpeedSending(WPARAM wpa, LPARAM lpa);

	afx_msg HRESULT ShowRecvAnotherFile(WPARAM wpa, LPARAM lpa);
	//根据文件的路径获得文件名
	inline CString GetFileNameFromPath(CString strFilePath);

	//读取配置文件，初始化界面元素值
	void InitNodeValues();

	//接收对象
public:
	CTransControlEx* m_pTransControlEx_File;
private:
	CTransControlEx* m_pTransControlEx_Data;
	CTransControlEx* m_pTransControlEx_Media;

public:
	//已注册使用此软件的第三方接收终端
	CStringList m_lstSoftReg_File;
	CStringList m_lstSoftReg_Data;
	CStringList m_lstSoftReg_Media;

	void SoftReg(CStringList& strLstSoft, CString strSoftInfo);
	void SoftUnReg(CStringList& strLstSoft, CString strSoftInfo);


	inline BOOL ValidatePermissions(BOOL bInitDlg, CString strModuleName);
    
	//Init接口
	BOOL InitServ_File();//初始化文件接收服务
	BOOL InitServ_Data();//初始化数据接收服务
	BOOL InitServ_Media();//初始化流媒体接收服务
	BOOL m_bInitDlg;//程序是否处于窗口初始化过程中(用户启动三类服务时的权限验证失败时，是否弹框提示)

	void InitLogCtrl();
	BOOL StopServ_FileByUser();//返回值:函数中是否进行了文件服务停止操作是否
	BOOL StopServ_DataByUser();//返回值:函数中是否进行了数据服务停止操作是否
	BOOL StopServ_MediaByUser();//返回值:函数中是否进行了流媒体服务停止操作是否

	//启动文件接收服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
    BOOL CheckUiAndXmlBeforeStartFile();
	//启动数据接收服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
	BOOL CheckUiAndXmlBeforeStartData();
	//启动流媒体接收服务前，先检查界面元素与配置文件中的数值是否相同，若不相同则提示
	BOOL CheckUiAndXmlBeforeStartMedia();
	//接收文件前，先检查界面上的接收路径是否与配置文件中的接收路径一致，若不一致，则提示
	BOOL CheckReceivePathBeforeSend();
	

	//保存参数时: 读取界面元素，进行合法性检查，写入配置文件
	BOOL ValidateParam_FileModule();
	BOOL ValidateParam_DataModule();
	BOOL ValidateParam_MediaModule();
	BOOL Validate_Port(CString strPort, CString strModuleName, CString strPortName);
	//软件加载时，读取配置文件，写界面，合法性检查。
	BOOL InitParam_File();
	BOOL InitParam_Data();
	BOOL InitParam_Media();
	BOOL Validate_IP(CString strIp, CString strModuleName);
	BOOL Port_Is_Use(int nPort, CString strModuleName);

	CString GetLog(ENUM_LOG_TYPE enuLogType, CString strErrinfo);

	BOOL InitSafeCheck();
	BOOL SafeCheck(CString strFile);
	void GetRSingSoftPath();
	CString m_strRSingSoftPath;
	//add lgp 11-28 begin
    CString m_strData;
	CString m_strMedia;
	//add lgp 1-28 end
    //add lgp 12-18 begin
	CString m_strFileRecving;
	//add lgp 12-18 end

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATARECEIVE_H__BF470A09_2602_4635_8201_358A08287B8F__INCLUDED_)
