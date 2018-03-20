/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DlgDataReceive.h
* ժ    Ҫ�����ݽ��ս���
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
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
  //�������
  char m_chSoftName[50];
  //��������
  //1���ļ����� 2�����ݽ��գ�3����ý����� 4������
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

	//��ʼ������
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

	//���ע����Ϣ��Ӧ����
	afx_msg HRESULT OnSoftReg(WPARAM wpa, LPARAM lpa);
	//���ע����Ϣ��Ӧ����
	afx_msg HRESULT OnSoftUnReg(WPARAM wpa, LPARAM lpa);

	afx_msg HRESULT SpeedSending(WPARAM wpa, LPARAM lpa);

	afx_msg HRESULT ShowRecvAnotherFile(WPARAM wpa, LPARAM lpa);
	//�����ļ���·������ļ���
	inline CString GetFileNameFromPath(CString strFilePath);

	//��ȡ�����ļ�����ʼ������Ԫ��ֵ
	void InitNodeValues();

	//���ն���
public:
	CTransControlEx* m_pTransControlEx_File;
private:
	CTransControlEx* m_pTransControlEx_Data;
	CTransControlEx* m_pTransControlEx_Media;

public:
	//��ע��ʹ�ô�����ĵ����������ն�
	CStringList m_lstSoftReg_File;
	CStringList m_lstSoftReg_Data;
	CStringList m_lstSoftReg_Media;

	void SoftReg(CStringList& strLstSoft, CString strSoftInfo);
	void SoftUnReg(CStringList& strLstSoft, CString strSoftInfo);


	inline BOOL ValidatePermissions(BOOL bInitDlg, CString strModuleName);
    
	//Init�ӿ�
	BOOL InitServ_File();//��ʼ���ļ����շ���
	BOOL InitServ_Data();//��ʼ�����ݽ��շ���
	BOOL InitServ_Media();//��ʼ����ý����շ���
	BOOL m_bInitDlg;//�����Ƿ��ڴ��ڳ�ʼ��������(�û������������ʱ��Ȩ����֤ʧ��ʱ���Ƿ񵯿���ʾ)

	void InitLogCtrl();
	BOOL StopServ_FileByUser();//����ֵ:�������Ƿ�������ļ�����ֹͣ�����Ƿ�
	BOOL StopServ_DataByUser();//����ֵ:�������Ƿ���������ݷ���ֹͣ�����Ƿ�
	BOOL StopServ_MediaByUser();//����ֵ:�������Ƿ��������ý�����ֹͣ�����Ƿ�

	//�����ļ����շ���ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
    BOOL CheckUiAndXmlBeforeStartFile();
	//�������ݽ��շ���ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
	BOOL CheckUiAndXmlBeforeStartData();
	//������ý����շ���ǰ���ȼ�����Ԫ���������ļ��е���ֵ�Ƿ���ͬ��������ͬ����ʾ
	BOOL CheckUiAndXmlBeforeStartMedia();
	//�����ļ�ǰ���ȼ������ϵĽ���·���Ƿ��������ļ��еĽ���·��һ�£�����һ�£�����ʾ
	BOOL CheckReceivePathBeforeSend();
	

	//�������ʱ: ��ȡ����Ԫ�أ����кϷ��Լ�飬д�������ļ�
	BOOL ValidateParam_FileModule();
	BOOL ValidateParam_DataModule();
	BOOL ValidateParam_MediaModule();
	BOOL Validate_Port(CString strPort, CString strModuleName, CString strPortName);
	//�������ʱ����ȡ�����ļ���д���棬�Ϸ��Լ�顣
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
