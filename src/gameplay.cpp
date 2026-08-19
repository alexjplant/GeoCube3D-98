#include <stdafx.h>
#include <math.h>
#include "launch.h"
#include "DxaWnd.h"
#include "dxamusic.h"
#include "gameplay.h"
#include "bullet.h"
#include "rocksh.h"

#define PI_180 (0.01745327777778) // radians in 1 degree
                                  // 90 degrees  is 1.57 radians
                                  // 180 degrees is 3.14 radians
                                  // 270 degrees is 4.71 radians

#define MAX_FIELD_OF_VIEW   1.0
#define MIN_FIELD_OF_VIEW   0.10
#define STEP_FIELD_OF_VIEW  0.05
#define SLIDER_HEIGHT       3

#define SQR(x)         ((x) * (x))

LEVELINFO g_liLevel[] =
{
  {  
    0,0,.85,
    1,0,
    0,0,
    0,0,
    0,0,0,
    0,
    "music.mid"
  },
  {  
    0,.85,0,
    2,1,
    0,1,
    0,1,
    0,0,0,
    0,
    "synth.mid"
  },
  {  
    .4,0,.4,
    3,2,
    0,2,
    0,2,
    0,0,0,
    0,
    "reggae.mid"
  },
  {  
    0,.4,.4,
    4,3,
    0,3,
    0,3,
    0,0,0,
    0,
    "spin.mid"
  },
  {
    .85,0,0,
    5,4,
    0,4,
    0,4,
    0,0,0,
    0,
    "funk.mid"
  },
};

int g_iLevel = 0;
int g_nLevels = sizeof(g_liLevel) / sizeof(LEVELINFO);
int g_nLevelWrap = 0;

char *g_aHelpText[] =
{
  "Aim Ship - Up Arrow      Zoom In  - Page Up  ",
  "           Down Arrow    Zoom Out - Page Down",
  "           Left Arrow                        ",
  "           Right Arrow                       ",
  "",
  "Thrust Forward   - A     Fire   - CTRL       ",
  "Thrust Backward  - Z     Shield - S",
  "Thrust Left      - <     Pause  - Pause",
  "Thrust Right     - >     ",
  "Full Stop        - 5     Quit   - ESC"
};

int g_nHelpText = sizeof(g_aHelpText) / sizeof(char *);

char *g_aRandShapes[] =
{
  "jack.x",
};
int g_nRandShapes = sizeof(g_aRandShapes) / sizeof(char *);

static int logoTick = 0;

static D3DVALUE GroundHeight(D3DVALUE x, D3DVALUE z, void* pArg);
static void CDECL listenerCallback( LPDIRECT3DRMFRAME obj, void* arg, D3DVALUE delta);
static BOOL PointInBox( CAVector *pPoint, D3DRMBOX *pBox );

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAGamePlay::CAGamePlay() : CADxaWnd()
{
  m_eGameState = eLoading;
  m_pController = NULL;
  m_pScene = NULL;
  g_pInvisible = NULL;
  m_pCamera = NULL;
  m_pAmbLight = NULL;
  m_pDirLight = NULL;
  m_nHillSteps = 0;
  m_pPlayer = NULL;
  m_pPlayerShapeList = new CAPlayerShapeList;
  m_pBoundaryList = new CAShapeList;
  m_pMoveableList = new CAShapeList;
  m_pBulletCache = new CABulletList;
  m_pBulletList = new CABulletList;
  m_pListener = NULL;
  m_pFlag = NULL;
  m_pPole = NULL;
  m_pPennant = NULL;
  m_pBase = NULL;
  m_pSndJump = NULL;
  m_pSndLand = NULL;
  m_pSndGun = NULL;
  m_pSndMachineGun = NULL;
  m_pSndLaser = NULL;
  m_pSndHit = NULL;
  m_pSndPlayerHit = NULL;
  m_pSndShieldHit = NULL;
  m_pSndHumm = NULL;
  m_pCanvas = new CADirect3DRMTexture( g_pDirect3DRM, "canvas.bmp" );

  m_valFieldOfView = .9;
  m_nScore = 0;
  m_nShips = 0;
  m_iCurrentScore = -1;
}

CAGamePlay::~CAGamePlay()
{
  DeleteAllSprites();

  delete m_pViewport;
  delete m_pDevice;

  delete m_pAmbLight;
  delete m_pDirLight;
  delete m_pCamera;
  delete m_pScene;
  delete g_pInvisible;
  delete m_pController;

  for (int i = 0; i < m_nHillSteps; i++)
    delete m_aHillSteps[i];

  delete m_pBoundaryList;

  while (m_pPlayerShapeList->GetCount())
    delete m_pPlayerShapeList->RemoveTail();
  delete m_pPlayerShapeList;

  while (m_pMoveableList->GetCount())
    delete m_pMoveableList->RemoveTail();
  delete m_pMoveableList;

  while (m_pBulletCache->GetCount())
    delete m_pBulletCache->RemoveTail();
  delete m_pBulletCache;

  while (m_pBulletList->GetCount())
    delete m_pBulletList->RemoveTail();
  delete m_pBulletList;

  delete m_pListener;
  delete m_pFlag;
  delete m_pPole;
  delete m_pPennant;
  delete m_pBase;
  delete m_pSndJump;
  delete m_pSndLand;
  delete m_pSndMachineGun;
  delete m_pSndLaser;
  delete m_pSndHit;
  delete m_pSndPlayerHit;
  delete m_pSndShieldHit;
  delete m_pSndHumm;
  delete m_pCanvas;
  delete m_pMusic;
}


BEGIN_MESSAGE_MAP(CAGamePlay, CADxaWnd)
	//{{AFX_MSG_MAP(CADxaWnd)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
  ON_MESSAGE( WMDPSYS_SETPLAYERORGROUPDATA, OnDpSetPlayerData )
  ON_MESSAGE( MM_MCINOTIFY, OnMCINotify )
END_MESSAGE_MAP()


