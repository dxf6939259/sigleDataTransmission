/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：OutLogIn.h
* 摘    要：单机身份验证模块导出类
* 
* 创建者：李国平
* 创建时间：2013/9/14
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

#define  SOFT_NAME_XML_SEND      _T("单 向 数 据 发 送 服 务")
#define  SOFT_NAME_XML_RECV      _T("单 向 数 据 接 收 服 务")


class OutLogIn_API COutLogIn  : public CObject  
{
private:
	COutLogIn(void);
public:
	 virtual ~COutLogIn(void);

public:
	//单例操作类
	static COutLogIn* GetInstance();
	
	//弹出登录对话框，验证用户是否有权限登录
	BOOL LogIn(CString strSfotName , CString &strUser);
	
	//验证传入的用户名和密码是否有权限登录
	//BOOL LogIn(CString strUser, CString strPWD);//此接口有异议
	BOOL ValidateUser(CString strUser, CString strPWD);
	
	//验证用户是否有权限操作此模块
	BOOL ValidatePermissions(CString strUser, CString strModule);

	//显示用户管理界面
	void UserManagement(CString strUser);

	//验证用户是否有权限使用此软件
	BOOL VerifySoftAuthority(CString strUserName, CString strSoftName);


private:
	//已登录成功的用户名
	CStringList m_strLstUserName;


};

#endif // !defined(AFX_OUTLOGIN_H__20A5EAB4_A58B_4ED1_BE01_A1D5366D5500__INCLUDED_)





















