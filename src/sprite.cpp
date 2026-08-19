#include <stdafx.h>
#include "dxadraw.h"
#include "dxa3drm.h"
//#include "dxautil.h"

#include "sprite.h"
#include "resource.h"

extern CADirectDraw  *g_pDirectDraw;
extern CADirect3DRM  *g_pDirect3DRM;

static CASprite *g_aSpriteCache[256];
static int g_nSprites = 0;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CASprite::CASprite()
{
  m_pSurface = NULL;
}

CASprite::~CASprite()
{
  delete m_pSurface;
}

BOOL CASprite::LoadFromFile( LPCTSTR szBitmap )
{
  //  try to load the bitmap as a resource, if that fails, try it as a file
  HBITMAP hbm = (HBITMAP) LoadImage(0, szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

  if (hbm == NULL)
      hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

  if (hbm == NULL)
      return FALSE;

  m_sName = szBitmap;

  // create a surface for this bitmap
  BITMAP bm;
  GetObject(hbm, sizeof(bm), &bm);

  ZeroMemory(&m_ddsd, sizeof(m_ddsd));
  m_ddsd.dwSize = sizeof(m_ddsd);
  m_ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
  m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  m_ddsd.dwWidth = bm.bmWidth;
  m_ddsd.dwHeight = bm.bmHeight;

  m_pSurface = new CADirectDrawSurface( g_pDirectDraw, &m_ddsd );

  // copy the bitmap to the surface
  HDC dcbm = CreateCompatibleDC(NULL);
  SelectObject( dcbm, hbm);

  HDC dcSurface;
  if (m_pSurface->GetDC(&dcSurface) == DD_OK)
  {
    BitBlt(dcSurface, 0, 0, bm.bmWidth, bm.bmHeight, dcbm, 0, 0, SRCCOPY);
    m_pSurface->ReleaseDC( dcSurface );
  }
  DeleteDC(dcbm);
  DeleteObject(hbm);
  return TRUE;
}

CASprite *GetCachedSprite( LPCTSTR szBitmap )
{
  //  look for an existing sprite
  for (int i = 0; i < g_nSprites; i++)
  {
    if (g_aSpriteCache[i]->m_sName.CompareNoCase( szBitmap ) == 0)
    {
      if (g_aSpriteCache[i]->m_pSurface->IsLost())
        g_aSpriteCache[i]->m_pSurface->Restore();
      return g_aSpriteCache[i];
    }
  }

  // add a new sprite to the list
  CASprite *pSprite = new CASprite();
  pSprite->LoadFromFile( szBitmap );
  g_aSpriteCache[ g_nSprites++ ] = pSprite;
  return pSprite;
}

void DeleteCachedSprite( LPCTSTR szBitmap )
{
  for (int i = 0; i < g_nSprites; i++)
  {
    if (g_aSpriteCache[i]->m_sName.CompareNoCase( szBitmap ) == 0)
    {
      delete g_aSpriteCache[i];
      for (i++; i < g_nSprites; i++)
        g_aSpriteCache[i-1] = g_aSpriteCache[i];
      g_nSprites--;
      return;
    }
  }
}

void DeleteAllSprites()
{
  for (int i = 0; i < g_nSprites; i++)
    delete g_aSpriteCache[i];

  g_nSprites = 0;
}