BOOL CAGamePlay::InitGame()
{
  // create the scene
  m_pScene = new CAScene( g_pDirect3DRM );
  m_pCamera = new CADirect3DRMFrame( g_pDirect3DRM, m_pScene->I() );
  g_pInvisible = new CAScene( g_pDirect3DRM );

  // create the device and viewport
  m_pDevice = new CADirect3DRMDevice( g_pDirect3DRM, g_pDirectDraw, m_pBack->I());
	m_pDevice->SetBufferCount( m_nBuffers );
  m_pDevice->SetQuality( D3DRMRENDER_FLAT );
//    D3DRMLIGHT_ON | D3DRMFILL_SOLID | D3DRMSHADE_GOURAUD );
//  m_pDevice->SetTextureQuality( D3DRMTEXTURE_LINEARMIPLINEAR );
  m_pDevice->SetTextureQuality( D3DRMTEXTURE_LINEAR );
  m_pViewport = new CADirect3DRMViewport(
    g_pDirect3DRM, m_pDevice->I(), m_pCamera->I(),
    m_rViewport.left, m_rViewport.top,
    m_rViewport.Width(), m_rViewport.Height());
  m_pViewport->SetBack( 10000 ); //sqrt( SQR(BOUNDARY*2) + SQR(BOUNDARY*2) ) );
  m_pViewport->SetField( m_valFieldOfView );

  // Set the render state
  DDPIXELFORMAT fmt;
  fmt.dwSize = sizeof fmt;
  m_pBack->GetPixelFormat( &fmt );

  switch (fmt.dwRGBBitCount)
  {
    case 1:
      m_pDevice->SetShades( 4 );
      g_pDirect3DRM->SetDefaultTextureShades(4);
      break;
    case 16:
      m_pDevice->SetShades(32);
      g_pDirect3DRM->SetDefaultTextureColors(64);
      g_pDirect3DRM->SetDefaultTextureShades(32);
      m_pDevice->SetDither(FALSE);
      break;
    case 24:
    case 32:
      m_pDevice->SetShades(256);
      g_pDirect3DRM->SetDefaultTextureColors(64);
      g_pDirect3DRM->SetDefaultTextureShades(256);
      m_pDevice->SetDither(FALSE);
      break;
    default:
      m_pDevice->SetDither(FALSE);
      break;
  }

  // add lights
  m_pAmbLight = new CADirect3DRMLight( g_pDirect3DRM,
    D3DRMLIGHT_AMBIENT, D3DRGB(0.5, 0.5, 0.5));
  m_pScene->AddLight( m_pAmbLight->I() );  

  m_pDirLight = new CADirect3DRMLight( g_pDirect3DRM,
    D3DRMLIGHT_DIRECTIONAL, D3DRGB(0.6, 0.6, 0.6));
  m_pScene->AddChild( m_pDirLight->m_pFrame->I() );  
  CAVector vDir(D3DVAL(1), D3DVAL(-1), D3DVAL(1));
  CAVector vUp = vDir.GenerateUp();
  m_pDirLight->m_pFrame->SetOrientation( m_pScene->I(),
    vDir.x, vDir.y, vDir.z,
    vUp.x, vUp.y, vUp.z);

  // create the players
  if (g_pPlayerList->GetCount() == 0)
  {
    m_pPlayer = CreatePlayer();
    m_pPlayer->SetOrientation( m_pScene->I(),
      0, 0, 1,
      0, 1, 0 );
    m_pPlayerShapeList->AddTail( m_pPlayer );
  }
  else  // multi-player online game
  {
    POSITION pos = g_pPlayerList->GetHeadPosition();
    int i = 0;
    while (pos)
    {
      CAPlayer *pOnlinePlayer = (CAPlayer *) g_pPlayerList->GetNext( pos );
      CAPlayerShape *pPlayer = CreatePlayer();
      pPlayer->SetPosition( m_pScene->I(), D3DVAL(0), D3DVAL(.5), D3DVAL( -20.0 * (i+1) ) );
      pPlayer->SetOrientation( m_pScene->I(),
        D3DVAL(0), D3DVAL(0), D3DVAL(1),
        D3DVAL(0), D3DVAL(1), D3DVAL(0) );
      pPlayer->SetPlayerData();
      pPlayer->m_pOnlinePlayer = pOnlinePlayer;

      m_pPlayerShapeList->AddTail( pPlayer );

      if (pOnlinePlayer->m_dpidPlayer == g_pPlayer->m_dpidPlayer)
        m_pPlayer = pPlayer;
      else
        m_pBoundaryList->AddTail( pPlayer );

      i++;
    }
  }

  // register for online player messages
  if (g_pPlayer)
    g_pPlayer->m_hwndOwner = *this;

  // attach the camera to it
  m_pPlayer->AddChild( m_pCamera->I() );
  m_pCamera->SetPosition( m_pPlayer->I(), 0, 2, -6);
  m_pCamera->SetOrientation( m_pPlayer->I(),  0, 0, 1,  0, 1, 0);                                         

  CreatePlayField();

  // setup the sound buffers
  g_pDirectSound->SetCooperativeLevel( *this, DSSCL_PRIORITY );

  CADirectSoundBuffer primary( DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D );
  m_pListener = new CADirectSound3DListener( &primary );
  m_pListener->SetRolloffFactor( D3DVALUE(0.1), DS3D_IMMEDIATE );
  // Add a callback to the camera's frame so that the listener is updated with the camera
  m_pCamera->AddMoveCallback( listenerCallback, this );

  WAVEFORMATEX wfx;
  wfx.wFormatTag = WAVE_FORMAT_PCM;
  wfx.nChannels = 2;
  wfx.wBitsPerSample = 16;
  wfx.nSamplesPerSec = 44100;
  wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
  wfx.cbSize = 0;
  primary.SetFormat(&wfx);

  m_pSndJump = new CADirectSoundBuffer( "jump.wav" );
  m_pSndLand = new CADirectSoundBuffer( "land.wav" );
  m_pSndMachineGun = new CADirectSoundBuffer( "fire.wav" );
  m_pSndLaser = new CADirectSoundBuffer( "laser.wav" );
  m_pSndGun = m_pSndMachineGun;

  m_pSndHit = new CADirectSound3DBuffer( "hit.wav" );
  m_pSndHit->SetMinDistance( D3DVAL(250), DS3D_DEFERRED );
  m_pSndHit->SetMaxDistance( D3DVAL(2000), DS3D_DEFERRED );
  m_pSndHit->SetPosition( D3DVAL(0), D3DVAL(-10), D3DVAL(0), DS3D_DEFERRED );
  m_pSndHit->SetConeOrientation( D3DVAL(0), D3DVAL(1), D3DVAL(0), DS3D_DEFERRED );
  m_pSndHit->SetConeAngles( D3DVAL(90), D3DVAL(360), DS3D_DEFERRED );
  m_pSndHit->SetVolume( D3DVAL(0) );
  m_pSndHit->SetConeOutsideVolume( D3DVAL(0), DS3D_DEFERRED );
  // TODO: duplicate this for each bullet and set the position
  // before calling play
m_pSndHit->SetMode( DS3DMODE_DISABLE, DS3D_DEFERRED );

  m_pSndPlayerHit = new CADirectSoundBuffer( "plyrhit.wav" );
  m_pSndShieldHit = new CADirectSoundBuffer( "shldhit.wav" );

  m_pSndHumm = new CADirectSound3DBuffer( "humm.wav" );
  m_pSndHumm->SetMinDistance( D3DVAL(2), DS3D_DEFERRED );
  m_pSndHumm->SetMaxDistance( D3DVAL(50), DS3D_DEFERRED );
  m_pSndHumm->SetPosition( D3DVAL(0), D3DVAL(-2), D3DVAL(0), DS3D_DEFERRED );
  m_pSndHumm->SetConeOrientation( D3DVAL(0), D3DVAL(1), D3DVAL(0), DS3D_DEFERRED );
  m_pSndHumm->SetConeAngles( D3DVAL(90), D3DVAL(120), DS3D_DEFERRED );
  m_pSndHumm->SetConeOutsideVolume( D3DVAL(-100), DS3D_DEFERRED );
m_pSndHumm->SetMode( DS3DMODE_DISABLE, DS3D_DEFERRED );
//  m_pSndHumm->Play(0, 0, DSBPLAY_LOOPING);

  primary.Play(0, 0, DSBPLAY_LOOPING);  // continuous quality
  m_pMusic = new CAMusic(*this);

  // create the input-polling object
  m_pController = new CAController( g_pDirectInput );

  // create the bullets
  for (int i = 0; i < 16; i++)
  {
    CABullet *pBullet = new CABullet( g_pDirect3DRM, g_pInvisible->I() );
    g_pInvisible->AddChild( pBullet->I() );
    m_pBulletCache->AddHead( pBullet );
  }

  // start the game logic
  g_nLevelWrap = 0;
  SetupLevel(0);
  m_nShips = 3;

  // setup GDI
  m_fontPanel.CreateFont( m_rPanel.Height() / 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    _T("Arial"));

  return TRUE;
}

