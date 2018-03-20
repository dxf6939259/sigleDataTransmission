/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�LogForm.cpp
* ժ    Ҫ����־�������
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

*/

#include "stdafx.h"
#include "LogMgr.h"
#include "LogForm.h"
#include "OutLogMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogForm dialog


CLogForm::CLogForm(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CLogForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_rectMaxList.SetRectEmpty();
	m_rectMaxTree.SetRectEmpty();
	m_rectRestoredList.SetRectEmpty();
	m_rectRestoredTree.SetRectEmpty();

}


void CLogForm::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogForm)
	DDX_Control(pDX, IDC_LST_LOG_CONTENT, m_lstLogContent);
	DDX_Control(pDX, IDC_TREE_LOG_FILE, m_treLogFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogForm, CBCGPDialog)
	//{{AFX_MSG_MAP(CLogForm)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_LOG_FILE, ClickTree)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogForm message handlers



BOOL CLogForm::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    InitTree();
	
	CRect rect;
	m_lstLogContent.GetWindowRect(&rect);
	UINT nWidth = rect.Width()/6;
	m_lstLogContent.InsertColumn(0, _T("��ˮ��"), LVCFMT_LEFT, nWidth);
	m_lstLogContent.InsertColumn(1, _T("����"), LVCFMT_LEFT, 5*nWidth-4);

	//�����ʼ״̬�Ŀؼ�λ�ñ���
	m_rectRestoredList = rect;
	m_treLogFile.GetWindowRect(&m_rectRestoredTree);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/***************************************************************
��������void InitTree()

  ������������ʼ����־�ļ��б����οؼ�
  
  ��������
	
  ȫ�ֱ�������
	  
  ����ֵ: ��
		
****************************************************************/
void CLogForm::InitTree()
{
	/************************************************************************/
	/* ������־�ļ����е���־�ļ�   ��־�ļ�����(�����ڽ�������)                                                                  */
	/************************************************************************/
	CStringList strLstLog;
	CString m_cstrFileList="";
    CFileFind tempFind;
    BOOL bFound; //�ж��Ƿ�ɹ��ҵ��ļ�
	
	//��ñ���·��
	TCHAR szFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
    TCHAR* pPath = _tcsrchr(szFilePath, '\\');
	*pPath = 0;
	_tcscat(szFilePath, _T("\\Log\\*.log"));
	
	
    bFound=tempFind.FindFile(szFilePath);   
	CString strTmp;   //����ҵ������ļ��д���ļ���·��
    while(bFound)      //���������ļ�
    {
        bFound=tempFind.FindNextFile(); 
		//����һ���ļ�
		if(tempFind.IsDots()) 
			continue;
		
		//��ȥ��չ�����õ�����
		strTmp=tempFind.GetFileName(); 
		strTmp.Replace(_T(".log"), _T(""));
		strLstLog.AddHead(strTmp);
    }
	
    tempFind.Close();



    /************************************************************************/
    /*       ��ʼ����־�ļ��б����οؼ�                                                               */
    /************************************************************************/
	HTREEITEM	hRootItem = m_treLogFile.InsertItem("��־�ļ�",TVI_ROOT);
	POSITION pos = strLstLog.GetHeadPosition();
	CString strYear;
	CString strMonth;
	strYear.Empty();
	strMonth.Empty();
	HTREEITEM hYear;
	HTREEITEM hMonth;
	for(; pos!=NULL; )
	{
		CString strLogFile = strLstLog.GetNext(pos);
		CString strYearTemp = strLogFile.Left(4);
		if(strYear.IsEmpty())//��һ����ݽڵ�
		{
            strYear = strLogFile.Left(4);
			hYear = m_treLogFile.InsertItem(strYear,hRootItem);
			m_treLogFile.Expand(hRootItem, TVE_EXPAND);
		}
		else if(strYear.Compare(strYearTemp)!=0)//�µ���ݽڵ�
		{
			strYear = strYearTemp;
			hYear = m_treLogFile.InsertItem(strYear,hRootItem);
			m_treLogFile.Expand(hRootItem, TVE_EXPAND);
		}
		
		CString strMonthTemp = strLogFile.Mid(4,2);
		if(strMonth.IsEmpty())//��һ���·ݽڵ�
		{
			strMonth = strLogFile.Mid(4,2);
			hMonth = m_treLogFile.InsertItem(strMonth,hYear);
			m_treLogFile.Expand(hYear, TVE_EXPAND);
		}
		else if(strMonth.Compare(strMonthTemp)!=0)//�µ��·ݽڵ�
		{
			strMonth = strMonthTemp;
			hMonth = m_treLogFile.InsertItem(strMonth,hYear);
			m_treLogFile.Expand(hYear, TVE_EXPAND);
		}
		
		CString strDay = strLogFile.Mid(6,2);
		m_treLogFile.InsertItem(strDay,hMonth);
		m_treLogFile.Expand(hMonth, TVE_EXPAND);
	}
	
}


