// BufZoneManager.cpp: implementation of the CBufZoneManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnidirectDataTransfer.h"
#include "BufZoneManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//用CList封装时，第二个元素会丢失
CBufZoneManager::CBufZoneManager()
{
	m_pDataBuf = NULL;
	m_nBufLen = 0;
	//delete lgp 12-16 begin
	/*m_pDataBuf = new byte[LEN_BUF_ZONE];
	memset(m_pDataBuf,0x00,LEN_BUF_ZONE);
	if(NULL == m_pDataBuf)
	{
		BCGPMessageBox(_T("发送端软件申请缓冲区内存失败!"));
	}*/
	//delete lgp 12-16 end
}

CBufZoneManager::~CBufZoneManager()
{
	
	if(NULL != m_pDataBuf)
	{
		delete []m_pDataBuf;
		m_pDataBuf = NULL;
        m_nBufLen = 0;
	}
	
}

CBufZoneManager::CBufZoneManager(byte* pData, int nLen)
{
	m_pDataBuf = NULL;
	//modify lgp 12-16 begin
	//m_pDataBuf = new byte[LEN_BUF_ZONE];
    //memset(m_pDataBuf,0x00,LEN_BUF_ZONE);

	m_pDataBuf = new byte[nLen];
	memset(m_pDataBuf,0x00,nLen);
	//modify lgp 12-16 end
	m_nBufLen = 0;
	if(NULL == m_pDataBuf)
	{
		BCGPMessageBox(_T("发送端软件申请缓冲区内存失败!"));
	}

	if(nLen<0)
		return;

	SetBufData(pData, nLen);
}

CBufZoneManager:: CBufZoneManager(const CBufZoneManager& rhs)
{
	if (this == &rhs)
		return;	
	m_nBufLen = rhs.m_nBufLen;
	//modify lgp 12-16 begin
	//m_pDataBuf = new byte[LEN_BUF_ZONE];
	//memset(m_pDataBuf,0x00,LEN_BUF_ZONE);

	m_pDataBuf = new byte[rhs.m_nBufLen];
	memset(m_pDataBuf,0x00,rhs.m_nBufLen);
	//modify lgp 12-16 end
	memcpy(m_pDataBuf,rhs.m_pDataBuf,rhs.m_nBufLen);
}

CBufZoneManager::SetBufData(byte* pData, int nLen)
{
	if(0 == nLen)
		return FALSE;
	
	//add lgp 12-16 begin
	if(NULL != m_pDataBuf)
	{
		delete []m_pDataBuf;
		m_pDataBuf = NULL;
        m_nBufLen = 0;
	}
	m_pDataBuf = new byte[nLen];
	memset(m_pDataBuf,0x00,nLen);
	//add lgp 12-16 end
	m_nBufLen = nLen;
    
	for (int i=0; i<m_nBufLen; i++)
	{
		m_pDataBuf[i] = *(pData+i);
	}
	
	return TRUE;
}
void CBufZoneManager::operator=( const CBufZoneManager& bufManager )
{
	// Copying not supported during file mode because of file pointer
	if (this == &bufManager)
	{
		return;
	}

	m_nBufLen = bufManager.m_nBufLen;
	memcpy(m_pDataBuf,bufManager.m_pDataBuf,bufManager.m_nBufLen);
}