CAPlayerShape *CAGamePlay::CreatePlayer()
{
  CAPlayerShape *pPlayer = new CAPlayerShape( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( pPlayer->I() );
  pPlayer->Load( "ship.x" );
  pPlayer->m_pMeshBuilder->Scale( .20, .20, .20 );
  LPDIRECT3DRMVISUAL pIShadow;
  g_pDirect3DRM->CreateShadow( pPlayer->m_pMeshBuilder->I(), m_pDirLight->I(),
    D3DVAL(0), D3DVAL(.05), D3DVAL(0),
    D3DVAL(0), D3DVAL(1), D3DVAL(0),
    &pIShadow);
//  pPlayer->AddVisual( pIShadow );

  return pPlayer;
}

BOOL CAGamePlay::ExitGame()
{
  return TRUE;
}

BOOL CAGamePlay::Poll()
{
  switch (m_eGameState)
  {
  case eLoading:
    return LoadGame();

  case ePaused:
    return PauseGame();

  case eRunning:
    return UpdateInput();

  case ePlayerHit:
    return PlayerHit();

  case eGameOver:
    return GameOver();

  case eHighScore:
    return HighScore();

  case eHelpScreen:
    return HelpScreen();

  default:
    break;
  }

  return FALSE;
}

BOOL CAGamePlay::LoadGame()
{
  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetBkMode( hdc, TRANSPARENT );
    CBrush br(RGB(0, 0, 128));
    FillRect( hdc, m_rBack, br);

    CFont fontText;
    fontText.CreateFont( m_rBack.Height() / 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Arial"));
    SelectObject( hdc, (HFONT) fontText );

    CString sText("Loading ...");
    CRect rText = m_rBack;
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT );
    rText.OffsetRect( 0, m_rBack.Height() / 2 - rText.Height() / 2 );
    rText.right = m_rBack.right;
    SetTextColor( hdc, RGB( 0, 0, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( -4, -4 );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );

    m_pBack->ReleaseDC(hdc);
  }

  FlipScreen();

  InitGame();

  m_eGameState = eRunning;
  
  return TRUE;
}

BOOL CAGamePlay::PauseGame()
{
  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetBkMode( hdc, TRANSPARENT );

    CFont fontText;
    fontText.CreateFont( m_rBack.Height() / 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Arial"));
    SelectObject( hdc, (HFONT) fontText );

    CString sText("Paused ...");
    CRect rText = m_rBack;
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT );
    rText.OffsetRect( 0, m_rBack.Height() / 2 - rText.Height() / 2 );
    rText.right = m_rBack.right;
    SetTextColor( hdc, RGB( 0, 0, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( -4, -4 );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );

    m_pBack->ReleaseDC(hdc);
  }

  FlipScreen();

  m_pMusic->Play( "snare.mid" );

  // wait to be un-paused
  m_pController->m_data.Reset();
  MSG msg;
  while (m_eGameState == ePaused)
  {
    if( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
      if( !::GetMessage( &msg, NULL, 0, 0 ) )
        return FALSE;

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }

    m_pController->Update();

    for (int i = 0; i < 256; i++)
      if (m_pController->m_data.m_aKeys[ i ].Down())
        m_eGameState = eRunning;
  }

  m_pController->Debounce();

  m_pMusic->Play( g_liLevel[ g_iLevel ].szMusic );

  g_dwLastTick = timeGetTime();  // time shouldn't pass

  return TRUE;
}

BOOL CAGamePlay::PlayerHit()
{
  if (timeGetTime() - m_timePlayerHit < 8000)
    return TRUE;

  if (--m_nShips == 0)
  {
    m_eGameState = eGameOver;
    return TRUE;
  }

  // Reset player to the next ship
  SetupLevel( g_iLevel );

  m_eGameState = eRunning;
  m_pPlayer->AddChild( m_pCamera->I() );
  m_pCamera->SetPosition( m_pPlayer->I(), 0, 2, -6);
  m_pCamera->SetOrientation( m_pPlayer->I(),  0, 0, 1,  0, 1, 0);                                         
  m_pMusic->Play( g_liLevel[ g_iLevel ].szMusic );

  return TRUE;
}

BOOL CAGamePlay::GameOver()
{
  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetBkMode( hdc, TRANSPARENT );

    CFont fontText;
    fontText.CreateFont( m_rBack.Height() / 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Arial"));
    SelectObject( hdc, (HFONT) fontText );

    CString sText("Game Over ...");
    CRect rText = m_rBack;
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT );
    rText.OffsetRect( 0, m_rBack.Height() / 2 - rText.Height() / 2 );
    rText.right = m_rBack.right;
    SetTextColor( hdc, RGB( 0, 0, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( -2, -2 );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );

    m_pBack->ReleaseDC(hdc);
  }

  FlipScreen();

  m_pMusic->Play( "wierd.mid" );

  // delay so the user can see it
  m_pController->m_data.Reset();
  MSG msg;
  DWORD time = timeGetTime();
  BOOL bKeyHit = FALSE;
  while (!bKeyHit && (timeGetTime() - time < 8000))
  {
    if( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
      if( !::GetMessage( &msg, NULL, 0, 0 ) )
        return FALSE;

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }

    m_pController->Update();

    for (int i = 0; i < 256; i++)
      if (m_pController->m_data.m_aKeys[ i ].Down() &&
          (timeGetTime() - time > 1000))
        bKeyHit = TRUE;
  }
  m_pController->Debounce();

  // insert the new high score
  m_iCurrentScore = -1;
  CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();
  CString sNewScore;
  sNewScore.Format("%9ld %-10.10s  Level %ld", m_nScore, pApp->m_sPlayerName, g_iLevel + 1 + g_nLevels * g_nLevelWrap );
  for (int iScore = 0; iScore < 10; iScore++)
  {
    if (m_nScore > atoi(pApp->m_aHighScores[ iScore ]))
    {
      m_iCurrentScore = iScore;
      // bump other scores down
      for (int iBump = 9; iBump > iScore; iBump--)
        pApp->m_aHighScores[ iBump ] = pApp->m_aHighScores[ iBump - 1 ];
      pApp->m_aHighScores[ iScore ] = sNewScore;
      break;
    }
  }
  pApp->SetProfile();

  m_eGameState = eHighScore;
  return TRUE;
}

BOOL CAGamePlay::HighScore()
{
  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetBkMode( hdc, TRANSPARENT );
    CBrush br(RGB(
      g_liLevel[ g_iLevel ].clrBackgroundR * 255,
      g_liLevel[ g_iLevel ].clrBackgroundG * 255,
      g_liLevel[ g_iLevel ].clrBackgroundB * 255));
    FillRect( hdc, m_rBack, br);

    CFont fontText;
    fontText.CreateFont( m_rBack.Height() / 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Arial"));
    SelectObject( hdc, (HFONT) fontText );

    CString sText("High Scores");
    CRect rText = m_rBack;
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT );
    rText.right = m_rBack.right;

    SetTextColor( hdc, RGB( 0, 0, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( -2, -2 );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( 2, 2 );
    rText.OffsetRect( 0, rText.Height() );
    
    CFont fontText2;
    fontText2.CreateFont( m_rBack.Height() / 16, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Courier New"));
    SelectObject( hdc, (HFONT) fontText2 );
    rText.bottom = rText.top + m_rBack.Height() / 16;
    rText.left = rText.Width() / 16;

    CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();
    for (int i = 0; i < 10; i++)
    {
      sText.Format(" %2ld)  %s", i+1, pApp->m_aHighScores[i] );
      rText.OffsetRect( 0, rText.Height() );

      SetTextColor( hdc, RGB( 0, 0, 0 ) );
      DrawText( hdc, sText, sText.GetLength(), &rText, DT_LEFT | DT_WORDBREAK );
      rText.OffsetRect( -2, -2 );
      if (i == m_iCurrentScore)
        SetTextColor( hdc, RGB( 255, 0, 0 ) );
      else
        SetTextColor( hdc, RGB( 255, 255, 0 ) );
      DrawText( hdc, sText, sText.GetLength(), &rText, DT_LEFT | DT_WORDBREAK );
      rText.OffsetRect( 2, 2 );
    }

    m_pBack->ReleaseDC(hdc);        
  }

  FlipScreen();

  m_pMusic->Play( "snare.mid" );

  // delay so the user can see it
  m_pController->Debounce();
  MSG msg;
  BOOL bKeyHit = FALSE;
  while (!bKeyHit)
  {
    if( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
      if( !::GetMessage( &msg, NULL, 0, 0 ) )
        return FALSE;

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }

    m_pController->Update();

    for (int i = 0; i < 256; i++)
      if (m_pController->m_data.m_aKeys[ i ].Down())
        bKeyHit = TRUE;
  }

  m_pController->Debounce();

  if (m_iCurrentScore == -1)
    m_pMusic->Play( g_liLevel[ g_iLevel ].szMusic );

  g_dwLastTick = timeGetTime();  // time shouldn't pass
  
  return FALSE;
}

BOOL CAGamePlay::HelpScreen()
{
  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetBkMode( hdc, TRANSPARENT );
    CBrush br(RGB(
      g_liLevel[ g_iLevel ].clrBackgroundR * 255,
      g_liLevel[ g_iLevel ].clrBackgroundG * 255,
      g_liLevel[ g_iLevel ].clrBackgroundB * 255));
    FillRect( hdc, m_rBack, br);

    CFont fontText;
    fontText.CreateFont( m_rBack.Height() / 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Arial"));
    SelectObject( hdc, (HFONT) fontText );

    CString sText("HELP");
    CRect rText = m_rBack;
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT );
    rText.right = m_rBack.right;

    SetTextColor( hdc, RGB( 0, 0, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( -2, -2 );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( 2, 2 );
    rText.OffsetRect( 0, rText.Height() );
    
    CFont fontText2;
    fontText2.CreateFont( m_rBack.Height() / 20, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Courier New"));
    SelectObject( hdc, (HFONT) fontText2 );
    rText.bottom = rText.top + m_rBack.Height() / 20;
    rText.left = rText.Width() / 16;

    CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();
    for (int i = 0; i < g_nHelpText; i++)
    {
      sText = g_aHelpText[i];
      rText.OffsetRect( 0, rText.Height() );

      SetTextColor( hdc, RGB( 0, 0, 0 ) );
      DrawText( hdc, sText, sText.GetLength(), &rText, DT_LEFT | DT_WORDBREAK );
      rText.OffsetRect( -2, -2 );
      SetTextColor( hdc, RGB( 255, 255, 0 ) );
      DrawText( hdc, sText, sText.GetLength(), &rText, DT_LEFT | DT_WORDBREAK );
      rText.OffsetRect( 2, 2 );
    }

    m_pBack->ReleaseDC(hdc);        
  }

  FlipScreen();

  m_pMusic->Play( "snare.mid" );

  // delay so the user can see it
  m_pController->Debounce();
  MSG msg;
  DWORD time = timeGetTime();
  BOOL bKeyHit = FALSE;
  while (!bKeyHit)
  {
    if( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
      if( !::GetMessage( &msg, NULL, 0, 0 ) )
        return FALSE;

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }

    m_pController->Update();

    for (int i = 0; i < 256; i++)
      if (m_pController->m_data.m_aKeys[ i ].Down())
        bKeyHit = TRUE;
  }

  m_pController->Debounce();

  m_pMusic->Play( g_liLevel[ g_iLevel ].szMusic );

  g_dwLastTick = timeGetTime();  // time shouldn't pass
  
  m_eGameState = eRunning;
  return TRUE;
}

BOOL CAGamePlay::UpdateInput()
{
  // update other players
  POSITION pos = m_pPlayerShapeList->GetHeadPosition();
  while (pos)
  {
    CAPlayerShape *pPlayer = (CAPlayerShape *) m_pPlayerShapeList->GetNext( pos );

    if (pPlayer->m_pOnlinePlayer &&
        pPlayer->m_pOnlinePlayer->m_dpidPlayer !=
        g_pPlayer->m_dpidPlayer)
    {
      pPlayer->GetPlayerData();
      pPlayer->SetPosition( NULL,
        pPlayer->m_data.vPos.x,
        pPlayer->m_data.vPos.y,
        pPlayer->m_data.vPos.z);
      pPlayer->SetOrientation( NULL,
        pPlayer->m_data.vDir.x,
        pPlayer->m_data.vDir.y,
        pPlayer->m_data.vDir.z,
        pPlayer->m_data.vUp.x,
        pPlayer->m_data.vUp.y,
        pPlayer->m_data.vUp.z);
    }
  }

  // poll the input controllers
  m_pController->Update();
  CAControllerData &data = m_pController->m_data;

  if (data.m_aKeys[ DIK_ESCAPE ].Down()) // ESC key
    return FALSE;

  if (data.m_aKeys[ DIK_H ].Down()) // H key
  {
    m_iCurrentScore = -1;
    HighScore();
    m_eGameState = eRunning;
  }

  if (data.m_aKeys[ DIK_F1 ].Down()) // F1 key
  {
    m_eGameState = eHelpScreen;
  }

  if (data.m_aKeys[ DIK_O ].Down()) // O key
  {
    if (m_bFullScreen)
    {
      g_pDirectDraw->FlipToGDISurface();
      RedrawWindow(NULL, NULL, RDW_INVALIDATE);
    }

//    CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();
//    pApp->OnSetupOptions();
  }

  if (data.m_aKeys[ DIK_PGDN ].Repeat() ||
      data.m_aKeys[ DIK_NUMPAD3 ].Repeat())
  {
    if (m_valFieldOfView < MAX_FIELD_OF_VIEW)
      m_valFieldOfView += STEP_FIELD_OF_VIEW;

    m_pViewport->SetField( m_valFieldOfView );
  }

  if (data.m_aKeys[ DIK_PGUP ].Repeat() ||
      data.m_aKeys[ DIK_NUMPAD9 ].Repeat())
  {
    if (m_valFieldOfView > MIN_FIELD_OF_VIEW)
      m_valFieldOfView -= STEP_FIELD_OF_VIEW;

    m_pViewport->SetField( m_valFieldOfView );
  }

  if (data.m_aKeys[ DIK_NUMPAD5 ].Down())
  {
    m_pPlayer->m_vVelocity.x = 0;
    m_pPlayer->m_vVelocity.y = 0;
    m_pPlayer->m_vVelocity.z = 0;
  }

  if (data.m_bFire)  // fire a hot round
  {
    if (m_pBulletCache->GetCount())
    {
      CABullet *pBullet = m_pBulletCache->RemoveHead();
      m_pBulletList->AddTail( pBullet );
      m_pScene->AddChild( pBullet->I() );
      pBullet->m_vVelocity = m_pPlayer->m_data.vDir;
      pBullet->m_vVelocity.Normalize();
      pBullet->m_vVelocity *= 750.0;
      pBullet->m_vVelocity += m_pPlayer->m_vVelocity;

	    pBullet->SetPosition( m_pScene->I(),
        m_pPlayer->m_data.vPos.x,
        m_pPlayer->m_data.vPos.y,
        m_pPlayer->m_data.vPos.z);
	    pBullet->SetOrientation( m_pScene->I(),
        m_pPlayer->m_data.vDir.x,
        m_pPlayer->m_data.vDir.y,
        m_pPlayer->m_data.vDir.z,
        m_pPlayer->m_data.vUp.x,
        m_pPlayer->m_data.vUp.y,
        m_pPlayer->m_data.vUp.z);

      // randomize shot
      pBullet->m_vVelocity.x +=
        ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) / 10;
      pBullet->m_vVelocity.y +=
        ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) / 10;
      pBullet->m_vVelocity.z +=
        ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) / 10;

      pBullet->m_eState = eStart;

      m_pSndGun->SetCurrentPosition(0);
      m_pSndGun->Play(0, 0, 0);
    }
  }

  // apply the shield
  if (data.m_aKeys[ DIK_S ].Down())
  {
    m_pPlayer->ShieldOn();
  }
  if (data.m_aKeys[ DIK_S ].Up())
  {
    m_pPlayer->ShieldOff();
  }

  // rotate, thrust, gravitate and check for borders
  m_pPlayer->UpdateInput( data );

  // check for collisions
  CAVector &vPos = m_pPlayer->m_data.vPos;
  if (SimplePlayerCollisions( m_pPlayer ))
    return TRUE;

  // update the player to the new position
  m_pPlayer->SetPosition( NULL, vPos.x, vPos.y, vPos.z );
  m_pPlayer->SetPlayerData();

  // update moveable shapes
  pos = m_pMoveableList->GetHeadPosition();
  while (pos)
  {
    CAShape *pShape = (CAShape *) m_pMoveableList->GetNext( pos );
    pShape->ApplyGravity( data );
    pShape->TestBorders( data );
    pShape->SetPosition( NULL,
      pShape->m_data.vPos.x,
      pShape->m_data.vPos.y,
      pShape->m_data.vPos.z);
  }

  // update bullets
  pos = m_pBulletList->GetHeadPosition();
  while (pos)
  {
    POSITION oldPos = pos;
    CABullet *pBullet = m_pBulletList->GetNext( pos );

    if (pBullet->m_dwTime > 5000)  // stop the bullet
    {
      pBullet->m_eState = eDone;
      pBullet->Tick();
    }
    else
    if (pBullet->m_eState == eDone)
    {
      // move bullet back to cache
      g_pInvisible->AddChild( pBullet->I() );
      m_pBulletList->RemoveAt( oldPos );
      m_pBulletCache->AddHead( pBullet );
    }
    else  // advance the bullet
    {
      if (pBullet->TestBorders( data ))
      {
        pBullet->m_eState = eDone;
        pBullet->Tick();
      }

      SimpleBulletCollisions( pBullet );

      pBullet->SetPosition( NULL,
        pBullet->m_data.vPos.x,
        pBullet->m_data.vPos.y,
        pBullet->m_data.vPos.z);
      pBullet->Tick();
    }
  }

  // check for the next game level
  if (m_pMoveableList->GetCount() == 0)
  {
    SetupLevel( ++g_iLevel );
  }

  return TRUE;
}

BOOL CAGamePlay::SimplePlayerCollisions( CAPlayerShape *pPlayer )
{
  D3DRMBOX boxPlayer;
  pPlayer->m_pMeshBuilder->GetBox( &boxPlayer );
  CAVector pt1( boxPlayer.min.x, boxPlayer.min.y, boxPlayer.min.z ); 
  CAVector pt2( boxPlayer.max.x, boxPlayer.min.y, boxPlayer.min.z ); 
  CAVector pt3( boxPlayer.max.x, boxPlayer.max.y, boxPlayer.max.z ); 
  CAVector pt4( boxPlayer.min.x, boxPlayer.max.y, boxPlayer.max.z ); 
  pt1 += pPlayer->m_data.vPos;
  pt2 += pPlayer->m_data.vPos;
  pt3 += pPlayer->m_data.vPos;
  pt4 += pPlayer->m_data.vPos;

  POSITION pos = m_pMoveableList->GetHeadPosition();
  while (pos)
  {
    CAShape *pShape = (CAShape *) m_pMoveableList->GetNext( pos );

    D3DRMBOX box;
    pShape->m_pMeshBuilder->GetBox( &box );
    box.min.x += pShape->m_data.vPos.x;
    box.min.y += pShape->m_data.vPos.y;
    box.min.z += pShape->m_data.vPos.z;
    box.max.x += pShape->m_data.vPos.x;
    box.max.y += pShape->m_data.vPos.y;
    box.max.z += pShape->m_data.vPos.z;

    if (PointInBox( &pt1, &box ) ||
        PointInBox( &pt2, &box ) ||
        PointInBox( &pt3, &box ) ||
        PointInBox( &pt4, &box ))
    {
      if (pPlayer->m_bShield)  // shield on
      {
        m_pSndShieldHit->SetCurrentPosition(0);
        m_pSndShieldHit->Play(0, 0, 0);
        return FALSE;
      }

      // the player was hit
      m_eGameState = ePlayerHit;
      m_timePlayerHit = timeGetTime();
      m_pSndPlayerHit->SetCurrentPosition(0);
      m_pSndPlayerHit->Play(0, 0, 0);

      // detach the camera and spin the player
    	CAVector ptCamera;
	    m_pCamera->GetPosition( m_pScene->I(), &ptCamera );
      m_pScene->AddChild( m_pCamera->I() );
      m_pCamera->SetPosition( m_pScene->I(),
        ptCamera.x,
        ptCamera.y,
        ptCamera.z);

      m_pViewport->SetField( MAX_FIELD_OF_VIEW );

      m_pPlayer->SetRotation( m_pScene->I(),
        (rand() > RAND_MAX / 2) ? 1 : -1,
        (rand() > RAND_MAX / 2) ? 1 : -1,
        (rand() > RAND_MAX / 2) ? 1 : -1,
        -0.4);

      m_pPlayer->m_vVelocity.x = 0;
      m_pPlayer->m_vVelocity.y = 1;
      m_pPlayer->m_vVelocity.z = 0;

      // remove all bullets
      POSITION posBullet = m_pBulletList->GetHeadPosition();
      while (posBullet)
      {
        POSITION oldPos = posBullet;
        CABullet *pBullet = m_pBulletList->GetNext( posBullet );

        // move bullet back to cache
        g_pInvisible->AddChild( pBullet->I() );
        m_pBulletList->RemoveAt( oldPos );
        m_pBulletCache->AddHead( pBullet );
      }

      return TRUE;
    }
  }

  return FALSE;
}

BOOL CAGamePlay::SimpleBulletCollisions( CABullet *pBullet )
{
  CAVector pt1, pt2, pt3, pt4;
  pt1 = pBullet->m_data.vPos;
  pt2.x = pt1.x + 100;
  pt2.y = pt1.y + 100;
  pt2.z = pt1.z + 100;
  pt3.x = pt1.x + 50;
  pt3.y = pt1.y + 50;
  pt3.z = pt1.z + 50;
  pt4.x = pt1.x - 50;
  pt4.y = pt1.y - 50;
  pt4.z = pt1.z - 50;

  POSITION pos = m_pMoveableList->GetHeadPosition();
  while (pos)
  {
    POSITION oldPos = pos;
    CARockShape *pRock = (CARockShape *) m_pMoveableList->GetNext( pos );

    D3DRMBOX box;
    pRock->m_pMeshBuilder->GetBox( &box );
    box.min.x += pRock->m_data.vPos.x;
    box.min.y += pRock->m_data.vPos.y;
    box.min.z += pRock->m_data.vPos.z;
    box.max.x += pRock->m_data.vPos.x;
    box.max.y += pRock->m_data.vPos.y;
    box.max.z += pRock->m_data.vPos.z;

    if (PointInBox( &pt1, &box ) ||
        PointInBox( &pt2, &box ) ||
        PointInBox( &pt3, &box ) ||
        PointInBox( &pt4, &box ))
    {
      // make the bullet explode
	    pBullet->SetPosition( NULL, pt1.x, pt1.y, pt1.z );
      pBullet->m_vVelocity.x = 0;
      pBullet->m_vVelocity.y = 0;
      pBullet->m_vVelocity.z = 0;
      pBullet->m_eState = eExplode;

      m_pSndHit->SetCurrentPosition(0);
      m_pSndHit->Play(0, 0, 0);

      // remove the original rock
      m_pMoveableList->RemoveAt(oldPos);
      g_pInvisible->AddChild( pRock->I() );

      // break the rock up into pieces
      CAShape *pNewRock;
      int i;
      switch (pRock->m_nSize)
      {
      case ROCK_SIZE_LARGE:
        for (i = 0; i < 4; i++)
        {
          pNewRock = CreateRock(
            g_liLevel[ g_iLevel ].iMediumRockType,
            ROCK_SIZE_MEDIUM);
          pNewRock->SetPosition( m_pScene->I(),
            pRock->m_data.vPos.x +
            ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * ROCK_SIZE_LARGE - ROCK_SIZE_LARGE / 2,
            pRock->m_data.vPos.y +
            ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * ROCK_SIZE_LARGE - ROCK_SIZE_LARGE / 2,
            pRock->m_data.vPos.z +
            ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * ROCK_SIZE_LARGE - ROCK_SIZE_LARGE / 2);
        }
        m_nScore += 50;
        break;

      case ROCK_SIZE_MEDIUM:
        for (i = 0; i < 2; i++)
        {
          pNewRock = CreateRock(
            g_liLevel[ g_iLevel ].iSmallRockType,
            ROCK_SIZE_SMALL);
          pNewRock->SetPosition( m_pScene->I(),
            pRock->m_data.vPos.x +
            ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * ROCK_SIZE_MEDIUM - ROCK_SIZE_MEDIUM / 2,
            pRock->m_data.vPos.y +
            ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * ROCK_SIZE_MEDIUM - ROCK_SIZE_MEDIUM / 2,
            pRock->m_data.vPos.z +
            ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * ROCK_SIZE_MEDIUM - ROCK_SIZE_MEDIUM / 2);
        }
        m_nScore += 100;
        break;

      case ROCK_SIZE_SMALL:
        m_nScore += 200;
        break;

      default:
        break;
      }

      delete pRock;

      return TRUE;
    }
  }

  return FALSE;
}

BOOL CAGamePlay::CheckPlayerCollisions( CAPlayerShape *pPlayer, CAVector vOldPos )
{
  CAVector &vPos = pPlayer->m_data.vPos;
  CAVector &vVelocity = pPlayer->m_vVelocity;

  CAShape *pCollide = NULL;
  D3DVALUE d = pPlayer->m_boundaryRadius * 2;

  if (vVelocity.Mag() > d)  // check the gap for collisions
  {
    int steps = (int)(vVelocity.Mag() / d);

    for (int i = 1; i <= steps; i++)
    {
      CAVector vStep = vVelocity;
      vStep.Normalize();
      vStep *= d * i;
      vStep += vOldPos;

      pCollide = pPlayer->Collide( vStep, m_pBoundaryList );

      if (pCollide)
      {
        vPos = vStep;
        break;
      }
    }
  }
  else  // no gap
    pCollide = pPlayer->Collide( vPos, m_pBoundaryList );

  if (pCollide)
  {
    ASSERT(pCollide != pPlayer);

    // now step back until no collision
    CAVector vGap = vVelocity;
    vGap.Normalize();
    
    if (vGap.Mag())
    {
      int i = 0;
      while (i++ < 50 && pPlayer->Collide( vPos, m_pBoundaryList ) != NULL)
        vPos -= vGap;
    }

    // do something with the hit
    if (pCollide == m_pFlag)
    {
      m_pSndJump->SetCurrentPosition(0);
      m_pSndJump->Play(0, 0, 0);
    }
    else // bounce off?
    if (vVelocity.Mag() > .25)
    {
      m_pSndHit->SetCurrentPosition(0);
      m_pSndHit->Play(0, 0, 0);

      // split the velocity into components and add to each object
    	CAVector ptCollide;
	    pCollide->GetPosition( NULL, &ptCollide );

    	CAVector vContact = vPos;
      vContact -= ptCollide;

      // project the velocity onto the contact vector
      D3DVALUE magContact = vVelocity.Dot( vContact ) / vContact.Mag();
      vContact.Normalize();
      vContact *= magContact;

      // add component to contacted object
      pCollide->m_vVelocity += vContact;

      // add component to player
      vVelocity = -vContact * .5;  // 50%
      if (pPlayer->Collide( vPos + vVelocity, m_pBoundaryList ) == NULL)
        vPos += vVelocity;
    }
    else  // slow to a stop
    {
      vVelocity.x = 0;
      vVelocity.y = 0;
      vVelocity.z = 0;
    }

    return TRUE;
  }

  return FALSE;
}

BOOL CAGamePlay::CheckBulletCollisions( CABullet *pBullet, CAVector &vOldPos )
{
  CAVector &vPos = pBullet->m_data.vPos;
  CAVector &vVelocity = pBullet->m_vVelocity;

  // pre-check the entire gap
  CAVector vDiff;
  vDiff.x = (vPos.x - vOldPos.x) / 2;
  vDiff.y = (vPos.y - vOldPos.y) / 2;
  vDiff.z = (vPos.z - vOldPos.z) / 2;
  CAVector vMiddle = vOldPos + vDiff;
  D3DVALUE r = vDiff.Mag();
  CAShape shMiddle( g_pDirect3DRM, NULL );
  shMiddle.SetPosition( m_pScene->I(),
    vMiddle.x,
    vMiddle.y,
    vMiddle.z);
  shMiddle.SetBoundary( CAVector(0, 0, 0), .1, .1 );
  CAShapeList newList;
  newList.AddTail( &shMiddle );

  CAShape *pCollide = pBullet->Collide( vMiddle, &newList );

  if (pCollide == NULL) return FALSE;


  D3DVALUE d = pBullet->m_boundaryRadius * 2;
  D3DVALUE speed = vVelocity.Mag();

  if (speed > d)  // check the gap for collisions
  {
    int steps = (int)(speed / d);

    for (int i = 1; i <= steps; i++)
    {
      CAVector vStep = vVelocity;
      vStep.Normalize();
      vStep *= d * i;
      vStep += vOldPos;

      pCollide = pBullet->Collide( vStep, m_pBoundaryList );

      if (pCollide)
      {
        vPos = vStep;
        break;
      }
    }
  }
  else  // no gap
    pCollide = pBullet->Collide( vPos, m_pBoundaryList );

  if (pCollide)
  {
    ASSERT(pCollide != pBullet);

    vPos = vOldPos;

    // make the bullet explode
    pBullet->m_vVelocity.x = 0;
    pBullet->m_vVelocity.y = 0;
    pBullet->m_vVelocity.z = 0;
    pBullet->m_eState = eExplode;
    m_pSndHit->SetCurrentPosition(0);
    m_pSndHit->Play(0, 0, 0);
  }

  return TRUE;
}

BOOL CAGamePlay::Render()
{
  // rendering sequence
  m_pScene->Move(D3DVAL(1.0));
  m_pViewport->Clear();
  m_pViewport->Render( m_pScene->I() );
  m_pDevice->Update();

  return TRUE;
}

void CAGamePlay::DisplayGDI( HDC hdc, CRect *pRect )
{
  CBrush br(RGB(
    g_liLevel[ g_iLevel ].clrBackgroundR * 255,
    g_liLevel[ g_iLevel ].clrBackgroundG * 255,
    g_liLevel[ g_iLevel ].clrBackgroundB * 255));
  FillRect( hdc, *pRect, br);

//  SetBkMode(hdc, TRANSPARENT);
  SetTextColor( hdc, RGB( 255, 255, 0 ) );
  SetBkColor( hdc, RGB(
      g_liLevel[ g_iLevel ].clrBackgroundR * 255,
      g_liLevel[ g_iLevel ].clrBackgroundG * 255,
      g_liLevel[ g_iLevel ].clrBackgroundB * 255));
  SelectObject( hdc, (HFONT) m_fontPanel );

  CRect rText = *pRect;
  CString sText;
  sText.Format( "Level: %ld", g_iLevel + 1 + g_nLevels * g_nLevelWrap );
  DrawText( hdc, sText, sText.GetLength(), &rText, 0 );

  sText.Format( "Score:  %ld", m_nScore );
  DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_CALCRECT );
  rText.OffsetRect( 0, pRect->Height() / 2 - pRect->Height() / 2 );
  rText.right = pRect->right;
  DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER );

  sText.Format( "Lives:  %ld", m_nShips );
  DrawText( hdc, sText, sText.GetLength(), &rText, DT_RIGHT );
}