void CLogForm::ClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint  cp; 
	GetCursorPos(&cp); 
	m_treLogFile.ScreenToClient(&cp);  //m_TREE��CTreeCtrl�ؼ�������
	
	HTREEITEM  titem = m_treLogFile.HitTest(cp,NULL); //��ȡ��ǰ����Ҽ�������λ���µ�item
	
	CString str = m_treLogFile.GetItemText(titem); //��ǰ������Ľڵ����ø�dom��
	
	//�����־��¼
	m_lstLogContent.DeleteAllItems();
	
	//��ǰѡ���˸��ڵ㣬��ڵ㣬���½ڵ㣬���˳�
    HTREEITEM hChild = m_treLogFile.GetChildItem(titem);
	if(NULL != hChild)
		return;
	
	HTREEITEM hMonth= m_treLogFile.GetParentItem(titem);
    HTREEITEM hYear = m_treLogFile.GetParentItem(hMonth);
	
	CString strYear = m_treLogFile.GetItemText(hYear);
	CString strMonty = m_treLogFile.GetItemText(hMonth);
	CString strDay = m_treLogFile.GetItemText(titem);
	
	//��ǰѡ��Ĳ����սڵ㣬���˳�
	if(strDay.GetLength()!=2)
		return;
	
	CString strLogFile;
	strLogFile.Format(_T("%s%s%s.log"),strYear,strMonty,strDay);
	CStringList strLstLogContent;
	strLstLogContent.RemoveAll();
	
	//��ȡ��־�ļ�����
	BOOL bRet = COutLogMgr::GetInstance()->DisplayLogContent(strLstLogContent, strLogFile);
	
	//��ʾ��־�ļ�����
	int nIndex=1;
	int nNum = strLstLogContent.GetCount();
	POSITION pos = strLstLogContent.GetHeadPosition();

	while(NULL != pos)
	{
		CString strIndex, strLogContent;
		strIndex.Format(_T("%d"), nNum--);
		strLogContent = strLstLogContent.GetNext(pos);
		m_lstLogContent.InsertItem(nIndex-1, strIndex);
		m_lstLogContent.SetItemText(nIndex-1, 1, strLogContent);
		nIndex++;
	}
 
	*pResult = 0;
}
BOOL CLogForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->wParam == VK_RETURN )
	{
		return TRUE;
	}
	
	
	return CBCGPDialog::PreTranslateMessage(pMsg);
}


void CLogForm::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDialog::OnSize(nType, cx, cy);
	if(nType== SIZE_MAXIMIZED)     
	{
		if( (!m_rectMaxList.IsRectEmpty()) && (!m_rectMaxTree.IsRectEmpty()))
		{
			m_lstLogContent.MoveWindow(&m_rectMaxList);
			m_treLogFile.MoveWindow(&m_rectMaxTree);

			UINT nWidth = m_rectMaxList.Width()/6;
			m_lstLogContent.SetColumnWidth(0,  nWidth);
	        m_lstLogContent.SetColumnWidth(1,  5*nWidth-4);

			return;
		}

        CRect rectDlg;
	    GetClientRect(&rectDlg);
		//ScreenToClient(&rectDlg);
		m_rectMaxTree = rectDlg;
		m_rectMaxList = rectDlg;
        
		m_rectMaxTree.right = m_rectMaxTree.left + m_rectMaxTree.Width()/8;
		m_rectMaxList.left = m_rectMaxList.left + m_rectMaxList.Width()/8+2;

		m_lstLogContent.MoveWindow(&m_rectMaxList);
		m_treLogFile.MoveWindow(&m_rectMaxTree);

		UINT nWidth = m_rectMaxList.Width()/6;
		m_lstLogContent.SetColumnWidth(0,  nWidth);
	    m_lstLogContent.SetColumnWidth(1,  5*nWidth-4);
		return;


	}
	else if  (nType==SIZE_RESTORED)     
	{   
		if( (m_rectRestoredList.IsRectEmpty()) && (m_rectRestoredTree.IsRectEmpty()))
		{
			return;
		}
		else
		{
			CRect rectDlg;
			GetClientRect(&rectDlg);
			//ScreenToClient(&rectDlg);
			m_rectRestoredList = rectDlg;
			m_rectRestoredTree = rectDlg;
			
			m_rectRestoredTree.right = m_rectRestoredTree.left + m_rectRestoredTree.Width()/4;
			m_rectRestoredList.left = m_rectRestoredList.left + m_rectRestoredList.Width()/4+2;
			
			m_lstLogContent.MoveWindow(&m_rectRestoredList);
			m_treLogFile.MoveWindow(&m_rectRestoredTree);

			UINT nWidth = m_rectRestoredList.Width()/6;
			m_lstLogContent.SetColumnWidth(0,  nWidth);
	        m_lstLogContent.SetColumnWidth(1,  5*nWidth-4);
			return;
		}
	}

	// TODO: Add your message handler code here
	
}

void CLogForm::OnDestroy() 
{
	//CBCGPDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->OnCancel();
}
