/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：OutLogMgr.h
* 摘    要：日志管理模块导出类
* 
* 创建者：李国平
* 创建时间：2013/9/14
*/
#pragma once


#ifdef OutLogMgr_EXPORTS
#define OutLogMgr_API  __declspec(dllexport)
#else
#define OutLogMgr_API __declspec(dllimport)
#endif


	
enum   ENUM_LOG_TYPE
{
	Success=1,    //成功日志
		Failure,    //失败日志
		General,    //普通日志
};



//初始密码
#define PASSWORD_FILE _T("123")

class OutLogMgr_API COutLogMgr
{
	//日志类型





private:
	COutLogMgr(void);
public:
	~COutLogMgr(void);
	
	//获得单例对象指针
	static COutLogMgr* GetInstance();

	//写日志
	int LogWriter(/*char* strLogRet*/ENUM_LOG_TYPE enuLogType, char* strErrinfo);

	//显示日志管理对话框
    void DisplayLog();

	//获取日志文件内容
    //BOOL DisplayLogContent(CStringList& strLstLogContent, CString strLogFile);

    
//private:

	//文件加密(加密过程加入验证密码)
	BOOL ecfile(LPCTSTR fpath);

	//文件解密(解密过程进行密码验证)
	BOOL dcfile(LPCTSTR fpath);

	//利用加密算法，获得新密码
    __int64 epass();

	__int64 m_password;

	//获得日志文件夹路径
	CString GetLogPath();

};