void CAGamePlay::DrawSlider(CDC* pDC, LPRECT pRect, double percent, LPCTSTR pMinLabel, LPCTSTR pMaxLabel, BOOL bBar )
{
  int nSavedDC = pDC->SaveDC();
  CPen pen( PS_NULL, 1, RGB(0,0,0) );
  pDC->SelectObject(&pen);
  CBrush brInside( RGB(0,232,232) );
  pDC->SelectObject( &brInside );
  pDC->SetBkMode(TRANSPARENT);

  TEXTMETRIC tm;
  pDC->GetTextMetrics( &tm );
  CRect rBar = pRect;
  int vBorder = tm.tmHeight;
  int hBorder = rBar.Width() * .25;
  rBar.DeflateRect( hBorder, vBorder );
  pDC->Rectangle( &rBar );

  // draw slider
  rBar.top += (rBar.Height() - SLIDER_HEIGHT) - (percent * ((double) rBar.Height() - SLIDER_HEIGHT));

  CBrush brBar( RGB(0,69,125) );
  pDC->SelectObject( &brBar );
  if (bBar) rBar.bottom = rBar.top + SLIDER_HEIGHT;
  pDC->Rectangle( &rBar );

  // draw labels
  UINT uFormat = DT_EDITCONTROL | DT_NOPREFIX | DT_WORDBREAK | DT_EXPANDTABS | DT_CENTER;
  CRect rLabel = pRect;
  pDC->DrawText( pMaxLabel, strlen(pMaxLabel), &rLabel, uFormat);
  rLabel.top = rLabel.bottom - vBorder;
  pDC->DrawText( pMinLabel, strlen(pMinLabel), &rLabel, uFormat);

  pDC->RestoreDC( nSavedDC );
}

