#include <stdafx.h>
#include "Launch.h"
#include "DxaWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CADxaWnd::CADxaWnd() : CWnd()
{
  g_dwLastTick = timeGetTime();

  m_bFullScreen = FALSE;
  m_ppBackPtr = NULL;
  m_bActive = FALSE;

  m_pPrimary = NULL;
  m_pBack = NULL;
  m_pClipper = NULL;
  m_pPalette = NULL;

  m_pDevice = NULL;
  m_pViewport = NULL;
}

CADxaWnd::~CADxaWnd()
{
  delete m_pPrimary;   
  delete m_pBack;   
  delete m_pClipper;   
  delete m_pPalette;   
}


BEGIN_MESSAGE_MAP(CADxaWnd, CWnd)
	//{{AFX_MSG_MAP(CADxaWnd)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_ACTIVATEAPP()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////

BOOL CADxaWnd::Create(HWND hwndParent, BOOL bFullScreen, CWnd **ppBackPtr)
{
  m_bFullScreen = bFullScreen;
  m_ppBackPtr = ppBackPtr;
  CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();

	// Register window class
	CString strClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
    NULL, (HBRUSH)::GetStockObject(GRAY_BRUSH));

	// Define window style and size
	DWORD dwStyle = WS_VISIBLE | WS_POPUP;
	RECT rc;
	if (m_bFullScreen)
  {
		rc.top = 0;
		rc.left = 0;
		rc.right = pApp->m_iModeX;
		rc.bottom = pApp->m_iModeY;
	}
  else  // Windowed
  {
		dwStyle |= WS_CAPTION | WS_SYSMENU;
		rc.top = 50;
		rc.left = 50;
		rc.bottom = rc.top + pApp->m_iModeY;
		rc.right = rc.left + pApp->m_iModeX;
	}

	// Create the window. 
	if (!CreateEx(0,
				  strClass,
				  "Geo-Cube 3D",
				  dwStyle,
				  rc.left, rc.top,
				  rc.right - rc.left, rc.bottom - rc.top,
				  hwndParent,
				  NULL))
  {
		return FALSE;
	}

	// Ensure window is visible
	UpdateWindow();
  ShowCursor(FALSE);

  InitSurfaces();

  // setup the input controller
  g_pDirectInput->mCreate(*this);

  return TRUE;
}

BOOL CADxaWnd::InitSurfaces()
{
  CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();

  GetClientRect(&m_rClient);
  ClientToScreen(&m_rClient);
  m_rBack.top = m_rBack.left = 0;
  m_rBack.right = pApp->m_iModeX;
  m_rBack.bottom = pApp->m_iModeY;
  m_rViewport = m_rBack;
  m_rViewport.top = m_rViewport.Height() * .1;
  m_rPanel = m_rBack;
  m_rPanel.bottom = m_rViewport.top;

  if (m_pBack)
  {
    delete m_pBack;
    m_pBack = NULL;
  }

  if (m_pPrimary)
  {
    delete m_pPrimary;
    m_pPrimary = NULL;
  }

	if (m_bFullScreen)
  {
    g_pDirectDraw->SetCooperativeLevel( *this,
      DDSCL_EXCLUSIVE |
      DDSCL_FULLSCREEN |
      DDSCL_ALLOWREBOOT);
//      DDSCL_ALLOWMODEX);  // no GetDC, no Blt, no GDI

    g_pDirectDraw->SetDisplayMode( pApp->m_iModeX, pApp->m_iModeY, pApp->m_iModeBPP );

    // Create primary surface
    DDSURFACEDESC ddsd;
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX |
                          DDSCAPS_3DDEVICE;
    ddsd.dwBackBufferCount = 1;
    m_nBuffers = ddsd.dwBackBufferCount + 1;
    m_pPrimary = new CADirectDrawSurface( g_pDirectDraw, &ddsd );

    // get a pointer to the back buffer
    DDSCAPS ddscaps;
    memset(&ddscaps, 0, sizeof(DDSCAPS));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    IDirectDrawSurface *pIBack;
    m_pPrimary->GetAttachedSurface( &ddscaps, &pIBack );
    m_pBack = new CADirectDrawSurface( pIBack );
  }
  else    // set windowed mode
  {
    g_pDirectDraw->SetCooperativeLevel(*this,
      DDSCL_NORMAL |
      DDSCL_ALLOWREBOOT);

    // create the primary surface
    DDSURFACEDESC ddsd;
    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    m_pPrimary = new CADirectDrawSurface( g_pDirectDraw, &ddsd );

    // create the back buffer
	  ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	  ddsd.dwWidth = pApp->m_iModeX;
	  ddsd.dwHeight = pApp->m_iModeY;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    m_pBack = new CADirectDrawSurface( g_pDirectDraw, &ddsd );

    m_nBuffers = 2;

    // create a clipper object
	  m_pClipper = new CADirectDrawClipper( g_pDirectDraw );
	  m_pPrimary->SetClipper( m_pClipper->I() );
    m_pClipper->SetHWnd(0, *this);
  }

  if (pApp->m_iModeBPP == 8)
  {
    // create a default palette
    PALETTEENTRY ape[256];
    for (int i=0; i < 256; i++)
    {
      ape[i].peRed   = (BYTE)(((i >> 5) & 0x07) * 255 / 7);
      ape[i].peGreen = (BYTE)(((i >> 2) & 0x07) * 255 / 7);
      ape[i].peBlue  = (BYTE)(((i >> 0) & 0x03) * 255 / 3);
      ape[i].peFlags = (BYTE)0;
    }

    if (m_pPalette)
    {
      delete m_pPalette;
      m_pPalette = NULL;
    }

    m_pPalette = new CADirectDrawPalette( g_pDirectDraw, DDPCAPS_8BIT, ape );
    m_pPrimary->SetPalette( m_pPalette->I() );
    m_pBack->SetPalette( m_pPalette->I() );
  }

  return TRUE;
}

