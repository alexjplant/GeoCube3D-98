#include "dxaplay.h"

DPID g_dpidPlayer;
int g_nPlayers = 0;
DirectPlayer g_aPlayers[20];

int g_nSP = 0;
DirectPlaySP g_aSP[20];

int g_nSPI = 0;
DirectPlaySP g_aSPI[20];

static BOOL FAR PASCAL DirectPlayEnumerateCallback(
						              LPGUID		lpSPGuid,
						              LPTSTR		lpszSPName,
						              DWORD		  dwMajorVersion,
						              DWORD		  dwMinorVersion,
						              LPVOID		lpContext)
{
  g_aSP[ g_nSP ].guid = *lpSPGuid;
  wsprintf(g_aSP[ g_nSP ].desc, "%s %d.%d", lpszSPName, dwMajorVersion, dwMinorVersion);
  g_aSP[ g_nSP ].desc[79] = '\0';
  g_nSP++;

  return TRUE;
}

static BOOL FAR PASCAL DirectPlayEnumSessionsCallback(
						LPCDPSESSIONDESC2	lpSessionDesc,
						LPDWORD				lpdwTimeOut,
						DWORD	  			dwFlags,
						LPVOID				lpContext)
{
	// see if last session has been enumerated
  if (dwFlags & DPESC_TIMEDOUT)
		return FALSE;						

  g_aSPI[ g_nSPI ].guid = lpSessionDesc->guidInstance;
  wsprintf(g_aSPI[ g_nSPI ].desc, "%s", lpSessionDesc->lpszSessionNameA);
  g_aSPI[ g_nSPI ].desc[79] = '\0';
  g_nSPI++;

  return TRUE;
}

static BOOL WINAPI DirectPlayEnumPlayersCallback(DPID dpId,
                      DWORD dwPlayerType, LPCDPNAME lpName,
                      DWORD dwFlags, LPVOID lpContext)
{
	// see if last session has been enumerated
  if (dwFlags & DPESC_TIMEDOUT)
		return FALSE;

  g_aPlayers[ g_nPlayers ].dpid = dpId;
  g_aPlayers[ g_nPlayers ].dwPlayerType = dwPlayerType;
  wsprintf(g_aPlayers[ g_nPlayers ].desc, "%s", lpName->lpszLongNameA);
  g_nPlayers++;

  return TRUE;
}

int CADirectPlay2::mEnumerate()
{
  g_nSP = 0;
  DirectPlayEnumerate( DirectPlayEnumerateCallback, NULL);
  return g_nSP;
}

int CADirectPlay2::mEnumerateSessions( DWORD dwFlags )
{
	DPSESSIONDESC2 dpdesc;
  memset(&dpdesc, 0, sizeof(DPSESSIONDESC2));
	dpdesc.dwSize = sizeof(DPSESSIONDESC2);

  g_nSPI = 0;
  EnumSessions( &dpdesc, 5000, DirectPlayEnumSessionsCallback, NULL, dwFlags );
  return g_nSPI;
}

int CADirectPlay2::mEnumeratePlayers( DWORD dwFlags )
{
  g_nPlayers = 0;

  EnumPlayers( NULL, DirectPlayEnumPlayersCallback, this, dwFlags );

  return g_nPlayers;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectPlay2::CADirectPlay2(LPGUID lpguid) : CADirectXObject()
{
  Check( DirectPlayCreate(lpguid, &m_lpDP, NULL) );

  Check( m_lpDP->QueryInterface(IID_IDirectPlay2, (LPVOID *)&m_pIDirectPlay2) );
}

CADirectPlay2::~CADirectPlay2()
{
  if (m_pIDirectPlay2) m_pIDirectPlay2->Release();
  if (m_lpDP) m_lpDP->Release();
}


HRESULT CADirectPlay2::AddPlayerToGroup(DPID idGroup, DPID idPlayer)
{
  return Check( m_pIDirectPlay2->AddPlayerToGroup( idGroup,  idPlayer) );
}

HRESULT CADirectPlay2::Close()
{
  return Check( m_pIDirectPlay2->Close() );
}

HRESULT CADirectPlay2::CreateGroup(LPDPID lpidGroup,
      LPDPNAME lpGroupName, LPVOID lpData, 
      DWORD dwDataSize, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->CreateGroup( lpidGroup,
       lpGroupName,  lpData, 
       dwDataSize,  dwFlags) );
}

