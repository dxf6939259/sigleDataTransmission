// ProcessApply.cpp : implementation file
//

#include "stdafx.h"
#include "demo54.h"
#include "ProcessApply.h"
#include "demo54Dlg.h"


extern CDemo54App theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessApply

CProcessApply::CProcessApply()
{
	m_mapFlag.clear();
}

CProcessApply::~CProcessApply()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CProcessApply, CAsyncSocket)
	//{{AFX_MSG_MAP(CProcessApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CProcessApply member functions

void CProcessApply::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	byte buf[15] = {0};
	CString csIp;
	UINT serPort;
	int nRead = ReceiveFrom((void*)buf,15,csIp,serPort,0);
	int iFlag = 0;
	map<int,int>::iterator it;
	CString str;
	if (*(buf+9) == 1)
	{
		while(1)
		{
			srand(time(0));
			iFlag = rand();
			it = m_mapFlag.find(iFlag);			
			if (it == m_mapFlag.end())
			{			
				break;
			}
			else
			{
				continue;
			}
		}
		memcpy(buf+10,&iFlag,4);
		*(buf+9) = 3;
		m_mapFlag[iFlag] = iFlag;
		SendTo(buf,15,serPort,csIp,0);
		str.Format("×¢²áºÅÎª£º%d\n",iFlag);
		int nCount = ((CDemo54Dlg*)theApp.GetMainWnd())->m_lstLog.GetItemCount();
	    ((CDemo54Dlg*)theApp.GetMainWnd())->m_lstLog.InsertItem(nCount,str);
	}

	else if (*(buf+9) == 2) //×¢ÏúÉêÇëºÅ
	{
		memcpy(&iFlag,buf+10,4); 
		it = m_mapFlag.find(iFlag);
		str.Format("×¢²áºÅ£º%d ±»×¢Ïú",iFlag);
		if (it != m_mapFlag.end())
		{
			m_mapFlag.erase(iFlag);
			//AfxMessageBox(str);
			int nCount = ((CDemo54Dlg*)theApp.GetMainWnd())->m_lstLog.GetItemCount();
	        ((CDemo54Dlg*)theApp.GetMainWnd())->m_lstLog.InsertItem(nCount,str);
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
