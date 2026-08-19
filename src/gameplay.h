#ifndef __DXAGAMEPLAY_H__
#define __DXAGAMEPLAY_H__

#include "DxaWnd.h"
#include "playersh.h"
#include "bullet.h"

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#define MAX_STEPS 64

typedef struct _levelinfo
{
  D3DVALUE clrBackgroundR;
  D3DVALUE clrBackgroundG;
  D3DVALUE clrBackgroundB;

  int      nLargeRocks;
  int      iLargeRockType;

  int      nMediumRocks;
  int      iMediumRockType;

  int      nSmallRocks;
  int      iSmallRockType;

  D3DVECTOR vStartPos;
  int      iMusicLine;

  char     szMusic[ 256 ];  // midi file name

} LEVELINFO;

class CAMusic;

class CAGamePlay : public CADxaWnd
{
public:               
  typedef enum
  {
    eLoading,
    eRunning,
    ePaused,
    ePlayerHit,
    eGameOver,
    eHighScore,
    eHelpScreen
  } GameState;

  CAGamePlay();
  ~CAGamePlay();

  // overrides
  virtual BOOL Poll();
  virtual BOOL InitGame();
  virtual BOOL LoadGame();
  virtual BOOL PauseGame();
  virtual BOOL PlayerHit();
  virtual BOOL GameOver();
  virtual BOOL HighScore();
  virtual BOOL HelpScreen();
  virtual BOOL UpdateInput();
  virtual BOOL Render();
  virtual BOOL ExitGame();
  virtual void DisplayGDI( HDC hdc, CRect *pRect );
  BOOL SetupLevel( int iLevel );
  CAShape *CreateRock( int iType, int nSize );

  void DrawSlider(CDC* pDC, LPRECT pRect, double percent, LPCTSTR pMinLabel, LPCTSTR pMaxLabel, BOOL bBar = FALSE );

  GameState           m_eGameState;
  CAMusic            *m_pMusic;
  CAController       *m_pController;

  CAScene            *m_pScene;
  CADirect3DRMFrame  *m_pCamera;
  CADirect3DRMLight  *m_pAmbLight;
  CADirect3DRMLight  *m_pDirLight;
  CAShape            *m_aHillSteps[ MAX_STEPS ];
  CAShape            *m_pFlag;
  CAShape            *m_pPole;
  CAShape            *m_pPennant;
  CAShape            *m_pBase;
  CADirect3DRMAnimation *m_pLogo;

  CADirectSoundBuffer   *m_pSndJump;
  CADirectSoundBuffer   *m_pSndLand;
  CADirectSoundBuffer   *m_pSndGun;
  CADirectSoundBuffer   *m_pSndMachineGun;
  CADirectSoundBuffer   *m_pSndLaser;
  CADirectSound3DBuffer *m_pSndHit;
  CADirectSoundBuffer   *m_pSndPlayerHit;
  CADirectSoundBuffer   *m_pSndShieldHit;
  CADirectSound3DBuffer *m_pSndHumm;

  CADirect3DRMTexture   *m_pCanvas;

  CAPlayerShape           *m_pPlayer;
  CAPlayerShapeList       *m_pPlayerShapeList;
  CADirectSound3DListener *m_pListener;
  CAShapeList             *m_pBoundaryList;
  CAShapeList             *m_pMoveableList;
  CABulletList            *m_pBulletCache;
  CABulletList            *m_pBulletList;

  D3DVALUE                 m_valFieldOfView;
  int                      m_nScore;
  int                      m_nShips;
  int                      m_iCurrentScore;
  CFont                    m_fontPanel;
  DWORD                    m_timePlayerHit;

  BOOL CreatePlayField();
  CAPlayerShape *CreatePlayer();
  BOOL CheckPlayerCollisions( CAPlayerShape *pPlayer, CAVector vOldPos );
  BOOL CheckBulletCollisions( CABullet *pBullet, CAVector &vOldPos );
  BOOL SimplePlayerCollisions( CAPlayerShape *pPlayer );
  BOOL SimpleBulletCollisions( CABullet *pPlayer );

  int m_nHillSteps;

protected:
	//{{AFX_MSG(CALaunchView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
  afx_msg LONG OnMCINotify( UINT wParam, LONG lParam );
  afx_msg LONG OnDpSetPlayerData( UINT wParam, LONG lParam );
	DECLARE_MESSAGE_MAP()
};

extern LEVELINFO g_liLevel[];
extern int       g_iLevel;
extern int       g_nLevels;

#endif