#if !defined(AFX_PROCESSAPPLY_H__9053C00E_4303_4134_AB80_250988296F57__INCLUDED_)
#define AFX_PROCESSAPPLY_H__9053C00E_4303_4134_AB80_250988296F57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessApply.h : header file
//


#include <map>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CProcessApply command target

class CProcessApply : public CAsyncSocket
{
// Attributes
public:
		map<int,int> m_mapFlag;
// Operations
public:
	CProcessApply();
	virtual ~CProcessApply();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessApply)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CProcessApply)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSAPPLY_H__9053C00E_4303_4134_AB80_250988296F57__INCLUDED_)
