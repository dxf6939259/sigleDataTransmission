/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：OutLogMgr.cpp
* 摘    要：日志管理界面
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

*/

#include "StdAfx.h"
#include "Shlwapi.h"
#include "OutLogMgr.h"
#include "LogForm.h"



static COutLogMgr* m_pCOutLogMgr = NULL;



COutLogMgr::COutLogMgr(void)
{
   
   
}

COutLogMgr::~COutLogMgr(void)
{
	if(NULL != m_pCOutLogMgr)
	{
		delete m_pCOutLogMgr;
		m_pCOutLogMgr = NULL;
	}
}
/***************************************************************
函数名：COutLogMgr* GetInstance()

  功能描述：获得单例类对象
  
  参数：无
	
  全局变量：无
	  
  返回值: COutLogMgr*  单例对象指针
		
****************************************************************/
COutLogMgr* COutLogMgr::GetInstance()
{
	//static COutLogMgr logMgr;
	//return &logMgr;

	if(NULL == m_pCOutLogMgr)
	{
		m_pCOutLogMgr = new COutLogMgr();
	}
	return m_pCOutLogMgr;
	
}

/***************************************************************
函数名：int LogWriter(char* strLogRet, char* strErrinfo)

  功能描述：初始化日志文件列表树形控件
  
  参数：char* strLogRet  操作结果
        char* strErrinfo 操作信息
	
  全局变量：无
	  
  返回值: int  0-成功 非0-失败
		
****************************************************************/
int COutLogMgr::LogWriter(/*char* strLogRet*/ENUM_LOG_TYPE enuLogType, char* strErrinfo)
{
	TCHAR strLogRet[MAX_PATH];
	memset(strLogRet, 0, MAX_PATH);
	switch(enuLogType)
	{
	case Success:
		_tcscat(strLogRet, _T("[Success]"));
		break;
	case Failure:
		_tcscat(strLogRet, _T("[Failure]"));
		break;
    case General:
		_tcscat(strLogRet, _T("[General]"));
		break;
	}
	//获取日志文件夹路径
    CString strLogFloder = GetLogPath();

	//获取当天日志文件的路径
	CString strLogName,strLogPath;
	CTime tTime = CTime::GetCurrentTime();
	strLogName = tTime.Format("%Y%m%d");
	strLogPath.Format("%s\%s",strLogFloder,strLogName);
	strLogPath +=".log";

	BOOL bRet;
	if(::GetFileAttributes(strLogPath)!=-1)
	{
		bRet = dcfile(strLogPath);
	}
	
	if(!bRet)
		return 1;

	CFile oFile;
	if (oFile.Open(strLogPath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
	{
		oFile.Seek(0,CFile::end);

		//整理日志内容格式
		CTime time = CTime::GetCurrentTime();
		CString strTime;
		strTime.Format(_T("%d-%d-%d %d:%d:%d"),
			time.GetYear(),time.GetMonth(),
			time.GetDay(),time.GetHour(),
			time.GetMinute(),time.GetSecond()); 
		
		TCHAR strMsg[MAX_PATH];
		memset(strMsg, 0, MAX_PATH);
		if(NULL == strstr(strErrinfo, _T("\r\n")))
		{
	        sprintf(strMsg, _T("%s %s  %s\r\n"),strLogRet,strTime,strErrinfo );
        }
		else
		{
			sprintf(strMsg, _T("%s %s  %s"),strLogRet,strTime,strErrinfo );
        }

        //写日志
		oFile.Write(strMsg,strlen(strMsg));
		oFile.Close();
		//加密
		BOOL bRet = ecfile(strLogPath);
		
	}
	else
	{
		return 2;
	}
	return 0;
}



/***************************************************************
函数名：void DisplayLog()

  功能描述：显示日志管理对话框
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void COutLogMgr::DisplayLog()
{
	//资源保护
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CLogForm dlgLogMgr; 
	dlgLogMgr.DoModal();
}

/***************************************************************
函数名：__int64 epass()

  功能描述：加密算法获得加密密码
  
  参数：无
	
  全局变量：无
	  
  返回值: __int64  加密密码
		
****************************************************************/
__int64 COutLogMgr::epass()
{
	DWORD plen;
	char *ppass;
	__int64 mc= 8757735233305;

	
	ppass = PASSWORD_FILE;
	
	plen = strlen(ppass);
	
	for(int i=0; i<(int)plen; i++)
	{
		mc ^= ppass[i]|128;
	}
	return mc;
}	

/***************************************************************
函数名：CString GetCurrentFloder()

  功能描述：获得日志文件夹路径
  
  参数：无
	
  全局变量：无
	  
  返回值: __int64  加密密码
		
****************************************************************/
CString COutLogMgr::GetLogPath()
{
	TCHAR szCurrentPath[MAX_PATH];
	GetModuleFileName(NULL,szCurrentPath,MAX_PATH);
	while (TRUE)
	{
		int iLen = strlen(szCurrentPath);
		if ('\\' == szCurrentPath[iLen-1])
		{
			break;
		}
		else
		{
			szCurrentPath[iLen-1] = 0;
		}	
	}

	//获取日志文件夹
	char lpcPath[MAX_PATH+1]={0};
	memcpy(lpcPath,szCurrentPath,MAX_PATH);
	memcpy(lpcPath+strlen(szCurrentPath),"Log\\",strlen("Log\\"));
	
	//若日志文件夹不存在，则创建
	if(!PathFileExists(lpcPath))
	{
		if (!CreateDirectory(lpcPath,NULL))
		{
			return 1;
		}
	}

	CString strCurrFloder;
	strCurrFloder.Format(_T("%s"), lpcPath);

	return strCurrFloder;
}

/***************************************************************
函数名：BOOL DisplayLogContent(CStringList& strLstLogContent, CString strLogFileName)

  功能描述：读取日志文件内容
  
  参数：CStringList& strLstLogContent  日志文件内容列表
        CString strLogFileName         日志文件名
	
  全局变量：无
	  
  返回值: BOOL  TRUE--读日志成功  FALSE--读日志失败
		
****************************************************************/
BOOL COutLogMgr::DisplayLogContent(CStringList& strLstLogContent, CString strLogFileName)
{
	//清空文件内容列表
	strLstLogContent.RemoveAll();

    //获取日志文件路径
    CString strCurrFloder = GetLogPath();
	CString strLogFile;
	strLogFile.Format(_T("%s\%s"),strCurrFloder,strLogFileName);

	//解密
	BOOL bRet = dcfile(strLogFile);
	if(!bRet)
		return FALSE;

	//读日志
	CStdioFile logFile;
	bRet = logFile.Open(strLogFile, CFile::modeRead|CFile::typeText);
    if(!bRet)
		return FALSE;

	CString strLogContent;
	while(logFile.ReadString(strLogContent))
	{
	  strLstLogContent.AddHead(strLogContent);
	}
    logFile.Close();

	//加密
	bRet = ecfile(strLogFile);

    return TRUE;

}

/***************************************************************
函数名：BOOL ecfile(LPCTSTR fpath)

  功能描述：文件加密
  
  参数：LPCTSTR fpath  日志文件路径
    
  全局变量：无
	  
  返回值: BOOL  TRUE--加密成功  FALSE--加密失败
		
****************************************************************/
BOOL COutLogMgr::ecfile(LPCTSTR fpath)
{
    char *data;
	CFile *file;
	DWORD flen;

	__int64 password = epass();

	file = new CFile;
	if ( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}

	flen = file->GetLength();
	data = new char[(int)flen];

	file->SeekToBegin();
	file->Read(data, flen);

	for(int i=0; i<(int)flen; i++)
	{
		data[i] ^= password;
		data[i] ^= flen;
	}

	file->SeekToBegin();
	file->Write(data, flen);
	delete[] data;

	//添加密码验证信息
	char cpass[5] = "love";
	for(int j=0; j<5; j++)
	{
		cpass[j] ^= password;
	}
	file->SeekToEnd();
	file->Write(&cpass, 5);
	file->Close();
	delete file;

	return TRUE;
}

/***************************************************************
函数名：BOOL dcfile(LPCTSTR fpath)

  功能描述：文件解密
  
  参数：LPCTSTR fpath  日志文件路径
    
  全局变量：无
	  
  返回值: BOOL  TRUE--解密成功  FALSE--解密失败
		
****************************************************************/
BOOL COutLogMgr::dcfile(LPCTSTR fpath)
{
	char *data;
	CFile *file;
	DWORD flen;
	char love[5];
	
	file = new CFile;
	if( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}
	
	flen = file->GetLength();
	data = new char[(int)flen];
	
	//检验密码是不是正确
	file->Seek(-5, CFile::end);
	file->Read(&love, 5);
	
	__int64 password = epass();
	
	for(int i=0; i<5; i++)
	{
		love[i] ^= password;
	}
	
	if(strcmp(love, "love")!=0)
	{
		return FALSE;
	}
	
	//解密
	file->SeekToBegin();
	file->Read(data, flen);
	
	for(int j=0; j<(int)flen; j++)
	{
		data[j] ^= password;
		data[j] ^= (flen-5);
	}
	file->SeekToBegin();
	file->Write(data, flen);
	file->SetLength(flen-5);
	file->Close();
	
	delete[] data;
	delete file;
	return TRUE;
}
