#ifndef __CAPLAYERSHAPE_H__
#define __CAPLAYERSHAPE_H__

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAPlayer;
class CAPlayerShape;
typedef CTypedPtrList< CPtrList, CAPlayerShape* > CAPlayerShapeList;

class CAPlayerShape : public CAShape
{
public:               
  CAPlayerShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef );
  ~CAPlayerShape();

  void SetPlayerData();
  void GetPlayerData();

  void UpdateInput( CAControllerData &data );
  void ApplyRotation( CAControllerData &data );
  BOOL ApplyThrust( CAControllerData &data );
  void ShieldOn();
  void ShieldOff();

  CADirectSoundBuffer *m_pSndThrust;
  CADirectSoundBuffer *m_pSndShield;
  CAPlayer            *m_pOnlinePlayer;
  BOOL                 m_bShield;
  CAShape             *m_pShield;
  D3DVALUE             m_valRotRudder;
  D3DVALUE             m_valRotHeadUpDown;
  D3DVALUE             m_valRotHeadTilt;

protected:

};

#endif

