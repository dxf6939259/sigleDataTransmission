/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�DbAccess.cpp
* ժ    Ҫ��ʵ��office access ���ݿ����
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 
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
	//�������ݿ����Ӷ���
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
��������BOOL ConnectDB()

  �����������������ݿ�
  
  ������CString strDataSource  ����Դ(���ݿ��ļ���)
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL     TRUE-�������ݿ�ɹ� FALSE-�������ݿ�ʧ��
		
****************************************************************/

BOOL CDbAccess::ConnectDB(CString strDataSource)
{
	//���ݿ������ַ���
	CString strSqlConn;
	strSqlConn.Format(_T("ODBC;DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s"),strDataSource);

	TRY
	{
		//�������ݿ�
		BOOL bRet = m_pDatabase->Open(NULL,false,false,strSqlConn);
		if(FALSE == bRet)
		{ 
			CString strMsg;
			strMsg.Format(_T("�Բ��𣬴�����Դ������������Դ�ļ�%s�Ƿ����"), strDataSource);
			BCGPMessageBox(NULL, strMsg, _T("��ʾ��"), MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		//�������ݼ�����
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
��������BOOL OpenQuery(CString strSelSql)

  ������������ȡ��ѯ���ݼ�
  
  ������CString strSelSql  ��ѯ�ַ���
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL     TRUE-��ѯ�ɹ� FALSE-��ѯʧ��
		
****************************************************************/

BOOL CDbAccess::OpenQuery(CString strSelSql)
{
	//�����ݼ�����Ϊ�գ���ֱ�ӷ���ʧ��
	if(NULL == m_pRecordset)
		return FALSE;
    
	TRY
	{
        //�����ݿ����Ӷ��󲻴��ڣ��򴴽����ݿ����Ӷ���
		if(NULL == m_pDatabase)
			m_pDatabase = new CDatabase();

        //�����ݼ����������ݣ���ر����ݿ����ӣ������������
		m_pRecordset->Close();
		
        //��ȡ��ѯ���ݼ�
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
��������BOOL MoveNext()

  ��������������ƶ����ݼ�ָ��
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL     TRUE-�ƶ����ݼ�ָ��ɹ� FALSE-�ƶ����ݼ�ָ��ʧ��
		
****************************************************************/

BOOL CDbAccess::MoveNext()
{
    //�����ݼ�����Ϊ�գ���ֱ�ӷ���ʧ��
	if(NULL == m_pRecordset)
		return FALSE;
	
	TRY
	{
		//������ݼ�ָ���Ѿ�����ĩβ���򷵻�FALSE
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
��������int GetIntValue(CString strItem)

  ������������ȡ�������ֶ���ֵ
  
  ������CString strItem   �ֶ�����
	
  ȫ�ֱ�������
	  
  ����ֵ: int     �ֶε�����ֵ�� ����-1�������
		
****************************************************************/
int CDbAccess::GetIntValue(CString strItem)
{
	//�����ݼ�����Ϊ�գ���ֱ�ӷ���ʧ��
    if(NULL == m_pRecordset)
		return DB_GET_FIELD_VALUE_ERROR;
	
	TRY
	{
		//������ݼ�ָ���Ѿ�����ĩβ���򷵻�-1
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
��������double GetFloatValue(CString strItem)

  ������������ȡ�������ֶ���ֵ
  
  ������CString strItem   �ֶ�����
	
  ȫ�ֱ�������
	  
  ����ֵ: double     �ֶεĸ�����ֵ
		
****************************************************************/
double CDbAccess::GetFloatValue(CString strItem)
{
    //�����ݼ�����Ϊ�գ���ֱ�ӷ��ش���
	if(NULL == m_pRecordset)
		return DB_GET_FIELD_VALUE_ERROR;
	
	TRY
	{
        //�����ݼ��Ѵﵽĩβ���򷵻ش���
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
��������CString GetStringValue(CString strItem)

  ������������ȡ�ı����ֶ���ֵ
  
  ������CString strItem   �ֶ�����
	
  ȫ�ֱ�������
	  
  ����ֵ: CString     �ֶε��ı�ֵ
		
****************************************************************/
CString CDbAccess::GetStringValue(const CString& strItem)
{
	//�����ݼ�����Ϊ�գ���ֱ�ӷ���ʧ��
	if(NULL == m_pRecordset)
		return _T("");
	
	TRY
	{
		//�����ݼ��Ѵﵽĩβ���򷵻ش���
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
��������void CloseRecordSet()

  �����������ر����ݼ�
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
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
��������void CloseDataBase()

  �����������ر���������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
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
��������BOOL IsEOF()

  �������������ݼ�����ָ���Ƿ�ﵽĩβ
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL   TRUE--���ݼ�����ָ���Ѵﵽĩβ   FALSE--���ݼ�����ָ��δ�ﵽĩβ
		
****************************************************************/
BOOL CDbAccess::IsEOF()
{
	//�����ݼ�����Ϊ�գ���ֱ�ӷ���ʧ��
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
��������void Execute()

  ����������ִ�����ݸ�������
  
  ������CString strSql   ���ݸ�������--���룬ɾ�����޸�
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
BOOL CDbAccess::Execute(CString strSql)
{
	//�����ݿ����Ӷ���Ϊ�գ���ֱ�ӷ��ش���
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
