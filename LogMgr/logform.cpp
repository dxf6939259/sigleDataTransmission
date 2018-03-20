/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：LogForm.cpp
* 摘    要：日志管理界面
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

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
	m_lstLogContent.InsertColumn(0, _T("流水号"), LVCFMT_LEFT, nWidth);
	m_lstLogContent.InsertColumn(1, _T("内容"), LVCFMT_LEFT, 5*nWidth-4);

	//计算初始状态的控件位置比例
	m_rectRestoredList = rect;
	m_treLogFile.GetWindowRect(&m_rectRestoredTree);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/***************************************************************
函数名：void InitTree()

  功能描述：初始化日志文件列表树形控件
  
  参数：无
	
  全局变量：无
	  
  返回值: 无
		
****************************************************************/
void CLogForm::InitTree()
{
	/************************************************************************/
	/* 遍历日志文件夹中的日志文件   日志文件集合(按日期降序排列)                                                                  */
	/************************************************************************/
	CStringList strLstLog;
	CString m_cstrFileList="";
    CFileFind tempFind;
    BOOL bFound; //判断是否成功找到文件
	
	//获得遍历路径
	TCHAR szFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
    TCHAR* pPath = _tcsrchr(szFilePath, '\\');
	*pPath = 0;
	_tcscat(szFilePath, _T("\\Log\\*.log"));
	
	
    bFound=tempFind.FindFile(szFilePath);   
	CString strTmp;   //如果找到的是文件夹存放文件夹路径
    while(bFound)      //遍历所有文件
    {
        bFound=tempFind.FindNextFile(); 
		//到下一个文件
		if(tempFind.IsDots()) 
			continue;
		
		//除去扩展名，得到日期
		strTmp=tempFind.GetFileName(); 
		strTmp.Replace(_T(".log"), _T(""));
		strLstLog.AddHead(strTmp);
    }
	
    tempFind.Close();



    /************************************************************************/
    /*       初始化日志文件列表树形控件                                                               */
    /************************************************************************/
	HTREEITEM	hRootItem = m_treLogFile.InsertItem("日志文件",TVI_ROOT);
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
		if(strYear.IsEmpty())//第一个年份节点
		{
            strYear = strLogFile.Left(4);
			hYear = m_treLogFile.InsertItem(strYear,hRootItem);
			m_treLogFile.Expand(hRootItem, TVE_EXPAND);
		}
		else if(strYear.Compare(strYearTemp)!=0)//新的年份节点
		{
			strYear = strYearTemp;
			hYear = m_treLogFile.InsertItem(strYear,hRootItem);
			m_treLogFile.Expand(hRootItem, TVE_EXPAND);
		}
		
		CString strMonthTemp = strLogFile.Mid(4,2);
		if(strMonth.IsEmpty())//第一个月份节点
		{
			strMonth = strLogFile.Mid(4,2);
			hMonth = m_treLogFile.InsertItem(strMonth,hYear);
			m_treLogFile.Expand(hYear, TVE_EXPAND);
		}
		else if(strMonth.Compare(strMonthTemp)!=0)//新的月份节点
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
	m_treLogFile.ScreenToClient(&cp);  //m_TREE是CTreeCtrl控件变量。
	
	HTREEITEM  titem = m_treLogFile.HitTest(cp,NULL); //获取当前鼠标右键单击的位置下的item
	
	CString str = m_treLogFile.GetItemText(titem); //当前被点击的节点设置给dom；
	
	//清空日志记录
	m_lstLogContent.DeleteAllItems();
	
	//当前选择了根节点，年节点，或月节点，则退出
    HTREEITEM hChild = m_treLogFile.GetChildItem(titem);
	if(NULL != hChild)
		return;
	
	HTREEITEM hMonth= m_treLogFile.GetParentItem(titem);
    HTREEITEM hYear = m_treLogFile.GetParentItem(hMonth);
	
	CString strYear = m_treLogFile.GetItemText(hYear);
	CString strMonty = m_treLogFile.GetItemText(hMonth);
	CString strDay = m_treLogFile.GetItemText(titem);
	
	//当前选择的不是日节点，则退出
	if(strDay.GetLength()!=2)
		return;
	
	CString strLogFile;
	strLogFile.Format(_T("%s%s%s.log"),strYear,strMonty,strDay);
	CStringList strLstLogContent;
	strLstLogContent.RemoveAll();
	
	//获取日志文件内容
	BOOL bRet = COutLogMgr::GetInstance()->DisplayLogContent(strLstLogContent, strLogFile);
	
	//显示日志文件内容
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
