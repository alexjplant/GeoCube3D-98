#include "stdafx.h"
#include "launch.h"
#include "LobbyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALobbyDlg dialog


CALobbyDlg::CALobbyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CALobbyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CALobbyDlg)
	m_sChat = _T("");
	//}}AFX_DATA_INIT
}

CALobbyDlg::~CALobbyDlg()
{
  g_pPlayer->m_hwndOwner = NULL;
}


void CALobbyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CALobbyDlg)
	DDX_Control(pDX, IDC_CHAT, m_ctlChat);
	DDX_Control(pDX, IDC_PLAYERS, m_lbPlayers);
	DDX_Control(pDX, IDC_CONVERSATION, m_lbConversation);
	DDX_Text(pDX, IDC_CHAT, m_sChat);
	//}}AFX_DATA_MAP

  if (!pDX->m_bSaveAndValidate)
  {
    LoadPlayers();
  }
}

void CALobbyDlg::LoadPlayers()
{
  CWaitCursor wait;
  if (g_pDirectPlay2)
  {
    m_lbPlayers.ResetContent();

    g_pDirectPlay2->mEnumeratePlayers(0);

    while (g_pPlayerList->GetCount())
      delete g_pPlayerList->RemoveTail();

    for (int i = 0; i < g_nPlayers; i++)
    {
      CAPlayer *pPlayer = new CAPlayer();
      pPlayer->m_dpidPlayer = g_aPlayers[i].dpid;
      pPlayer->m_dwPlayerType = g_aPlayers[i].dwPlayerType;
      strcpy(pPlayer->m_szName, g_aPlayers[i].desc);

      g_pPlayerList->AddTail( pPlayer );

      m_lbPlayers.AddString( pPlayer->m_szName );
    }
  }
}

BEGIN_MESSAGE_MAP(CALobbyDlg, CDialog)
	//{{AFX_MSG_MAP(CALobbyDlg)
	ON_BN_CLICKED(IDB_SEND, OnSend)
	//}}AFX_MSG_MAP
  ON_MESSAGE( WMDP_CHATSTRING, OnDpChatString )
  ON_MESSAGE( WMDPSYS_CREATEPLAYERORGROUP, OnDpCreatePlayerOrGroup )
  ON_MESSAGE( WMDPSYS_DESTROYPLAYERORGROUP, OnDpDestroyPlayerOrGroup )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALobbyDlg message handlers

void CALobbyDlg::OnSend() 
{
  UpdateData();
  
  if (m_sChat.GetLength())
  {
    CString sMsg = g_pPlayer->m_szName;
    sMsg += ">  ";
    sMsg += m_sChat;
    m_lbConversation.InsertString( 0, sMsg );
    m_ctlChat.SetFocus();
    m_ctlChat.SetWindowText("");

    g_pPlayer->SendChatMessage( sMsg );
  }
}

BOOL CALobbyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  g_pPlayer->m_hwndOwner = *this;
  
	return TRUE;
}

LONG CALobbyDlg::OnDpChatString( UINT wParam, LONG lParam )
{
  DPAPPMSG_CHATSTRING *lpChat = (DPAPPMSG_CHATSTRING *) lParam;

  if (strlen( lpChat->szMsg ))
    m_lbConversation.InsertString( 0, lpChat->szMsg );

  return 0L;
}

LONG CALobbyDlg::OnDpCreatePlayerOrGroup( UINT wParam, LONG lParam )
{
  LoadPlayers();
  return 0L;
}

LONG CALobbyDlg::OnDpDestroyPlayerOrGroup( UINT wParam, LONG lParam )
{
  LoadPlayers();
  return 0L;
}

