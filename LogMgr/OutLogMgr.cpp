/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�OutLogMgr.cpp
* ժ    Ҫ����־�������
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

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
��������COutLogMgr* GetInstance()

  ������������õ��������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: COutLogMgr*  ��������ָ��
		
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
��������int LogWriter(char* strLogRet, char* strErrinfo)

  ������������ʼ����־�ļ��б����οؼ�
  
  ������char* strLogRet  �������
        char* strErrinfo ������Ϣ
	
  ȫ�ֱ�������
	  
  ����ֵ: int  0-�ɹ� ��0-ʧ��
		
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
	//��ȡ��־�ļ���·��
    CString strLogFloder = GetLogPath();

	//��ȡ������־�ļ���·��
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

		//������־���ݸ�ʽ
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

        //д��־
		oFile.Write(strMsg,strlen(strMsg));
		oFile.Close();
		//����
		BOOL bRet = ecfile(strLogPath);
		
	}
	else
	{
		return 2;
	}
	return 0;
}



/***************************************************************
��������void DisplayLog()

  ������������ʾ��־����Ի���
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void COutLogMgr::DisplayLog()
{
	//��Դ����
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CLogForm dlgLogMgr; 
	dlgLogMgr.DoModal();
}

/***************************************************************
��������__int64 epass()

  ���������������㷨��ü�������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: __int64  ��������
		
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
��������CString GetCurrentFloder()

  ���������������־�ļ���·��
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: __int64  ��������
		
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

	//��ȡ��־�ļ���
	char lpcPath[MAX_PATH+1]={0};
	memcpy(lpcPath,szCurrentPath,MAX_PATH);
	memcpy(lpcPath+strlen(szCurrentPath),"Log\\",strlen("Log\\"));
	
	//����־�ļ��в����ڣ��򴴽�
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
��������BOOL DisplayLogContent(CStringList& strLstLogContent, CString strLogFileName)

  ������������ȡ��־�ļ�����
  
  ������CStringList& strLstLogContent  ��־�ļ������б�
        CString strLogFileName         ��־�ļ���
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL  TRUE--����־�ɹ�  FALSE--����־ʧ��
		
****************************************************************/
BOOL COutLogMgr::DisplayLogContent(CStringList& strLstLogContent, CString strLogFileName)
{
	//����ļ������б�
	strLstLogContent.RemoveAll();

    //��ȡ��־�ļ�·��
    CString strCurrFloder = GetLogPath();
	CString strLogFile;
	strLogFile.Format(_T("%s\%s"),strCurrFloder,strLogFileName);

	//����
	BOOL bRet = dcfile(strLogFile);
	if(!bRet)
		return FALSE;

	//����־
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

	//����
	bRet = ecfile(strLogFile);

    return TRUE;

}

/***************************************************************
��������BOOL ecfile(LPCTSTR fpath)

  �����������ļ�����
  
  ������LPCTSTR fpath  ��־�ļ�·��
    
  ȫ�ֱ�������
	  
  ����ֵ: BOOL  TRUE--���ܳɹ�  FALSE--����ʧ��
		
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

	//���������֤��Ϣ
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
��������BOOL dcfile(LPCTSTR fpath)

  �����������ļ�����
  
  ������LPCTSTR fpath  ��־�ļ�·��
    
  ȫ�ֱ�������
	  
  ����ֵ: BOOL  TRUE--���ܳɹ�  FALSE--����ʧ��
		
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
	
	//���������ǲ�����ȷ
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
	
	//����
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
