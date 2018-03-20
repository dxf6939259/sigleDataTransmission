/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�OutLogIn.h
* ժ    Ҫ�����������֤ģ�鵼����
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/

#if !defined(AFX_OUTLOGIN_H__20A5EAB4_A58B_4ED1_BE01_A1D5366D5500__INCLUDED_)
#define AFX_OUTLOGIN_H__20A5EAB4_A58B_4ED1_BE01_A1D5366D5500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef OutLogIn_EXPORTS
#define OutLogIn_API  __declspec(dllexport)
#else
#define OutLogIn_API __declspec(dllimport)
#endif

#define  SOFT_NAME_XML_SEND      _T("�� �� �� �� �� �� �� ��")
#define  SOFT_NAME_XML_RECV      _T("�� �� �� �� �� �� �� ��")


class OutLogIn_API COutLogIn  : public CObject  
{
private:
	COutLogIn(void);
public:
	 virtual ~COutLogIn(void);

public:
	//����������
	static COutLogIn* GetInstance();
	
	//������¼�Ի�����֤�û��Ƿ���Ȩ�޵�¼
	BOOL LogIn(CString strSfotName , CString &strUser);
	
	//��֤������û����������Ƿ���Ȩ�޵�¼
	//BOOL LogIn(CString strUser, CString strPWD);//�˽ӿ�������
	BOOL ValidateUser(CString strUser, CString strPWD);
	
	//��֤�û��Ƿ���Ȩ�޲�����ģ��
	BOOL ValidatePermissions(CString strUser, CString strModule);

	//��ʾ�û��������
	void UserManagement(CString strUser);

	//��֤�û��Ƿ���Ȩ��ʹ�ô����
	BOOL VerifySoftAuthority(CString strUserName, CString strSoftName);


private:
	//�ѵ�¼�ɹ����û���
	CStringList m_strLstUserName;


};

#endif // !defined(AFX_OUTLOGIN_H__20A5EAB4_A58B_4ED1_BE01_A1D5366D5500__INCLUDED_)





















