#ifndef __DXADRAW_H__
#define __DXADRAW_H__

#include "dxaobj.h"

#include "ddraw.h"  // Direct X

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectDraw : public CADirectXObject
{
public:               
  CADirectDraw(BOOL bTryHardware = TRUE);
  ~CADirectDraw();

  IDirectDraw *I() { return m_pIDirectDraw; };

  HRESULT Compact();

  HRESULT CreateClipper(DWORD dwFlags,
      LPDIRECTDRAWCLIPPER FAR * lplpDDClipper,
      IUnknown FAR * pUnkOuter);

  HRESULT CreatePalette(DWORD dwFlags, 
      LPPALETTEENTRY lpColorTable,
      LPDIRECTDRAWPALETTE FAR * lplpDDPalette,
      IUnknown FAR * pUnkOuter);

  HRESULT CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,
      LPDIRECTDRAWSURFACE FAR * lplpDDSurface,
      IUnknown FAR * pUnkOuter);

  HRESULT DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface,
      LPDIRECTDRAWSURFACE FAR * lplpDupDDSurface);

  HRESULT EnumDisplayModes(DWORD dwFlags,
      LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext,
      LPDDENUMMODESCALLBACK lpEnumModesCallback);

  HRESULT EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSD, 
      LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback);

  HRESULT FlipToGDISurface();

  HRESULT GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps);

  HRESULT GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc);

  HRESULT GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes);

  HRESULT GetGDISurface(LPDIRECTDRAWSURFACE FAR * lplpGDIDDSSurface);

  HRESULT GetMonitorFrequency(LPDWORD lpdwFrequency);

  HRESULT GetScanLine(LPDWORD lpdwScanLine);

  HRESULT GetVerticalBlankStatus(LPBOOL lpbIsInVB);

  HRESULT Initialize(GUID FAR * lpGUID);

  HRESULT RestoreDisplayMode();

  HRESULT SetCooperativeLevel(HWND hWnd, DWORD dwFlags);

  HRESULT SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);

  HRESULT WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent);

  DDSURFACEDESC m_ddescMode[100];
  int           m_nModes;

protected:
  IDirectDraw *m_pIDirectDraw;

private:
  LPDIRECTDRAW  m_lpDD; // don't use this interface
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectDrawClipper : public CADirectXObject
{
public:               
  CADirectDrawClipper( CADirectDraw *pDirectDraw );
  ~CADirectDrawClipper();

  IDirectDrawClipper *I() { return m_pIDirectDrawClipper; };

  HRESULT GetClipList(LPRECT lpRect, LPRGNDATA lpClipList,
      LPDWORD lpdwSize);

  HRESULT GetHWnd(HWND FAR * lphWnd);

  HRESULT Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags);

  HRESULT IsClipListChanged(BOOL FAR * lpbChanged);

  HRESULT SetClipList(LPRGNDATA lpClipList, DWORD dwFlags);

  HRESULT SetHWnd(DWORD dwFlags, HWND hWnd);

protected:

  IDirectDrawClipper *m_pIDirectDrawClipper;
};



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectDrawPalette : public CADirectXObject
{
public:               
  CADirectDrawPalette( CADirectDraw *pDirectDraw, DWORD dwFlags, 
    LPPALETTEENTRY lpColorTable );
  ~CADirectDrawPalette();

  IDirectDrawPalette *I() { return m_pIDirectDrawPalette; };

  HRESULT GetCaps(LPDWORD lpdwCaps);

  HRESULT GetEntries(DWORD dwFlags, DWORD dwBase,
      DWORD dwNumEntries, LPPALETTEENTRY lpEntries);

  HRESULT Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags,
      LPPALETTEENTRY lpDDColorTable);

  HRESULT SetEntries(DWORD dwFlags, DWORD dwStartingEntry,
      DWORD dwCount, LPPALETTEENTRY lpEntries);


protected:

  IDirectDrawPalette *m_pIDirectDrawPalette;
};





/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectDrawSurface : public CADirectXObject
{
public:               
  CADirectDrawSurface( CADirectDraw *pDirectDraw, LPDDSURFACEDESC pDesc);
  CADirectDrawSurface( IDirectDrawSurface *pIDirectDrawSurface ); //attached
  ~CADirectDrawSurface();

  IDirectDrawSurface *I() { return m_pIDirectDrawSurface; };

  HRESULT AddAttachedSurface(
      LPDIRECTDRAWSURFACE lpDDSAttachedSurface);

  HRESULT AddOverlayDirtyRect(LPRECT lpRect);

  HRESULT Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface,
      LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx);

  HRESULT BltBatch(LPDDBLTBATCH lpDDBltBatch,
      DWORD dwCount, DWORD dwFlags);

  HRESULT BltFast(DWORD dwX, DWORD dwY, 
      LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, 
      DWORD dwTrans);

  HRESULT DeleteAttachedSurface(DWORD dwFlags,
      LPDIRECTDRAWSURFACE lpDDSAttachedSurface);

  HRESULT EnumAttachedSurfaces(LPVOID lpContext,
      LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback);

  HRESULT EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext,
      LPDDENUMSURFACESCALLBACK lpfnCallback);

  HRESULT Flip(
      LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride,
      DWORD dwFlags);

  HRESULT GetAttachedSurface(LPDDSCAPS lpDDSCaps,
      LPDIRECTDRAWSURFACE FAR * lplpDDAttachedSurface);

  HRESULT GetBltStatus(DWORD dwFlags);

  HRESULT GetCaps(LPDDSCAPS lpDDSCaps);

  HRESULT GetClipper(LPDIRECTDRAWCLIPPER FAR * lplpDDClipper);

  HRESULT GetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey);

  HRESULT GetDC(HDC FAR * lphDC);

  HRESULT GetFlipStatus(DWORD dwFlags);

  HRESULT GetOverlayPosition(LPLONG lplX, LPLONG lplY);

  HRESULT GetPalette(LPDIRECTDRAWPALETTE FAR * lplpDDPalette);

  HRESULT GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat);

  HRESULT GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc);

  HRESULT Initialize(LPDIRECTDRAW lpDD,
      LPDDSURFACEDESC lpDDSurfaceDesc);

  HRESULT IsLost();

  HRESULT Lock(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc,
      DWORD dwFlags, HANDLE hEvent);

  HRESULT ReleaseDC(HDC hDC);

  HRESULT Restore();

  HRESULT SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper);

  HRESULT SetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey);

  HRESULT SetOverlayPosition(LONG lX, LONG lY);

  HRESULT SetPalette(LPDIRECTDRAWPALETTE lpDDPalette);

  HRESULT Unlock(LPVOID lpSurfaceData);

  HRESULT UpdateOverlay(LPRECT lpSrcRect,
      LPDIRECTDRAWSURFACE lpDDDestSurface,
      LPRECT lpDestRect, DWORD dwFlags,
      LPDDOVERLAYFX lpDDOverlayFx);

  HRESULT UpdateOverlayDisplay(DWORD dwFlags);

  HRESULT UpdateOverlayZOrder(DWORD dwFlags,
      LPDIRECTDRAWSURFACE lpDDSReference);


protected:
  IDirectDrawSurface *m_pIDirectDrawSurface;

private:
  BOOL m_bAttached;
  LPDIRECTDRAWSURFACE  m_lpDDS; // don't use this interface
};


#endif