HRESULT CADirectPlay2::CreatePlayer(LPDPID lpidPlayer,
      LPDPNAME lpPlayerName, HANDLE hEvent,
      LPVOID lpData, DWORD dwDataSize, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->CreatePlayer( lpidPlayer,
       lpPlayerName,  hEvent,
       lpData,  dwDataSize,  dwFlags) );
}

HRESULT CADirectPlay2::DeletePlayerFromGroup(DPID idGroup,
      DPID idPlayer)
{
  return Check( m_pIDirectPlay2->DeletePlayerFromGroup( idGroup,
       idPlayer) );
}

HRESULT CADirectPlay2::DestroyGroup(DPID idGroup)
{
  return Check( m_pIDirectPlay2->DestroyGroup( idGroup) );
}

HRESULT CADirectPlay2::DestroyPlayer(DPID idPlayer)
{
  return Check( m_pIDirectPlay2->DestroyPlayer( idPlayer) );
}

HRESULT CADirectPlay2::EnumGroupPlayers(DPID idGroup,
      LPGUID lpguidInstance,
      LPDPENUMPLAYERSCALLBACK2 lpEnumPlayersCallback2,
      LPVOID lpContext, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->EnumGroupPlayers( idGroup,
       lpguidInstance,
       lpEnumPlayersCallback2,
       lpContext,  dwFlags) );
}

HRESULT CADirectPlay2::EnumGroups(LPGUID lpguidInstance,
      LPDPENUMPLAYERSCALLBACK2 lpEnumPlayersCallback2,
      LPVOID lpContext, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->EnumGroups( lpguidInstance,
       lpEnumPlayersCallback2,
       lpContext,  dwFlags) );
}

HRESULT CADirectPlay2::EnumPlayers(LPGUID lpguidInstance,
      LPDPENUMPLAYERSCALLBACK2 lpEnumPlayersCallback2,
      LPVOID lpContext, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->EnumPlayers( lpguidInstance,
       lpEnumPlayersCallback2,
       lpContext,  dwFlags) );
}

HRESULT CADirectPlay2::EnumSessions(LPDPSESSIONDESC2 lpsd,
      DWORD dwTimeout,
      LPDPENUMSESSIONSCALLBACK2 lpEnumSessionsCallback2,
      LPVOID lpContext, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->EnumSessions( lpsd,
       dwTimeout,
       lpEnumSessionsCallback2,
       lpContext,  dwFlags) );
}

HRESULT CADirectPlay2::GetCaps(LPDPCAPS lpDPCaps,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->GetCaps( lpDPCaps,
       dwFlags) );
}

HRESULT CADirectPlay2::GetGroupData(DPID idGroup,
      LPVOID lpData, LPDWORD lpdwDataSize,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->GetGroupData( idGroup,
       lpData,  lpdwDataSize,
       dwFlags) );
}

HRESULT CADirectPlay2::GetGroupName(DPID idGroup,
      LPVOID lpData, LPDWORD lpdwDataSize)
{
  return Check( m_pIDirectPlay2->GetGroupName( idGroup,
       lpData,  lpdwDataSize) );
}

HRESULT CADirectPlay2::GetMessageCount(DPID idPlayer, LPDWORD lpdwCount)
{
  return Check( m_pIDirectPlay2->GetMessageCount( idPlayer,  lpdwCount) );
}

