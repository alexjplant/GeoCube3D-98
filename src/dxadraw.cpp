#include <stdafx.h>
#include "dxadraw.h"


//////////////////////////////////////////////////////////////////////////////////
// EnumDD - enumerates DD drivers, chooses hardware if available
//////////////////////////////////////////////////////////////////////////////////
static BOOL FAR PASCAL EnumDD(GUID FAR* lpGUID, LPSTR lpDriverDesc, LPSTR lpDriverName, LPVOID lpContext)
{
	LPDIRECTDRAW lpDD;
	DDCAPS HALCaps, HELCaps;

	// Make sure the guid is valid
	if (lpGUID)
	{
		// Try to create a DirectDraw object
		DirectDrawCreate(lpGUID, &lpDD, NULL);

		// Get the DirectDraw capabilities
		memset(&HALCaps, 0, sizeof(DDCAPS));
		HALCaps.dwSize = sizeof(DDCAPS);

		memset(&HELCaps, 0, sizeof(DDCAPS));
		HELCaps.dwSize = sizeof(DDCAPS);

		lpDD->GetCaps(&HALCaps, &HELCaps);

		// Does this driver have 3D hardware capabilites?
		if (HALCaps.dwCaps & DDCAPS_3D)
		{
			*(LPDIRECTDRAW*)lpContext = lpDD;
			return DDENUMRET_CANCEL;
		}

		*(LPDIRECTDRAW*)lpContext = NULL;
		lpDD->Release();
	}
	return DDENUMRET_OK;
}

//////////////////////////////////////////////////////////////////////////////////
// EnumModes - enumerates video modes
//////////////////////////////////////////////////////////////////////////////////
static HRESULT CALLBACK EnumModes(LPDDSURFACEDESC pddsd, LPVOID pContext)
{
  CADirectDraw *pDD = (CADirectDraw *) pContext;
	pDD->m_ddescMode[ pDD->m_nModes ] = *pddsd;
	pDD->m_nModes++;
	
	return DDENUMRET_OK;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectDraw::CADirectDraw(BOOL bTryHardware) : CADirectXObject(),
  m_lpDD(NULL), m_nModes(0)
{
  if (bTryHardware)
  	DirectDrawEnumerate(EnumDD, &m_lpDD);

  if (m_lpDD == NULL)
    Check( DirectDrawCreate(NULL, &m_lpDD, NULL) );

  Check( m_lpDD->QueryInterface(IID_IDirectDraw, (LPVOID *)&m_pIDirectDraw) );

  EnumDisplayModes(0, NULL, (LPVOID) this, EnumModes);
}


CADirectDraw::~CADirectDraw()
{
  if (m_pIDirectDraw) m_pIDirectDraw->Release();
  if (m_lpDD) m_lpDD->Release();
}

HRESULT CADirectDraw::Compact()
{
  return Check( m_pIDirectDraw->Compact() );
}

HRESULT CADirectDraw::CreateClipper(DWORD dwFlags,
      LPDIRECTDRAWCLIPPER FAR * lplpDDClipper,
      IUnknown FAR * pUnkOuter)
{
  return Check( m_pIDirectDraw->CreateClipper( dwFlags,
        lplpDDClipper,
        pUnkOuter) );
}

HRESULT CADirectDraw::CreatePalette(DWORD dwFlags, 
      LPPALETTEENTRY lpColorTable,
      LPDIRECTDRAWPALETTE FAR * lplpDDPalette,
      IUnknown FAR * pUnkOuter)
{
  return Check( m_pIDirectDraw->CreatePalette( dwFlags, 
       lpColorTable,
       lplpDDPalette,
       pUnkOuter) );
}

HRESULT CADirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,
      LPDIRECTDRAWSURFACE FAR * lplpDDSurface,
      IUnknown FAR * pUnkOuter)
{
  return Check( m_pIDirectDraw->CreateSurface( lpDDSurfaceDesc,
      lplpDDSurface,
      pUnkOuter) );
}

HRESULT CADirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface,
      LPDIRECTDRAWSURFACE FAR * lplpDupDDSurface)
{
  return Check( m_pIDirectDraw->DuplicateSurface( lpDDSurface,
      lplpDupDDSurface) );
}

HRESULT CADirectDraw::EnumDisplayModes(DWORD dwFlags,
      LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext,
      LPDDENUMMODESCALLBACK lpEnumModesCallback)
{
  return Check( m_pIDirectDraw->EnumDisplayModes( dwFlags,
       lpDDSurfaceDesc,  lpContext,
       lpEnumModesCallback) );
}

HRESULT CADirectDraw::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSD, 
      LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
  return Check( m_pIDirectDraw->EnumSurfaces( dwFlags, lpDDSD, 
       lpContext,  lpEnumSurfacesCallback) );
}

