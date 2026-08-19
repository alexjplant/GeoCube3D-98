#ifndef __CASPRITE_H__
#define __CASPRITE_H__

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

class CASprite
{
public:               
  CASprite();
  virtual ~CASprite();

  BOOL LoadFromFile( LPCTSTR pFile );

  CString              m_sName;
  DDSURFACEDESC        m_ddsd;
  CADirectDrawSurface *m_pSurface;

protected:

};

// global sprite cache
CASprite *GetCachedSprite( LPCTSTR szBitmap );
void DeleteCachedSprite( LPCTSTR szBitmap );
void DeleteAllSprites();

#endif

