// BufZoneManager.h: interface for the CBufZoneManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFZONEMANAGER_H__26C7AF7F_15CF_430E_BA13_98A7FEA56FC6__INCLUDED_)
#define AFX_BUFZONEMANAGER_H__26C7AF7F_15CF_430E_BA13_98A7FEA56FC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LEN_BUF_ZONE 1301 

class CBufZoneManager  
{
public:
	CBufZoneManager();
	CBufZoneManager(const CBufZoneManager& rhs);
	virtual ~CBufZoneManager();

    CBufZoneManager(byte* pData, int nLen);
    
    BOOL SetBufData(byte* pData, int nLen);
	void operator=( const CBufZoneManager& bufManager );
public:
	byte* m_pDataBuf;
	int m_nBufLen;
};

#endif // !defined(AFX_BUFZONEMANAGER_H__26C7AF7F_15CF_430E_BA13_98A7FEA56FC6__INCLUDED_)