LONG CAGamePlay::OnMCINotify( UINT wParam, LONG lParam )
{
  if (wParam == MCI_NOTIFY_SUCCESSFUL)
  {
    m_pMusic->Restart();
  }

  return 0L;
}

////////////////////////////////////////////////////////////////////////////
BOOL CAGamePlay::CreatePlayField()
{
  // create the border grids
  CAShape topBorder( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( topBorder.I() );
  topBorder.CreateMesh( -BOUNDARY, BOUNDARY, BOUNDARY_STEP,  -BOUNDARY, BOUNDARY, BOUNDARY_STEP, NULL, this );
  topBorder.AddRotation( D3DRMCOMBINE_REPLACE, 0,0,1, -3.14); // 180 degs
  topBorder.SetPosition( m_pScene->I(), 0, BOUNDARY, 0 );
  topBorder.m_pMeshBuilder->SetColorRGB( 0, .5, 1);
  topBorder.m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );

  CAShape bottomBorder( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( bottomBorder.I() );
  bottomBorder.CreateMesh( -BOUNDARY, BOUNDARY, BOUNDARY_STEP,  -BOUNDARY, BOUNDARY, BOUNDARY_STEP, NULL, this );
  bottomBorder.SetPosition( m_pScene->I(), 0, -BOUNDARY, 0 );
  bottomBorder.m_pMeshBuilder->SetColorRGB( 0, .5, 1);
  bottomBorder.m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );

  CAShape leftBorder( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( leftBorder.I() );
  leftBorder.CreateMesh( -BOUNDARY, BOUNDARY, BOUNDARY_STEP,  -BOUNDARY, BOUNDARY, BOUNDARY_STEP, NULL, this );
  leftBorder.AddRotation( D3DRMCOMBINE_REPLACE, 0,0,1, -1.57); // 90 degs
  leftBorder.SetPosition( m_pScene->I(), -BOUNDARY, 0, 0 );
  leftBorder.m_pMeshBuilder->SetColorRGB( 0, .5, 1);
  leftBorder.m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );

  CAShape rightBorder( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( rightBorder.I() );
  rightBorder.CreateMesh( -BOUNDARY, BOUNDARY, BOUNDARY_STEP,  -BOUNDARY, BOUNDARY, BOUNDARY_STEP, NULL, this );
  rightBorder.AddRotation( D3DRMCOMBINE_REPLACE, 0,0,1, 1.57); // 90 degs
  rightBorder.SetPosition( m_pScene->I(), BOUNDARY, 0, 0 );
  rightBorder.m_pMeshBuilder->SetColorRGB( 0, .5, 1);
  rightBorder.m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );

  CAShape frontBorder( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( frontBorder.I() );
  frontBorder.CreateMesh( -BOUNDARY, BOUNDARY, BOUNDARY_STEP,  -BOUNDARY, BOUNDARY, BOUNDARY_STEP, NULL, this );
  frontBorder.AddRotation( D3DRMCOMBINE_REPLACE, 1,0,0, -1.57); // 90 degs
  frontBorder.SetPosition( m_pScene->I(), 0, 0, BOUNDARY );
  frontBorder.m_pMeshBuilder->SetColorRGB( 0, .5, 1);
  frontBorder.m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );

  CAShape backBorder( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( backBorder.I() );
  backBorder.CreateMesh( -BOUNDARY, BOUNDARY, BOUNDARY_STEP,  -BOUNDARY, BOUNDARY, BOUNDARY_STEP, NULL, this );
  backBorder.AddRotation( D3DRMCOMBINE_REPLACE, 1,0,0, 1.57); // 90 degs
  backBorder.SetPosition( m_pScene->I(), 0, 0, -BOUNDARY );
  backBorder.m_pMeshBuilder->SetColorRGB( 0, .5, 1);
  backBorder.m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );

/*
  // create the portals
  CAShape portal( g_pDirect3DRM, m_pScene->I() );
  portal.Load( "portal.x" );
  portal.m_pMeshBuilder->SetColorRGB( 1, 1, 0 );
  portal.SetRotation( m_pScene->I(),
    (rand() > RAND_MAX / 2) ? 1 : -1,
    (rand() > RAND_MAX / 2) ? 1 : -1,
    (rand() > RAND_MAX / 2) ? 1 : -1,
    -0.2);
*/
  return TRUE;
}

