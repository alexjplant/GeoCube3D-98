#include <stdafx.h>
#include "dxadraw.h"
#include "dxa3drm.h"
#include "dxainput.h"
#include "dxautil.h"
#include "dxaplay.h"

#include "bullet.h"
#include "launch.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CABullet::CABullet( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef ) :
  CADecal( pD3D, pRef )
{
  m_pTexture->SetDecalSize( .25, .25 );

  m_pBulletSprite = GetCachedSprite( "bullet.bmp" );
  m_pExplodeSprite = GetCachedSprite( "explode.bmp" );
  m_eState = eStart;
  m_dwTickCount = 0;
  m_dwTime = 0;

  DrawLaser();
}

CABullet::~CABullet()
{
}

void CABullet::Tick()
{
  m_dwTickCount++;
  m_dwTime += g_dwDiffTime;

  if (m_eState == eStart)
  {
    m_eState = eRunning;
    m_dwTime = 0;

    DDBLTFX ddbltfx;
    memset(&ddbltfx, 0, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof ddbltfx;
    m_pSurface->Blt( NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);

    m_pTexture->SetDecalSize( .1, .1 );
    m_pTexture->Changed( TRUE, FALSE );
  }
  else
  if (m_eState == eExplode)
  {
    m_eState = eDone;
    m_dwTime = 0;

    RECT r;
    r.top = 0;
    r.left = 0;
    r.right = 256; // m_pSurface->m_ddsd.dwWidth;
    r.bottom = 256; // m_pSurface->m_ddsd.dwHeight;
    m_pSurface->Blt( &r, m_pExplodeSprite->m_pSurface->I(), &r, DDBLT_WAIT, NULL);

    m_pTexture->SetDecalSize( 256, 256 );
    m_pTexture->Changed( TRUE, FALSE );
  }
  else
  if (m_eState == eDone)
  {
    m_dwTickCount = 0;
    m_dwTime = 0;
  }
}

BOOL CABullet::DrawLaser()
{
  // draw a laser spike
  CreateRod( 0, 0, 0,  0,0,100, .75, 3 );
//CreateCube(1);
  m_pMeshBuilder->SetColorRGB( 1, 1, 0 );
  m_pMeshBuilder->SetQuality( D3DRMRENDER_UNLITFLAT );
  return TRUE;
}

BOOL CABullet::DrawSpot()
{
  ASSERT(m_pSurface);
  ASSERT(m_pTexture);

  CBrush red(RGB(255,0,0));
  HDC hdc;
  m_pSurface->GetDC(&hdc);
  if (hdc)
  {
    SetBkColor( hdc, RGB( 0, 0, 0 ) );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    SelectObject( hdc, red );
    PatBlt(hdc, 0, 0, m_dwWidth, m_dwHeight, BLACKNESS);

    Ellipse( hdc, 0, 0, m_dwWidth, m_dwHeight );
    m_pSurface->ReleaseDC(hdc);
  }

  m_pTexture->Changed( TRUE, FALSE );
  return TRUE;
}

BOOL CABullet::DrawAVI()
{
  int fSuccess = TRUE;
  SetTextLabel("");  // erases background

  // load an AVI File
	HRESULT hr;
	PAVISTREAM pStream = NULL;
  hr = ::AVIStreamOpenFromFile( &pStream,
					   "test.avi",
	           streamtypeVIDEO,
             0, OF_READ, NULL);

	if (FAILED(hr)) return FALSE;
 
  AVISTREAMINFO info;
  AVIStreamInfo( pStream, &info, sizeof(AVISTREAMINFO) );

  LONG fmtSize = 0;
  AVIStreamFormatSize( pStream, 0, &fmtSize);

  BITMAPINFO *pBmi = (BITMAPINFO *) new BYTE[ fmtSize + (sizeof(RGBQUAD)*256)];
  BITMAPINFO *pBmiTarget = (BITMAPINFO *) new BYTE[ fmtSize + (sizeof(RGBQUAD)*256)];
  AVIStreamReadFormat( pStream, 0, pBmi, &fmtSize );
  memcpy(pBmiTarget, pBmi, fmtSize + (sizeof(RGBQUAD)*256));

  DDSURFACEDESC ddsd;
	ddsd.dwSize = sizeof(ddsd);
  m_pSurface->GetSurfaceDesc( &ddsd );
  pBmiTarget->bmiHeader.biBitCount = ddsd.ddpfPixelFormat.dwRGBBitCount;
  pBmiTarget->bmiHeader.biCompression = BI_RGB;
  pBmiTarget->bmiHeader.biSizeImage =
    pBmiTarget->bmiHeader.biHeight *
    pBmiTarget->bmiHeader.biWidth *
    pBmiTarget->bmiHeader.biBitCount / 8;

  LONG lSize;
  AVIStreamSampleSize( pStream, 0, &lSize );

  VOID *pData = new BYTE[ lSize ];
  VOID *pDataTarget = new BYTE[ pBmiTarget->bmiHeader.biSizeImage ];
  AVIStreamRead( pStream, 0, 1, pData, lSize, NULL, NULL );

  HIC hic = ICDecompressOpen( ICTYPE_VIDEO, info.fccHandler, &pBmi->bmiHeader, &pBmiTarget->bmiHeader);
  if (hic == 0)
  {
    fSuccess = FALSE;
    goto FINISHED;
  }
  ICDecompressGetPalette( hic, pBmi, pBmiTarget );
  ICDecompress( hic, 0, &pBmi->bmiHeader, pData, &pBmiTarget->bmiHeader, pDataTarget );

  // draw the bitmap to the surface
  HRESULT ddrval;
	ddrval = m_pSurface->I()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
  if (ddrval == DD_OK)
  {
    BYTE *pSrc = (BYTE*)pDataTarget;
    LONG bmPitch = pBmiTarget->bmiHeader.biWidth * pBmiTarget->bmiHeader.biBitCount / 8;
    for (int i = pBmiTarget->bmiHeader.biHeight - 1; i >= 0; i--)
    {
  	  memcpy(	(BYTE*)ddsd.lpSurface + i * ddsd.lPitch, pSrc, bmPitch );
  	  pSrc += bmPitch;
    }

	  m_pSurface->I()->Unlock(NULL);
	}

  m_pTexture->Changed( TRUE, FALSE );

FINISHED:
  AVIStreamRelease( pStream );

  delete pData;
  delete pDataTarget;
  delete pBmi;
  delete pBmiTarget;

  return fSuccess;
}