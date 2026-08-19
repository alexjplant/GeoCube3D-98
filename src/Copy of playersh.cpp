#include <stdafx.h>
#include "launch.h"

#include "playersh.h"
#include "player.h"

#define BOUNDARY     2000  // meters

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAPlayerShape::CAPlayerShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef ) :
  CAShape( pD3D, pRef )
{
	m_pOnlinePlayer = NULL;
  m_pSndThrust = new CADirectSoundBuffer( "thrust.wav" );
}

CAPlayerShape::~CAPlayerShape()
{
  delete m_pSndThrust;
}

void CAPlayerShape::SetPlayerData()
{
  if (m_pOnlinePlayer)
    m_pOnlinePlayer->SetPlayerData( &m_data, m_data.size );
}

void CAPlayerShape::GetPlayerData()
{
  if (m_pOnlinePlayer)
    m_pOnlinePlayer->GetPlayerData( &m_data, &m_data.size );
}

void CAPlayerShape::UpdateInput( CAControllerData &data )
{
  ApplyRotation( data );

  if (ApplyThrust( data ))
  {
//    m_pSndThrust->SetCurrentPosition(0);
    m_pSndThrust->Play( 0, 0, DSBPLAY_LOOPING );
  }
  else
    m_pSndThrust->Stop();

  ApplyGravity( data );
  TestBorders( data );
}

void CAPlayerShape::ApplyRotation( CAControllerData &data )
{
  CAVector &vPos = m_data.vPos;
  CAVector &vDir = m_data.vDir;
  CAVector &vUp  = m_data.vUp;
  CAVector &vVelocity = m_vVelocity;
  CAVector vNorm;

/*
  // level-off
  if (data.m_aKeys[ DIK_NUMPAD5 ].Down())
  {
    vUp.x = 0;  vUp.y = 1;  vUp.z = 0;

    D3DVALUE mag = vDir.Mag();
    vDir.y = 0;
    vDir.Normalize();
    vDir *= mag;

    SetOrientation( NULL,
      vDir.x, vDir.y, vDir.z,
      vUp.x,  vUp.y,  vUp.z);
  }
*/

  vNorm = CAVector( vDir ) * CAVector( vUp );  // cross product

  //  rotate
  if (data.m_dRudder || data.m_dHeadUpDown || data.m_dHeadTilt )
  {
    CAMatrix mRotate;

    float fRotSpeed = 75 * g_valFrameRatio;

    if ((data.m_aKeys[ DIK_RSHIFT ].Down()) ||  // SHIFT keys
        (data.m_aKeys[ DIK_LSHIFT ].Down()))
      fRotSpeed *= 1.5;

    // Rudder control:  rotate the camera about its up vector
    if ( data.m_dRudder )
    {
      mRotate.MakeUnit();
      mRotate.Translate( -vPos.x, -vPos.y, -vPos.z );  // to origin
      if ( data.m_dRudder > 0 )
        mRotate.Rotate( vUp.x * fRotSpeed, vUp.y * fRotSpeed, vUp.z * fRotSpeed );
      else
      if ( data.m_dRudder < 0 )
        mRotate.Rotate( -vUp.x * fRotSpeed, -vUp.y * fRotSpeed, -vUp.z * fRotSpeed );
      mRotate.Translate( vPos.x, vPos.y, vPos.z );     // and back
      vDir = mRotate * vDir;
    }

    // HeadUpDown control:  rotate the camera about its normal vector
    if ( data.m_dHeadUpDown )
    {
      mRotate.MakeUnit();
      mRotate.Translate( -vPos.x, -vPos.y, -vPos.z );  // to origin
      if ( data.m_dHeadUpDown > 0 )
        mRotate.Rotate( vNorm.x * fRotSpeed, vNorm.y * fRotSpeed, vNorm.z * fRotSpeed );
      else
      if ( data.m_dHeadUpDown < 0 )
        mRotate.Rotate( -vNorm.x * fRotSpeed, -vNorm.y * fRotSpeed, - vNorm.z * fRotSpeed );
      mRotate.Translate( vPos.x, vPos.y, vPos.z );     // and back
      vDir = mRotate * vDir;
      vUp  = mRotate * vUp;
    }

    // HeadTilt control:  rotate the camera about its direction vector
    if ( data.m_dHeadTilt )
    {
      mRotate.MakeUnit();
      mRotate.Translate( -vPos.x, -vPos.y, -vPos.z );  // to origin
      if ( data.m_dHeadTilt > 0 )
        mRotate.Rotate( vDir.x * fRotSpeed, vDir.y * fRotSpeed, vDir.z * fRotSpeed );
      else
      if ( data.m_dHeadTilt < 0 )
        mRotate.Rotate( -vDir.x * fRotSpeed, -vDir.y * fRotSpeed, -vDir.z * fRotSpeed );
      mRotate.Translate( vPos.x, vPos.y, vPos.z );     // and back
      vUp = mRotate * vUp;
      vNorm = mRotate * vNorm;
    }

    SetOrientation( NULL,
      vDir.x, vDir.y, vDir.z,
      vUp.x,  vUp.y,  vUp.z);
  }
}

