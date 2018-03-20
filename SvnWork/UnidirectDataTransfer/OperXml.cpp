/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�OperXml.cpp
* ժ    Ҫ�������ļ�������
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

*/

#include "stdafx.h"
#include "UnidirectDataTransfer.h"
#include "OperXml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

E_SOFT_TYPE COperXml::g_softType=ENU_SOFT_ERROR;

static COperXml * m_pOperXml = NULL;


COperXml::COperXml()
{

}

COperXml::~COperXml()
{
    delete m_pOperXml;
	m_pOperXml = NULL;
}

/***************************************************************
��������COperXml* GetInstance()

  ������������ȡ�����ļ������൥������
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: COperXml*     �����ļ����������ָ��
		
****************************************************************/
COperXml* COperXml::GetInstance()
{
	if(NULL == m_pOperXml)
	{
		m_pOperXml = new COperXml();
	}
	return m_pOperXml;
    
}

/***************************************************************
��������BOOL GetSoftType()

  ������������ȡ������ͣ������ھ�̬����g_softType��
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: CString     �������
		
****************************************************************/
CString COperXml::GetSoftType()
{
   	CMarkup xml;
	bool    flag; 
	CString strSoftName;

	//����Xml�ļ�  
	flag = xml.Load(_T("DXSJCS.xml"));   
	if (!flag)   
	{   
		return _T("");  
	}  
	xml.ResetPos(); 
	flag = xml.FindElem("DXSJCS");    
	if (!flag)  
	{  
		return _T("");  
	}  
	
	CString strSoftType; 
	xml.IntoElem();    
	if(xml.FindElem(TEXT("SoftType")))
	{
		strSoftType = xml.GetData();   
	}
	 
    
	if(_T("a") == strSoftType)
	{
		g_softType = ENU_SOFT_SENDER;
		if(xml.FindElem(TEXT("a")))
		{
			strSoftName = xml.GetData();   
		}
		xml.OutOfElem();
		return strSoftName;
	}
	else if(_T("b") == strSoftType)
	{
		g_softType = ENU_SOFT_RECEIVER;
		if(xml.FindElem(TEXT("b")))
		{
			strSoftName = xml.GetData();   
		}
		xml.OutOfElem();
		return strSoftName;
	}
	else 
	{
		g_softType = ENU_SOFT_ERROR;
        return _T(""); 
	}
	
	return strSoftName;

}

/***************************************************************
��������CString GetModuleName(E_MODULE_TYPE moduleType)

  ������������ȡģ������
  
  ������E_MODULE_TYPE moduleType ģ������
	
  ȫ�ֱ�������
	  
  ����ֵ: CString     ģ������
		
****************************************************************/
CString COperXml::GetModuleName(E_MODULE_TYPE moduleType)
{
	switch(moduleType)
	{
	case ENU_MODULE_DATA:
		return _T("Data");
	case ENU_MODULE_FILE:
		return _T("File");
	case ENU_MODULE_MEDIA:
		return _T("Media");
	case ENU_MODULE_END:
		return _T("End");
	default:
		return _T("");
   }
}

/***************************************************************
��������CString GetNodeName(E_NODE_TYPE nodeType)

  ������������ȡ����Ԫ������
  
  ������E_NODE_TYPE nodeType ����Ԫ������
	
  ȫ�ֱ�������
	  
  ����ֵ: CString     ����Ԫ������
		
****************************************************************/
CString COperXml::GetNodeName(E_NODE_TYPE nodeType)
{
	switch(nodeType)
	{
	case ENU_NODE_AIM_IP:
		return _T("AimIp");
	case ENU_NODE_AIM_PORT:
		return _T("AimPort");
	case ENU_NODE_IS_DEL_FILE:
		return _T("IsDelFile");
	case ENU_NODE_IS_RESAVE_FILE:
		return _T("IsReSaveFile");
	case ENU_NODE_IS_SAFE_CHECK:
		return _T("IsSafeCheck");
	case ENU_NODE_IS_VERIF_RETURN:
		return _T("IsVerifReturn");
	case ENU_NODE_LOCAL_PORT:
		return _T("LocalPort");
	case ENU_NODE_MONITOR_PATH:
		return _T("MonitorPath");
	case ENU_NODE_MONITOR_TYPE:
		return _T("MonitorType");
	case ENU_NODE_SAVE_PATH:
		return _T("SavePath");
	default:
		return _T("");
   }
}

