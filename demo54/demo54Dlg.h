// demo54Dlg.h : header file
//

#if !defined(AFX_DEMO54DLG_H__5322D0EF_B383_496E_8F98_ADC5EE8D886C__INCLUDED_)
#define AFX_DEMO54DLG_H__5322D0EF_B383_496E_8F98_ADC5EE8D886C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TreatyPack.h"
#include "ProcessApply.h"
#include "..\TransControl\UserDefine.h"
// ˫��У��Э��
// typedef struct _BOTHWAY_PROTOCOL_
// {
// 	USHORT frameHead; //֡ͷ
// 	//Ŀ�Ķ�IP��ַ��
// 	BYTE desAddr[4];
// 	//Ŀ�Ķ˶˿�
// 	USHORT desPort;
// 	//��������
// 	BYTE transType;
// 	//��������
// 	BYTE opType;
// 	//У���ʶ
// 	int checkFlag;
// 	//У����
// 	byte checkCode;
// 	
// 	_BOTHWAY_PROTOCOL_()
// 	{
// 		frameHead = 0xee18;
// 		memset(desAddr,0x00,4);
// 		desPort=0;
// 		transType=0;
// 		opType=0;
// 		checkFlag=0;
// 		checkCode=0;
// 	}
// 	void Reset()
// 	{
// 		frameHead = 0xee18;
// 		memset(desAddr,0x00,4);
// 		desPort=0;
// 		transType=0;
// 		opType=0;
// 		checkFlag=0;
// 		checkCode=0;
// 	}
// 	
// 	byte GetCheckCode()
// 	{
// 		DWORD dwCheckSum=0;
// 		dwCheckSum+=desAddr[0];
// 		dwCheckSum+=desAddr[1];
// 		dwCheckSum+=desAddr[2];
// 		dwCheckSum+=desAddr[3];
// 		dwCheckSum+=desPort;
// 		dwCheckSum+=transType;
// 		dwCheckSum+=opType;
// 		dwCheckSum+=checkFlag;
// 		
// 		byte ckCode = dwCheckSum & 0x00000000000000ff;
// 		return ckCode;
// 	}
// }BothProtocol;

/////////////////////////////////////////////////////////////////////////////
// CDemo54Dlg dialog

class CDemo54Dlg : public CDialog
{
// Construction
public:
	CDemo54Dlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDemo54Dlg();

// Dialog Data
	//{{AFX_DATA(CDemo54Dlg)
	enum { IDD = IDD_DEMO54_DIALOG };
	CListCtrl	m_lstLog;
	UINT	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo54Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemo54Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartServer();
	afx_msg void OnStartserver();
	afx_msg void OnClosesoc();
	afx_msg void OnBtnclear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bStartSever;
public:
	CProcessApply m_proApply;
	SOCKET m_udp_socket;
	SOCKET m_udp_send;
	BOOL m_bStop;

	SOCKADDR_IN m_sendAddr;

	BOOL m_doubleCheck;

	//���ݻ�����ָ��
	byte* m_pBufData;
	long m_readPos; //��������λ��
	long m_writePos; //д������λ��
	LONG m_curThreadCount; //��ǰ�߳���


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO54DLG_H__5322D0EF_B383_496E_8F98_ADC5EE8D886C__INCLUDED_)