void CADxaWnd::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	
  CALaunchApp *pApp = (CALaunchApp *) AfxGetApp();
  GetClientRect(&m_rClient);
  ClientToScreen(&m_rClient);
  m_rBack.top = m_rBack.left = 0;
  m_rBack.right = pApp->m_iModeX;
  m_rBack.bottom = pApp->m_iModeY;
  m_rViewport = m_rBack;
  m_rViewport.top = m_rViewport.Height() * .1;
  m_rPanel = m_rBack;
  m_rPanel.bottom = m_rViewport.top;
}

static BOOL g_phase = 0;

BOOL CADxaWnd::UpdateFrame()
{
  g_dwDiffTime = timeGetTime() - g_dwLastTick;
  g_dwLastTick = timeGetTime();
  g_valFrameRatio = (D3DVALUE) g_dwDiffTime / 1000.0;
  g_dwFrameCount++;
  g_dwFrameTime += g_dwDiffTime;

  if (g_dwFrameTime > 1000)
  {
    g_dwFrames = (g_dwFrameCount * 1000) / g_dwFrameTime;
    g_dwFrameCount = 0;
    g_dwFrameTime = 0;

    if( g_dwFrames > 99 ) g_dwFrames = 99;
  }

  if (!Poll())  // derived-class
  {
    ExitGame();  // derived-class
    return FALSE;
  }

  Render();   // derived-class

  HDC hdc;
  if (m_pBack->I()->GetDC(&hdc) == DD_OK)
  {
    DisplayGDI( hdc, &m_rPanel );
//    DisplayFrameRate( hdc, m_rPanel.left, m_rPanel.top );

    m_pBack->ReleaseDC(hdc);
  }

  // copy the back buffer to the primary
  FlipScreen();

  return TRUE;
}


void CADxaWnd::OnTimer(UINT nIDEvent) 
{
  if (nIDEvent != 1)
  {
	  CWnd::OnTimer(nIDEvent);
    return;
  }

  UpdateFrame();
}

void CADxaWnd::FlipScreen()
{
  if (!m_pPrimary)
    return;

  // Flip the surfaces
  if (m_bFullScreen)
  {
    while( 1 )
    {
      m_pPrimary->I()->Flip( NULL, 0 );

      if( m_pPrimary->GetResult() == DD_OK )
        break;

      if( m_pPrimary->GetResult() == DDERR_SURFACELOST )
      {
        m_pPrimary->I()->Restore();
        m_pBack->I()->Restore();
      }
      else
      if( m_pPrimary->GetResult() != DDERR_WASSTILLDRAWING )
        break;
    }
  }
  else
  {
    m_pPrimary->Blt( &m_rClient,
      m_pBack->I(),
      &m_rBack,
      DDBLT_WAIT, NULL);
  }
}

void CADxaWnd::DisplayFrameRate( HDC hdc, int x, int y )
{
  CString sRate;
  sRate.Format("Frame Rate: %02d ", g_dwFrames);
  TextOut( hdc, x, y, sRate, sRate.GetLength() );
}

///////////////////////////////////////////////////////////////////

void CADxaWnd::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CWnd::OnActivateApp(bActive, hTask);

  m_bActive = bActive;
}

void CADxaWnd::OnDestroy() 
{
	if (m_bFullScreen)
  {
    g_pDirectDraw->SetCooperativeLevel(*this,
      DDSCL_NORMAL |
      DDSCL_ALLOWREBOOT);
    g_pDirectDraw->RestoreDisplayMode();
  }

  ShowCursor(TRUE);

	CWnd::OnDestroy();
}

BOOL CADxaWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CADxaWnd::PostNcDestroy() 
{
  if (m_ppBackPtr)
    *m_ppBackPtr = NULL;

  delete this;
}

void CADxaWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_pDevice)
  {
    IDirect3DRMWinDevice* pWinDev;

    if (m_pDevice->I()->QueryInterface(IID_IDirect3DRMWinDevice, (void**)&pWinDev) == D3DRM_OK)
    {
			pWinDev->HandlePaint(dc.m_hDC);
      pWinDev->Release(); // also decreases ref count for m_pRMDev
    }
		else
			return;
  }
	// Do not call CView::OnPaint() for painting messages
}

void CADxaWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  if (m_pDevice)
  {
    LPDIRECT3DRMWINDEVICE pWinDev;
    if (SUCCEEDED(m_pDevice->I()->QueryInterface( 
      IID_IDirect3DRMWinDevice, (void **) &pWinDev)))
    {
      pWinDev->HandleActivate( (WORD) nState );
      pWinDev->Release();
    }
  }
}