BOOL CAPlayerShape::ApplyThrust( CAControllerData &data )
{
  CAVector &vPos = m_data.vPos;
  CAVector &vDir = m_data.vDir;
  CAVector &vUp  = m_data.vUp;
  CAVector &vVelocity = m_vVelocity;
  CAVector vNorm = CAVector( vDir ) * CAVector( vUp );

  // update the thrusters 
  if ( data.m_dX || data.m_dY || data.m_dZ )
  {
    // thrusting without gravity
    CAVector vDelta(0, 0, 0);
    if ( data.m_dX > 0 ) vDelta = -vNorm;
    else
    if ( data.m_dX < 0 ) vDelta = vNorm;

    if ( data.m_dY > 0 ) vDelta += -vDir;
    else
    if ( data.m_dY < 0 ) vDelta += vDir;

    if ( data.m_dZ > 0 ) vDelta += -vUp;
    else
    if ( data.m_dZ < 0 ) vDelta += vUp;

    vDelta *= 500.0 * g_valFrameRatio;  // accelerate by 100 m/s

    vVelocity += vDelta;

    if (vVelocity.Mag() > MAX_VELOCITY)
    {
      vVelocity.Normalize();
      vVelocity *= MAX_VELOCITY;
    }

    /*
    // thrusting with gravity
    if (vVelocity.Mag() < 15.0)
    {
      CAVector vDelta(0, 0, 0);

      if (vPos.y <= 0.5)  // if on the ground
      {
        if ( data.m_dX > 0 ) vDelta = -vNorm;
        else
        if ( data.m_dX < 0 ) vDelta = vNorm;

        if ( data.m_dY > 0 ) vDelta = -vDir;
        else
        if ( data.m_dY < 0 ) vDelta = vDir;
      }

      if ( data.m_dZ > 0 ) vDelta = -vUp;
      else
      if ( data.m_dZ < 0 ) vDelta = vUp;

      vDelta *= fSpeed;

      vVelocity += vDelta;
    }
    */
    return TRUE;
  }

  return FALSE;
}

BOOL CAPlayerShape::TestBorders( CAControllerData &data )
{
  CAVector &vPos = m_data.vPos;
  CAVector &vVelocity = m_vVelocity;

  // advance to the new position
  CAVector vNewPos( vPos + vVelocity * g_valFrameRatio );

/*
  // have we hit the ground yet?
  if (vNewPos.y < 0)  
  {
    // bounce?
    if (vVelocity.y < -.75)
    {
      D3DVALUE mag = vVelocity.Mag();
      vVelocity.y = -vVelocity.y;
      vVelocity.Normalize();
      vVelocity *= mag * 0.6; // %60 of force
    }
    else
      vVelocity.y = 0;

    vNewPos.y = 0;
  }
*/

  // have we hit a border yet?
  int bHit = FALSE;
  if (vNewPos.x > BOUNDARY)
  {
    CAVector vNorm( 0, vVelocity.y, vVelocity.z );
    vNorm.Normalize();
    if (vVelocity.y || vVelocity.z)
      vVelocity.Reflect( vNorm );
    else
      vVelocity *= -1;
    bHit = TRUE;
  }
  else
  if (vNewPos.x < -BOUNDARY)
  {
    CAVector vNorm( 0, vVelocity.y, vVelocity.z );
    vNorm.Normalize();
    if (vVelocity.y || vVelocity.z)
      vVelocity.Reflect( vNorm );
    else
      vVelocity *= -1;
    bHit = TRUE;
  }
  else
  if (vNewPos.y > BOUNDARY)
  {
    CAVector vNorm( vVelocity.x, 0, vVelocity.z );
    vNorm.Normalize();
    if (vVelocity.x || vVelocity.z)
      vVelocity.Reflect( vNorm );
    else
      vVelocity *= -1;
    bHit = TRUE;
  }
  else
  if (vNewPos.y < -BOUNDARY)
  {
    CAVector vNorm( vVelocity.x, 0, vVelocity.z );
    vNorm.Normalize();
    if (vVelocity.x || vVelocity.z)
      vVelocity.Reflect( vNorm );
    else
      vVelocity *= -1;
    bHit = TRUE;
  }
  else
  if (vNewPos.z > BOUNDARY)
  {
    CAVector vNorm( vVelocity.x, vVelocity.y, 0 );
    vNorm.Normalize();
    if (vVelocity.x || vVelocity.y)
      vVelocity.Reflect( vNorm );
    else
      vVelocity *= -1;
    bHit = TRUE;
  }
  else
  if (vNewPos.z < -BOUNDARY)
  {
    CAVector vNorm( vVelocity.x, vVelocity.y, 0 );
    vNorm.Normalize();
    if (vVelocity.x || vVelocity.y)
      vVelocity.Reflect( vNorm );
    else
      vVelocity *= -1;
    bHit = TRUE;
  }

  if (vVelocity.Mag() > MAX_VELOCITY) // needed?
  {
    vVelocity.Normalize();
    vVelocity *= MAX_VELOCITY;
  }

  // clip
  if (vNewPos.x > BOUNDARY)
    vNewPos.x = BOUNDARY;
  if (vNewPos.x < -BOUNDARY)
    vNewPos.x = -BOUNDARY;
  if (vNewPos.y > BOUNDARY)
    vNewPos.y = BOUNDARY;
  if (vNewPos.y < -BOUNDARY)
    vNewPos.y = -BOUNDARY;
  if (vNewPos.z > BOUNDARY)
    vNewPos.z = BOUNDARY;
  if (vNewPos.z < -BOUNDARY)
    vNewPos.z = -BOUNDARY;

  vPos = vNewPos;
  return bHit;
}