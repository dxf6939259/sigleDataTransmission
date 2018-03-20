/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�OperXml.h
* ժ    Ҫ�������ļ�������
* 
* �����ߣ����ƽ
* ����ʱ�䣺2013/9/14
*/

#if !defined(AFX_OperXml_H__9094F36C_B525_4C09_8039_68873B33962D__INCLUDED_)
#define AFX_OperXml_H__9094F36C_B525_4C09_8039_68873B33962D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Markup.h"

//�������ö��
typedef enum _E_SOFT_TYPE
{
  ENU_SOFT_ERROR=0,
  ENU_SOFT_SENDER=1,
  ENU_SOFT_RECEIVER,
}E_SOFT_TYPE;

//ģ������ö��
typedef enum _E_MODULE_TYPE
{
  ENU_MODULE_FILE=1,
  ENU_MODULE_DATA,
  ENU_MODULE_MEDIA,
  ENU_MODULE_END,
  ENU_MODULE_NONE,
}E_MODULE_TYPE;

//����Ԫ������ö��
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

	//�������
	static E_SOFT_TYPE g_softType;
    
	//��ȡ��������
	static COperXml* GetInstance();

	//��ȡ�������
	CString GetSoftType();

	//��ȡ����Ԫ����ʷֵ
	CString GetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType);

	//�������Ԫ��ֵ
    BOOL SetNodeValue(E_MODULE_TYPE moduleType, E_NODE_TYPE nodeType, CString strValue);
    
private:

	//��ȡģ������
    CString GetModuleName(E_MODULE_TYPE moduleType);

	//��ȡ����Ԫ����������
    CString GetNodeName(E_NODE_TYPE nodeType);
};

#endif // !defined(AFX_OperXml_H__9094F36C_B525_4C09_8039_68873B33962D__INCLUDED_)
