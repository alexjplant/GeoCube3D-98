// OnlinePages.cpp : implementation file
#include "stdafx.h"
#include "Launch.h"
#include "OnlineSheet.h"
#include "OnlinePages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAOnlineMainPage, CPropertyPage)
BEGIN_MESSAGE_MAP(CAOnlineMainPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOnlineMainPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAOnlineMainPage::CAOnlineMainPage() : CPropertyPage(CAOnlineMainPage::IDD)
{
	//{{AFX_DATA_INIT(CAOnlineMainPage)
	m_iHostOrJoin = 0;
	//}}AFX_DATA_INIT
}

CAOnlineMainPage::~CAOnlineMainPage()
{
}

void CAOnlineMainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOnlineMainPage)
	DDX_Radio(pDX, IDC_HOST, m_iHostOrJoin);
	//}}AFX_DATA_MAP
}

BOOL CAOnlineMainPage::OnKillActive() 
{
	return CPropertyPage::OnKillActive();
}

BOOL CAOnlineMainPage::OnSetActive() 
{
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  pSheet->SetWizardButtons( PSWIZB_NEXT );

	return CPropertyPage::OnSetActive();
}

LRESULT CAOnlineMainPage::OnWizardNext() 
{
	return CPropertyPage::OnWizardNext();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAOnlineTypePage, CPropertyPage)
BEGIN_MESSAGE_MAP(CAOnlineTypePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOnlineTypePage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAOnlineTypePage::CAOnlineTypePage() : CPropertyPage(CAOnlineTypePage::IDD)
{
	//{{AFX_DATA_INIT(CAOnlineTypePage)
	m_sType = _T("");
	//}}AFX_DATA_INIT
  memset( &m_guid, 0, sizeof m_guid );
}

CAOnlineTypePage::~CAOnlineTypePage()
{
}

void CAOnlineTypePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOnlineTypePage)
	DDX_Control(pDX, IDC_LIST1, m_lbList);
	DDX_LBString(pDX, IDC_LIST1, m_sType);
	//}}AFX_DATA_MAP

  if (!pDX->m_bSaveAndValidate)
  {
    m_lbList.ResetContent();

    for (int i = 0; i < g_nSP; i++)
      m_lbList.SetItemDataPtr(
        m_lbList.AddString( g_aSP[i].desc ), &g_aSP[i].guid );

    if (m_lbList.SelectString( -1, m_sType ) == LB_ERR)
      m_lbList.SetCurSel(0);
  }
  else
  {
    if (m_lbList.GetCurSel() != LB_ERR)
      m_guid = *((GUID *)m_lbList.GetItemDataPtr( m_lbList.GetCurSel() ));
  }
}

BOOL CAOnlineTypePage::OnSetActive() 
{
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT );

	return CPropertyPage::OnSetActive();
}

LRESULT CAOnlineTypePage::OnWizardNext() 
{
  UpdateData();

  if (m_lbList.GetCurSel() == LB_ERR)
    return IDD_TYPEPAGE;

  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  if (pSheet->m_MainPage.m_iHostOrJoin == 0)
    return IDD_NEWPAGE;
  else
  if (pSheet->m_MainPage.m_iHostOrJoin == 1)
    return IDD_LISTPAGE;
  else
    return IDD_MAINPAGE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAOnlineNewPage, CPropertyPage)
BEGIN_MESSAGE_MAP(CAOnlineNewPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOnlineNewPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAOnlineNewPage::CAOnlineNewPage() : CPropertyPage(CAOnlineNewPage::IDD)
{
	//{{AFX_DATA_INIT(CAOnlineNewPage)
	m_nMaxPlayers = 10;
	m_sSession = _T("Game1");
	//}}AFX_DATA_INIT
}

CAOnlineNewPage::~CAOnlineNewPage()
{
}

void CAOnlineNewPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOnlineNewPage)
	DDX_Text(pDX, IDC_MAX_PLAYERS, m_nMaxPlayers);
	DDX_Text(pDX, IDC_SESSION, m_sSession);
	//}}AFX_DATA_MAP
}

BOOL CAOnlineNewPage::OnSetActive() 
{
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT );

	return CPropertyPage::OnSetActive();
}

LRESULT CAOnlineNewPage::OnWizardBack() 
{
  return IDD_TYPEPAGE;
}

LRESULT CAOnlineNewPage::OnWizardNext() 
{
  UpdateData();

  CWaitCursor wait;
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  if (g_pDirectPlay2)
  {
  	g_pDirectPlay2->Close();
    delete g_pDirectPlay2;
  }
  g_pDirectPlay2 = new CADirectPlay2( &pSheet->m_TypePage.m_guid );

  // create a new session
	DPSESSIONDESC2 sessionDesc;
  memset(&sessionDesc, 0, sizeof(DPSESSIONDESC2));
	sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
  sessionDesc.dwFlags =
    DPSESSION_MIGRATEHOST | DPSESSION_KEEPALIVE | DPSESSION_NODATAMESSAGES;
//  sessionDesc.guidApplication = NULL; // add CALaunchApp guid later
  sessionDesc.dwMaxPlayers = m_nMaxPlayers;
	sessionDesc.lpszSessionNameA = (LPSTR) (LPCTSTR) m_sSession;

	if (g_pDirectPlay2->Open( &sessionDesc, DPOPEN_CREATE ) != DP_OK)
    return IDD_NEWPAGE;
  else
    return IDD_PLAYERPAGE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAOnlineListPage, CPropertyPage)
