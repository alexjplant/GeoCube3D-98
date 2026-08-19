#ifndef __DXAPLAY_H__
#define __DXAPLAY_H__

#include "dxaobj.h"

#include "dplay.h"  // Direct X
#include "dplobby.h"

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
extern DPID g_dpidPlayer;

typedef struct
{
  DPID  dpid;
  DWORD dwPlayerType;
  char  desc[80];
} DirectPlayer;

extern int g_nPlayers;
extern DirectPlayer g_aPlayers[];

typedef struct
{
  GUID guid;
  char desc[80];
} DirectPlaySP;

extern int g_nSP;  // available service providers
extern DirectPlaySP g_aSP[];

extern int g_nSPI;  // active sessions
extern DirectPlaySP g_aSPI[];

class CADirectPlay2 : public CADirectXObject
{
public:               
  CADirectPlay2(LPGUID lpguid);
  ~CADirectPlay2();

  IDirectPlay2 *I() { return m_pIDirectPlay2; };

  HRESULT AddPlayerToGroup(DPID idGroup, DPID idPlayer);

  HRESULT Close();

  HRESULT CreateGroup(LPDPID lpidGroup,
      LPDPNAME lpGroupName, LPVOID lpData, 
      DWORD dwDataSize, DWORD dwFlags);

  HRESULT CreatePlayer(LPDPID lpidPlayer,
      LPDPNAME lpPlayerName, HANDLE hEvent,
      LPVOID lpData, DWORD dwDataSize, DWORD dwFlags);

  HRESULT DeletePlayerFromGroup(DPID idGroup,
      DPID idPlayer);

  HRESULT DestroyGroup(DPID idGroup);

  HRESULT DestroyPlayer(DPID idPlayer);

  HRESULT EnumGroupPlayers(DPID idGroup,
      LPGUID lpguidInstance,
      LPDPENUMPLAYERSCALLBACK2 lpEnumPlayersCallback2,
      LPVOID lpContext, DWORD dwFlags);

  HRESULT EnumGroups(LPGUID lpguidInstance,
      LPDPENUMPLAYERSCALLBACK2 lpEnumPlayersCallback2,
      LPVOID lpContext, DWORD dwFlags);

  HRESULT EnumPlayers(LPGUID lpguidInstance,
      LPDPENUMPLAYERSCALLBACK2 lpEnumPlayersCallback2,
      LPVOID lpContext, DWORD dwFlags);

  HRESULT EnumSessions(LPDPSESSIONDESC2 lpsd,
      DWORD dwTimeout,
      LPDPENUMSESSIONSCALLBACK2 lpEnumSessionsCallback2,
      LPVOID lpContext, DWORD dwFlags);

  HRESULT GetCaps(LPDPCAPS lpDPCaps,
      DWORD dwFlags);

  HRESULT GetGroupData(DPID idGroup,
      LPVOID lpData, LPDWORD lpdwDataSize,
      DWORD dwFlags);

  HRESULT GetGroupName(DPID idGroup,
      LPVOID lpData, LPDWORD lpdwDataSize);

  HRESULT GetMessageCount(DPID idPlayer, LPDWORD lpdwCount);

  HRESULT GetPlayerAddress(DPID idPlayer,
      LPVOID lpAddress, LPDWORD lpdwAddressSize);

  HRESULT GetPlayerCaps(DPID idPlayer,
      LPDPCAPS lpPlayerCaps, DWORD dwFlags);

  HRESULT GetPlayerData(DPID idPlayer,
      LPVOID lpData, LPDWORD lpdwDataSize,
      DWORD dwFlags);

  HRESULT GetPlayerName(DPID idPlayer,
      LPVOID lpData, LPDWORD lpdwDataSize);

  HRESULT GetSessionDesc(LPVOID lpData,
      LPDWORD lpdwDataSize);

  HRESULT Initialize(LPGUID lpGUID);

  HRESULT Open(LPDPSESSIONDESC2 lpsd,
      DWORD dwFlags);

  HRESULT Receive(LPDPID lpidFrom, LPDPID lpidTo,
      DWORD dwFlags, LPVOID lpData, LPDWORD lpdwDataSize);

  HRESULT Send(DPID idFrom, DPID idTo, DWORD dwFlags,
      LPVOID lpData, DWORD dwDataSize);

  HRESULT SetGroupData(DPID idGroup,
      LPVOID lpData, DWORD dwDataSize,
      DWORD dwFlags);

  HRESULT SetGroupName(DPID idGroup,
      LPDPNAME lpGroupName, DWORD dwFlags);

  HRESULT SetPlayerData(DPID idPlayer, LPVOID lpData, 
      DWORD dwDataSize, DWORD dwFlags);

  HRESULT SetPlayerName(DPID idPlayer,
      LPDPNAME lpPlayerName, DWORD dwFlags);

  HRESULT SetSessionDesc(LPDPSESSIONDESC2 lpSessDesc,
      DWORD dwFlags);

  static int mEnumerate();
  int mEnumerateSessions( DWORD dwFlags );
  int mEnumeratePlayers( DWORD dwFlags );

protected:
  IDirectPlay2 *m_pIDirectPlay2;

private:
  LPDIRECTPLAY m_lpDP;  // don't use this interface
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectPlayLobby : public CADirectXObject
{
public:               
  CADirectPlayLobby();
  ~CADirectPlayLobby();

  IDirectPlayLobby *I() { return m_pIDirectPlayLobby; };

  HRESULT WINAPI Connect(DWORD dwFlags,
      LPDIRECTPLAY2 FAR *lplpDP, IUnknown FAR *pUnk);

  HRESULT CreateAddress(REFGUID guidSP,
      REFGUID guidDataType, LPCVOID lpData,
      DWORD dwDataSize, LPVOID lpAddress,
      LPDWORD lpdwAddressSize);

  HRESULT EnumAddress(LPDPENUMADDRESSCALLBACK lpEnumAddressCallback,
      LPCVOID lpAddress, DWORD dwAddressSize,
      LPVOID lpContext);

  HRESULT EnumAddressTypes(
      LPDPLENUMADDRESSTYPESCALLBACK lpEnumAddressTypeCallback,
      REFGUID guidSP, LPVOID lpContext,
      DWORD dwFlags);

  HRESULT EnumLocalApplications(
      LPDPLENUMLOCALAPPLICATIONSCALLBACK lpEnumLocalAppCallback,
      LPVOID lpContext, DWORD dwFlags);

  HRESULT GetConnectionSettings(DWORD dwAppID,
      LPVOID lpData, LPDWORD lpdwDataSize);

  HRESULT ReceiveLobbyMessage(DWORD dwFlags,
      DWORD dwAppID, LPDWORD lpdwMessageFlags,
      LPVOID lpData, LPDWORD lpdwDataSize);

  HRESULT RunApplication(DWORD dwFlags,
      LPDWORD lpdwAppID, LPDPLCONNECTION lpConn,
      HANDLE hReceiveEvent);

  HRESULT SendLobbyMessage( DWORD dwFlags,
      DWORD dwAppID, LPVOID lpData,
      DWORD dwDataSize);

  HRESULT SetConnectionSettings(DWORD dwFlags,
      DWORD dwAppID, LPDPLCONNECTION lpConn);

  HRESULT SetLobbyMessageEvent(DWORD dwFlags,
      DWORD dwAppID, HANDLE hReceiveEvent);


protected:

  IDirectPlayLobby *m_pIDirectPlayLobby;
};


#endif