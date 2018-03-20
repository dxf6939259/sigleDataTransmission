// DogClass.cpp: implementation of the CDogClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogIn.h"
#include "DlgLogIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDogClass::CDogClass()
{
	Init();
}

CDogClass::~CDogClass()
{

}

void CDogClass::Init()
{
	 m_nNum  = 0;
	 m_sData = "";
}

int CDogClass::ShowDlg()
{
	CDlgLogIn dlg;	
	dlg.DoModal();
	/*dlg.m_nNum  = GetNum();
	dlg.m_sData = GetData();
	if (IDOK==dlg.DoModal())
	{
		SetNum(dlg.m_nNum);
		SetData(dlg.m_sData);
		return 1;
	}*/
	return 0;
}
