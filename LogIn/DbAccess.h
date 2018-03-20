/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DbAccess.h
* 摘    要：访问office access 2003数据库
* 
* 创建者：李国平
* 创建时间：2013/9/14
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

	//连接到access数据库
	BOOL ConnectDB(CString strDataSource);

	//打开数据集
	BOOL OpenQuery(CString strSelSql);

	//向后移动数据集指针
	BOOL MoveNext();

	//获得整型字段数据
	int GetIntValue(CString strItem);

	//获得浮点型字段数据
	double GetFloatValue(CString strItem);

	//获得文本型字段数据
	CString GetStringValue(const CString& strItem);
	
	//数据集指针是否到达末尾
	BOOL IsEOF();

	//执行sql操作， 如：添加，修改，删除命令
	BOOL Execute(CString strSql);
	
	//关闭数据集
	void CloseRecordSet();

	//断开数据库连接
	void CloseDataBase();

private:

	//数据库连接对象
    CDatabase* m_pDatabase;

	//数据集对象
	CRecordset* m_pRecordset;

};

#endif // !defined(AFX_OUTLOGIN_H__289A9BAF_D18E_4D80_A3B3_424202938B74__INCLUDED_)