/***************************************************************
��������CString GetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType)

  ������������ȡ����Ԫ��ֵ
  
  ������E_MODULE_TYPE moduleType ģ������
        E_NODE_TYPE nodeType     ����Ԫ������
	
  ȫ�ֱ�������
	  
  ����ֵ: CString     ����Ԫ��ֵ
		
****************************************************************/
CString COperXml::GetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType)
{
   //���ģ�����ƺͽڵ�����
   CString strModuleType=GetModuleName(moduleType);
   CString strNodeType=GetNodeName(nodeType);
   if(strModuleType.IsEmpty() || strNodeType.IsEmpty())
	   return _T("");
   
   CMarkup xml;
   bool    flag; 
   //����Xml�ļ�  
   flag = xml.Load(_T("DXSJCS.xml"));   
   if (!flag)   
   {   
	   return _T("");  
   }  

   //��λ����
   xml.ResetPos(); 
   flag = xml.FindElem("DXSJCS");    
   if (!flag)  
   {  
	   return _T("");  
   }  

   //��λ���������
   if(ENU_SOFT_SENDER==COperXml::g_softType)
   {
	   xml.IntoElem();    
       flag = xml.FindElem(TEXT("Sender"));
   }
   else if(ENU_SOFT_RECEIVER==COperXml::g_softType)
   {
	   xml.IntoElem();    
	   flag = xml.FindElem(TEXT("Recevier"));
   }
   else
   {
	   xml.OutOfElem();
	   return _T("");  
   }

   if (!flag)  
   {  
	   return _T("");  
   } 

   //��λ��ģ��
   xml.IntoElem();    
   flag = xml.FindElem(strModuleType);
   if (!flag)  
   {  
	   return _T("");  
   } 

   //��λ���ڵ�
   xml.IntoElem();    
   flag = xml.FindElem(strNodeType);
   if (!flag)  
   {  
	   return _T("");  
   } 

   //��ýڵ�ֵ
   CString strNodeValue; 
   strNodeValue = xml.GetData(); 
	   
   xml.OutOfElem();//�˳��ڵ�
   xml.OutOfElem();//�˳�ģ��
   xml.OutOfElem();//�˳��������
   
   return strNodeValue;
   
}

/***************************************************************
��������BOOL SetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType, CString strValue)

  �����������������Ԫ��ֵ
  
  ������E_MODULE_TYPE moduleType ģ������
        E_NODE_TYPE nodeType     ����Ԫ������
		CString strValue         ����Ԫ��ֵ
	
  ȫ�ֱ�������
	  
  ����ֵ: BOOL     TRUE--����ɹ� FALSE--����ʧ��
		
****************************************************************/
BOOL COperXml::SetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType, CString strValue)
{
	//���ģ�����ƺͽڵ�����
	CString strModuleType=GetModuleName(moduleType);
	CString strNodeType=GetNodeName(nodeType);
	CString strNodeValue;
	if(strModuleType.IsEmpty() || strNodeType.IsEmpty())
		return FALSE;
	
	CMarkup xml;
	bool    flag; 
	//����Xml�ļ�  
	flag = xml.Load(_T("DXSJCS.xml"));   
	if (!flag)   
	{   
		return FALSE; 
	}  
	
	//��λ����
	xml.ResetPos(); 
	flag = xml.FindElem("DXSJCS");    
	if (!flag)  
	{  
		return FALSE; 
	}  
	
	//��λ���������
	if(ENU_SOFT_SENDER==COperXml::g_softType)
	{
		xml.IntoElem();    
		flag = xml.FindElem(TEXT("Sender"));
	}
	else if(ENU_SOFT_RECEIVER==COperXml::g_softType)
	{
		xml.IntoElem();    
		flag = xml.FindElem(TEXT("Recevier"));
	}
	else
	{
		xml.OutOfElem();
		return FALSE; 
	}
	
	if (!flag)  
	{  
		return FALSE; 
	} 
	
	//��λ��ģ��
	xml.IntoElem();    
	flag = xml.FindElem(strModuleType);
	if (!flag)  
	{  
		return FALSE;  
	} 
	
	//��λ���ڵ�
	xml.IntoElem();    
	flag = xml.FindElem(strNodeType);
	if (!flag)  
	{  
		return FALSE; 
	} 
	
	//�޸Ľڵ�ֵ
	flag = xml.SetData(strValue); 
	   
	xml.OutOfElem();//�˳��ڵ�
	xml.OutOfElem();//�˳�ģ��
    xml.OutOfElem();//�˳��������
	flag = xml.Save(_T("DXSJCS.xml"));

    return TRUE;
}