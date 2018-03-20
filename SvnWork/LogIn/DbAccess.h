/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DbAccess.h
* ժ    Ҫ������office access 2003���ݿ�
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/

#if !defined(AFX_OUTLOGIN_H__289A9BAF_D18E_4D80_A3B3_424202938B74__INCLUDED_)
#define AFX_OUTLOGIN_H__289A9BAF_D18E_4D80_A3B3_424202938B74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxdb.h"
#include "odbcinst.h"

#define  DB_GET_FIELD_VALUE_ERROR  -1
class  CDbAccess : public CObject  
{
public:
	CDbAccess();
	virtual ~CDbAccess();
	

public:

	//���ӵ�access���ݿ�
	BOOL ConnectDB(CString strDataSource);

	//�����ݼ�
	BOOL OpenQuery(CString strSelSql);

	//����ƶ����ݼ�ָ��
	BOOL MoveNext();

	//��������ֶ�����
	int GetIntValue(CString strItem);

	//��ø������ֶ�����
	double GetFloatValue(CString strItem);

	//����ı����ֶ�����
	CString GetStringValue(const CString& strItem);
	
	//���ݼ�ָ���Ƿ񵽴�ĩβ
	BOOL IsEOF();

	//ִ��sql������ �磺��ӣ��޸ģ�ɾ������
	BOOL Execute(CString strSql);
	
	//�ر����ݼ�
	void CloseRecordSet();

	//�Ͽ����ݿ�����
	void CloseDataBase();

private:

	//���ݿ����Ӷ���
    CDatabase* m_pDatabase;

	//���ݼ�����
	CRecordset* m_pRecordset;

};

#endif // !defined(AFX_OUTLOGIN_H__289A9BAF_D18E_4D80_A3B3_424202938B74__INCLUDED_)
