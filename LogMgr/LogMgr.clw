; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=3
Class1=CLogMgrApp
LastClass=CLogForm
NewFileInclude2=#include "LogMgr.h"
ResourceCount=1
NewFileInclude1=#include "stdafx.h"
Class2=CLogForm
LastTemplate=CTreeCtrl
Class3=CLogMgrTree
Resource1=IDD_DLG_LOGMGR

[CLS:CLogMgrApp]
Type=0
HeaderFile=LogMgr.h
ImplementationFile=LogMgr.cpp
Filter=N
LastObject=CLogMgrApp
BaseClass=CWinApp
VirtualFilter=AC

[DLG:IDD_DLG_LOGMGR]
Type=1
Class=CLogForm
ControlCount=2
Control1=IDC_TREE_LOG_FILE,SysTreeView32,1350631427
Control2=IDC_LST_LOG_CONTENT,SysListView32,1350631425

[CLS:CLogMgrTree]
Type=0
HeaderFile=LogMgrTree.h
ImplementationFile=LogMgrTree.cpp
BaseClass=CTreeCtrl
Filter=W
VirtualFilter=GWC

[CLS:CLogForm]
Type=0
HeaderFile=logform.h
ImplementationFile=logform.cpp
BaseClass=CBCGPDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON2

