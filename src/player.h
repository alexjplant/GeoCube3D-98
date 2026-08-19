#ifndef __CAPLAYER_H__
#define __CAPLAYER_H__

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAPlayer;
typedef CTypedPtrList< CPtrList, CAPlayer* > CAPlayerList;

extern CAPlayerList *g_pPlayerList;
extern CAPlayer     *g_pPlayer;

class CAPlayer
{
public:               
  CAPlayer();
  ~CAPlayer();

  // online connection methods
  HRESULT SetupConnection();
  HRESULT ShutdownConnection();
  void    HandleApplicationMessage( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
                                    DPID idFrom, DPID idTo);
  void    HandleSystemMessage( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
                               DPID idFrom, DPID idTo);

  HRESULT ReceiveMessage();
  HRESULT SendChatMessage(LPCTSTR pText);
  CString GetPlayerName();
  HRESULT SetPlayerData( void *pData, DWORD size );
  HRESULT GetPlayerData( void *pData, DWORD *pSize );

  TCHAR  m_szName[81];
  DPID   m_dpidPlayer;
  DWORD  m_dwPlayerType;
	HANDLE m_hPlayerEvent;
  HANDLE m_hReceiveThread;
  HANDLE m_hKillReceiveEvent;
  DWORD  m_gidReceiveThread;
  BOOL   m_bIsHost;
  HWND   m_hwndOwner;

protected:

};

// message structures used to send messages to another player
#define WMDP_CHATSTRING  100

typedef struct
{
  DWORD dwType;
  char  szMsg[1];
}
DPAPPMSG_CHATSTRING;

#define WMDPSYS_CREATEPLAYERORGROUP   (WM_USER + DPSYS_CREATEPLAYERORGROUP)
#define WMDPSYS_DESTROYPLAYERORGROUP  (WM_USER + DPSYS_DESTROYPLAYERORGROUP)
#define WMDPSYS_ADDPLAYERTOGROUP      (WM_USER + DPSYS_ADDPLAYERTOGROUP)
#define WMDPSYS_DELETEPLAYERFROMGROUP (WM_USER + DPSYS_DELETEPLAYERFROMGROUP)
#define WMDPSYS_SESSIONLOST           (WM_USER + DPSYS_SESSIONLOST)
#define WMDPSYS_HOST                  (WM_USER + DPSYS_HOST)
#define WMDPSYS_SETPLAYERORGROUPDATA  (WM_USER + DPSYS_SETPLAYERORGROUPDATA)
#define WMDPSYS_SETPLAYERORGROUPNAME  (WM_USER + DPSYS_SETPLAYERORGROUPNAME)

#endif

