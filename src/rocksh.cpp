#include <stdafx.h>
#include "launch.h"

#include "rocksh.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CARockShape::CARockShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef ) :
  CAShape( pD3D, pRef )
{
  m_iType = 0;
  m_nSize = ROCK_SIZE_LARGE;
}

CARockShape::~CARockShape()
{
}
