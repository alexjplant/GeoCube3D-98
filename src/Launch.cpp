// Launch.cpp : Defines the class behaviors for the application.
//
#define INITGUID  // defines the GUID constants
#include "stdafx.h"
#include <afxdisp.h>

#include "Launch.h"

#include "MainFrm.h"
#include "LaunchDoc.h"
#include "LaunchView.h"
#include "NewDlg.h"
#include "LoadDlg.h"
#include "OptionsDlg.h"
#include "LobbyDlg.h"

#include "gameplay.h"
#include "OnlineSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CADirectInput *g_pDirectInput = NULL;
CADirectSound *g_pDirectSound = NULL;
CADirectDraw  *g_pDirectDraw = NULL;
CADirect3DRM  *g_pDirect3DRM = NULL;
CADirectPlay2 *g_pDirectPlay2 = NULL;

D3DVALUE       g_valFrameRatio;
DWORD          g_dwFrames;
DWORD          g_dwFrameCount;
DWORD          g_dwFrameTime;
DWORD          g_dwLastTick;
DWORD          g_dwDiffTime;
  
CAScene       *g_pInvisible;

/////////////////////////////////////////////////////////////////////////////
// CALaunchApp

BEGIN_MESSAGE_MAP(CALaunchApp, CWinApp)
	//{{AFX_MSG_MAP(CALaunchApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SETUP_OPTIONS, OnSetupOptions)
	ON_UPDATE_COMMAND_UI(ID_SETUP_OPTIONS, OnUpdateSetupOptions)
	ON_COMMAND(ID_FILE_NEW_GAME, OnFileNewGame)
	ON_COMMAND(ID_FILE_LOAD_GAME, OnFileLoadGame)
	ON_COMMAND(ID_FILE_ONLINEGAMES, OnFileOnlinegames)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALaunchApp construction

CALaunchApp::CALaunchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CALaunchApp object

CALaunchApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CALaunchApp initialization

