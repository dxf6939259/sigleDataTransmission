/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�RegistationSocket.h
* ժ    Ҫ��Ϊĳһ���ͻ�ȡһ�������
* 
* �����ߣ�dgh
* ����ʱ�䣺2013/9/14
*/

#if !defined(AFX_REGISTATIONSOCKET_H__860C8465_2DAA_4F64_91D7_8715E4A37510__INCLUDED_)
#define AFX_REGISTATIONSOCKET_H__860C8465_2DAA_4F64_91D7_8715E4A37510__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegistationSocket.h : header file
//

class CTransControlEx;
#include <map>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CRegistationSocket command target

class CRegistationSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	static CRegistationSocket* GetInstance();
	virtual ~CRegistationSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegistationSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CRegistationSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation

	//���뷢��ע���
	void ApplyRegisNo(CTransControlEx* pcontrol);	
	
	//���ͽ���ע��ע���
	void LogRegisNo(CTransControlEx* pcontrol);

	map<int,CTransControlEx*> m_mapControl;

protected:
	//У���ʶ
	map<int,int> m_mapCheckFlag;
	CTransControlEx *m_pControl;

private:
	CRegistationSocket();
	CTreatyPack trPack;
	BothProtocol bpro;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTATIONSOCKET_H__860C8465_2DAA_4F64_91D7_8715E4A37510__INCLUDED_)
