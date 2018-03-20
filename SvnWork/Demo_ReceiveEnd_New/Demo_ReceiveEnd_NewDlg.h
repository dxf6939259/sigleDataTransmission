// Demo_ReceiveEnd_NewDlg.h : header file
//

#if !defined(AFX_DEMO_RECEIVEEND_NEWDLG_H__976D48CD_2BF4_4A4F_A408_B89A9ED4FB4E__INCLUDED_)
#define AFX_DEMO_RECEIVEEND_NEWDLG_H__976D48CD_2BF4_4A4F_A408_B89A9ED4FB4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemo_ReceiveEnd_NewDlg dialog
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


class CDemo_ReceiveEnd_NewDlg : public CDialog
{
// Construction
public:
	CDemo_ReceiveEnd_NewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemo_ReceiveEnd_NewDlg)
	enum { IDD = IDD_DEMO_RECEIVEEND_NEW_DIALOG };
	CListCtrl	m_lstMedia;
	CListCtrl	m_lstData;
	CListCtrl	m_lstFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo_ReceiveEnd_NewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemo_ReceiveEnd_NewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButRegFile();
	afx_msg void OnBtnUnRegFile();
	afx_msg void OnBtnClearFile();
	afx_msg void OnButRegData();
	afx_msg void OnBtnUnRegData();
	afx_msg void OnBtnClearData();
	afx_msg void OnButRegMedia();
	afx_msg void OnBtnUnRegMedia();
	afx_msg void OnBtnClearMedia();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//ע�ᣬע������
	BOOL Reg_UnReg(BOOL bReg, StSoftInfo softInfo);
	//���մӽ��ն�����������ļ�������Ϣ
	afx_msg HRESULT OnRecvFile(WPARAM wpa, LPARAM lpa);
	//���մӽ��ն�������������ݽ�����Ϣ
	afx_msg HRESULT OnRecvData(WPARAM wpa, LPARAM lpa);
	//���մӽ��ն������������ý�������Ϣ
	afx_msg HRESULT OnRecvMedia(WPARAM wpa, LPARAM lpa);
    //���ܲ��ԣ�ÿ���дӽ��ն�������������ݽ�����Ϣ������
	int m_nCountDataRecv;
	//���ܲ��ԵĿ�ʼʱ���
	DWORD m_dTestTimeBegin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_RECEIVEEND_NEWDLG_H__976D48CD_2BF4_4A4F_A408_B89A9ED4FB4E__INCLUDED_)
