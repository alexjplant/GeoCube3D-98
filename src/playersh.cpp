#include <stdafx.h>
#include "launch.h"

#include "playersh.h"
#include "player.h"

#define MAX_ROT_SPEED    50
#define STEP_ROT_SPEED    5
#define MIN_ROT_SPEED   -50

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAPlayerShape::CAPlayerShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef ) :
  CAShape( pD3D, pRef )
{
  m_pShield = new CAShape( g_pDirect3DRM, NULL );
  m_pShield->CreateSphere(1, 8);
  m_pShield->m_pMeshBuilder->SetColorRGB( 1, 0, 0 );
  m_pShield->m_pMeshBuilder->SetQuality( D3DRMRENDER_WIREFRAME );
  m_bShield = FALSE;

  m_pSndThrust = new CADirectSoundBuffer( "thrust.wav" );
  m_pSndShield = new CADirectSoundBuffer( "shield.wav" );
	
  m_pOnlinePlayer = NULL;
  m_valRotRudder = 0;
  m_valRotHeadUpDown = 0;
  m_valRotHeadTilt = 0;
}

CAPlayerShape::~CAPlayerShape()
{
  delete m_pSndThrust;
  delete m_pSndShield;
  delete m_pShield;
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
  CAVector vNorm = CAVector( vDir ) * CAVector( vUp );  // cross product

  //  rotate rudder
  if (data.m_dRudder > 0)
//  if (data.m_aKeys[ DIK_LEFTARROW ].Down() ||
//      data.m_aKeys[ DIK_NUMPAD4 ].Down() )
  {
    if (m_valRotRudder < 0)
      m_valRotRudder = 0;
    if (m_valRotRudder < MAX_ROT_SPEED)
      m_valRotRudder += STEP_ROT_SPEED;
  }
  else
  if (data.m_dRudder < 0)
//  if (data.m_aKeys[ DIK_RIGHTARROW ].Down() ||
//      data.m_aKeys[ DIK_NUMPAD6 ].Down() )
  {
    if (m_valRotRudder > 0)
      m_valRotRudder = 0;
    if (m_valRotRudder > MIN_ROT_SPEED)
      m_valRotRudder -= STEP_ROT_SPEED;
  }
  else  // float back to 0
  {
    m_valRotRudder = 0;
  }

  if (m_valRotRudder)
  {
    // rotate the camera about its up vector
    CAMatrix mRotate;
    D3DVALUE valRotSpeed = m_valRotRudder * g_valFrameRatio;

    mRotate.MakeUnit();
    mRotate.Translate( -vPos.x, -vPos.y, -vPos.z );  // to origin
    mRotate.Rotate( vUp.x * valRotSpeed, vUp.y * valRotSpeed, vUp.z * valRotSpeed );
    mRotate.Translate( vPos.x, vPos.y, vPos.z );     // and back
    vDir = mRotate * vDir;
  }

  //  rotate head up down
  if (data.m_dHeadUpDown < 0)
//  if (data.m_aKeys[ DIK_DOWNARROW ].Down() ||
//      data.m_aKeys[ DIK_NUMPAD2 ].Down() )
  {
    if (m_valRotHeadUpDown < 0)
      m_valRotHeadUpDown = 0;
    if (m_valRotHeadUpDown < MAX_ROT_SPEED)
      m_valRotHeadUpDown += STEP_ROT_SPEED;
  }
  else
  if (data.m_dHeadUpDown > 0)
//  if (data.m_aKeys[ DIK_UPARROW ].Down() ||
//      data.m_aKeys[ DIK_NUMPAD8 ].Down() )
  {
    if (m_valRotHeadUpDown > 0)
      m_valRotHeadUpDown = 0;
    if (m_valRotHeadUpDown > MIN_ROT_SPEED)
      m_valRotHeadUpDown -= STEP_ROT_SPEED;
  }
  else  // float back to 0
  {
    m_valRotHeadUpDown = 0;
  }

  if (m_valRotHeadUpDown)
  {
    // rotate the camera about its normal vector
    CAMatrix mRotate;
    D3DVALUE valRotSpeed = m_valRotHeadUpDown * g_valFrameRatio;

    mRotate.MakeUnit();
    mRotate.Translate( -vPos.x, -vPos.y, -vPos.z );  // to origin
    mRotate.Rotate( vNorm.x * valRotSpeed, vNorm.y * valRotSpeed, vNorm.z * valRotSpeed );
    mRotate.Translate( vPos.x, vPos.y, vPos.z );     // and back
    vDir = mRotate * vDir;
    vUp  = mRotate * vUp;
  }

  //  tilt head
  if (data.m_dHeadTilt > 0)
//  if (data.m_aKeys[ DIK_RBRACKET ].Down())
  {
    if (m_valRotHeadTilt < 0)
      m_valRotHeadTilt = 0;
    if (m_valRotHeadTilt < MAX_ROT_SPEED)
      m_valRotHeadTilt += STEP_ROT_SPEED;
  }
  else
  if (data.m_dHeadTilt < 0)
//  if (data.m_aKeys[ DIK_LBRACKET ].Down())
  {
    if (m_valRotHeadTilt > 0)
      m_valRotHeadTilt = 0;
    if (m_valRotHeadTilt > MIN_ROT_SPEED)
      m_valRotHeadTilt -= STEP_ROT_SPEED;
  }
  else  // float back to 0
  {
    m_valRotHeadTilt = 0;
  }

  if (m_valRotHeadTilt)
  {
    // rotate the camera about its direction vector
    CAMatrix mRotate;
    D3DVALUE valRotSpeed = m_valRotHeadTilt * g_valFrameRatio;

    mRotate.MakeUnit();
    mRotate.Translate( -vPos.x, -vPos.y, -vPos.z );  // to origin
    mRotate.Rotate( vDir.x * valRotSpeed, vDir.y * valRotSpeed, vDir.z * valRotSpeed );
    mRotate.Translate( vPos.x, vPos.y, vPos.z );     // and back
    vUp = mRotate * vUp;
    vNorm = mRotate * vNorm;
  }

  if (m_valRotRudder || m_valRotHeadUpDown || m_valRotHeadTilt)
    SetOrientation( NULL,  vDir.x, vDir.y, vDir.z,  vUp.x, vUp.y, vUp.z );
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

void CAPlayerShape::ShieldOn()
{
  if (m_bShield == FALSE)
  {
    m_bShield = TRUE;
    AddVisual( m_pShield->I() );
    m_pSndShield->Play( 0, 0, DSBPLAY_LOOPING );
  }
}

void CAPlayerShape::ShieldOff()
{
  if (m_bShield == TRUE)
  {
    m_bShield = FALSE;
    DeleteVisual( m_pShield->I() );
    m_pSndShield->Stop();
  }
}
