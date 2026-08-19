#include <stdafx.h>
#include "Launch.h"
#include "dxa3drm.h"
#include "dxainput.h"
#include "dxautil.h"
#include "dxaplay.h"
#include "player.h"

CAPlayerList *g_pPlayerList = NULL;
CAPlayer     *g_pPlayer = NULL;

static DWORD WINAPI  ReceiveThread(LPVOID lpThreadParameter);

// message buffers
static DPAPPMSG_CHATSTRING g_dpChatstring;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAPlayer::CAPlayer()
{
  m_szName[0] = NULL;
  m_dwPlayerType = DPPLAYERTYPE_PLAYER;
  m_hPlayerEvent = 0;
  m_hReceiveThread = 0;
  m_hKillReceiveEvent = 0;
  m_gidReceiveThread = 0;
  m_dpidPlayer = 0;
  m_bIsHost = FALSE;
  m_hwndOwner = 0L;
}

CAPlayer::~CAPlayer()
{
  ShutdownConnection();
}

HRESULT CAPlayer::SetupConnection()
{
  // create event used by DirectPlay to signal a message has arrived
  m_hPlayerEvent = CreateEvent(NULL,    // no security
                               FALSE,    // auto reset
                               FALSE,    // initial event reset
                               NULL);    // no name
  if (m_hPlayerEvent == NULL)
    return DPERR_NOMEMORY;

  // create event used to signal that the receive thread should exit
  m_hKillReceiveEvent = CreateEvent(NULL,    // no security
                                    FALSE,    // auto reset
                                    FALSE,    // initial event reset
                                    NULL);    // no name
  if (m_hKillReceiveEvent == NULL)
    return DPERR_NOMEMORY;

  // create thread to receive player messages
  m_hReceiveThread = CreateThread(NULL,      // default security
                                 0,        // default stack size
                                 ReceiveThread,  // pointer to thread routine
                                 this,    // argument for thread
                                 0,        // start it right away
                                 &m_gidReceiveThread);
  if (m_hReceiveThread == NULL)
    return DPERR_NOMEMORY;

  return (DP_OK);  
}

HRESULT CAPlayer::ShutdownConnection()
{
  if (m_hReceiveThread)
  {
    // wake up receive thread and wait for it to quit
    SetEvent(m_hKillReceiveEvent);
    WaitForSingleObject(m_hReceiveThread, INFINITE);

    CloseHandle(m_hReceiveThread);
    m_hReceiveThread = NULL;
  }

  if (m_hKillReceiveEvent)
  {
    CloseHandle(m_hKillReceiveEvent);
    m_hKillReceiveEvent = NULL;
  }

  if (m_hPlayerEvent)
  {
    CloseHandle(m_hPlayerEvent);
    m_hPlayerEvent = NULL;
  }

  return (DP_OK);
}

static DWORD WINAPI ReceiveThread(LPVOID lpThreadParameter)
{
  CAPlayer *pPlayer = (CAPlayer *) lpThreadParameter;
  HANDLE eventHandles[2];

  eventHandles[0] = pPlayer->m_hPlayerEvent;
  eventHandles[1] = pPlayer->m_hKillReceiveEvent;

  // loop waiting for player events. If the kill event is signaled
  // the thread will exit
  while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
  {
    // receive any messages in the queue
    pPlayer->ReceiveMessage();
  }

  ExitThread(0);

  return (0);
}

HRESULT CAPlayer::ReceiveMessage()
{
  DPID    idFrom, idTo;
  LPVOID  lpvMsgBuffer;
  DWORD   dwMsgBufferSize;
  HRESULT hr;

  lpvMsgBuffer = NULL;
  dwMsgBufferSize = 0;

  // loop to read all messages in queue
  do
  {
    // loop until a single message is successfully read
    do
    {
      // read messages from any player, including system player
      idFrom = 0;
      idTo = 0;

      hr = g_pDirectPlay2->I()->Receive( &idFrom, &idTo, DPRECEIVE_ALL,
                                         lpvMsgBuffer, &dwMsgBufferSize );

      // not enough room, so resize buffer
      if (hr == DPERR_BUFFERTOOSMALL)
      {
        delete lpvMsgBuffer;
        lpvMsgBuffer = new char[ dwMsgBufferSize ];
        if (lpvMsgBuffer == NULL)
          hr = DPERR_OUTOFMEMORY;
      }
    } while (hr == DPERR_BUFFERTOOSMALL);

    if ((SUCCEEDED(hr)) && (dwMsgBufferSize >= sizeof(DPMSG_GENERIC)))
    {
      if (idFrom == DPID_SYSMSG)
      {
        HandleSystemMessage( (LPDPMSG_GENERIC) lpvMsgBuffer,
                             dwMsgBufferSize, idFrom, idTo );
      }
      else
      {
        HandleApplicationMessage( (LPDPMSG_GENERIC) lpvMsgBuffer,
                                  dwMsgBufferSize, idFrom, idTo );
      }
    }
  } while (SUCCEEDED(hr));

  delete lpvMsgBuffer;

  return (DP_OK);
}