LONG CAGamePlay::OnDpSetPlayerData( UINT wParam, LONG lParam )
{
  DPMSG_SETPLAYERORGROUPDATA *lp = (DPMSG_SETPLAYERORGROUPDATA *) lParam;
  ASSERT( lp->dwType == DPSYS_SETPLAYERORGROUPDATA );
  ASSERT( lp->dwPlayerType == DPPLAYERTYPE_PLAYER );
  ASSERT( lp->dwDataSize == sizeof SAShapeData );

  if (lp->dpId == g_pPlayer->m_dpidPlayer)  // don't allow self-messages?
    return 0L;

  // find the player and set its data
  POSITION pos = m_pPlayerShapeList->GetHeadPosition();
  while (pos)
  {
    CAPlayerShape *pPlayer = (CAPlayerShape *) m_pPlayerShapeList->GetNext( pos );

    if (lp->dpId == pPlayer->m_pOnlinePlayer->m_dpidPlayer)
    {
      SAShapeData *pPlayerData = (SAShapeData *) lp->lpData;
      pPlayer->m_data = *pPlayerData;
      pPlayer->SetPosition( NULL,
        pPlayerData->vPos.x,
        pPlayerData->vPos.y,
        pPlayerData->vPos.z);
      pPlayer->SetOrientation( NULL,
        pPlayerData->vDir.x,
        pPlayerData->vDir.y,
        pPlayerData->vDir.z,
        pPlayerData->vUp.x,
        pPlayerData->vUp.y,
        pPlayerData->vUp.z);
      break;
    }
  }
  
  return 0L;
}

void CAGamePlay::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if (nChar == VK_PAUSE)
    if (m_eGameState == eRunning)
      m_eGameState = ePaused;
    else
      m_eGameState = eRunning;

	CADxaWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CAGamePlay::SetupLevel( int iLevel )
{
  DWORD time = timeGetTime();

  if (iLevel >= g_nLevels)
  {
    g_iLevel = 0;
    g_nLevelWrap++;
  }
  else
    g_iLevel = iLevel;

  // display the splash screen
  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetBkMode( hdc, TRANSPARENT );
    CBrush br(RGB(
      g_liLevel[ g_iLevel ].clrBackgroundR * 255,
      g_liLevel[ g_iLevel ].clrBackgroundG * 255,
      g_liLevel[ g_iLevel ].clrBackgroundB * 255));
    FillRect( hdc, m_rBack, br);

    CFont fontText;
    fontText.CreateFont( m_rBack.Height() / 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      _T("Arial"));
    SelectObject( hdc, (HFONT) fontText );

    CString sText;
    sText.Format("Level %d ...", g_iLevel + 1 + g_nLevels * g_nLevelWrap);
    CRect rText = m_rBack;
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK | DT_CALCRECT );
    rText.OffsetRect( 0, m_rBack.Height() / 2 - rText.Height() / 2 );
    rText.right = m_rBack.right;
    SetTextColor( hdc, RGB( 0, 0, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );
    rText.OffsetRect( -4, -4 );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    DrawText( hdc, sText, sText.GetLength(), &rText, DT_CENTER | DT_WORDBREAK );

    m_pBack->ReleaseDC(hdc);
  }

  FlipScreen();

  // setup the level
  m_pMusic->Play( "wierd.mid" );

  m_pScene->SetSceneBackgroundRGB(
    g_liLevel[ g_iLevel ].clrBackgroundR,
    g_liLevel[ g_iLevel ].clrBackgroundG,
    g_liLevel[ g_iLevel ].clrBackgroundB);

  // remove any existing rocks
  POSITION pos = m_pMoveableList->GetHeadPosition();
  while (pos)
  {
    POSITION oldPos = pos;
    CARockShape *pRock = (CARockShape *) m_pMoveableList->GetNext( pos );

    m_pMoveableList->RemoveAt( oldPos );
    g_pInvisible->AddChild( pRock->I() );

    delete pRock;
  }

  // create new rocks
  for (int i = 0; i < g_liLevel[ g_iLevel ].nLargeRocks + (2 * g_nLevelWrap); i++)
    CreateRock(
      g_liLevel[ g_iLevel ].iLargeRockType,
      ROCK_SIZE_LARGE);

  for (i = 0; i < g_liLevel[ g_iLevel ].nMediumRocks + (2 * g_nLevelWrap); i++)
    CreateRock( 
      g_liLevel[ g_iLevel ].iMediumRockType,
      ROCK_SIZE_MEDIUM);

  for (i = 0; i < g_liLevel[ g_iLevel ].nSmallRocks + 2 * g_nLevelWrap; i++)
    CreateRock(
      g_liLevel[ g_iLevel ].iSmallRockType,
      ROCK_SIZE_SMALL);

  // reset the player position
  m_pPlayer->SetPosition( m_pScene->I(), 0, 0, 0 );
  m_pPlayer->SetOrientation( m_pScene->I(),  0, 0, 1,  0, 1, 0 );
  m_pPlayer->SetRotation( m_pScene->I(), 0, 0, 0, 0 );

  // delay so the user can get ready
  while (timeGetTime() - time < 3000)
    ;

  // start the background music
  m_pMusic->Play( g_liLevel[ g_iLevel ].szMusic );

  return TRUE;
}