HRESULT CADirectDraw::FlipToGDISurface()
{
  return Check( m_pIDirectDraw->FlipToGDISurface() );
}

HRESULT CADirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
  return Check( m_pIDirectDraw->GetCaps( lpDDDriverCaps,  lpDDHELCaps) );
}

HRESULT CADirectDraw::GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
{
  return Check( m_pIDirectDraw->GetDisplayMode( lpDDSurfaceDesc) );
}

HRESULT CADirectDraw::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes)
{
  return Check( m_pIDirectDraw->GetFourCCCodes( lpNumCodes,  lpCodes) );
}

HRESULT CADirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE FAR * lplpGDIDDSSurface)
{
  return Check( m_pIDirectDraw->GetGDISurface( lplpGDIDDSSurface) );
}

HRESULT CADirectDraw::GetMonitorFrequency(LPDWORD lpdwFrequency)
{
  return Check( m_pIDirectDraw->GetMonitorFrequency( lpdwFrequency) );
}

HRESULT CADirectDraw::GetScanLine(LPDWORD lpdwScanLine)
{
  return Check( m_pIDirectDraw->GetScanLine( lpdwScanLine) );
}

HRESULT CADirectDraw::GetVerticalBlankStatus(LPBOOL lpbIsInVB)
{
  return Check( m_pIDirectDraw->GetVerticalBlankStatus( lpbIsInVB) );
}

HRESULT CADirectDraw::Initialize(GUID FAR * lpGUID)
{
  return Check( m_pIDirectDraw->Initialize(lpGUID) );
}

HRESULT CADirectDraw::RestoreDisplayMode()
{
  return Check( m_pIDirectDraw->RestoreDisplayMode() );
}

HRESULT CADirectDraw::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
  return Check( m_pIDirectDraw->SetCooperativeLevel( hWnd,  dwFlags) );
}

HRESULT CADirectDraw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight,
      DWORD dwBPP)
{
  return Check( m_pIDirectDraw->SetDisplayMode( dwWidth,  dwHeight,
       dwBPP) );
}