BEGIN_MESSAGE_MAP(CAOnlineListPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOnlineListPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAOnlineListPage::CAOnlineListPage() : CPropertyPage(CAOnlineListPage::IDD)
{
	//{{AFX_DATA_INIT(CAOnlineListPage)
	m_sSession = _T("");
	//}}AFX_DATA_INIT
  memset( &m_guid, 0, sizeof m_guid );
}

CAOnlineListPage::~CAOnlineListPage()
{
}

void CAOnlineListPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOnlineListPage)
	DDX_Control(pDX, IDC_LIST1, m_lbList);
	DDX_LBString(pDX, IDC_LIST1, m_sSession);
	//}}AFX_DATA_MAP

  if (!pDX->m_bSaveAndValidate)
  {
    CWaitCursor wait;
    CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
    if (g_pDirectPlay2) delete g_pDirectPlay2;
    g_pDirectPlay2 = new CADirectPlay2( &pSheet->m_TypePage.m_guid );
    ASSERT(g_pDirectPlay2);

    g_pDirectPlay2->mEnumerateSessions(0);

    m_lbList.ResetContent();

    for (int i = 0; i < g_nSPI; i++)
      m_lbList.SetItemDataPtr(
        m_lbList.AddString( g_aSPI[i].desc ), &g_aSPI[i].guid );

    if (m_lbList.SelectString( -1, m_sSession ) == LB_ERR)
      m_lbList.SetCurSel(0);
  }
  else
  {
    if (m_lbList.GetCurSel() != LB_ERR)
      m_guid = *((GUID *)m_lbList.GetItemDataPtr( m_lbList.GetCurSel() ));
  }
}

BOOL CAOnlineListPage::OnSetActive() 
{
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT );

	return CPropertyPage::OnSetActive();
}

LRESULT CAOnlineListPage::OnWizardBack() 
{
  return IDD_TYPEPAGE;
}

LRESULT CAOnlineListPage::OnWizardNext() 
{
  UpdateData();

  if (m_lbList.GetCurSel() == LB_ERR)
    return IDD_LISTPAGE;

  CWaitCursor wait;
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();

  // create a new session
	DPSESSIONDESC2 sessionDesc;
  memset(&sessionDesc, 0, sizeof(DPSESSIONDESC2));
	sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
  sessionDesc.dwFlags = DPSESSION_MIGRATEHOST | DPSESSION_KEEPALIVE;
//  sessionDesc.guidApplication = NULL; // add CALaunchApp guid later
  sessionDesc.guidInstance = m_guid;

	if (g_pDirectPlay2->Open( &sessionDesc, DPOPEN_JOIN ) != DP_OK)
    return IDD_NEWPAGE;
  else
    return IDD_PLAYERPAGE;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAOnlinePlayerPage, CPropertyPage)
BEGIN_MESSAGE_MAP(CAOnlinePlayerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOnlinePlayerPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAOnlinePlayerPage::CAOnlinePlayerPage() : CPropertyPage(CAOnlinePlayerPage::IDD)
{
	//{{AFX_DATA_INIT(CAOnlinePlayerPage)
	m_sPlayer = _T("");
	//}}AFX_DATA_INIT
}

CAOnlinePlayerPage::~CAOnlinePlayerPage()
{
}

void CAOnlinePlayerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOnlinePlayerPage)
	DDX_Text(pDX, IDC_PLAYER, m_sPlayer);
	//}}AFX_DATA_MAP
}

BOOL CAOnlinePlayerPage::OnSetActive() 
{
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();
  pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH );

	return CPropertyPage::OnSetActive();
}

BOOL CAOnlinePlayerPage::OnWizardFinish() 
{
  UpdateData();

  CWaitCursor wait;
  CAOnlineSheet *pSheet = (CAOnlineSheet *) GetParent();

  // create a new player
	DPNAME dpName;
	memset(&dpName, 0, sizeof(DPNAME));
	dpName.dwSize = sizeof(DPNAME);
	dpName.lpszShortNameA = (LPSTR) (LPCTSTR) pSheet->m_PlayerPage.m_sPlayer;
	dpName.lpszLongNameA = (LPSTR) (LPCTSTR) pSheet->m_PlayerPage.m_sPlayer;

	// create a player with this name
	if (g_pPlayer)
  {
    g_pDirectPlay2->I()->DestroyPlayer( g_pPlayer->m_dpidPlayer );
    delete g_pPlayer;
  }

  g_pPlayer = new CAPlayer();
  strcpy(g_pPlayer->m_szName, pSheet->m_PlayerPage.m_sPlayer);
  g_pPlayer->SetupConnection();

	if (g_pDirectPlay2->CreatePlayer(&g_pPlayer->m_dpidPlayer, &dpName, g_pPlayer->m_hPlayerEvent, NULL, 0, 0) == DP_OK)
    return TRUE;
  else
    return FALSE;
}
