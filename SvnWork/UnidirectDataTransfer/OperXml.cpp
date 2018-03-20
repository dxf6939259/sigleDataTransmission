/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：OperXml.cpp
* 摘    要：配置文件操作类
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

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
函数名：COperXml* GetInstance()

  功能描述：获取配置文件操作类单例对象
  
  参数：无
	
  全局变量：无
	  
  返回值: COperXml*     配置文件操作类对象指针
		
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
函数名：BOOL GetSoftType()

  功能描述：获取软件类型，保存在静态变量g_softType中
  
  参数：无
	
  全局变量：无
	  
  返回值: CString     软件名称
		
****************************************************************/
CString COperXml::GetSoftType()
{
   	CMarkup xml;
	bool    flag; 
	CString strSoftName;

	//加载Xml文件  
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
函数名：CString GetModuleName(E_MODULE_TYPE moduleType)

  功能描述：获取模块名称
  
  参数：E_MODULE_TYPE moduleType 模块类型
	
  全局变量：无
	  
  返回值: CString     模块名称
		
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
函数名：CString GetNodeName(E_NODE_TYPE nodeType)

  功能描述：获取界面元素名称
  
  参数：E_NODE_TYPE nodeType 界面元素类型
	
  全局变量：无
	  
  返回值: CString     界面元素名称
		
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
函数名：CString GetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType)

  功能描述：获取界面元素值
  
  参数：E_MODULE_TYPE moduleType 模块类型
        E_NODE_TYPE nodeType     界面元素类型
	
  全局变量：无
	  
  返回值: CString     界面元素值
		
****************************************************************/
CString COperXml::GetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType)
{
   //获得模块名称和节点名称
   CString strModuleType=GetModuleName(moduleType);
   CString strNodeType=GetNodeName(nodeType);
   if(strModuleType.IsEmpty() || strNodeType.IsEmpty())
	   return _T("");
   
   CMarkup xml;
   bool    flag; 
   //加载Xml文件  
   flag = xml.Load(_T("DXSJCS.xml"));   
   if (!flag)   
   {   
	   return _T("");  
   }  

   //定位到根
   xml.ResetPos(); 
   flag = xml.FindElem("DXSJCS");    
   if (!flag)  
   {  
	   return _T("");  
   }  

   //定位到软件类型
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

   //定位到模块
   xml.IntoElem();    
   flag = xml.FindElem(strModuleType);
   if (!flag)  
   {  
	   return _T("");  
   } 

   //定位到节点
   xml.IntoElem();    
   flag = xml.FindElem(strNodeType);
   if (!flag)  
   {  
	   return _T("");  
   } 

   //获得节点值
   CString strNodeValue; 
   strNodeValue = xml.GetData(); 
	   
   xml.OutOfElem();//退出节点
   xml.OutOfElem();//退出模块
   xml.OutOfElem();//退出软件类型
   
   return strNodeValue;
   
}

/***************************************************************
函数名：BOOL SetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType, CString strValue)

  功能描述：保存界面元素值
  
  参数：E_MODULE_TYPE moduleType 模块类型
        E_NODE_TYPE nodeType     界面元素类型
		CString strValue         界面元素值
	
  全局变量：无
	  
  返回值: BOOL     TRUE--保存成功 FALSE--保存失败
		
****************************************************************/
BOOL COperXml::SetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType, CString strValue)
{
	//获得模块名称和节点名称
	CString strModuleType=GetModuleName(moduleType);
	CString strNodeType=GetNodeName(nodeType);
	CString strNodeValue;
	if(strModuleType.IsEmpty() || strNodeType.IsEmpty())
		return FALSE;
	
	CMarkup xml;
	bool    flag; 
	//加载Xml文件  
	flag = xml.Load(_T("DXSJCS.xml"));   
	if (!flag)   
	{   
		return FALSE; 
	}  
	
	//定位到根
	xml.ResetPos(); 
	flag = xml.FindElem("DXSJCS");    
	if (!flag)  
	{  
		return FALSE; 
	}  
	
	//定位到软件类型
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
	
	//定位到模块
	xml.IntoElem();    
	flag = xml.FindElem(strModuleType);
	if (!flag)  
	{  
		return FALSE;  
	} 
	
	//定位到节点
	xml.IntoElem();    
	flag = xml.FindElem(strNodeType);
	if (!flag)  
	{  
		return FALSE; 
	} 
	
	//修改节点值
	flag = xml.SetData(strValue); 
	   
	xml.OutOfElem();//退出节点
	xml.OutOfElem();//退出模块
    xml.OutOfElem();//退出软件类型
	flag = xml.Save(_T("DXSJCS.xml"));

    return TRUE;
}