HRESULT CADirectDraw::WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent)
{
  return Check( m_pIDirectDraw->WaitForVerticalBlank( dwFlags,  hEvent) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectDrawClipper::CADirectDrawClipper( CADirectDraw *pDirectDraw ) : CADirectXObject()
{
  pDirectDraw->CreateClipper( 0, &m_pIDirectDrawClipper, NULL );
}

CADirectDrawClipper::~CADirectDrawClipper()
{
  if (m_pIDirectDrawClipper) m_pIDirectDrawClipper->Release();
}


HRESULT CADirectDrawClipper::GetClipList(LPRECT lpRect, LPRGNDATA lpClipList,
      LPDWORD lpdwSize)
{
  return Check( m_pIDirectDrawClipper->GetClipList( lpRect,  lpClipList,
       lpdwSize) );
}

HRESULT CADirectDrawClipper::GetHWnd(HWND FAR * lphWnd)
{
  return Check( m_pIDirectDrawClipper->GetHWnd( lphWnd) );
}

HRESULT CADirectDrawClipper::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags)
{
  return Check( m_pIDirectDrawClipper->Initialize( lpDD,  dwFlags) );
}

HRESULT CADirectDrawClipper::IsClipListChanged(BOOL FAR * lpbChanged)
{
  return Check( m_pIDirectDrawClipper->IsClipListChanged( lpbChanged) );
}

HRESULT CADirectDrawClipper::SetClipList(LPRGNDATA lpClipList, DWORD dwFlags)
{
  return Check( m_pIDirectDrawClipper->SetClipList( lpClipList,  dwFlags) );
}

HRESULT CADirectDrawClipper::SetHWnd(DWORD dwFlags, HWND hWnd)
{
  return Check( m_pIDirectDrawClipper->SetHWnd( dwFlags,  hWnd)  );
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectDrawPalette::CADirectDrawPalette( CADirectDraw *pDirectDraw, DWORD dwFlags, 
    LPPALETTEENTRY lpColorTable ) : CADirectXObject()
{
  pDirectDraw->CreatePalette( dwFlags, lpColorTable, &m_pIDirectDrawPalette, NULL );
}

CADirectDrawPalette::~CADirectDrawPalette()
{
  if (m_pIDirectDrawPalette) m_pIDirectDrawPalette->Release();
}


HRESULT CADirectDrawPalette::GetCaps(LPDWORD lpdwCaps)
{
  return Check( m_pIDirectDrawPalette->GetCaps( lpdwCaps) );
}

HRESULT CADirectDrawPalette::GetEntries(DWORD dwFlags, DWORD dwBase,
      DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
  return Check( m_pIDirectDrawPalette->GetEntries( dwFlags,  dwBase,
       dwNumEntries,  lpEntries) );
}

HRESULT CADirectDrawPalette::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags,
      LPPALETTEENTRY lpDDColorTable)
{
  return Check( m_pIDirectDrawPalette->Initialize( lpDD,  dwFlags,
       lpDDColorTable) );
}

HRESULT CADirectDrawPalette::SetEntries(DWORD dwFlags, DWORD dwStartingEntry,
      DWORD dwCount, LPPALETTEENTRY lpEntries)
{
  return Check( m_pIDirectDrawPalette->SetEntries( dwFlags,  dwStartingEntry,
       dwCount, lpEntries) );
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectDrawSurface::CADirectDrawSurface( CADirectDraw *pDirectDraw, LPDDSURFACEDESC pDesc)
    : CADirectXObject()
{
  m_bAttached = FALSE;

  Check( pDirectDraw->CreateSurface(pDesc, &m_lpDDS, NULL) );

  Check( m_lpDDS->QueryInterface(IID_IDirectDrawSurface, (LPVOID *)&m_pIDirectDrawSurface) );
}

CADirectDrawSurface::CADirectDrawSurface( IDirectDrawSurface *pIDirectDrawSurface )
    : CADirectXObject()
{
  m_bAttached = TRUE;
  m_pIDirectDrawSurface = pIDirectDrawSurface;
}

CADirectDrawSurface::~CADirectDrawSurface()
{
  if (!m_bAttached)
  {
    if (m_pIDirectDrawSurface) m_pIDirectDrawSurface->Release();
    if (m_lpDDS) m_lpDDS->Release();
  }
}


HRESULT CADirectDrawSurface::AddAttachedSurface(
      LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
{
  return Check( m_pIDirectDrawSurface->AddAttachedSurface(
       lpDDSAttachedSurface) );
}

HRESULT CADirectDrawSurface::AddOverlayDirtyRect(LPRECT lpRect)
{
  return Check( m_pIDirectDrawSurface->AddOverlayDirtyRect( lpRect) );
}

HRESULT CADirectDrawSurface::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface,
      LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
  return Check( m_pIDirectDrawSurface->Blt( lpDestRect,  lpDDSrcSurface,
       lpSrcRect,  dwFlags,  lpDDBltFx) );
}

HRESULT CADirectDrawSurface::BltBatch(LPDDBLTBATCH lpDDBltBatch,
      DWORD dwCount, DWORD dwFlags)
{
  return Check( m_pIDirectDrawSurface->BltBatch( lpDDBltBatch,
       dwCount,  dwFlags) );
}

HRESULT CADirectDrawSurface::BltFast(DWORD dwX, DWORD dwY, 
      LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, 
      DWORD dwTrans)
{
  return Check( m_pIDirectDrawSurface->BltFast( dwX,  dwY, 
       lpDDSrcSurface,  lpSrcRect, 
       dwTrans) );
}

HRESULT CADirectDrawSurface::DeleteAttachedSurface(DWORD dwFlags,
      LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
{
  return Check( m_pIDirectDrawSurface->DeleteAttachedSurface( dwFlags,
       lpDDSAttachedSurface) );
}

HRESULT CADirectDrawSurface::EnumAttachedSurfaces(LPVOID lpContext,
      LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
  return Check( m_pIDirectDrawSurface->EnumAttachedSurfaces( lpContext,
       lpEnumSurfacesCallback) );
}

HRESULT CADirectDrawSurface::EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext,
      LPDDENUMSURFACESCALLBACK lpfnCallback)
{
  return Check( m_pIDirectDrawSurface->EnumOverlayZOrders( dwFlags,  lpContext,
       lpfnCallback) );
}

HRESULT CADirectDrawSurface::Flip(
      LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride,
      DWORD dwFlags)
{
  m_hr = m_pIDirectDrawSurface->Flip( lpDDSurfaceTargetOverride, dwFlags );

  if ( m_hr == DDERR_SURFACELOST || m_hr == DDERR_WASSTILLDRAWING )
    return m_hr;

  return Check( m_hr );
}

HRESULT CADirectDrawSurface::GetAttachedSurface(LPDDSCAPS lpDDSCaps,
      LPDIRECTDRAWSURFACE FAR * lplpDDAttachedSurface)
{
  return Check( m_pIDirectDrawSurface->GetAttachedSurface( lpDDSCaps,
      lplpDDAttachedSurface) );
}

HRESULT CADirectDrawSurface::GetBltStatus(DWORD dwFlags)
{
  return Check( m_pIDirectDrawSurface->GetBltStatus( dwFlags) );
}

HRESULT CADirectDrawSurface::GetCaps(LPDDSCAPS lpDDSCaps)
{
  return Check( m_pIDirectDrawSurface->GetCaps( lpDDSCaps) );
}

HRESULT CADirectDrawSurface::GetClipper(LPDIRECTDRAWCLIPPER FAR * lplpDDClipper)
{
  return Check( m_pIDirectDrawSurface->GetClipper(lplpDDClipper) );
}

HRESULT CADirectDrawSurface::GetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
  return Check( m_pIDirectDrawSurface->GetColorKey( dwFlags, lpDDColorKey) );
}

HRESULT CADirectDrawSurface::GetDC(HDC FAR * lphDC)
{
  return Check( m_pIDirectDrawSurface->GetDC( lphDC) );
}

HRESULT CADirectDrawSurface::GetFlipStatus(DWORD dwFlags)
{
  return Check( m_pIDirectDrawSurface->GetFlipStatus( dwFlags) );
}

HRESULT CADirectDrawSurface::GetOverlayPosition(LPLONG lplX, LPLONG lplY)
{
  return Check( m_pIDirectDrawSurface->GetOverlayPosition( lplX,  lplY) );
}

HRESULT CADirectDrawSurface::GetPalette(LPDIRECTDRAWPALETTE FAR * lplpDDPalette)
{
  return Check( m_pIDirectDrawSurface->GetPalette( lplpDDPalette) );
}

HRESULT CADirectDrawSurface::GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
{
  return Check( m_pIDirectDrawSurface->GetPixelFormat( lpDDPixelFormat) );
}

HRESULT CADirectDrawSurface::GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc)
{
  return Check( m_pIDirectDrawSurface->GetSurfaceDesc( lpDDSurfaceDesc) );
}

