; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=4
Class1=CLogInApp
LastClass=CDlgPrompt
NewFileInclude2=#include "LogIn.h"
ResourceCount=3
NewFileInclude1=#include "stdafx.h"
Resource1=IDD_DLG_USER_MANAGER
Class2=CLstUser
LastTemplate=CDialog
Class3=CLstRole
Resource2=IDD_DLG_LOGIN
Class4=CDlgPrompt
Resource3=IDD_DIALOG_MESSAGE

[CLS:CLogInApp]
Type=0
HeaderFile=LogIn.h
ImplementationFile=LogIn.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=IDC_LST_USER

[DLG:IDD_DLG_LOGIN]
Type=1
Class=?
ControlCount=7
Control1=IDC_STCS_SOFT,static,1342308608
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDT_USER,edit,1350631552
Control5=IDC_EDT_PASSWORD,edit,1350631584
Control6=ID_BUT_LOGIN,button,1342242817
Control7=IDC_BUT_CANCEL,button,1342242817

[DLG:IDD_DLG_USER_MANAGER]
Type=1
Class=?
ControlCount=16
Control1=IDC_LST_USER,SysListView32,1350631425
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDT_USER,edit,1350631552
Control7=IDC_EDT_PASSWORD,edit,1350631584
Control8=IDC_EDT_CONFIRM,edit,1350631584
Control9=IDC_EDT_REMARK,edit,1350631552
Control10=IDC_BUT_SAVE,button,1342242816
Control11=IDC_LST_ROLE,SysListView32,1350631425
Control12=IDC_LST_UN_ROLE,SysListView32,1350631425
Control13=IDC_BUT_ADD,button,1342242816
Control14=IDC_BUT_REMOVE,button,1342242816
Control15=IDC_BUT_DEL,button,1342242816
Control16=IDC_BUT_NEW,button,1342242816

[CLS:CLstUser]
Type=0
HeaderFile=LstUser.h
ImplementationFile=LstUser.cpp
BaseClass=CBCGPListCtrl
Filter=W
VirtualFilter=FWC
LastObject=IDC_BUT_REMOVE

[CLS:CLstRole]
Type=0
HeaderFile=LstRole.h
ImplementationFile=LstRole.cpp
BaseClass=CBCGPListCtrl
Filter=W
VirtualFilter=FWC
LastObject=IDC_STCS_SOFT

[CLS:CDlgPrompt]
Type=0
HeaderFile=DlgPrompt.h
ImplementationFile=DlgPrompt.cpp
BaseClass=CBCGPDialog
Filter=D
LastObject=CDlgPrompt
VirtualFilter=dWC

[DLG:IDD_DIALOG_MESSAGE]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_STATIC_SHOW,static,1342308353