CAShape *CAGamePlay::CreateRock( int iType, int nSize )
{
  // create the rocks
  CARockShape *pShape = new CARockShape( g_pDirect3DRM, NULL );
  pShape->m_iType = iType;
  pShape->m_nSize = nSize;
  m_pScene->AddChild( pShape->I() );

  switch (iType)
  {
  case 0:
    pShape->CreateCube(nSize);
    break;
  case 1:
    pShape->CreateRod( 0, 0, 0,  0,nSize,0,  nSize, 16 );
    break;
  case 2:
    pShape->CreateSphere(nSize);
    break;
  case 3:
    {
      D3DVALUE height = nSize + ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * 5;
      pShape->CreateCone( 0,0,0, nSize, TRUE,  0,height,0, 0, TRUE, 16 );
    }
    break;
  case 4:
    pShape->CreateRock( nSize, 6);
    break;
  case 5:  // random .x
    pShape->Load( g_aRandShapes[ (int) ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * (g_nRandShapes - 1) ] );
    pShape->m_pMeshBuilder->Scale( nSize / 5, nSize / 5, nSize / 5 );
    break;
  }

  pShape->m_pMeshBuilder->SetColorRGB(
    ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) / 2 + .5,
    ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) / 2 + .5,
    ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) / 2 + .5);

  CAVector vPos;
  vPos.x = (((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * (BOUNDARY - 200) + 200);
  vPos.x *= (rand() > RAND_MAX / 2) ? 1 : -1;
  vPos.y = (((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * (BOUNDARY - 200) + 200);
  vPos.y *= (rand() > RAND_MAX / 2) ? 1 : -1;
  vPos.z = (((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * (BOUNDARY - 200) + 200);
  vPos.z *= (rand() > RAND_MAX / 2) ? 1 : -1;
  pShape->SetPosition( m_pScene->I(), vPos.x, vPos.y, vPos.z );

  pShape->SetRotation( pShape->I(),
    (rand() > RAND_MAX / 2) ? 1 : -1,
    (rand() > RAND_MAX / 2) ? 1 : -1,
    (rand() > RAND_MAX / 2) ? 1 : -1,
    -0.2);

  pShape->m_vVelocity.x = 
    ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * 300 - 150;
  pShape->m_vVelocity.y =
    ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * 300 - 150;
  pShape->m_vVelocity.z = 
    ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * 300 - 150;
/*
  CADirect3DRMWrap wrap3(
    g_pDirect3DRM,
    D3DRMWRAP_CYLINDER, NULL,
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0),
    D3DVAL(0.0), D3DDivide(0 , 4 ),
    D3DVAL(1.0), D3DDivide(-D3DVAL(1.0), 4 ));
  wrap3.Apply( (LPDIRECT3DRMOBJECT) pShape->m_pMeshBuilder->I() );
  pShape->m_pMeshBuilder->SetTexture( m_pCanvas->I() );
*/
  m_pMoveableList->AddTail( pShape );

  return pShape;
}

static D3DVALUE GroundHeight(D3DVALUE x, D3DVALUE z, void* pArg)
{
  if ((x >= -900 && x <= 900) &&
      (z >= -900 && z <= 900))
      return 0;

  D3DVALUE val = ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * 5;

  if (val >= 4.0)
    return 50 + val;
  else
  if (val <= 1.0)
    return 0;
  else
    return val;
}

static void CDECL listenerCallback( LPDIRECT3DRMFRAME obj, void* arg, D3DVALUE delta)
{
  CAGamePlay *pGame = (CAGamePlay *) arg;

  CAVector vPos, vDir, vUp;
	pGame->m_pCamera->GetPosition( NULL, &vPos );
	pGame->m_pCamera->GetOrientation( NULL, &vDir, &vUp);

  pGame->m_pListener->SetPosition( vPos.x, vPos.y, vPos.z, DS3D_DEFERRED );
  pGame->m_pListener->SetOrientation( vDir.x, vDir.y, vDir.z,  vUp.x,  vUp.y,  vUp.z,  DS3D_DEFERRED);
	pGame->m_pListener->CommitDeferredSettings();
}

static BOOL PointInBox( CAVector *pPoint, D3DRMBOX *pBox )
{
  if ((pPoint->x >= pBox->min.x) && (pPoint->x <= pBox->max.x) &&
      (pPoint->y >= pBox->min.y) && (pPoint->y <= pBox->max.y) &&
      (pPoint->z >= pBox->min.z) && (pPoint->z <= pBox->max.z) )
    return TRUE;

  return FALSE;
}

#ifdef SAVE_ME  // create the earth
{
  CADirect3DRMMeshBuilder *pMesh = new CADirect3DRMMeshBuilder( g_pDirect3DRM );
  pMesh->Load( "sphere3.x", NULL, D3DRMLOAD_FROMFILE, NULL, NULL);
  pMesh->Scale(D3DVAL(2), D3DVAL(2), D3DVAL(2));
  pMesh->SetColorRGB(D3DVAL(1), D3DVAL(1), D3DVAL(1));

  D3DRMBOX box;
  pMesh->GetBox( &box );

  D3DVALUE miny, maxy, height;
  maxy = box.max.y;
  miny = box.min.y;
  height = maxy - miny;

  CADirect3DRMWrap *pWrap = new CADirect3DRMWrap(
    g_pDirect3DRM,
    D3DRMWRAP_CYLINDER, NULL,
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0),
    D3DVAL(0.0), D3DDivide(miny, height),
    D3DVAL(1.0), D3DDivide(-D3DVAL(1.0), height) );
  pWrap->Apply( (LPDIRECT3DRMOBJECT) pMesh->I() );

  CADirect3DRMTexture *pTexture =
    new CADirect3DRMTexture( g_pDirect3DRM, "hello.ppm" );
  pMesh->SetTexture( pTexture->I() );

  CADirect3DRMFrame *pEarthFrame =
    new CADirect3DRMFrame( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( pEarthFrame->I() );
  pEarthFrame->SetRotation(NULL, 0, 1, 0, -0.03490555555556);
  pEarthFrame->AddVisual( pMesh->I() );

  CADirect3DRMMeshBuilder *pMesh2 = new CADirect3DRMMeshBuilder( g_pDirect3DRM );
  pMesh2->Load( "sphere3.x", NULL, D3DRMLOAD_FROMFILE, NULL, NULL);
  pMesh2->Scale(D3DVAL(.25), D3DVAL(.25), D3DVAL(.25));
  pMesh2->SetColorRGB(D3DVAL(1), D3DVAL(1), D3DVAL(1));
  pWrap->Apply( (LPDIRECT3DRMOBJECT) pMesh2->I() );
  pMesh2->SetTexture( pTexture->I() );

  CADirect3DRMFrame *pEarthFrame2 =
    new CADirect3DRMFrame( g_pDirect3DRM, m_pScene->I() );
  m_pScene->AddChild( pEarthFrame2->I() );
  pEarthFrame2->SetPosition(NULL, D3DVAL(10), D3DVAL(0), D3DVAL(0));
  pEarthFrame2->SetRotation(m_pScene->I(), 0, 1, 0, -0.1);
  pEarthFrame2->AddVisual( pMesh2->I() );
}
#endif

#ifdef SAVE_ME  // logo animation
{
static int logoTick = 0;
m_pLogo->SetTime( logoTick = logoTick++ % 20 );

  // setup 'Weston'
  CAShape w( g_pDirect3DRM, m_pScene->I() );
  w.Load( "w.x" );
  w.m_pMeshBuilder->Scale(D3DVAL(2), D3DVAL(2), D3DVAL(2));
  w.SetPosition( m_pScene->I(), D3DVAL(0), D3DVAL(4), D3DVAL(-15) );
//  w.SetRotation( m_pScene->I(), 0, 1, 0, -0.03);
  m_pScene->AddChild( w.I() );

  // setup 'Software'
  CAShape s( g_pDirect3DRM, m_pScene->I() );
  s.Load( "s.x" );
  s.m_pMeshBuilder->Scale(D3DVAL(2), D3DVAL(2), D3DVAL(2));
  s.SetPosition( m_pScene->I(), D3DVAL(0), D3DVAL(2), D3DVAL(-15) );
//  s.SetRotation( m_pScene->I(), 0, 1, 0, -0.03);
  m_pScene->AddChild( s.I() );

  // setup an animation
  m_pLogo = new CADirect3DRMAnimation( g_pDirect3DRM );
  m_pLogo->SetOptions( D3DRMANIMATION_LINEARPOSITION | D3DRMANIMATION_POSITION );
  m_pLogo->SetFrame( w.I() );
  for (int j = 0; j < 10; j++)
    m_pLogo->AddPositionKey( j, D3DVAL(0), D3DVAL(j), D3DVAL(-15) );
  for (; j < 20; j++)
    m_pLogo->AddPositionKey( j, D3DVAL(0), D3DVAL(j-10), D3DVAL(-15) );
}
#endif

#ifdef SAVE_ME  // use a surface as a texture

DDSURFACEDESC ddsd;
memset(&ddsd, 0, sizeof(DDSURFACEDESC));
ddsd.dwSize = sizeof( ddsd );
ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
ddsd.dwWidth = 256;
ddsd.dwHeight = 256;
ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
CADirectDrawSurface *pTmpSurface = new CADirectDrawSurface( g_pDirectDraw, &ddsd );

HDC hdc;
pTmpSurface->GetDC(&hdc);
SetBkColor( hdc, RGB( 0, 0, 255 ) );
SetTextColor( hdc, RGB( 255, 255, 0 ) );
CString sTmp("Andy Weston is certainly quite creative!");
PatBlt(hdc, 0, 0, 256,256, BLACKNESS);
TextOut( hdc, 0, 0, sTmp, sTmp.GetLength() );
pTmpSurface->ReleaseDC(hdc);

LPDIRECT3DRMTEXTURE lpD3DTexture;
g_pDirect3DRM->CreateTextureFromSurface( pTmpSurface->I(), &lpD3DTexture );

  CADirect3DRMWrap wrap(
    g_pDirect3DRM,
    D3DRMWRAP_FLAT, NULL,
    D3DVAL(-100.0), D3DVAL(0.0), D3DVAL(-100.0),   // origin of the wrap
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),   // normal of the texture 
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0),   // direction to "unroll"
    D3DVAL(0.0), D3DVAL(0.0),                // origin in texture
    D3DVAL(.005), D3DVAL(.005) );                // scale factor 1=%100, 1.5=%50, .5=x2 ???

  CADirect3DRMWrap wrap2(
    g_pDirect3DRM,
    D3DRMWRAP_FLAT, NULL,
    D3DVAL(0), D3DVAL(.5), D3DVAL(0),   // origin of the wrap
    D3DVAL(0.0), D3DVAL(0), D3DVAL(-1.0),   // normal of the texture 
    D3DVAL(0.0), D3DVAL(1), D3DVAL(0),   // direction to "unroll"
    D3DVAL(0.0), D3DVAL(0.0),                // origin in texture
    D3DVAL(1), D3DVAL(1) );                // scale factor 1=%100, 1.5=%50, .5=x2 ???

  CADirect3DRMWrap wrap3(
    g_pDirect3DRM,
    D3DRMWRAP_CYLINDER, NULL,
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0),
    D3DVAL(0.0), D3DDivide(0 /*miny*/, 4 /*height*/),
    D3DVAL(1.0), D3DDivide(-D3DVAL(1.0), 4 /*height*/));

CADirect3DRMWrap wrap2(
  g_pDirect3DRM,
  D3DRMWRAP_FLAT, NULL,
  D3DVAL(0), D3DVAL(0), D3DVAL(0),   // origin of the wrap
  D3DVAL(0.0), D3DVAL(0), D3DVAL(-1.0),   // normal of the texture 
  D3DVAL(0.0), D3DVAL(1), D3DVAL(0),   // direction to "unroll"
  D3DVAL(0.0), D3DVAL(0.0),                // origin in texture
  D3DVAL(1), D3DVAL(1) );                // scale factor 1=%100, 1.5=%50, .5=x2 ???
wrap2.Apply( (LPDIRECT3DRMOBJECT) m_pPennant->m_pMeshBuilder->I() );
... SetTexture()
lpD3DTexture->Release();

#endif

#ifdef SAVE_ME  // setup fog params
{
m_pScene->SetSceneFogMode( D3DRMFOG_LINEAR );
m_pScene->SetSceneFogEnable( TRUE );
D3DVALUE start, end, density;
m_pScene->GetSceneFogParams( &start, &end, &density );
m_pScene->SetSceneFogParams( 100, 1000, density );
}
#endif

#ifdef SAVE_ME  // add my head to the top
  CAShape headFrame( g_pDirect3DRM, m_pScene->I() );
  headFrame.SetPosition( m_pScene->I(), 0, nSteps * 2, 0 );
  CADirect3DRMTexture head( g_pDirect3DRM, "head.bmp" );  // upside-down
  head.SetDecalScale( TRUE );
  head.SetDecalSize( 4.0, 4.0 );
  head.SetDecalOrigin( 128, 256 );
  head.SetDecalTransparency( TRUE );
  head.SetDecalTransparentColor( D3DRGB(0,0,0) );
  headFrame.AddVisual( (LPDIRECT3DRMVISUAL)head.I() );
#endif

#ifdef SAVE_ME  
/*
  int nSteps = 6;
  for (i = 0; i < nSteps; i++)
  {
    m_aHillSteps[ i ] = new CAShape( g_pDirect3DRM, m_pScene->I() );
    m_aHillSteps[ i ]->CreateRod( 0, i*2, 0,  0, i*2+2, 0, (nSteps-i)*2, 16 );
    m_aHillSteps[ i ]->m_pMeshBuilder->SetColorRGB(
      D3DVAL((i+1)*.25), D3DVAL(0.1), D3DVAL(0.1));

    m_aHillSteps[ i ]->SetBoundary( CAVector(0, 0, 0), 2, (nSteps-i)*2 );
    m_pBoundaryList->AddTail( m_aHillSteps[ i ] );

    m_pScene->AddChild( m_aHillSteps[ i ]->I() );
    m_nHillSteps++;
  }
*/
/*
  // create the ground
  CAShape ground( g_pDirect3DRM, m_pScene->I() );
  ground.CreateMesh( -1000, 1000, 50,  -1000, 1000, 50, GroundHeight, this );
  ground.m_pMeshBuilder->SetColorRGB(D3DVAL(0), D3DVAL(1), D3DVAL(1));
  ground.m_pMeshBuilder->SetQuality( D3DRMRENDER_UNLITFLAT );

  wrap.Apply( (LPDIRECT3DRMOBJECT) ground.m_pMeshBuilder->I() );
  ground.m_pMeshBuilder->SetTexture( texture.I() );

  m_pScene->AddChild( ground.I() );
*/
/*
  // create bouys
  for (int ib = -BOUNDARY; ib <= BOUNDARY; ib += BOUNDARY/2)
  for (int jb = -BOUNDARY; jb <= BOUNDARY; jb += BOUNDARY/2)
  for (int kb = -BOUNDARY; kb <= BOUNDARY; kb += BOUNDARY/2)
  {
    if ((ib == -BOUNDARY || ib == BOUNDARY) ||
        (jb == -BOUNDARY || jb == BOUNDARY) ||
        (kb == -BOUNDARY || kb == BOUNDARY) )
    {
      CAShape cube( g_pDirect3DRM, m_pScene->I() );
      m_pScene->AddChild( cube.I() );
      cube.CreateCube(4);
      cube.SetPosition( m_pScene->I(), ib, jb, kb );
    }
  }
*/

/*
  // create the flag frame
  m_pFlag = new CAShape( g_pDirect3DRM, NULL );
  m_pScene->AddChild( m_pFlag->I() );
  m_pFlag->SetPosition( m_pScene->I(), D3DVAL(-15.0), D3DVAL(0), D3DVAL(-15.0) );

  m_pPole = new CAShape( g_pDirect3DRM, NULL );
  m_pFlag->AddVisual( m_pPole->I() );
  m_pPole->CreateRod( 0, 0, 0,  0, 1.5, 0, 0.1, 16 );
  m_pPole->m_pMeshBuilder->SetColorRGB( D3DVAL(0.5), D3DVAL(0.25), D3DVAL(0));

  m_pPennant = new CAShape( g_pDirect3DRM, NULL );
  m_pFlag->AddVisual( m_pPennant->I() );
  m_pPennant->CreateCuboid( 1, 1, .05 );
  m_pPennant->m_pMeshBuilder->Translate( .5, .5 + .5, 0.0);
  m_pPennant->m_pMeshBuilder->SetColorRGB( D3DVAL(1), D3DVAL(1), D3DVAL(1));

  m_pFlag->SetRotation( m_pFlag->I(), 0, 1, 0, -0.06);

  m_pFlag->SetBoundary( CAVector(0, 0, 0), 1.5, .5 );
  m_pBoundaryList->AddTail( m_pFlag );

  wrap2.Apply( (LPDIRECT3DRMOBJECT) m_pPennant->m_pMeshBuilder->I() );
  m_pPennant->m_pMeshBuilder->SetTexture( texture.I() );

*/
  // add the heads-up display
//  CADecal decal( g_pDirect3DRM, NULL );
//  decal.SetTextLabel( "Hullo there how are you I am fine very nice day how'bout those Cubies?" );

/*
  // create the rotating squares
  m_pBase = new CAShape( g_pDirect3DRM, NULL );
  m_pScene->AddChild( m_pBase->I() );
  m_pBase->SetPosition( m_pScene->I(), D3DVAL(15.0), D3DVAL(0), D3DVAL(-15.0) );

  CAShape bar( g_pDirect3DRM, NULL );
  bar.CreateCuboid( 4, 1, 1 );
  bar.m_pMeshBuilder->SetColorRGB( 0, 1, 0 );
  bar.m_pMeshBuilder->Translate( 0, 0.5, 0 );

  CAShape bar2( g_pDirect3DRM, NULL );
  bar2.CreateCuboid( 4, 1, 1 );
  bar2.m_pMeshBuilder->SetColorRGB( 0, 1, 0 );
  bar2.m_pMeshBuilder->Translate( 0, 0.5 + 3, 0 );

  CAShape bar3( g_pDirect3DRM, NULL );
  bar3.CreateCuboid( 1, 2, 1 );
  bar3.m_pMeshBuilder->SetColorRGB( 0, 1, 0 );
  bar3.m_pMeshBuilder->Translate( -.5 - 1, 1 + 1, 0 );

  CAShape bar4( g_pDirect3DRM, NULL );
  bar4.CreateCuboid( 1, 2, 1 );
  bar4.m_pMeshBuilder->SetColorRGB( 0, 1, 0 );
  bar4.m_pMeshBuilder->Translate( .5 + 1, 1 + 1, 0 );

  m_pBase->m_pMeshBuilder->AddMeshBuilder( bar.m_pMeshBuilder->I() );
  m_pBase->m_pMeshBuilder->AddMeshBuilder( bar2.m_pMeshBuilder->I() );
  m_pBase->m_pMeshBuilder->AddMeshBuilder( bar3.m_pMeshBuilder->I() );
  m_pBase->m_pMeshBuilder->AddMeshBuilder( bar4.m_pMeshBuilder->I() );
  m_pBase->SetRotation( m_pBase->I(), 0, 1, 0, -0.06);

  wrap3.Apply( (LPDIRECT3DRMOBJECT) m_pBase->m_pMeshBuilder->I() );
  m_pBase->m_pMeshBuilder->SetTexture( texture.I() );

  LPDIRECT3DRMVISUAL pIShadow;
  g_pDirect3DRM->CreateShadow( m_pBase->m_pMeshBuilder->I(), m_pDirLight->I(),
    D3DVAL(0), D3DVAL(.05), D3DVAL(0),
    D3DVAL(0), D3DVAL(1), D3DVAL(0),
    &pIShadow);
  m_pBase->AddVisual( pIShadow );

  // duplicate the shape
  CAShape dup1( g_pDirect3DRM, NULL );
  m_pScene->AddChild( dup1.I() );
  dup1.m_pMeshBuilder->AddMeshBuilder( m_pBase->m_pMeshBuilder->I() );
  dup1.SetPosition( m_pScene->I(), D3DVAL(20.0), D3DVAL(0), D3DVAL(-15.0) );
  dup1.SetRotation( m_pBase->I(), 0, 1, 0, 0.06);
  wrap3.Apply( (LPDIRECT3DRMOBJECT) dup1.m_pMeshBuilder->I() );
  dup1.m_pMeshBuilder->SetTexture( texture.I() );
  g_pDirect3DRM->CreateShadow( dup1.m_pMeshBuilder->I(), m_pDirLight->I(),
    D3DVAL(0), D3DVAL(.05), D3DVAL(0),
    D3DVAL(0), D3DVAL(1), D3DVAL(0),
    &pIShadow);
  dup1.AddVisual( pIShadow );
*/
/*  shadow
    LPDIRECT3DRMVISUAL pIShadow;
    g_pDirect3DRM->CreateShadow( pCube->m_pMeshBuilder->I(), m_pDirLight->I(),
      D3DVAL(0), D3DVAL(.05), D3DVAL(0),
      D3DVAL(0), D3DVAL(1), D3DVAL(0),
      &pIShadow);
    pCube->AddVisual( pIShadow );
*/
#endif