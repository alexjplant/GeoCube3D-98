; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CALaunchView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "launch.h"
LastPage=0

ClassCount=18
Class1=CADxaWnd
Class2=CALaunchApp
Class3=CAboutDlg
Class4=CALaunchDoc
Class5=CALaunchView
Class6=CALoadDlg
Class7=CALobbyDlg
Class8=CAMainFrame
Class9=CANewDlg
Class10=CAOnlineDlg
Class11=CAOnlineMainPage
Class12=CAOnlineTypePage
Class13=CAOnlineNewPage
Class14=CAOnlineListPage
Class15=CAOnlinePlayerPage
Class16=CAOnlineSheet
Class17=CAOptionsDlg
Class18=CPreviewWnd

ResourceCount=14
Resource1=IDD_OPTIONS
Resource2=IDD_LOBBY
Resource3=CG_IDD_BAR
Resource4=IDD_LOAD_GAME
Resource5=IDR_MAINFRAME
Resource6=IDD_ONLINE_GAMES
Resource7=IDD_ABOUTBOX
Resource8=IDD_NEW_GAME
Resource9=IDD_MAINPAGE
Resource10=IDD_TYPEPAGE
Resource11=IDD_LISTPAGE
Resource12=IDD_NEWPAGE
Resource13=IDD_PLAYERPAGE
Resource14=IDD_LAUNCH_FORM

[CLS:CADxaWnd]
Type=0
BaseClass=CWnd
HeaderFile=DxaWnd.h
ImplementationFile=DxaWnd.cpp

[CLS:CALaunchApp]
Type=0
BaseClass=CWinApp
HeaderFile=Launch.h
ImplementationFile=Launch.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Launch.cpp
ImplementationFile=Launch.cpp
LastObject=CAboutDlg

[CLS:CALaunchDoc]
Type=0
BaseClass=CDocument
HeaderFile=LaunchDoc.h
ImplementationFile=LaunchDoc.cpp

[CLS:CALaunchView]
Type=0
BaseClass=CFormView
HeaderFile=LaunchView.h
ImplementationFile=LaunchView.cpp
Filter=M
VirtualFilter=VWC

[CLS:CALoadDlg]
Type=0
BaseClass=CDialog
HeaderFile=LoadDlg.h
ImplementationFile=LoadDlg.cpp

[CLS:CALobbyDlg]
Type=0
BaseClass=CDialog
HeaderFile=LobbyDlg.h
ImplementationFile=LobbyDlg.cpp

[CLS:CAMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CANewDlg]
Type=0
BaseClass=CDialog
HeaderFile=NewDlg.h
ImplementationFile=NewDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_PLAYER_NAME

[CLS:CAOnlineDlg]
Type=0
BaseClass=CDialog
HeaderFile=OnlineDlg.h
ImplementationFile=OnlineDlg.cpp

[CLS:CAOnlineMainPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=OnlinePages.h
ImplementationFile=onlinepages.cpp

[CLS:CAOnlineTypePage]
Type=0
BaseClass=CPropertyPage
HeaderFile=OnlinePages.h
ImplementationFile=onlinepages.cpp

[CLS:CAOnlineNewPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=OnlinePages.h
ImplementationFile=onlinepages.cpp

[CLS:CAOnlineListPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=OnlinePages.h
ImplementationFile=onlinepages.cpp

[CLS:CAOnlinePlayerPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=OnlinePages.h
ImplementationFile=onlinepages.cpp

[CLS:CAOnlineSheet]
Type=0
BaseClass=CPropertySheet
HeaderFile=OnlineSheet.h
ImplementationFile=OnlineSheet.cpp

[CLS:CAOptionsDlg]
Type=0
BaseClass=CDialog
HeaderFile=OptionsDlg.h
ImplementationFile=OptionsDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_MUSIC

[CLS:CPreviewWnd]
Type=0
BaseClass=CWnd
HeaderFile=PreviewWnd.h
ImplementationFile=PreviewWnd.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_LIST1,listbox,1352728835

[DLG:IDD_LAUNCH_FORM]
Type=1
Class=CALaunchView
ControlCount=1
Control1=IDC_STATIC,static,1342177806

[DLG:IDD_LOAD_GAME]
Type=1
Class=CALoadDlg
ControlCount=3
Control1=IDC_LIST1,listbox,1352728835
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[DLG:IDD_LOBBY]
Type=1
Class=CALobbyDlg
ControlCount=8
Control1=IDC_PLAYERS,listbox,1352679683
Control2=IDC_CHAT,edit,1350631552
Control3=IDB_SEND,button,1342242817
Control4=IDC_CONVERSATION,listbox,1352679683
Control5=IDOK,button,1342242816
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_NEW_GAME]
Type=1
Class=CANewDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PLAYER_NAME,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_ONLINE_GAMES]
Type=1
Class=CAOnlineDlg
ControlCount=8
Control1=IDC_JOIN,button,1342373897
Control2=IDC_CREATE,button,1342177289
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PLAYER_NAME,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SESSION_NAME,edit,1350631552
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[DLG:IDD_MAINPAGE]
Type=1
Class=CAOnlineMainPage
ControlCount=2
Control1=IDC_HOST,button,1342373897
Control2=IDC_JOIN_GAME,button,1342242825

[DLG:IDD_TYPEPAGE]
Type=1
Class=CAOnlineTypePage
ControlCount=1
Control1=IDC_LIST1,listbox,1352728835

[DLG:IDD_NEWPAGE]
Type=1
Class=CAOnlineNewPage
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SESSION,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_MAX_PLAYERS,edit,1350631552

[DLG:IDD_LISTPAGE]
Type=1
Class=CAOnlineListPage
ControlCount=1
Control1=IDC_LIST1,listbox,1352728835

[DLG:IDD_PLAYERPAGE]
Type=1
Class=CAOnlinePlayerPage
ControlCount=6
Control1=65535,static,1342308352
Control2=IDC_PLAYER,edit,1350631552
Control3=IDC_DIFFICULTY,button,1342373897
Control4=IDC_RADIO2,button,1342177289
Control5=IDC_RADIO3,button,1342177289
Control6=IDC_RADIO4,button,1342177289

[DLG:IDD_OPTIONS]
Type=1
Class=CAOptionsDlg
ControlCount=15
Control1=IDC_STATIC,button,1342308359
Control2=IDC_JOYSTICK,button,1342373897
Control3=IDC_MOUSE,button,1342242825
Control4=IDC_KEYBOARD,button,1342242825
Control5=IDC_SETUP_JOYSTICK,button,1342373888
Control6=IDC_SETUP_MOUSE,button,1342242816
Control7=IDC_SETUP_KEYBOARD,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_VIDEO_MODE,combobox,1344471043
Control10=IDC_WINDOWED,button,1342242819
Control11=IDC_MUSIC,button,1342242819
Control12=IDC_EFFECTS,button,1342242819
Control13=IDOK,button,1342242817
Control14=IDB_DEFAULTS,button,1342242816
Control15=IDCANCEL,button,1342242816

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:CG_IDD_BAR]
Type=1
Class=?
ControlCount=6
Control1=ID_FILE_NEW_GAME,button,1342242817
Control2=ID_FILE_LOAD_GAME,button,1073807360
Control3=ID_FILE_SAVE,button,1073807360
Control4=ID_FILE_ONLINEGAMES,button,1073807360
Control5=ID_SETUP_OPTIONS,button,1342242816
Control6=ID_APP_EXIT,button,1342242816

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW_GAME
Command2=ID_APP_EXIT
Command3=ID_SETUP_OPTIONS
Command4=ID_APP_ABOUT
CommandCount=4