HRESULT CADirectPlay2::GetPlayerAddress(DPID idPlayer,
      LPVOID lpAddress, LPDWORD lpdwAddressSize)
{
  return Check( m_pIDirectPlay2->GetPlayerAddress( idPlayer,
       lpAddress,  lpdwAddressSize) );
}

HRESULT CADirectPlay2::GetPlayerCaps(DPID idPlayer,
      LPDPCAPS lpPlayerCaps, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->GetPlayerCaps( idPlayer,
       lpPlayerCaps,  dwFlags) );
}

HRESULT CADirectPlay2::GetPlayerData(DPID idPlayer,
      LPVOID lpData, LPDWORD lpdwDataSize,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->GetPlayerData( idPlayer,
       lpData,  lpdwDataSize,
       dwFlags) );
}

HRESULT CADirectPlay2::GetPlayerName(DPID idPlayer,
      LPVOID lpData, LPDWORD lpdwDataSize)
{
  return Check( m_pIDirectPlay2->GetPlayerName( idPlayer,
       lpData,  lpdwDataSize) );
}

HRESULT CADirectPlay2::GetSessionDesc(LPVOID lpData,
      LPDWORD lpdwDataSize)
{
  return Check( m_pIDirectPlay2->GetSessionDesc( lpData,
       lpdwDataSize) );
}

HRESULT CADirectPlay2::Initialize(LPGUID lpGUID)
{
  return Check( m_pIDirectPlay2->Initialize( lpGUID) );
}

HRESULT CADirectPlay2::Open(LPDPSESSIONDESC2 lpsd,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->Open( lpsd,
       dwFlags) );
}

HRESULT CADirectPlay2::Receive(LPDPID lpidFrom, LPDPID lpidTo,
      DWORD dwFlags, LPVOID lpData, LPDWORD lpdwDataSize)
{
  return Check( m_pIDirectPlay2->Receive( lpidFrom,  lpidTo,
       dwFlags,  lpData,  lpdwDataSize) );
}

HRESULT CADirectPlay2::Send(DPID idFrom, DPID idTo, DWORD dwFlags,
      LPVOID lpData, DWORD dwDataSize)
{
  return Check( m_pIDirectPlay2->Send( idFrom,  idTo,  dwFlags,
       lpData,  dwDataSize) );
}

HRESULT CADirectPlay2::SetGroupData(DPID idGroup,
      LPVOID lpData, DWORD dwDataSize,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->SetGroupData( idGroup,
       lpData,  dwDataSize,
       dwFlags) );
}

HRESULT CADirectPlay2::SetGroupName(DPID idGroup,
      LPDPNAME lpGroupName, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->SetGroupName( idGroup,
       lpGroupName,  dwFlags) );
}

HRESULT CADirectPlay2::SetPlayerData(DPID idPlayer, LPVOID lpData, 
      DWORD dwDataSize, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->SetPlayerData( idPlayer,  lpData, 
       dwDataSize,  dwFlags) );
}

HRESULT CADirectPlay2::SetPlayerName(DPID idPlayer,
      LPDPNAME lpPlayerName, DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->SetPlayerName( idPlayer,
       lpPlayerName,  dwFlags) );
}

