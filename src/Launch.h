#if !defined(AFX_LAUNCH_H__8E13A03A_101A_11D1_90FF_444553540000__INCLUDED_)
#define AFX_LAUNCH_H__8E13A03A_101A_11D1_90FF_444553540000__INCLUDED_

#include "resource.h"       // main symbols

#include "dxainput.h"
#include "dxasound.h"
#include "dxadraw.h"
#include "dxaplay.h"
#include "dxa3drm.h"
#include "dxerror.h"
#include "dxautil.h"
#include "player.h"

#define INI_SETTINGS        "Settings"
#define INI_PLAYER_NAME     "PlayerName"
#define INI_EFFECTS         "Effects"
#define INI_MUSIC           "Music"
#define INI_MODE_X          "ModeX"
#define INI_MODE_Y          "ModeY"
#define INI_MODE_BPP        "ModeBPP"
#define INI_WINDOWED        "Windowed"

class CAGamePlay;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class CALaunchApp : public CWinApp
{
public:
	CALaunchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALaunchApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
  BOOL InitDirectX();
  BOOL ExitDirectX();

  BOOL GetProfile();
  BOOL SetProfile();

  BOOL RunGame();

  CAGamePlay *m_pGameWnd;

  CString m_sPlayerName;
	BOOL	m_bEffects;
	BOOL	m_bMusic;
	BOOL	m_bWindowed;
  int   m_iModeX;
  int   m_iModeY;
  int   m_iModeBPP;

  CString m_aHighScores[ 10 ];

	//{{AFX_MSG(CALaunchApp)
	afx_msg void OnAppAbout();
	afx_msg void OnSetupOptions();
	afx_msg void OnUpdateSetupOptions(CCmdUI* pCmdUI);
	afx_msg void OnFileNewGame();
	afx_msg void OnFileLoadGame();
	afx_msg void OnFileOnlinegames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern CADirectInput *g_pDirectInput;
extern CADirectSound *g_pDirectSound;
extern CADirectDraw  *g_pDirectDraw;
extern CADirectPlay2 *g_pDirectPlay2;
extern CADirect3DRM  *g_pDirect3DRM;

extern D3DVALUE       g_valFrameRatio;
extern DWORD          g_dwFrames;
extern DWORD          g_dwFrameCount;
extern DWORD          g_dwFrameTime;
extern DWORD          g_dwLastTick;
extern DWORD          g_dwDiffTime;

extern CAScene       *g_pInvisible;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCH_H__8E13A03A_101A_11D1_90FF_444553540000__INCLUDED_)
