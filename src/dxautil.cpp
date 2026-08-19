#include <stdafx.h>
#include <math.h>
#include "dxadraw.h"
#include "dxa3drm.h"
#include "dxainput.h"
#include "dxautil.h"
#include "sprite.h"

extern CADirectDraw  *g_pDirectDraw;
extern CADirect3DRM  *g_pDirect3DRM;
extern CADirectInput *g_pDirectInput;

extern D3DVALUE       g_valFrameRatio;
extern DWORD          g_dwFrames;
extern DWORD          g_dwFrameCount;
extern DWORD          g_dwLastTick;
extern DWORD          g_dwDiffTime;

// some math constants
static D3DVALUE _pi =  3.14159265359;
static D3DVALUE _twopi =  6.28318530718;

#define SQR(x)       ((x) * (x))
#define D2R          D3DVALUE(0.01745329251994)


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static UINT ControllerThreadProc( LPVOID pParam );
//static CCriticalSection running;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAControllerData::CAControllerData()
{
}

CAControllerData::~CAControllerData()
{
}

void CAControllerData::Reset()
{
  m_dX = 0;
  m_dY = 0;
  m_dZ = 0;
  m_dRudder = 0;
  m_dHeadUpDown = 0;
  m_dHeadTilt = 0;
  m_iThrust = 0;
  m_bFire = 0;
  m_bJump = 0;

  for (int i = 0; i < 256; i++)
    m_aKeys[i].Clear();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAController::CAController( CADirectInput *pInput ) : m_pInput(pInput)
{
  m_data.Reset();

  ahEvents[0] = (HANDLE) eKill;
  ahEvents[1] = m_pInput->m_hKeyboard;
  ahEvents[2] = m_pInput->m_hMouse;

//  AfxBeginThread( ControllerThreadProc, (LPVOID) ahEvents);
}

CAController::~CAController()
{
  eKill.SetEvent();
}

BOOL CAController::Update()
{
  LONG lTmp;
  m_data.Reset();

  // consume any previous Up() and Repeat() events
  for (int i = 0; i < 256; i++)
  {
    if (m_pInput->m_aKeys[i].Up())
      m_pInput->m_aKeys[i].iState = 0;

    if (m_pInput->m_aKeys[i].Repeat())
      m_pInput->m_aKeys[i].bRepeat = FALSE;
  }

  m_pInput->mUpdate();

  // advance the repeater
  for (i = 0; i < 256; i++)
  {
    if (m_pInput->m_aKeys[i].Down())
    {
      m_pInput->m_aKeys[i].dwRepeatTime += g_dwDiffTime;

      if (m_pInput->m_aKeys[i].dwRepeatTime > REPEAT_INTERVAL)
      {
        m_pInput->m_aKeys[i].dwRepeatTime = 0;
        m_pInput->m_aKeys[i].bRepeat = TRUE;
      }
    }

    m_data.m_aKeys[i] = m_pInput->m_aKeys[i]; //needed?
  }

  // check Joystick
/*
  lTmp = m_pInput->m_joyInfo.dwXpos / 1000;
  if (lTmp > 34) m_data.m_dX = lTmp - 34;
  else
  if (lTmp < 30) m_data.m_dX = -(30 - lTmp);

  lTmp = m_pInput->m_joyInfo.dwYpos / 1000;
  if (lTmp > 34) m_data.m_dY = lTmp - 34;
  else
  if (lTmp < 30) m_data.m_dY = -(30 - lTmp);
*/
  lTmp = m_pInput->m_joyInfo.dwYpos / 1000;
  if (lTmp > 34) m_data.m_dHeadUpDown = -1;
  else
  if (lTmp < 30) m_data.m_dHeadUpDown = 1;

  lTmp = m_pInput->m_joyInfo.dwXpos / 1000;
  if (lTmp > 34) m_data.m_dRudder = -1;
  else
  if (lTmp < 30) m_data.m_dRudder = 1;
    
  m_data.m_iThrust = m_pInput->m_joyInfo.dwZpos / 1000;

  lTmp = m_pInput->m_joyInfo.dwRpos / 1000;
  if (lTmp > 34) m_data.m_dRudder = -(lTmp - 34);
  else
  if (lTmp < 30) m_data.m_dRudder = 30 - lTmp;

  m_data.m_bFire = m_pInput->m_joyInfo.dwButtons & JOY_BUTTON1;

  // check mouse
  lTmp = -m_pInput->m_MouseState.lX;
  if (lTmp > 1) m_data.m_dRudder = 1;
  else
  if (lTmp < -1) m_data.m_dRudder = -1;

  lTmp = -m_pInput->m_MouseState.lY;
  if (lTmp > 1) m_data.m_dHeadUpDown = 1;
  else
  if (lTmp < -1) m_data.m_dHeadUpDown = -1;

  if (m_pInput->m_MouseState.rgbButtons[0] & 0x80)  // left button
    m_data.m_dY = -1;
  else
  if (m_pInput->m_MouseState.rgbButtons[1] & 0x80)  // right button
    m_data.m_dY = 1;


  // check keyboard
  if (m_pInput->m_aKeys[ DIK_RIGHTARROW ].Down() ||  // right
      m_pInput->m_aKeys[ DIK_NUMPAD6 ].Down())  
    m_data.m_dRudder = -1;

  if (m_pInput->m_aKeys[ DIK_LEFTARROW ].Down() ||  // left
      m_pInput->m_aKeys[ DIK_NUMPAD4 ].Down())  
    m_data.m_dRudder = 1;

  if (m_pInput->m_aKeys[ DIK_UPARROW ].Down() ||  // up
      m_pInput->m_aKeys[ DIK_NUMPAD8 ].Down())  
    m_data.m_dHeadUpDown = 1;

  if (m_pInput->m_aKeys[ DIK_DOWNARROW ].Down() ||  // down
      m_pInput->m_aKeys[ DIK_NUMPAD2 ].Down())  
    m_data.m_dHeadUpDown = -1;

  if (m_pInput->m_aKeys[ DIK_ADD ].Down() ||  // +
      m_pInput->m_aKeys[ DIK_EQUALS ].Down())  
    m_data.m_dZ = -1;

  if (m_pInput->m_aKeys[ DIK_SUBTRACT ].Down() ||  // -
      m_pInput->m_aKeys[ DIK_MINUS ].Down())  
    m_data.m_dZ = 1;

  if (m_pInput->m_aKeys[ DIK_COMMA ].Down())  // <
    m_data.m_dX = -1;

  if (m_pInput->m_aKeys[ DIK_PERIOD ].Down())  // >
    m_data.m_dX = 1;

  if (m_pInput->m_aKeys[ DIK_LBRACKET ].Down())  // [
    m_data.m_dHeadTilt = -1;

  if (m_pInput->m_aKeys[ DIK_RBRACKET ].Down())  // ]
    m_data.m_dHeadTilt = 1;

  if (m_pInput->m_aKeys[ DIK_A ].Down()) // A
    m_data.m_dY = -1;

  if (m_pInput->m_aKeys[ DIK_Z ].Down()) // Z
    m_data.m_dY = 1;

  if (m_pInput->m_aKeys[ DIK_LCONTROL ].Repeat() ||  // ctrl
      m_pInput->m_aKeys[ DIK_RCONTROL ].Repeat())  
    m_data.m_bFire = TRUE;

  if (m_pInput->m_aKeys[ DIK_SPACE ].Down())  // SPACE
    m_data.m_bJump = TRUE;

  return TRUE;
}

BOOL CAController::Debounce()
{
  BOOL bKeyDown = TRUE;

  while (bKeyDown)
  {
    bKeyDown = FALSE;
    Update();

    for (int i = 0; i < 256; i++)
    {
      if (m_data.m_aKeys[ i ].Down())
      {
        bKeyDown = TRUE;
        break;
      }
    }

    MSG msg;
    if (::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))
    {
      if (!::GetMessage( &msg, NULL, 0, 0 ))
        return FALSE;

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }

  return TRUE;
}

#ifdef NEVER
UINT ControllerThreadProc( LPVOID pParam )
{
  DWORD dwResult;

  while (TRUE)
  {
    dwResult = WaitForMultipleObjects(
      3, (HANDLE *) pParam, FALSE, INFINITE );

    if (dwResult == WAIT_OBJECT_0)  // kill event
      break;

    if ((dwResult == WAIT_OBJECT_0 + 1) || // keyboard event
        (dwResult == WAIT_OBJECT_0 + 2))   // mouse event
    {
      CSingleLock test( &running );
      test.Lock();
      g_pDirectInput->mUpdate();
      test.Unlock();
    }
  }

  return 0;    // thread completed successfully
}
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAScene::CAScene( CADirect3DRM *pD3D ) :
  CADirect3DRMFrame( pD3D, NULL )
{
}

CAScene::~CAScene()
{
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAShape::CAShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef ) :
  CADirect3DRMFrame( pD3D, pRef )
{
	m_pMeshBuilder = new CADirect3DRMMeshBuilder( g_pDirect3DRM );

	AddVisual( m_pMeshBuilder->I() );

  m_vVelocity.x = 0;
  m_vVelocity.y = 0;
  m_vVelocity.z = 0;

  m_data.size = sizeof m_data;
}

CAShape::~CAShape()
{
  delete m_pMeshBuilder;
}

void CAShape::New()
{
	// delete visual
	DeleteVisual( m_pMeshBuilder->I() );

	// stop rotation, reset orientation and position
	SetPosition(NULL, 0.0, 0.0, 0.0);
	SetRotation(NULL, 0.0, 1.0, 0.0, 0.0);
	SetOrientation(NULL, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
}

HRESULT CAShape::SetPosition(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ)
{
  m_data.vPos.x = rvX;
  m_data.vPos.y = rvY;
  m_data.vPos.z = rvZ;
  return CADirect3DRMFrame::SetPosition( lpRef, rvX, rvY, rvZ) ;
}

HRESULT CAShape::SetOrientation(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvDx,
      D3DVALUE rvDy, D3DVALUE rvDz, D3DVALUE rvUx, D3DVALUE rvUy,
      D3DVALUE rvUz)
{
  m_data.vDir.x = rvDx;
  m_data.vDir.y = rvDy;
  m_data.vDir.z = rvDz;

  m_data.vUp.x = rvUx;
  m_data.vUp.y = rvUy;
  m_data.vUp.z = rvUz;

  return CADirect3DRMFrame::SetOrientation( lpRef, rvDx, rvDy, rvDz, rvUx, rvUy, rvUz );
}

// Create from a vector set and a normal set
BOOL CAShape::Create(D3DVECTOR* pVectors, int iVectors,
                     D3DVECTOR* pNormals, int iNormals,
                     int* pFaceData, BOOL bAutoGen)
{
  ASSERT(m_pMeshBuilder);

  // Build mesh from vector list
  m_pMeshBuilder->AddFaces(iVectors, pVectors, 
                           iNormals, pNormals,
                           (ULONG*)pFaceData, NULL);

	if ((iNormals == 0) && bAutoGen)
  {
		m_pMeshBuilder->GenerateNormals();
	}

	AddVisual( m_pMeshBuilder->I() );

	// Enable perspective correction
	m_pMeshBuilder->SetPerspective(TRUE);

  return TRUE;
}

BOOL CAShape::AddFaces(D3DVECTOR* pVectors, int iVectors,
              				 D3DVECTOR* pNormals, int iNormals,
              				 int* pFaceData)
{
  ASSERT(m_pMeshBuilder);

  m_pMeshBuilder->AddFaces(iVectors, pVectors, 
                           iNormals, pNormals,
                           (ULONG*)pFaceData, NULL);
  return TRUE;
}

	
// Create a cube centered at the origin of given side size
BOOL CAShape::CreateCuboid( D3DVALUE x, D3DVALUE y, D3DVALUE z )
{
  New();

  Load( "cube.x" );  // 2x2 cube
  m_pMeshBuilder->Scale( D3DVALUE(.5), D3DVALUE(.5), D3DVALUE(.5) );
  m_pMeshBuilder->Scale( x, y, z );
  return TRUE;

#ifdef NEVER
	D3DVALUE x1 = D3DVAL(x / 2);
	D3DVALUE y1 = D3DVAL(y / 2);
	D3DVALUE z1 = D3DVAL(z / 2);
  D3DVECTOR vert[] =
  {
    {-x1, -y1, -z1},
    {-x1, -y1,  z1},
    { x1, -y1,  z1},
    { x1, -y1, -z1},
    {-x1,  y1, -z1},
    {-x1,  y1,  z1},
    { x1,  y1,  z1},
    { x1,  y1, -z1}
  };

	D3DVECTOR nlist[] =
  {
		{ 1,  0,  0},
		{ 0,  1,  0},
		{ 0,  0,  1},
		{-1,  0,  0},
		{ 0, -1,  0},
		{ 0,  0, -1}
	};
    
	int flist [] =
  {
    4, 0, 4, 3, 4, 2, 4, 1, 4,
    4, 3, 0, 7, 0, 6, 0, 2,	0,
    4, 4, 1, 5, 1, 6, 1, 7,	1,
    4, 0, 3, 1, 3, 5, 3, 4,	3,
    4, 0, 5, 4, 5, 7, 5, 3,	5,
    4, 2, 2, 6, 2, 5, 2, 1,	2,
    0
  };

  return Create( vert, 8, nlist, 6, flist);
#endif
}


BOOL CAShape::CreateSphere(D3DVALUE r, int nBands)
{
  New();
	
	// make sure the number of bands is >= 3
	if (nBands < 3)	nBands = 3;

	// compute the vertex count
	int iVertices = (nBands - 1) * nBands + 2;

	// compute the face count
	int iFaces = nBands * nBands;

    // create the list of vertices
  D3DVECTOR* Vertices = new D3DVECTOR[ iVertices ];
	D3DVECTOR* pv = Vertices;

  // create the face list
  int* FaceData = new int[iFaces * 5];
  int* pfd = FaceData;

	// set up the first vertex at the top
	pv->x = D3DVAL(0);
	pv->y = D3DVAL(r);
	pv->z = D3DVAL(0);
	pv++;
	int iv = 1;	// next free vertex number

	// top band
	D3DVALUE da = _pi / nBands;
	D3DVALUE a = da;
	
	// compute the y value for the bottom of the band
	D3DVALUE y = r * cos(a);
	D3DVALUE rband = r * sin(a);
	D3DVALUE ab = 0;

	// Write out the vertices for the bottom of this band
	int iv1 = iv; // first vertex on this band
	for (int i = 0; i < nBands; i++)
  {
		pv->x = D3DVAL(rband * sin(ab));
		pv->y = D3DVAL(y);
		pv->z = D3DVAL(rband * cos(ab));
		pv++;
		iv++;
		ab += da * 2;
	}

	// Write out the face values
	for (i = 0; i < nBands; i++)
  {
		*pfd++ = 3;
		*pfd++ = iv1 + (i % nBands);
		*pfd++ = iv1 + ((i + 1) % nBands);
		*pfd++ = 0; // top point
	}

	// now do the middle bands
	for (int iBand = 1; iBand < nBands-1; iBand++)
  {
		a += da;
		y = r * cos(a);
		rband = r * sin(a);
		// Write out the vertices for the bottom of this band
		ab = 0;
		int iv1 = iv; // first vertex on this band
		for (int i = 0; i < nBands; i++)
    {
			pv->x = D3DVAL(rband * sin(ab));
			pv->y = D3DVAL(y);
			pv->z = D3DVAL(rband * cos(ab));
			pv++;
			iv++;
			ab += da * 2;
		}

		// Write out the face values
		for (i = 0; i < nBands; i++)
    {
			*pfd++ = 4;
			*pfd++ = iv1 + (i % nBands);
			*pfd++ = iv1 + ((i + 1) % nBands);
			*pfd++ = iv1 - nBands + ((i + 1) % nBands);
			*pfd++ = iv1 - nBands + (i % nBands);
		}
	}

	// now do the last band
	// Write out the vertex for the bottom of this band
	iv1 = iv; // first vertex on this band
	pv->x = D3DVAL(0);
	pv->y = D3DVAL(-r);
	pv->z = D3DVAL(0);
	pv++;
	iv++;

	// Write out the face values
	for (i = 0; i < nBands; i++)
  {
		*pfd++ = 3;
		*pfd++ = iv1; // bottom point
		*pfd++ = iv1 - nBands + ((i + 1) % nBands);
		*pfd++ = iv1 - nBands + (i % nBands);
	}

	*pfd = 0; // end the face list

  BOOL b = Create(Vertices, iv, NULL, 0, FaceData, TRUE);
	delete [] Vertices;
	delete [] FaceData;

	return b;
}

BOOL CAShape::CreateRock(D3DVALUE r, int nBands)
{
  CreateSphere( r, nBands );

  ULONG count = m_pMeshBuilder->GetVertexCount();
  D3DVECTOR *pVertices = new D3DVECTOR[count];
  m_pMeshBuilder->GetVertices( &count, pVertices, NULL, NULL, NULL, NULL);

  D3DVALUE roughness = r / 2;
  for (int i = 0; i < count; i++)
  {
    D3DVECTOR v = pVertices[i];
    v.x += ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * roughness - roughness / 2;
    v.y += ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * roughness - roughness / 2;
    v.z += ((D3DVALUE) rand() / (D3DVALUE) RAND_MAX) * roughness - roughness / 2;
    m_pMeshBuilder->SetVertex( i, v.x, v.y, v.z);
  }

  delete pVertices;

	m_pMeshBuilder->GenerateNormals();
  return TRUE;
}

BOOL CAShape::CreateMesh(D3DVALUE x1, D3DVALUE x2, D3DVALUE dx,
                         D3DVALUE z1, D3DVALUE z2, D3DVALUE dz,
                         SURFHTFN pfnHeight, void* pArg)
{
  New();
  ASSERT(dx != 0);
  ASSERT(dz != 0);
  int iXSteps = (int)((x2 - x1) / dx);
  int iZSteps = (int)((z2 - z1) / dz);
  if ((iXSteps < 1) || (iZSteps < 1)) return FALSE;

  // Create the array for the vertices
  int iVertices = (iXSteps + 1) * (iZSteps + 1);
  D3DVECTOR* Vertices = new D3DVECTOR [iVertices];
  D3DVECTOR* pv = Vertices;

  // Create the array for the face data
  // each face will have 4 vertices
  int iFaces = iXSteps * iZSteps;
  int* FaceData = new int [iFaces * 5 + 1];
  int* pfd = FaceData;

  // write out the vertex set
  D3DVALUE x = x1;
  D3DVALUE z;
  for (int iRow = 0; iRow <= iXSteps; iRow++)
  {
    z = z1;
    for (int iCol = 0; iCol <= iZSteps; iCol++)
    {
      pv->x = D3DVAL(x);
      pv->z = D3DVAL(z);
      if (pfnHeight)
        pv->y = D3DVAL(pfnHeight(x, z, pArg));
      else
        pv->y = 0;
      pv++;
      z += dz;
    }
    x += dx;
  }

  // write out the face list
  int iFirst = iZSteps + 1;
  for (iRow = 0; iRow < iXSteps; iRow++)
  {
    for (int iCol = 0; iCol < iZSteps; iCol++)
    {
      *pfd++ = 4; // no of vertices per face
      *pfd++ = iFirst + iCol;
      *pfd++ = iFirst - iZSteps - 1 + iCol;
      *pfd++ = iFirst - iZSteps + iCol;
      *pfd++ = iFirst + iCol + 1;
    }
    iFirst += iZSteps + 1;
  }
  *pfd = 0; // end the list

  // Create the surface with auto-generation of the normals
  BOOL b = Create( Vertices, iVertices, NULL, 0, FaceData, TRUE );
	delete [] Vertices;
	delete [] FaceData;

	return b;
}

// Create a solid of revolution
BOOL CAShape::CreateRSolid(D3DVALUE z1, D3DVALUE z2, D3DVALUE dz,
							             BOOL bClosed1, BOOL bClosed2,
			                     SOLIDRFN pfnRad, void* pArg,
							             int nFacets)
{
  New();
  ASSERT(pfnRad);
  ASSERT(dz != 0);
  int iZSteps = (int)((z2 - z1) / dz);
  if (iZSteps < 1) return FALSE;

	int iRSteps = nFacets;
//  if (iRSteps < 8) iRSteps = 8;
  if (iRSteps < 3) iRSteps = 3;
  D3DVALUE da = _twopi / iRSteps;

  // Create the array for vertices
  int iVertices = (iZSteps + 1) * iRSteps;
  D3DVECTOR* Vertices = new D3DVECTOR [iVertices];
  D3DVECTOR* pv = Vertices;

  // Create the array for face data.
  // Each face has 4 vertices except the ends.
  int iFaces = iZSteps * iRSteps;
	int iFaceEntries = iFaces * 5 + 1;
  if (bClosed1) iFaceEntries += iRSteps + 1;
	if (bClosed2) iFaceEntries += iRSteps + 1;
  int* FaceData = new int [iFaceEntries];
  int* pfd = FaceData;

  // Write out the vertex set
  D3DVALUE z = z1;
  D3DVALUE r, a;
  for (int iZ = 0; iZ <= iZSteps; iZ++)
  {
    if (pfnRad)
	    r = pfnRad(z, pArg);
    else
      r = 1;

	  a = 0;

    for (int iR = 0; iR < iRSteps; iR++)
    {
      pv->x = D3DVAL(r * sin(a));
      pv->y = D3DVAL(r * cos(a));
      pv->z = D3DVAL(z);
      pv++;
      a += da;
    }

    z += dz;
  }

  // Write out the face list
	int iFirst = iRSteps;
  for (iZ = 0; iZ < iZSteps; iZ++)
  {
    for (int iR = 0; iR < iRSteps; iR++)
    {
      *pfd++ = 4; // No. of vertices per face
      *pfd++ = iFirst + iR;
      *pfd++ = iFirst + ((iR + 1) % iRSteps);
      *pfd++ = iFirst - iRSteps +
			 ((iR + 1) % iRSteps);
      *pfd++ = iFirst - iRSteps + iR;
    }
    iFirst += iRSteps;
  }
  *pfd = 0; // End the list

  // Create the round surface with autogeneration of the
	// normals
  BOOL b = Create(Vertices, iVertices, NULL, 0, FaceData, TRUE);

	delete [] FaceData;

	FaceData = new int [iRSteps * 2 + 2];
	D3DVECTOR nvect [] = {
		{0, 0, 1},
		{0, 0, -1}
	};

	if (bClosed1)
  {
		pfd = FaceData;
		*pfd++ = iRSteps;
    for (int iR = 0; iR < iRSteps; iR++)
    {
			*pfd++ = iR;
			*pfd++ = 1;
		}
		*pfd = 0;
		m_pMeshBuilder->AddFaces(iVertices, Vertices, 2, nvect,
       								      (ULONG*)FaceData, NULL);
	}

	if (bClosed2)
  {
		pfd = FaceData;
		*pfd++ = iRSteps;
		iFirst = iRSteps * iZSteps;
    for (int iR = 0; iR < iRSteps; iR++)
    {
			*pfd++ = iRSteps - 1 - iR + iFirst;
			*pfd++ = 0;
		}
		*pfd = 0;
		m_pMeshBuilder->AddFaces(iVertices, Vertices, 2, nvect,
								     (ULONG*)FaceData, NULL);
	}

	delete [] Vertices;
	delete [] FaceData;

	return b;
}

// Private data for cone function
typedef struct _LINEDATA {
	D3DVALUE c;
	D3DVALUE m;
} LINEDATA;

// Cone fn
static D3DVALUE LineFn(D3DVALUE z, void* pArg)
{
	LINEDATA* pld = (LINEDATA*) pArg;
	ASSERT(pld);
	return pld->m * z + pld->c;
}

// Create a cone between points
BOOL CAShape::CreateCone(D3DVALUE x1, D3DVALUE y1, D3DVALUE z1,
					               D3DVALUE r1, BOOL bClosed1,
					               D3DVALUE x2, D3DVALUE y2, D3DVALUE z2,
					               D3DVALUE r2, BOOL bClosed2,
					               int nFacets)
{
	if ((r1 == 0) && (r2 == 0)) return FALSE;

	// compute the length
	D3DVALUE l = sqrt(SQR(x2-x1)+SQR(y2-y1)+SQR(z2-z1));
	if (l <= 0) return FALSE;

	// construct the private function data
	LINEDATA ld;
	ld.c = r1;
	ld.m = (r2 - r1) / l;

	// build the cone from (0, 0, 0) to (0, 0, l)
	BOOL bResult = CreateRSolid(0, l, l,
								              bClosed1, bClosed2,
								              LineFn, &ld,
								              nFacets);
	if (!bResult) return FALSE;
/*
  // rotate it correctly
  // U*V/|U||V| = cos(theta)
  CAVector vAxis( x2-x1, y2-y1, z2-z1 );
  CAVector vCross = CAVector(0,0,1) * vAxis;
  D3DVALUE dTheta = acos( CAVector(0,0,1).Dot( vAxis ) / vAxis.Mag() );

  ULONG count = m_pMeshBuilder->GetVertexCount();
  D3DVECTOR *pVertices = new D3DVECTOR[count];
  m_pMeshBuilder->GetVertices( &count, pVertices, NULL, NULL, NULL, NULL);

  for (int i = 0; i < count; i++)
  {
    D3DVECTOR v = pVertices[i];
    D3DVECTOR vRotate = vCross;
    D3DVECTOR *pResult = D3DRMVectorRotate( &v, &v, &vRotate, dTheta );
    m_pMeshBuilder->SetVertex( i, v.x, v.y, v.z);
  }

  delete pVertices;
*/
  // translate to the correct origin
	SetPosition(NULL, x1, y1, z1);

	// point it in the right direction
  D3DVECTOR vDir, vUp;
  GetOrientation( NULL, &vDir, &vUp);
	SetOrientation( NULL, x2 - x1, y2 - y1, z2 - z1, vUp.x, vUp.y, vUp.z );

	return TRUE;
}

// Load a shape from a .X file.
BOOL CAShape::Load(const char* pszFileName)
{
  // Remove any existing visual elements
//	New();

  // Try to load file
  ASSERT(m_pMeshBuilder);
  m_pMeshBuilder->Load((void*) pszFileName,
	                     NULL,
							         D3DRMLOAD_FROMFILE | D3DRMLOAD_FIRST,
							         NULL,
							         NULL);

	AddVisual( m_pMeshBuilder->I() );

	return TRUE;
}

// Load a shape from an XOF resource
BOOL CAShape::Load(UINT uiResid)
{
	// Remove any existing visual
	New();

	// Try to load file
  ASSERT(m_pMeshBuilder);
	D3DRMLOADRESOURCE info;
	info.hModule = AfxGetResourceHandle();
	info.lpName = MAKEINTRESOURCE(uiResid);
	info.lpType = "XOF";
  m_pMeshBuilder->Load(&info,
							        NULL,
							        D3DRMLOAD_FROMRESOURCE,
							        NULL,
 							        NULL);

	AddVisual( m_pMeshBuilder->I() );

	return TRUE;
}

void CAShape::SetBoundary( D3DVECTOR vPos, D3DVALUE height, D3DVALUE radius )
{
// assume vPos is origin for now ...

  m_boundaryHeight = height;
  m_boundaryRadius = radius;
}

CAShape *CAShape::Collide( D3DVECTOR vPos, CAShapeList *pBoundaryList )
{
  // boundaries are cylinders 

  // pre-screen the boundaries before checking distance
  CAShapeList newList;
  POSITION pos = pBoundaryList->GetHeadPosition();
  while (pos)
  {
    CAShape *pShape = pBoundaryList->GetNext(pos);
    CAVector vBndPos;
    pShape->GetPosition( NULL, &vBndPos ); 

    // check that the heights intersect
    if ((vPos.y >= vBndPos.y) &&
        (vPos.y <= (vBndPos.y + pShape->m_boundaryHeight))
        ||
        ((vPos.y + m_boundaryHeight) >= vBndPos.y) &&
        ((vPos.y + m_boundaryHeight) <= (vBndPos.y + pShape->m_boundaryHeight)))
    {
      // check that the x z bounds intersect
      D3DVALUE minx = vPos.x - m_boundaryRadius;
      D3DVALUE maxx = vPos.x + m_boundaryRadius;
      D3DVALUE minz = vPos.z - m_boundaryRadius;
      D3DVALUE maxz = vPos.z + m_boundaryRadius;

      D3DVALUE bminx = vBndPos.x - pShape->m_boundaryRadius;
      D3DVALUE bmaxx = vBndPos.x + pShape->m_boundaryRadius;
      D3DVALUE bminz = vBndPos.z - pShape->m_boundaryRadius;
      D3DVALUE bmaxz = vBndPos.z + pShape->m_boundaryRadius;

      for (int i = 0; i < 4; i++)
      {
        D3DVALUE x, z;
        switch (i)
        {
        case 0:  x = minx;  z = minz;  break;
        case 1:  x = minx;  z = maxz;  break;
        case 2:  x = maxx;  z = minz;  break;
        case 3:  x = maxx;  z = maxz;  break;
        }

        if ((x >= bminx) && (x <= bmaxx) &&
            (z >= bminz) && (z <= bmaxz))
        {
          newList.AddTail( pShape );
          break;
        }
      }
    }
  }

  if (newList.GetCount() == 0)
    return NULL;

  // check the distances
  pos = newList.GetHeadPosition();
  while (pos)
  {
    CAShape *pShape = newList.GetNext(pos);
    CAVector vBndPos;
    pShape->GetPosition( NULL, &vBndPos ); 

    D3DVALUE dist =
      sqrt( SQR( vPos.x - vBndPos.x) + SQR( vPos.z - vBndPos.z ) );

    if ((dist - m_boundaryRadius - pShape->m_boundaryRadius) <= 0)
    {
      return pShape;  // collision
    }
  }

  return NULL;
}

BOOL CAShape::SpheresIntersect( CAVector *pPosS1, D3DVALUE radiusS1, CAVector *pPosS2, D3DVALUE radiusS2 )
{
  D3DVALUE distance =
    sqrt( SQR( pPosS1->x - pPosS2->x ) + SQR( pPosS1->y - pPosS2->y ) + SQR( pPosS1->z - pPosS2->z ) );

  if ((distance - radiusS1 - radiusS2) <= 0)
    return TRUE;

  return FALSE;
}

void CAShape::ApplyGravity( CAControllerData &data )
{
if (TRUE) return;

  CAVector &vPos = m_data.vPos;
  CAVector &vVelocity = m_vVelocity;

  // if in the air
  if (vPos.y > 0)
  {
    // apply the gravity vector
    CAVector vGravity(0, -0.98 * .5 /*speed*/, 0);
    vVelocity += vGravity;

    // no drag

  }
  else  // when on the ground
  {
    // stop at a minimum velocity
    if (vVelocity.Mag() <= 0.25)
    {
      vVelocity.x = 
      vVelocity.y = 
      vVelocity.z = 0;
    }
    else
    {
      // apply drag
      CAVector vDrag( vVelocity );
      vDrag.Normalize();
      vDrag = -vDrag * 0.25;
      vVelocity += vDrag;
    }

    // no gravity

    // start a jump
    if (data.m_aKeys[ DIK_SPACE ].Up())  // SPACE
    {
      DWORD dwTime = data.m_aKeys[ DIK_SPACE ].dwTime;

      if (dwTime < 2000)
        vVelocity.y = (D3DVALUE) dwTime / 64.0;
      else
        vVelocity.y = 30.0;
    }
  }
}

BOOL CAShape::TestBorders( CAControllerData &data )
{
  CAVector &vPos = m_data.vPos;
  CAVector &vVelocity = m_vVelocity;

  // advance to the new position
  CAVector vNewPos( vPos + vVelocity * g_valFrameRatio );

  // have we hit a border yet?  bounce off
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

#ifdef SAVE_ME // wrap-around borders
BOOL CAShape::TestBorders( CAControllerData &data )
{
  CAVector &vPos = m_data.vPos;
  CAVector &vVelocity = m_vVelocity;

  // advance to the new position
  CAVector vNewPos( vPos + vVelocity * g_valFrameRatio );

  // have we hit a border yet?  wrap around
  int bHit = FALSE;

  if (vNewPos.x > BOUNDARY)
  {
    vNewPos.x -= BOUNDARY * 2;
    bHit = TRUE;
  }
  else
  if (vNewPos.x < -BOUNDARY)
  {
    vNewPos.x += BOUNDARY * 2;
    bHit = TRUE;
  }

  if (vNewPos.y > BOUNDARY)
  {
    vNewPos.y -= BOUNDARY * 2;
    bHit = TRUE;
  }
  else
  if (vNewPos.y < -BOUNDARY)
  {
    vNewPos.y += BOUNDARY * 2;
    bHit = TRUE;
  }

  if (vNewPos.z > BOUNDARY)
  {
    vNewPos.z -= BOUNDARY * 2;
    bHit = TRUE;
  }
  else
  if (vNewPos.z < -BOUNDARY)
  {
    vNewPos.z += BOUNDARY * 2;
    bHit = TRUE;
  }

  vPos = vNewPos;

  return bHit;
}
#endif

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CADecal::CADecal( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef, DWORD dwWidth, DWORD dwHeight ) :
  m_dwWidth(dwWidth),
  m_dwHeight(dwHeight),
  CAShape( pD3D, pRef )
{
  // create the surface
  DDSURFACEDESC ddsd;
  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof( ddsd );
  ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_CKSRCBLT;
  ddsd.dwWidth = m_dwWidth;
  ddsd.dwHeight = m_dwHeight;
  ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = 0;  // color 0 is transparent
  ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = 0;
  ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE;
  m_pSurface = new CADirectDrawSurface( g_pDirectDraw, &ddsd );
  m_pSurface->GetSurfaceDesc( &ddsd );

  // attach the surface to a texture
  m_pTexture = new CADirect3DRMTexture ( g_pDirect3DRM, m_pSurface->I() );
  m_pTexture->SetDecalScale( TRUE );
  m_pTexture->SetDecalSize( 4.0, 4.0   );
  m_pTexture->SetDecalOrigin( m_dwWidth / 2, m_dwHeight / 2 );
  m_pTexture->SetDecalTransparency( TRUE );
  m_pTexture->SetDecalTransparentColor( D3DRGB(0,0,0) );
  AddVisual( (LPDIRECT3DRMVISUAL) m_pTexture->I() );
}

CADecal::~CADecal()
{
  delete m_pTexture;
  delete m_pSurface;
}

BOOL CADecal::SetTextLabel( LPCTSTR pText )
{
  CString sTmp( pText );
  m_rText.top = 0;
  m_rText.bottom = m_dwHeight-1;
  m_rText.left = 0;
  m_rText.right = m_dwWidth-1;

  HDC hdc;
  m_pSurface->GetDC(&hdc);
  SetBkColor( hdc, RGB( 0, 0, 0 ) );
  SetTextColor( hdc, RGB( 255, 255, 0 ) );
  PatBlt(hdc, 0, 0, m_dwWidth, m_dwHeight, BLACKNESS);

  DrawText( hdc, sTmp, sTmp.GetLength(), &m_rText,
    DT_CALCRECT | DT_CENTER | DT_WORDBREAK );
  int offset = m_dwWidth / 2 - m_rText.Height() / 2;
  if (offset < 0) offset = 0;
  m_rText.OffsetRect( 0, offset );
  
  DrawText( hdc, sTmp, sTmp.GetLength(), &m_rText,
    DT_CENTER | DT_WORDBREAK );

  m_pSurface->ReleaseDC(hdc);

  m_pTexture->Changed( TRUE, FALSE );
  return TRUE;
}

BOOL CADecal::SetBitmap( LPCTSTR pFile )
{
  CASprite sprite;
  sprite.LoadFromFile( pFile );

  RECT r;
  r.top = 0;
  r.left = 0;
  r.right = sprite.m_ddsd.dwWidth-1;
  r.bottom = sprite.m_ddsd.dwHeight-1;
  m_pSurface->Blt( &r, sprite.m_pSurface->I(), &r, DDBLT_WAIT, NULL);

  return TRUE;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

CAVector::CAVector()
{
	x = 1;
	y = 1;
	z = 1;
}

CAVector::~CAVector()
{
}

CAVector::CAVector(const D3DVECTOR& r)
{
	x = r.x;
	y = r.y;
	z = r.z;
}

CAVector::CAVector(D3DVALUE _x, D3DVALUE _y, D3DVALUE _z)
{
	x = _x;
	y = _y;
	z = _z;
}

CAVector& CAVector::operator = (const D3DVECTOR& r)
{
	x = r.x;
	y = r.y;
	z = r.z;
	return *this;
}

CAVector operator + (const D3DVECTOR& a, const D3DVECTOR& b)
{
    return CAVector(a.x + b.x,
                     a.y + b.y,
                     a.z + b.z);
}

CAVector& CAVector::operator += (const D3DVECTOR& r)
{
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
}

CAVector operator - (const D3DVECTOR& a, const D3DVECTOR& b)
{
    return CAVector(a.x - b.x,
                     a.y - b.y,
                     a.z - b.z);
}

CAVector& CAVector::operator -= (const D3DVECTOR& r)
{
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
}

CAVector CAVector::operator - ()
{
	return CAVector(-x, -y, -z);
}

// vector (cross) product
CAVector operator * (const D3DVECTOR& a, const D3DVECTOR& b)
{
    return CAVector(a.y * b.z - a.z * b.y,
					 a.z * b.x - a.x * b.z,
					 a.x * b.y - a.y * b.x);
}

// vector (cross) product
CAVector& CAVector::operator *= (const D3DVECTOR& r)
{
    // deliberately create a temp to hold result
    *this = *this * r;
    return *this;
}
                      
// magnitude change
CAVector operator * (const D3DVECTOR& a, const D3DVALUE s)
{
    return CAVector(a.x * s,
					 a.y * s,
					 a.z * s);
}

// magnitude change
CAVector& CAVector::operator *= (const D3DVALUE s)
{
	x *= s;
	y *= s;
	z *= s;
    return *this;
}

CAVector& CAVector::operator + (const D3DVALUE t)
{
	x += t;
	y += t;
	z += t;
  return *this;
}

BOOL CAVector::operator == (const D3DVECTOR& r)
{
  return ((x == r.x) && (y == r.y) && (z == r.z));
}


BOOL CAVector::operator != (const D3DVECTOR& r)
{
  return ((x != r.x) || (y != r.y) || (z != r.z));
}



// dot (scalar) product
D3DVALUE CAVector::Dot(const D3DVECTOR&r)
{
	return x * r.x + y * r.y + z * r.z;
}

// magnitude (length)
D3DVALUE CAVector::Mag()
{
	return (D3DVALUE) sqrt(x * x + y * y + z * z);
}

// Normalize a vector (make it unit lebgth)
BOOL CAVector::Normalize()
{
	D3DVALUE l = Mag();
	if (l == 0) return FALSE;
	x /= l;
	y /= l;
	z /= l;
	return TRUE;
}

// test for coincidence
BOOL CAVector::Coincident(const D3DVECTOR& r)
{
	CAVector a(*this);
	a.Normalize();
	CAVector b(r);
	b.Normalize();
	if ((a.x == b.x) 
	&& (a.y == b.y)
	&& (a.z == b.z)) {
		return TRUE;
	}
	return FALSE;
}

// Compute an arbitrary up vector
CAVector CAVector::GenerateUp()
{
	Normalize();

	// Create an initial up vector
	CAVector vu(0, 1, 0);

	// make sure the up and dir vectors are not the same
	if (Coincident(vu) || Coincident(-vu)) {

		// d is 'up' or 'down' so just set up to be x
		return CAVector(1, 0, 0);

	} 

	// compute the 'right' vector from the requested up
	// vector and the direction vector
	CAVector vr = *this * vu;

	// compute the actual up vector as from the dir and right vectors
	return vr * *this;
}

BOOL CAVector::IsNull()
{
	if ((x == 0.0) && (y == 0) && (z ==  0)) {
		return TRUE;
	}
	return FALSE;
}

BOOL CAVector::Reflect(const D3DVECTOR&rNorm)
{
  return (D3DRMVectorReflect(this, this, (D3DVECTOR *)&rNorm) == this);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CAMatrix::CAMatrix()
{
    m_00=1.0; m_01=0.0;  m_02=0.0; m_03=0.0;
    m_10=0.0; m_11=1.0;  m_12=0.0; m_13=0.0;
    m_20=0.0; m_21=0.0;  m_22=1.0; m_23=0.0;
    m_30=0.0; m_31=0.0;  m_32=0.0; m_33=1.0;
}

CAMatrix::~CAMatrix()
{
}

CAMatrix::CAMatrix(const CAMatrix& r)
{
    m_00=r.m_00; m_01=r.m_01; m_02=r.m_02; m_03=r.m_03; 
    m_10=r.m_10; m_11=r.m_11; m_12=r.m_12; m_13=r.m_13; 
    m_20=r.m_20; m_21=r.m_21; m_22=r.m_22; m_23=r.m_23; 
    m_30=r.m_30; m_31=r.m_31; m_32=r.m_32; m_33=r.m_33; 
}

CAMatrix::CAMatrix(D3DVALUE v00, D3DVALUE v01, D3DVALUE v02, D3DVALUE v03,
                   D3DVALUE v10, D3DVALUE v11, D3DVALUE v12, D3DVALUE v13,
                   D3DVALUE v20, D3DVALUE v21, D3DVALUE v22, D3DVALUE v23,
                   D3DVALUE v30, D3DVALUE v31, D3DVALUE v32, D3DVALUE v33)
{
    m_00=v00; m_01=v01; m_02=v02; m_03=v03; 
    m_10=v10; m_11=v11; m_12=v12; m_13=v13; 
    m_20=v20; m_21=v21; m_22=v22; m_23=v23; 
    m_30=v30; m_31=v31; m_32=v32; m_33=v33; 
}

CAMatrix& CAMatrix::operator = (const CAMatrix& r)
{
    m_00=r.m_00; m_01=r.m_01; m_02=r.m_02; m_03=r.m_03; 
    m_10=r.m_10; m_11=r.m_11; m_12=r.m_12; m_13=r.m_13; 
    m_20=r.m_20; m_21=r.m_21; m_22=r.m_22; m_23=r.m_23; 
    m_30=r.m_30; m_31=r.m_31; m_32=r.m_32; m_33=r.m_33; 
    return *this;
}

CAMatrix operator + (const CAMatrix& a, const CAMatrix& b)
{
    return CAMatrix(a.m_00 + b.m_00,
                     a.m_01 + b.m_01,
                     a.m_02 + b.m_02,
                     a.m_03 + b.m_03,
                     a.m_10 + b.m_10,
                     a.m_11 + b.m_11,
                     a.m_12 + b.m_12,
                     a.m_13 + b.m_13,
                     a.m_20 + b.m_20,
                     a.m_21 + b.m_21,
                     a.m_22 + b.m_22,
                     a.m_23 + b.m_23,
                     a.m_30 + b.m_30,
                     a.m_31 + b.m_31,
                     a.m_32 + b.m_32,
                     a.m_33 + b.m_33);
}

CAMatrix& CAMatrix::operator += (const CAMatrix& r)
{
    m_00+=r.m_00; m_01+=r.m_01; m_02+=r.m_02; m_03+=r.m_03;
    m_10+=r.m_10; m_11+=r.m_11; m_12+=r.m_12; m_13+=r.m_13;
    m_20+=r.m_20; m_21+=r.m_21; m_22+=r.m_22; m_23+=r.m_23;
    m_30+=r.m_30; m_31+=r.m_31; m_32+=r.m_32; m_33+=r.m_33;
    return *this;
}

// friend
CAMatrix operator * (const CAMatrix& a, const CAMatrix& b)
{
    return CAMatrix(a.m_00*b.m_00 + a.m_01*b.m_10 + a.m_02*b.m_20 + a.m_03*b.m_30,
                     a.m_00*b.m_01 + a.m_01*b.m_11 + a.m_02*b.m_21 + a.m_03*b.m_31,
                     a.m_00*b.m_02 + a.m_01*b.m_12 + a.m_02*b.m_22 + a.m_03*b.m_32,
                     a.m_00*b.m_03 + a.m_01*b.m_13 + a.m_02*b.m_23 + a.m_03*b.m_33,
                     a.m_10*b.m_00 + a.m_11*b.m_10 + a.m_12*b.m_20 + a.m_13*b.m_30,
                     a.m_10*b.m_01 + a.m_11*b.m_11 + a.m_12*b.m_21 + a.m_13*b.m_31,
                     a.m_10*b.m_02 + a.m_11*b.m_12 + a.m_12*b.m_22 + a.m_13*b.m_32,
                     a.m_10*b.m_03 + a.m_11*b.m_13 + a.m_12*b.m_23 + a.m_13*b.m_33,
                     a.m_20*b.m_00 + a.m_21*b.m_10 + a.m_22*b.m_20 + a.m_23*b.m_30,
                     a.m_20*b.m_01 + a.m_21*b.m_11 + a.m_22*b.m_21 + a.m_23*b.m_31,
                     a.m_20*b.m_02 + a.m_21*b.m_12 + a.m_22*b.m_22 + a.m_23*b.m_32,
                     a.m_20*b.m_03 + a.m_21*b.m_13 + a.m_22*b.m_23 + a.m_23*b.m_33,
                     a.m_30*b.m_00 + a.m_31*b.m_10 + a.m_32*b.m_20 + a.m_33*b.m_30,
                     a.m_30*b.m_01 + a.m_31*b.m_11 + a.m_32*b.m_21 + a.m_33*b.m_31,
                     a.m_30*b.m_02 + a.m_31*b.m_12 + a.m_32*b.m_22 + a.m_33*b.m_32,
                     a.m_30*b.m_03 + a.m_31*b.m_13 + a.m_32*b.m_23 + a.m_33*b.m_33);
}

// friend
D3DVECTOR operator * (const CAMatrix& m, const D3DVECTOR& v)
{
	D3DVECTOR r;
//	r.x = m.m_00 * v.x + m.m_01 * v.y + m.m_02 * v.z;
//	r.y = m.m_10 * v.x + m.m_11 * v.y + m.m_12 * v.z;
//	r.z = m.m_20 * v.x + m.m_21 * v.y + m.m_22 * v.z;
	r.x = m.m_00 * v.x + m.m_01 * v.y + m.m_02 * v.z + m.m_03;
	r.y = m.m_10 * v.x + m.m_11 * v.y + m.m_12 * v.z + m.m_13;
	r.z = m.m_20 * v.x + m.m_21 * v.y + m.m_22 * v.z + m.m_23;
	return r;
}

CAMatrix& CAMatrix::operator *= (const CAMatrix& r)
{
    // deliberately create a temp to hold result
    *this = *this * r;
    return *this;
}
                      
// rotate n degrees about each axis
void CAMatrix::Rotate(D3DVALUE rx, D3DVALUE ry, D3DVALUE rz, BOOL bRadians)
{
  D3DVALUE sinx;
	D3DVALUE cosx;
  D3DVALUE siny;
	D3DVALUE cosy;
  D3DVALUE sinz;
	D3DVALUE cosz;

  if (bRadians)
  {
    sinx = (D3DVALUE) sin(rx);
	  cosx = (D3DVALUE) cos(rx);
    siny = (D3DVALUE) sin(ry);
	  cosy = (D3DVALUE) cos(ry);
    sinz = (D3DVALUE) sin(rz);
	  cosz = (D3DVALUE) cos(rz);
  }
  else // degrees
  {
    sinx = (D3DVALUE) sin(rx * D2R);
	  cosx = (D3DVALUE) cos(rx * D2R);
    siny = (D3DVALUE) sin(ry * D2R);
	  cosy = (D3DVALUE) cos(ry * D2R);
    sinz = (D3DVALUE) sin(rz * D2R);
	  cosz = (D3DVALUE) cos(rz * D2R);
  }

	CAMatrix mx(1, 0,     0,    0,
              0,  cosx, sinx, 0,
              0, -sinx, cosx, 0,
              0, 0,     0,    1);

  CAMatrix my(cosy, 0, -siny, 0,
              0,    1, 0,     0,
              siny, 0,  cosy, 0,
              0,    0, 0,     1);

  CAMatrix mz( cosz, sinz, 0, 0,
               -sinz, cosz, 0, 0,
               0,     0,    1, 0,
               0,     0,    0, 1);

  *this *= mx * my * mz;
}

void CAMatrix::Translate(D3DVALUE dx, D3DVALUE dy, D3DVALUE dz)
{
    CAMatrix tx( 1, 0,  0, 0,
                 0, 1,  0, 0,
                 0, 0,  1, 0,
                 dx, dy, dz,1);

    *this *= tx;
}

void CAMatrix::Scale(D3DVALUE sx, D3DVALUE sy, D3DVALUE sz)
{
    CAMatrix tx(sx, 0,  0, 0,
                0, sy,  0, 0,
                0,  0, sz, 0,
                0,  0,  0, 1);

    *this *= tx;
}

void CAMatrix::MakeUnit()
{
    m_00=1.0; m_01=0.0;  m_02=0.0; m_03=0.0;
    m_10=0.0; m_11=1.0;  m_12=0.0; m_13=0.0;
    m_20=0.0; m_21=0.0;  m_22=1.0; m_23=0.0;
    m_30=0.0; m_31=0.0;  m_32=0.0; m_33=1.0;
}

void CAMatrix::Initialize(D3DRMMATRIX4D& rlm)
{
	rlm[0][0] = D3DVAL(m_00);
	rlm[0][1] = D3DVAL(m_01);
	rlm[0][2] = D3DVAL(m_02);
	rlm[0][3] = D3DVAL(m_03);
	rlm[1][0] = D3DVAL(m_10);
	rlm[1][1] = D3DVAL(m_11);
	rlm[1][2] = D3DVAL(m_12);
	rlm[1][3] = D3DVAL(m_13);
	rlm[2][0] = D3DVAL(m_20);
	rlm[2][1] = D3DVAL(m_21);
	rlm[2][2] = D3DVAL(m_22);
	rlm[2][3] = D3DVAL(m_23);
	rlm[3][0] = D3DVAL(m_30);
	rlm[3][1] = D3DVAL(m_31);
	rlm[3][2] = D3DVAL(m_32);
	rlm[3][3] = D3DVAL(m_33);
}

/////////////////////////////////////////////////////////////////