HRESULT CADirectPlay2::SetSessionDesc(LPDPSESSIONDESC2 lpSessDesc,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlay2->SetSessionDesc( lpSessDesc,
       dwFlags) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectPlayLobby::CADirectPlayLobby() : CADirectXObject()
{
  Check( DirectPlayLobbyCreate(NULL, &m_pIDirectPlayLobby, NULL, NULL, 0) );
}

CADirectPlayLobby::~CADirectPlayLobby()
{
  if (m_pIDirectPlayLobby) m_pIDirectPlayLobby->Release();
}


HRESULT CADirectPlayLobby::Connect(DWORD dwFlags,
      LPDIRECTPLAY2 FAR *lplpDP, IUnknown FAR *pUnk)
{
  return Check( m_pIDirectPlayLobby->Connect( dwFlags,
       lplpDP,  pUnk) );
}

HRESULT CADirectPlayLobby::CreateAddress(REFGUID guidSP,
      REFGUID guidDataType, LPCVOID lpData,
      DWORD dwDataSize, LPVOID lpAddress,
      LPDWORD lpdwAddressSize)
{
  return Check( m_pIDirectPlayLobby->CreateAddress( guidSP,
       guidDataType,  lpData,
       dwDataSize,  lpAddress,
       lpdwAddressSize) );
}

HRESULT CADirectPlayLobby::EnumAddress(LPDPENUMADDRESSCALLBACK lpEnumAddressCallback,
      LPCVOID lpAddress, DWORD dwAddressSize,
      LPVOID lpContext)
{
  return Check( m_pIDirectPlayLobby->EnumAddress( lpEnumAddressCallback,
       lpAddress,  dwAddressSize,
       lpContext) );
}

HRESULT CADirectPlayLobby::EnumAddressTypes(
      LPDPLENUMADDRESSTYPESCALLBACK lpEnumAddressTypeCallback,
      REFGUID guidSP, LPVOID lpContext,
      DWORD dwFlags)
{
  return Check( m_pIDirectPlayLobby->EnumAddressTypes(
       lpEnumAddressTypeCallback,
       guidSP,  lpContext,
       dwFlags) );
}

HRESULT CADirectPlayLobby::EnumLocalApplications(
      LPDPLENUMLOCALAPPLICATIONSCALLBACK lpEnumLocalAppCallback,
      LPVOID lpContext, DWORD dwFlags)
{
  return Check( m_pIDirectPlayLobby->EnumLocalApplications(
       lpEnumLocalAppCallback,
       lpContext,  dwFlags) );
}

HRESULT CADirectPlayLobby::GetConnectionSettings(DWORD dwAppID,
      LPVOID lpData, LPDWORD lpdwDataSize)
{
  return Check( m_pIDirectPlayLobby->GetConnectionSettings( dwAppID,
       lpData,  lpdwDataSize) );
}

HRESULT CADirectPlayLobby::ReceiveLobbyMessage(DWORD dwFlags,
      DWORD dwAppID, LPDWORD lpdwMessageFlags,
      LPVOID lpData, LPDWORD lpdwDataSize)
{
  return Check( m_pIDirectPlayLobby->ReceiveLobbyMessage( dwFlags,
       dwAppID,  lpdwMessageFlags,
       lpData,  lpdwDataSize) );
}

HRESULT CADirectPlayLobby::RunApplication(DWORD dwFlags,
      LPDWORD lpdwAppID, LPDPLCONNECTION lpConn,
      HANDLE hReceiveEvent)
{
  return Check( m_pIDirectPlayLobby->RunApplication( dwFlags,
       lpdwAppID,  lpConn,
       hReceiveEvent) );
}

HRESULT CADirectPlayLobby::SendLobbyMessage( DWORD dwFlags,
      DWORD dwAppID, LPVOID lpData,
      DWORD dwDataSize)
{
  return Check( m_pIDirectPlayLobby->SendLobbyMessage(  dwFlags,
       dwAppID,  lpData,
       dwDataSize) );
}

HRESULT CADirectPlayLobby::SetConnectionSettings(DWORD dwFlags,
      DWORD dwAppID, LPDPLCONNECTION lpConn)
{
  return Check( m_pIDirectPlayLobby->SetConnectionSettings( dwFlags,
       dwAppID,  lpConn) );
}

HRESULT CADirectPlayLobby::SetLobbyMessageEvent(DWORD dwFlags,
      DWORD dwAppID, HANDLE hReceiveEvent)
{
  return Check( m_pIDirectPlayLobby->SetLobbyMessageEvent( dwFlags,
       dwAppID,  hReceiveEvent) );
}