HRESULT CADirectDrawSurface::Initialize(LPDIRECTDRAW lpDD,
      LPDDSURFACEDESC lpDDSurfaceDesc)
{
  return Check( m_pIDirectDrawSurface->Initialize( lpDD,
       lpDDSurfaceDesc) );
}

HRESULT CADirectDrawSurface::IsLost()
{
  return Check( m_pIDirectDrawSurface->IsLost() );
}

HRESULT CADirectDrawSurface::Lock(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc,
      DWORD dwFlags, HANDLE hEvent)
{
  return Check( m_pIDirectDrawSurface->Lock( lpDestRect,  lpDDSurfaceDesc,
       dwFlags,  hEvent) );
}

HRESULT CADirectDrawSurface::ReleaseDC(HDC hDC)
{
  return Check( m_pIDirectDrawSurface->ReleaseDC( hDC) );
}

HRESULT CADirectDrawSurface::Restore()
{
  return Check( m_pIDirectDrawSurface->Restore() );
}

HRESULT CADirectDrawSurface::SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper)
{
  return Check( m_pIDirectDrawSurface->SetClipper( lpDDClipper) );
}

HRESULT CADirectDrawSurface::SetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
  return Check( m_pIDirectDrawSurface->SetColorKey( dwFlags,  lpDDColorKey) );
}

HRESULT CADirectDrawSurface::SetOverlayPosition(LONG lX, LONG lY)
{
  return Check( m_pIDirectDrawSurface->SetOverlayPosition( lX,  lY) );
}

HRESULT CADirectDrawSurface::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
{
  return Check( m_pIDirectDrawSurface->SetPalette( lpDDPalette) );
}

HRESULT CADirectDrawSurface::Unlock(LPVOID lpSurfaceData)
{
  return Check( m_pIDirectDrawSurface->Unlock( lpSurfaceData) );
}

HRESULT CADirectDrawSurface::UpdateOverlay(LPRECT lpSrcRect,
      LPDIRECTDRAWSURFACE lpDDDestSurface,
      LPRECT lpDestRect, DWORD dwFlags,
      LPDDOVERLAYFX lpDDOverlayFx)
{
  return Check( m_pIDirectDrawSurface->UpdateOverlay( lpSrcRect,
       lpDDDestSurface,
       lpDestRect,  dwFlags,
       lpDDOverlayFx) );
}

HRESULT CADirectDrawSurface::UpdateOverlayDisplay(DWORD dwFlags)
{
  return Check( m_pIDirectDrawSurface->UpdateOverlayDisplay( dwFlags) );
}

HRESULT CADirectDrawSurface::UpdateOverlayZOrder(DWORD dwFlags,
      LPDIRECTDRAWSURFACE lpDDSReference)
{
  return Check( m_pIDirectDrawSurface->UpdateOverlayZOrder( dwFlags,
       lpDDSReference) );
}

