#ifndef __CABULLET_H__
#define __CABULLET_H__

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#include "sprite.h"

class CABullet;
typedef CTypedPtrList< CPtrList, CABullet* > CABulletList;

typedef enum
{
  eStart,
  eRunning,
  eExplode,
  eDone,
} EBulletState;

class CABullet : public CADecal
{
public:               
  CABullet( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef );
  ~CABullet();

  void Tick();
  BOOL DrawLaser();
  BOOL DrawSpot();
  BOOL DrawAVI();

  EBulletState m_eState;
  DWORD m_dwTickCount;
  DWORD m_dwTime;

protected:

  CASprite *m_pBulletSprite;
  CASprite *m_pExplodeSprite;

};

#endif