BOOL CALaunchApp::InitInstance()
{
  srand( (unsigned)time( NULL ) );

  AfxOleInit();
  AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey(_T("Weston Software"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  GetProfile();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CALaunchDoc),
		RUNTIME_CLASS(CAMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CALaunchView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

  InitDirectX();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();


	return TRUE;
}


BOOL CALaunchApp::InitDirectX() 
{
  try
  {
    g_pDirectInput = new CADirectInput(AfxGetInstanceHandle());

    g_pDirectSound = new CADirectSound;

    g_pDirectDraw = new CADirectDraw;  // add FALSE for hardware

    g_pPlayerList = new CAPlayerList;

    CADirectPlay2::mEnumerate();

    g_pDirect3DRM = new CADirect3DRM;
  }
  catch (DxError e)
  {
    e;
  	return FALSE;
  }

	return TRUE;
}


int CALaunchApp::ExitInstance() 
{
  ExitDirectX();
	
	return CWinApp::ExitInstance();
}


BOOL CALaunchApp::ExitDirectX() 
{
  delete g_pDirectInput;	
  delete g_pDirectSound;	

  while (g_pPlayerList->GetCount())
    delete g_pPlayerList->RemoveTail();
  delete g_pPlayerList;
  delete g_pDirectPlay2;	

  delete g_pDirectDraw;	
  delete g_pDirect3DRM;	

  return TRUE;
}


BOOL CALaunchApp::GetProfile()
{
	m_sPlayerName = GetProfileString( INI_SETTINGS, INI_PLAYER_NAME, "" );
	m_bEffects = GetProfileInt( INI_SETTINGS, INI_EFFECTS, TRUE );
	m_bMusic = GetProfileInt( INI_SETTINGS, INI_MUSIC, TRUE );
	m_bWindowed = GetProfileInt( INI_SETTINGS, INI_WINDOWED, FALSE );
	m_iModeX = GetProfileInt( INI_SETTINGS, INI_MODE_X, 640 );
	m_iModeY = GetProfileInt( INI_SETTINGS, INI_MODE_Y, 480 );
	m_iModeBPP = GetProfileInt( INI_SETTINGS, INI_MODE_BPP, 16 );

  for (int i = 0; i < 10; i++)
  {
    CString sEntry;
    sEntry.Format("HighScore%ld", i);
    m_aHighScores[i] = GetProfileString( INI_SETTINGS, sEntry, "" );
  }

  return TRUE;
}

BOOL CALaunchApp::SetProfile()
{
	WriteProfileString( INI_SETTINGS, INI_PLAYER_NAME, m_sPlayerName );
	WriteProfileInt( INI_SETTINGS, INI_EFFECTS, m_bEffects );
	WriteProfileInt( INI_SETTINGS, INI_MUSIC, m_bMusic );
	WriteProfileInt( INI_SETTINGS, INI_WINDOWED, m_bWindowed );
	WriteProfileInt( INI_SETTINGS, INI_MODE_X, m_iModeX );
	WriteProfileInt( INI_SETTINGS, INI_MODE_Y, m_iModeY );
	WriteProfileInt( INI_SETTINGS, INI_MODE_BPP, m_iModeBPP );

  for (int i = 0; i < 10; i++)
  {
    CString sEntry;
    sEntry.Format("HighScore%ld", i);
    WriteProfileString( INI_SETTINGS, sEntry, m_aHighScores[i] );
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CALaunchApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CALaunchApp::OnSetupOptions() 
{
  CAOptionsDlg dlg;

  dlg.m_bWindowed =   m_bWindowed;
  dlg.m_iModeX =      m_iModeX;
  dlg.m_iModeY =      m_iModeY;
  dlg.m_iModeBPP =    m_iModeBPP;
  dlg.m_bEffects =    m_bEffects;
  dlg.m_bMusic =      m_bMusic;

  if (dlg.DoModal() == IDOK)
  {
    m_bWindowed =     dlg.m_bWindowed;   
    m_iModeX =        dlg.m_iModeX;      
    m_iModeY =        dlg.m_iModeY;      
    m_iModeBPP =      dlg.m_iModeBPP;    
    m_bEffects =      dlg.m_bEffects;    
    m_bMusic =        dlg.m_bMusic;      

    SetProfile();
  }
}

void CALaunchApp::OnUpdateSetupOptions(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable();
}


void CALaunchApp::OnFileNewGame() 
{
  CANewDlg dlg;
  dlg.m_sPlayerName = m_sPlayerName;

  if (dlg.DoModal() == IDOK)
  {
    m_sPlayerName = dlg.m_sPlayerName;

    RunGame();
  }
}

void CALaunchApp::OnFileLoadGame() 
{
  CALoadDlg dlg;

  if (dlg.DoModal() == IDOK)
  {
    // read a game from a file

    RunGame();
  }
}


void CALaunchApp::OnFileOnlinegames() 
{
	CAOnlineSheet online;

  if (online.DoModal() == ID_WIZFINISH)
  {
    // read a game from a file

    // show the lobby while waiting to start the game
    CALobbyDlg dlg;

    if (dlg.DoModal() == IDOK)
      RunGame();
  }
}

BOOL CALaunchApp::RunGame()
{
  AfxGetMainWnd()->ShowWindow(SW_HIDE);

  m_pGameWnd = new CAGamePlay();
  m_pGameWnd->Create( NULL, !m_bWindowed, (CWnd **) &m_pGameWnd );

  MSG msg;
  while( TRUE )
  {
    if (TRUE) // m_bActive)
    {
      // any windows messages ? (returns immediately)
      if( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
      {
        if( !::GetMessage( &msg, NULL, 0, 0 ) )
        {
          break; // quit
        }
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
      }
      else
      {
        if (!m_pGameWnd)
          break;
        
        if (!IsWindow( *m_pGameWnd ))
          break;

        if (!m_pGameWnd->UpdateFrame())
        {
          m_pGameWnd->DestroyWindow();
          break;
        }
      }
    }
    else  // inactive
    {
      // any windows messages ? (blocks until a message arrives)
      if( !::GetMessage( &msg, NULL, 0, 0 ) )
      {
        break;
      }
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }

  AfxGetMainWnd()->ShowWindow(SW_SHOW);

  return TRUE;
}
