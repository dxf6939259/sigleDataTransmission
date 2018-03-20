/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�OutLogMgr.h
* ժ    Ҫ����־����ģ�鵼����
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/
#pragma once


#ifdef OutLogMgr_EXPORTS
#define OutLogMgr_API  __declspec(dllexport)
#else
#define OutLogMgr_API __declspec(dllimport)
#endif


	
enum   ENUM_LOG_TYPE
{
	Success=1,    //�ɹ���־
		Failure,    //ʧ����־
		General,    //��ͨ��־
};



//��ʼ����
#define PASSWORD_FILE _T("123")

class OutLogMgr_API COutLogMgr
{
	//��־����





private:
	COutLogMgr(void);
public:
	~COutLogMgr(void);
	
	//��õ�������ָ��
	static COutLogMgr* GetInstance();

	//д��־
	int LogWriter(/*char* strLogRet*/ENUM_LOG_TYPE enuLogType, char* strErrinfo);

	//��ʾ��־����Ի���
    void DisplayLog();

	//��ȡ��־�ļ�����
    //BOOL DisplayLogContent(CStringList& strLstLogContent, CString strLogFile);

    
//private:

	//�ļ�����(���ܹ��̼�����֤����)
	BOOL ecfile(LPCTSTR fpath);

	//�ļ�����(���ܹ��̽���������֤)
	BOOL dcfile(LPCTSTR fpath);

	//���ü����㷨�����������
    __int64 epass();

	__int64 m_password;

	//�����־�ļ���·��
	CString GetLogPath();

};





