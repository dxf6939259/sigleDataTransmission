/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：OperXml.h
* 摘    要：配置文件操作类
* 
* 创建者：李国平
* 创建时间：2013/9/14
*/

#if !defined(AFX_OperXml_H__9094F36C_B525_4C09_8039_68873B33962D__INCLUDED_)
#define AFX_OperXml_H__9094F36C_B525_4C09_8039_68873B33962D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Markup.h"

//软件类型枚举
typedef enum _E_SOFT_TYPE
{
  ENU_SOFT_ERROR=0,
  ENU_SOFT_SENDER=1,
  ENU_SOFT_RECEIVER,
}E_SOFT_TYPE;

//模块类型枚举
typedef enum _E_MODULE_TYPE
{
  ENU_MODULE_FILE=1,
  ENU_MODULE_DATA,
  ENU_MODULE_MEDIA,
  ENU_MODULE_END,
  ENU_MODULE_NONE,
}E_MODULE_TYPE;

//界面元素类型枚举
typedef enum _E_NODE_TYPE
{
	ENU_NODE_LOCAL_PORT=1,
	ENU_NODE_AIM_IP,
	ENU_NODE_AIM_PORT,
	ENU_NODE_MONITOR_PATH,
	ENU_NODE_MONITOR_TYPE,
	ENU_NODE_SAVE_PATH,
	ENU_NODE_IS_DEL_FILE,
	ENU_NODE_IS_RESAVE_FILE,
	ENU_NODE_IS_SAFE_CHECK,
	ENU_NODE_IS_VERIF_RETURN,
}E_NODE_TYPE;

class COperXml  
{

private:
	COperXml();
public:
	virtual ~COperXml();
public:

	//软件类型
	static E_SOFT_TYPE g_softType;
    
	//获取单例对象
	static COperXml* GetInstance();

	//获取软件类型
	CString GetSoftType();

	//获取界面元素历史值
	CString GetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType);

	//保存界面元素值
    BOOL SetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType, CString strValue);
    
private:

	//获取模块类型
    CString GetModuleName(E_MODULE_TYPE moduleType);

	//获取界面元素类型名称
    CString GetNodeName(E_NODE_TYPE nodeType);
};

#endif // !defined(AFX_OperXml_H__9094F36C_B525_4C09_8039_68873B33962D__INCLUDED_)
