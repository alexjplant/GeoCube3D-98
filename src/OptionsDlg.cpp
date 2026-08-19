// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launch.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAOptionsDlg dialog


CAOptionsDlg::CAOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAOptionsDlg)
	m_bEffects = TRUE;
	m_bMusic = TRUE;
	m_bWindowed = FALSE;
	m_iController = 0;
	//}}AFX_DATA_INIT
  m_iVideoMode = 0;
}


void CAOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOptionsDlg)
	DDX_Control(pDX, IDC_VIDEO_MODE, m_cbVideoMode);
	DDX_Check(pDX, IDC_EFFECTS, m_bEffects);
	DDX_Check(pDX, IDC_MUSIC, m_bMusic);
	DDX_Check(pDX, IDC_WINDOWED, m_bWindowed);
	DDX_Radio(pDX, IDC_JOYSTICK, m_iController);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
  {
    m_iVideoMode = m_cbVideoMode.GetCurSel();
    m_iModeX = g_pDirectDraw->m_ddescMode[ m_iVideoMode ].dwWidth;
    m_iModeY = g_pDirectDraw->m_ddescMode[ m_iVideoMode ].dwHeight;
    m_iModeBPP= g_pDirectDraw->m_ddescMode[ m_iVideoMode ].ddpfPixelFormat.dwRGBBitCount;
  }
  else
  {
    m_cbVideoMode.ResetContent();
    m_iVideoMode = 0;
    for (int i = 0; i < g_pDirectDraw->m_nModes; i++)
    {
      int width = g_pDirectDraw->m_ddescMode[i].dwWidth;
      int height = g_pDirectDraw->m_ddescMode[i].dwHeight;
      int BPP = g_pDirectDraw->m_ddescMode[i].ddpfPixelFormat.dwRGBBitCount;
      CString sDesc;
      sDesc.Format( "%ld X %ld X %ld", width, height, BPP);
      m_cbVideoMode.AddString( sDesc );

      if ((m_iModeX == width) && (m_iModeY == height) && (m_iModeBPP == BPP))
        m_iVideoMode = i;
    }
  	m_cbVideoMode.SetCurSel( m_iVideoMode );
  }
}


BEGIN_MESSAGE_MAP(CAOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CAOptionsDlg)
	ON_BN_CLICKED(IDB_DEFAULTS, OnDefaults)
	ON_BN_CLICKED(IDC_SETUP_JOYSTICK, OnSetupJoystick)
	ON_BN_CLICKED(IDC_SETUP_KEYBOARD, OnSetupKeyboard)
	ON_BN_CLICKED(IDC_SETUP_MOUSE, OnSetupMouse)
	ON_BN_CLICKED(IDC_MUSIC, OnMusic)
	ON_BN_CLICKED(IDC_EFFECTS, OnEffects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAOptionsDlg message handlers

void CAOptionsDlg::OnDefaults() 
{
	m_bEffects = TRUE;
	m_bMusic = TRUE;
	m_bWindowed = FALSE;
	m_iController = 0;
	m_iVideoMode = 0;
	m_cbVideoMode.SetCurSel( m_iVideoMode );

  UpdateData(FALSE);
}

void CAOptionsDlg::OnSetupJoystick() 
{
  g_pDirectInput->RunControlPanel( *this, 0 );
}

void CAOptionsDlg::OnSetupKeyboard() 
{
	CADirectInputDevice keyboardDev( g_pDirectInput, GUID_SysKeyboard);
  keyboardDev.RunControlPanel( *this, 0 );
}

void CAOptionsDlg::OnSetupMouse() 
{
	CADirectInputDevice mouseDev( g_pDirectInput, GUID_SysMouse);
  mouseDev.RunControlPanel( *this, 0 );
}

void CAOptionsDlg::OnMusic() 
{
}

void CAOptionsDlg::OnEffects() 
{
}