void CAPlayer::HandleApplicationMessage( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
                                         DPID idFrom, DPID idTo)
{
  LPSTR    lpszStr = NULL;

  switch (lpMsg->dwType)
  {
    case WMDP_CHATSTRING:
    {
      DPAPPMSG_CHATSTRING *lp = (DPAPPMSG_CHATSTRING *) lpMsg;

      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDP_CHATSTRING, 0, (LPARAM) lp);
    }
    break;
  }
}

void CAPlayer::HandleSystemMessage( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
                                    DPID idFrom, DPID idTo)
{
  switch (lpMsg->dwType)
  {
    case DPSYS_CREATEPLAYERORGROUP:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_CREATEPLAYERORGROUP, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_DESTROYPLAYERORGROUP:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_DESTROYPLAYERORGROUP, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_ADDPLAYERTOGROUP:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_ADDPLAYERTOGROUP, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_DELETEPLAYERFROMGROUP:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_DELETEPLAYERFROMGROUP, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_SESSIONLOST:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_SESSIONLOST, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_HOST:
    {
      // we are now the host
      m_bIsHost = TRUE;

      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_HOST, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_SETPLAYERORGROUPDATA:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_SETPLAYERORGROUPDATA, 0, (LPARAM) lpMsg);
    }
    break;

  case DPSYS_SETPLAYERORGROUPNAME:
    {
      if (m_hwndOwner)
        SendMessage( m_hwndOwner, WMDPSYS_SETPLAYERORGROUPNAME, 0, (LPARAM) lpMsg);
    }
    break;

  default:
    TRACE("CAPlayer::HandleSystemMessage() - DPSYS_xxx message unhandled\n");
    break;
  }
}

HRESULT CAPlayer::SendChatMessage(LPCTSTR pText)
{
  CString sMsg = pText;

  DPAPPMSG_CHATSTRING *pAppmsg = (DPAPPMSG_CHATSTRING *)
    new char[ sizeof(DPAPPMSG_CHATSTRING) + sMsg.GetLength() ];
  pAppmsg->dwType = WMDP_CHATSTRING;
  strcpy( pAppmsg->szMsg, (LPCTSTR) sMsg );

  // send this string to all other players
  g_pDirectPlay2->Send( m_dpidPlayer,
                        DPID_ALLPLAYERS,
                        DPSEND_GUARANTEED,
                        (LPSTR) pAppmsg,
                        sizeof(DPAPPMSG_CHATSTRING) + sMsg.GetLength());
  delete pAppmsg;
  return TRUE;
}

CString CAPlayer::GetPlayerName()
{
  CString   sRet;
  DPNAME    *lpName = NULL;
  DWORD     dwNameSize;
  HRESULT   hr;

  // get size of player name data
  hr = g_pDirectPlay2->GetPlayerName(m_dpidPlayer, NULL, &dwNameSize);
  if (hr != DPERR_BUFFERTOOSMALL)
    goto FAILURE;

  // make room for it
  lpName = (DPNAME *) new char[ dwNameSize ];
  if (lpName == NULL)
  {
    hr = DPERR_OUTOFMEMORY;
    goto FAILURE;
  }

  // get player name data
  hr = g_pDirectPlay2->GetPlayerName(m_dpidPlayer, lpName, &dwNameSize);
  if FAILED(hr)
    goto FAILURE;

  sRet = lpName->lpszShortNameA;
  delete lpName;
  return sRet;

FAILURE:
  delete lpName;

  return sRet;
}

HRESULT CAPlayer::SetPlayerData( void *pData, DWORD size )
{
  return g_pDirectPlay2->SetPlayerData( m_dpidPlayer, pData, size, DPSET_REMOTE );
}

HRESULT CAPlayer::GetPlayerData( void *pData, DWORD *pSize )
{
  return g_pDirectPlay2->GetPlayerData( m_dpidPlayer, pData, pSize, DPGET_REMOTE );
}
