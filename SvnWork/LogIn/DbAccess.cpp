/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：DbAccess.cpp
* 摘    要：实现office access 数据库访问
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 
* 
*/




#include "stdafx.h"
#include "LogIn.h"
#include "DbAccess.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbAccess::CDbAccess()
{
	m_pDatabase = NULL;
	m_pRecordset = NULL;
	//创建数据库连接对象
	m_pDatabase = new CDatabase();

}

CDbAccess::~CDbAccess()
{
	if(NULL != m_pDatabase)
	{
		m_pDatabase->Close();
		delete m_pDatabase;
		m_pDatabase = NULL;
	}
	if(NULL != m_pRecordset)
	{
		m_pRecordset->Close();
		delete m_pRecordset;
		m_pRecordset = NULL;
  }
}

/***************************************************************
函数名：BOOL ConnectDB()

  功能描述：连接数据库
  
  参数：CString strDataSource  数据源(数据库文件名)
	
  全局变量：无
	  
  返回值: BOOL     TRUE-连接数据库成功 FALSE-连接数据库失败
		
****************************************************************/

BOOL CDbAccess::ConnectDB(CString strDataSource)
{
	//数据库连接字符串
	CString strSqlConn;
	strSqlConn.Format(_T("ODBC;DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s"),strDataSource);

	TRY
	{
		//连接数据库
		BOOL bRet = m_pDatabase->Open(NULL,false,false,strSqlConn);
		if(FALSE == bRet)
		{ 
			CString strMsg;
			strMsg.Format(_T("对不起，打开数据源出错，请检查数据源文件%s是否存在"), strDataSource);
			BCGPMessageBox(NULL, strMsg, _T("提示框"), MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		//创建数据集对象
		m_pRecordset = new CRecordset(m_pDatabase);
	}
	CATCH(CDBException, e)
	{
		return FALSE;
	}
	END_CATCH;

	return TRUE;
}


/***************************************************************
函数名：BOOL OpenQuery(CString strSelSql)

  功能描述：获取查询数据集
  
  参数：CString strSelSql  查询字符串
	
  全局变量：无
	  
  返回值: BOOL     TRUE-查询成功 FALSE-查询失败
		
****************************************************************/

BOOL CDbAccess::OpenQuery(CString strSelSql)
{
	//若数据集对象为空，则直接返回失败
	if(NULL == m_pRecordset)
		return FALSE;
    
	TRY
	{
        //若数据库连接对象不存在，则创建数据库连接对象
		if(NULL == m_pDatabase)
			m_pDatabase = new CDatabase();

        //若数据集对象有数据，则关闭数据库连接，清空数据内容
		m_pRecordset->Close();
		
        //获取查询数据集
		BOOL bRet = m_pRecordset->Open(CRecordset::forwardOnly,strSelSql,CRecordset::readOnly);
		if(FALSE == bRet)
			return bRet;

	}
	CATCH(CDBException, e)
	{
		return FALSE;
	}
	END_CATCH;
	
	return TRUE;
}

/***************************************************************
函数名：BOOL MoveNext()

  功能描述：向后移动数据集指针
  
  参数：无
	
  全局变量：无
	  
  返回值: BOOL     TRUE-移动数据集指针成功 FALSE-移动数据集指针失败
		
****************************************************************/

BOOL CDbAccess::MoveNext()
{
    //若数据集对象为空，则直接返回失败
	if(NULL == m_pRecordset)
		return FALSE;
	
	TRY
	{
		//如果数据集指针已经到达末尾，则返回FALSE
		if(m_pRecordset->IsEOF())
			return FALSE;
		
		m_pRecordset->MoveNext();
	}
	CATCH(CDBException, e)
	{
		return FALSE;
	}
	END_CATCH;
	
	return TRUE;
}

/***************************************************************
函数名：int GetIntValue(CString strItem)

  功能描述：获取整数型字段数值
  
  参数：CString strItem   字段名称
	
  全局变量：无
	  
  返回值: int     字段的整数值。 返回-1代表出错
		
****************************************************************/
int CDbAccess::GetIntValue(CString strItem)
{
	//若数据集对象为空，则直接返回失败
    if(NULL == m_pRecordset)
		return DB_GET_FIELD_VALUE_ERROR;
	
	TRY
	{
		//如果数据集指针已经到达末尾，则返回-1
		if(m_pRecordset->IsEOF())
			return DB_GET_FIELD_VALUE_ERROR;

		CDBVariant var;
		m_pRecordset->GetFieldValue(strItem, var);

		return var.m_iVal;

	}
	CATCH(CDBException, e)
	{
		return DB_GET_FIELD_VALUE_ERROR;
	}
	END_CATCH;
	
}

/***************************************************************
函数名：double GetFloatValue(CString strItem)

  功能描述：获取浮点型字段数值
  
  参数：CString strItem   字段名称
	
  全局变量：无
	  
  返回值: double     字段的浮点数值
		
****************************************************************/
double CDbAccess::GetFloatValue(CString strItem)
{
    //若数据集对象为空，则直接返回错误
	if(NULL == m_pRecordset)
		return DB_GET_FIELD_VALUE_ERROR;
	
	TRY
	{
        //若数据集已达到末尾，则返回错误
		if(m_pRecordset->IsEOF())
			return DB_GET_FIELD_VALUE_ERROR;

		CDBVariant var;
		m_pRecordset->GetFieldValue(strItem, var);

		return var.m_dblVal;
		
	}
	CATCH(CDBException, e)
	{
		return DB_GET_FIELD_VALUE_ERROR;
	}
	END_CATCH;

}

/***************************************************************
函数名：CString GetStringValue(CString strItem)

  功能描述：获取文本型字段数值
  
  参数：CString strItem   字段名称
	
  全局变量：无
	  
  返回值: CString     字段的文本值
		
****************************************************************/
CString CDbAccess::GetStringValue(const CString& strItem)
{
	//若数据集对象为空，则直接返回失败
	if(NULL == m_pRecordset)
		return _T("");
	
	TRY
	{
		//若数据集已达到末尾，则返回错误
		if(m_pRecordset->IsEOF())
			return _T("");

		CDBVariant var;
		m_pRecordset->GetFieldValue(strItem, var);
		return *var.m_pstring;
		
	}
	CATCH(CDBException, e)
	{
		return _T("");
	}
	END_CATCH;
}

/***************************************************************
函数名：void CloseRecordSet()

  功能描述：关闭数据集
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/

void CDbAccess::CloseRecordSet()
{
	TRY
	{
		if(NULL != m_pRecordset)
		{
			m_pRecordset->Close();
			delete m_pRecordset;
			m_pRecordset = NULL;
		}
	}
	CATCH(CDBException, e)
	{
		return;
	}
	END_CATCH;
}

/***************************************************************
函数名：void CloseDataBase()

  功能描述：关闭数据连接
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CDbAccess::CloseDataBase()
{
	TRY
	{
		if(NULL != m_pRecordset)
             CloseRecordSet();

		if(NULL != m_pDatabase)
		{
			m_pDatabase->Close();
			delete m_pDatabase;
			m_pDatabase = NULL;
		}
	}
	CATCH(CDBException, e)
	{
		return ;
	}
	END_CATCH;
}

/***************************************************************
函数名：BOOL IsEOF()

  功能描述：数据集对象指针是否达到末尾
  
  参数：无
	
  全局变量：无
	  
  返回值: BOOL   TRUE--数据集对象指针已达到末尾   FALSE--数据集对象指针未达到末尾
		
****************************************************************/
BOOL CDbAccess::IsEOF()
{
	//若数据集对象为空，则直接返回失败
	if(NULL == m_pRecordset)
		return TRUE;
	
	TRY
	{
		if(m_pRecordset->IsEOF())
			return TRUE;
		
	}
	CATCH(CDBException, e)
	{
		return TRUE;
	}
	END_CATCH;

	return FALSE;
}

/***************************************************************
函数名：void Execute()

  功能描述：执行数据更新命令
  
  参数：CString strSql   数据更行命令--插入，删除，修改
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
BOOL CDbAccess::Execute(CString strSql)
{
	//若数据库连接对象为空，则直接返回错误
	if(NULL == m_pDatabase)
		return FALSE;
	
	TRY
	{
	    m_pDatabase->ExecuteSQL(strSql);
		DWORD dErr = GetLastError();

	}
	CATCH(CDBException, e)
	{
		return FALSE;
	}
	END_CATCH;
	
	return TRUE;
}
