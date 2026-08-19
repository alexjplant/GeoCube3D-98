//#include <stdafx.h>
#include "dxa3drm.h"
#include "dxadraw.h"

static DWORD bppToddbd(int bpp)
{
    switch(bpp) {
    case 1:
	return DDBD_1;
    case 2:
	return DDBD_2;
    case 4:
	return DDBD_4;
    case 8:
	return DDBD_8;
    case 16:
	return DDBD_16;
    case 24:
	return DDBD_24;
    case 32:
	return DDBD_32;
    }
    return 0;
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CADirect3DRM::CADirect3DRM() : CADirectXObject()
{
  Check( Direct3DRMCreate( &m_pIDirect3DRM ) );
}

CADirect3DRM::~CADirect3DRM()
{
  if (m_pIDirect3DRM) m_pIDirect3DRM->Release();
}


HRESULT CADirect3DRM::AddSearchPath(LPCSTR lpPath)
{
  return Check( m_pIDirect3DRM->AddSearchPath( lpPath) );
}

HRESULT CADirect3DRM::CreateAnimation(LPDIRECT3DRMANIMATION * lplpD3DRMAnimation)
{
  return Check( m_pIDirect3DRM->CreateAnimation( lplpD3DRMAnimation) );
}

HRESULT CADirect3DRM::CreateAnimationSet (LPDIRECT3DRMANIMATIONSET * lplpD3DRMAnimationSet)
{
  return Check( m_pIDirect3DRM->CreateAnimationSet ( lplpD3DRMAnimationSet) );
}

HRESULT CADirect3DRM::CreateDevice(DWORD dwWidth, DWORD dwHeight,
      LPDIRECT3DRMDEVICE* lplpD3DRMDevice)
{
  return Check( m_pIDirect3DRM->CreateDevice( dwWidth,  dwHeight,
      lplpD3DRMDevice) );
}

HRESULT CADirect3DRM::CreateDeviceFromClipper(LPDIRECTDRAWCLIPPER lpDDClipper, 
      LPGUID lpGUID, int width, int height, 
      LPDIRECT3DRMDEVICE * lplpD3DRMDevice)
{
  return Check( m_pIDirect3DRM->CreateDeviceFromClipper( lpDDClipper, 
      lpGUID, width, height, 
      lplpD3DRMDevice) );
}

HRESULT CADirect3DRM::CreateDeviceFromD3D(LPDIRECT3D lpD3D, 
      LPDIRECT3DDEVICE lpD3DDev, LPDIRECT3DRMDEVICE * lplpD3DRMDevice)
{
  return Check( m_pIDirect3DRM->CreateDeviceFromD3D( lpD3D, 
      lpD3DDev, lplpD3DRMDevice) );
}

HRESULT CADirect3DRM::CreateDeviceFromSurface(LPGUID lpGUID, LPDIRECTDRAW lpDD,
      LPDIRECTDRAWSURFACE lpDDSBack,
      LPDIRECT3DRMDEVICE * lplpD3DRMDevice)
{
  return Check( m_pIDirect3DRM->CreateDeviceFromSurface( lpGUID, lpDD,
      lpDDSBack,
      lplpD3DRMDevice) );
}

HRESULT CADirect3DRM::CreateFace(LPDIRECT3DRMFACE * lplpd3drmFace)
{
  return Check( m_pIDirect3DRM->CreateFace(lplpd3drmFace) );
}

HRESULT CADirect3DRM::CreateFrame(LPDIRECT3DRMFRAME lpD3DRMFrame,
      LPDIRECT3DRMFRAME* lplpD3DRMFrame)
{
  return Check( m_pIDirect3DRM->CreateFrame(lpD3DRMFrame,
      lplpD3DRMFrame) );
}

HRESULT CADirect3DRM::CreateLight(D3DRMLIGHTTYPE d3drmltLightType, 
      D3DCOLOR cColor, LPDIRECT3DRMLIGHT* lplpD3DRMLight)
{
  return Check( m_pIDirect3DRM->CreateLight( d3drmltLightType, 
       cColor, lplpD3DRMLight) );
}

HRESULT CADirect3DRM::CreateLightRGB(D3DRMLIGHTTYPE ltLightType, D3DVALUE vRed,
      D3DVALUE vGreen, D3DVALUE vBlue, LPDIRECT3DRMLIGHT* lplpD3DRMLight)
{
  return Check( m_pIDirect3DRM->CreateLightRGB( ltLightType,  vRed,
      vGreen, vBlue, lplpD3DRMLight) );
}

HRESULT CADirect3DRM::CreateMaterial(D3DVALUE vPower,
      LPDIRECT3DRMMATERIAL * lplpD3DRMMaterial)
{
  return Check( m_pIDirect3DRM->CreateMaterial( vPower,
      lplpD3DRMMaterial) );
}

HRESULT CADirect3DRM::CreateMesh(LPDIRECT3DRMMESH* lplpD3DRMMesh)
{
  return Check( m_pIDirect3DRM->CreateMesh(lplpD3DRMMesh) );
}

HRESULT CADirect3DRM::CreateMeshBuilder(LPDIRECT3DRMMESHBUILDER* lplpD3DRMMeshBuilder)
{
  return Check( m_pIDirect3DRM->CreateMeshBuilder(lplpD3DRMMeshBuilder) );
}

HRESULT CADirect3DRM::CreateObject(REFCLSID rclsid, LPUNKNOWN pUnkOuter, 
      REFIID riid, LPVOID FAR* ppv)
{
  return Check( m_pIDirect3DRM->CreateObject( rclsid, pUnkOuter, 
      riid,  ppv) );
}

HRESULT CADirect3DRM::CreateShadow(LPDIRECT3DRMVISUAL lpVisual, 
      LPDIRECT3DRMLIGHT lpLight, D3DVALUE px, D3DVALUE py, D3DVALUE pz, 
      D3DVALUE nx, D3DVALUE ny, D3DVALUE nz, 
      LPDIRECT3DRMVISUAL * lplpShadow)
{
  return Check( m_pIDirect3DRM->CreateShadow( lpVisual, 
       lpLight, px, py, pz, 
       nx, ny, nz, 
       lplpShadow) );
}

HRESULT CADirect3DRM::CreateTexture(LPD3DRMIMAGE lpImage,
      LPDIRECT3DRMTEXTURE* lplpD3DRMTexture)
{
  return Check( m_pIDirect3DRM->CreateTexture( lpImage,
      lplpD3DRMTexture) );
}

HRESULT CADirect3DRM::CreateTextureFromSurface(LPDIRECTDRAWSURFACE lpDDS,
      LPDIRECT3DRMTEXTURE * lplpD3DRMTexture)
{
  return Check( m_pIDirect3DRM->CreateTextureFromSurface( lpDDS,
      lplpD3DRMTexture) );
}

HRESULT CADirect3DRM::CreateUserVisual(D3DRMUSERVISUALCALLBACK fn,
      LPVOID  lpArg, LPDIRECT3DRMUSERVISUAL * lplpD3DRMUV)
{
  return Check( m_pIDirect3DRM->CreateUserVisual( fn,
      lpArg, lplpD3DRMUV) );
}

HRESULT CADirect3DRM::CreateViewport(LPDIRECT3DRMDEVICE lpDev,
      LPDIRECT3DRMFRAME lpCamera, DWORD dwXPos,
      DWORD dwYPos, DWORD dwWidth, DWORD dwHeight,
      LPDIRECT3DRMVIEWPORT* lplpD3DRMViewport)
{
  return Check( m_pIDirect3DRM->CreateViewport( lpDev,
      lpCamera, dwXPos,
      dwYPos, dwWidth, dwHeight,
      lplpD3DRMViewport) );
}

HRESULT CADirect3DRM::CreateWrap(D3DRMWRAPTYPE type, LPDIRECT3DRMFRAME lpRef, 
      D3DVALUE ox, D3DVALUE oy, D3DVALUE oz, D3DVALUE dx, D3DVALUE dy, 
      D3DVALUE dz, D3DVALUE ux, D3DVALUE uy, D3DVALUE uz, D3DVALUE ou, 
      D3DVALUE ov, D3DVALUE su, D3DVALUE sv, 
      LPDIRECT3DRMWRAP* lplpD3DRMWrap)
{
  return Check( m_pIDirect3DRM->CreateWrap( type, lpRef, 
      ox, oy, oz, dx, dy, 
      dz, ux, uy, uz, ou, 
      ov, su, sv, 
      lplpD3DRMWrap) );
}

HRESULT CADirect3DRM::EnumerateObjects(D3DRMOBJECTCALLBACK func, LPVOID lpArg)
{
  return Check( m_pIDirect3DRM->EnumerateObjects( func, lpArg) );
}

HRESULT CADirect3DRM::GetDevices(LPDIRECT3DRMDEVICEARRAY* lplpDevArray)
{
  return Check( m_pIDirect3DRM->GetDevices(lplpDevArray) );
}

HRESULT CADirect3DRM::GetNamedObject(const char * lpName,
      LPDIRECT3DRMOBJECT* lplpD3DRMObject)
{
  return Check( m_pIDirect3DRM->GetNamedObject(lpName,
      lplpD3DRMObject) );
}

HRESULT CADirect3DRM::GetSearchPath(DWORD * lpdwSize, LPSTR lpszPath)
{
  return Check( m_pIDirect3DRM->GetSearchPath( lpdwSize, lpszPath) );
}

HRESULT CADirect3DRM::Load(LPVOID lpvObjSource, LPVOID lpvObjID,
      LPIID * lplpGUIDs, DWORD dwcGUIDs, D3DRMLOADOPTIONS d3drmLOFlags, 
      D3DRMLOADCALLBACK d3drmLoadProc, LPVOID lpArgLP, 
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpArgLTP, 
      LPDIRECT3DRMFRAME lpParentFrame)
{
  return Check( m_pIDirect3DRM->Load( lpvObjSource, lpvObjID,
      lplpGUIDs, dwcGUIDs, d3drmLOFlags, 
      d3drmLoadProc, lpArgLP, 
      d3drmLoadTextureProc, lpArgLTP, 
      lpParentFrame) );
}

HRESULT CADirect3DRM::LoadTexture(const char * lpFileName,
      LPDIRECT3DRMTEXTURE* lplpD3DRMTexture)
{
  return Check( m_pIDirect3DRM->LoadTexture( lpFileName,
      lplpD3DRMTexture) );
}

HRESULT CADirect3DRM::LoadTextureFromResource(HRSRC rs, 
      LPDIRECT3DRMTEXTURE * lplpD3DRMTexture)
{
  return Check( m_pIDirect3DRM->LoadTextureFromResource( rs, 
      lplpD3DRMTexture) );
}

HRESULT CADirect3DRM::SetDefaultTextureColors(DWORD dwColors)
{
  return Check( m_pIDirect3DRM->SetDefaultTextureColors(dwColors) );
}

HRESULT CADirect3DRM::SetDefaultTextureShades(DWORD dwShades)
{
  return Check( m_pIDirect3DRM->SetDefaultTextureShades(dwShades) );
}

HRESULT CADirect3DRM::SetSearchPath(LPCSTR lpPath)
{
  return Check( m_pIDirect3DRM->SetSearchPath(lpPath) );
}

HRESULT CADirect3DRM::Tick(D3DVALUE d3dvalTick)
{
  return Check( m_pIDirect3DRM->Tick(d3dvalTick) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMObject::CADirect3DRMObject() : CADirectXObject()
{
  // Derived-classes should call SetBaseInterface()
}

CADirect3DRMObject::~CADirect3DRMObject()
{
  if (m_pIDirect3DRMObject) m_pIDirect3DRMObject->Release();
}


HRESULT CADirect3DRMObject::AddDestroyCallback(D3DRMOBJECTCALLBACK lpCallback,
      LPVOID lpArg)
{
  return Check( m_pIDirect3DRMObject->AddDestroyCallback( lpCallback, lpArg) );
}

HRESULT CADirect3DRMObject::Clone(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppvObj)
{
  return Check( m_pIDirect3DRMObject->Clone(pUnkOuter, riid, ppvObj) );
}

HRESULT CADirect3DRMObject::DeleteDestroyCallback(D3DRMOBJECTCALLBACK d3drmObjProc,
      LPVOID lpArg)
{
  return Check( m_pIDirect3DRMObject->DeleteDestroyCallback( d3drmObjProc, lpArg) );
}

DWORD CADirect3DRMObject::GetAppData()
{
  return Check( m_pIDirect3DRMObject->GetAppData() );
}

HRESULT CADirect3DRMObject::GetClassName(LPDWORD lpdwSize, LPSTR lpName)
{
  return Check( m_pIDirect3DRMObject->GetClassName(lpdwSize, lpName) );
}

HRESULT CADirect3DRMObject::GetName(LPDWORD lpdwSize, LPSTR lpName)
{
  return Check( m_pIDirect3DRMObject->GetName(lpdwSize, lpName) );
}

HRESULT CADirect3DRMObject::SetAppData(DWORD ulData)
{
  return Check( m_pIDirect3DRMObject->SetAppData(ulData) );
}

HRESULT CADirect3DRMObject::SetName(const char * lpName)
{
  return Check( m_pIDirect3DRMObject->SetName( lpName) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMAnimation::CADirect3DRMAnimation( CADirect3DRM *pD3D )
  : CADirect3DRMObject()
{
  pD3D->CreateAnimation( &m_pIDirect3DRMAnimation );

  SetBaseInterface();
}

CADirect3DRMAnimation::~CADirect3DRMAnimation()
{
  if (m_pIDirect3DRMAnimation) m_pIDirect3DRMAnimation->Release();
}

void CADirect3DRMAnimation::SetBaseInterface()
{
  Check( m_pIDirect3DRMAnimation->QueryInterface( IID_IDirect3DRMObject, (void **) &m_pIDirect3DRMObject ));
}


HRESULT CADirect3DRMAnimation::AddPositionKey(D3DVALUE rvTime, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ)
{
  return Check( m_pIDirect3DRMAnimation->AddPositionKey( rvTime, rvX, rvY, rvZ) );
}

HRESULT CADirect3DRMAnimation::AddRotateKey(D3DVALUE rvTime, D3DRMQUATERNION *rqQuat)
{
  return Check( m_pIDirect3DRMAnimation->AddRotateKey(rvTime, rqQuat) );
}

HRESULT CADirect3DRMAnimation::AddScaleKey(D3DVALUE rvTime, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ)
{
  return Check( m_pIDirect3DRMAnimation->AddScaleKey( rvTime, rvX, rvY, rvZ) );
}

HRESULT CADirect3DRMAnimation::DeleteKey(D3DVALUE rvTime)
{
  return Check( m_pIDirect3DRMAnimation->DeleteKey( rvTime) );
}

D3DRMANIMATIONOPTIONS CADirect3DRMAnimation::GetOptions()
{
  return Check( m_pIDirect3DRMAnimation->GetOptions() );
}

HRESULT CADirect3DRMAnimation::SetFrame(LPDIRECT3DRMFRAME lpD3DRMFrame)
{
  return Check( m_pIDirect3DRMAnimation->SetFrame(lpD3DRMFrame) );
}

HRESULT CADirect3DRMAnimation::SetOptions(D3DRMANIMATIONOPTIONS d3drmanimFlags)
{
  return Check( m_pIDirect3DRMAnimation->SetOptions(d3drmanimFlags) );
}

HRESULT CADirect3DRMAnimation::SetTime(D3DVALUE rvTime)
{
  return Check( m_pIDirect3DRMAnimation->SetTime(rvTime) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMAnimationSet::CADirect3DRMAnimationSet( CADirect3DRM *pD3D )
  : CADirect3DRMObject()
{
  pD3D->CreateAnimationSet( &m_pIDirect3DRMAnimationSet );

  SetBaseInterface();
}

CADirect3DRMAnimationSet::~CADirect3DRMAnimationSet()
{
  if (m_pIDirect3DRMAnimationSet) m_pIDirect3DRMAnimationSet->Release();
}

void CADirect3DRMAnimationSet::SetBaseInterface()
{
  Check( m_pIDirect3DRMAnimationSet->QueryInterface( IID_IDirect3DRMObject, (void **) &m_pIDirect3DRMObject ));
}


HRESULT CADirect3DRMAnimationSet::AddAnimation(LPDIRECT3DRMANIMATION lpD3DRMAnimation)
{
  return Check( m_pIDirect3DRMAnimationSet->AddAnimation( lpD3DRMAnimation) );
}

HRESULT CADirect3DRMAnimationSet::DeleteAnimation(LPDIRECT3DRMANIMATION lpD3DRMAnimation)
{
  return Check( m_pIDirect3DRMAnimationSet->DeleteAnimation(lpD3DRMAnimation) );
}

HRESULT CADirect3DRMAnimationSet::Load(LPVOID lpvObjSource, LPVOID lpvObjID, 
      D3DRMLOADOPTIONS d3drmLOFlags, 
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpArgLTP, 
      LPDIRECT3DRMFRAME lpParentFrame)
{
  return Check( m_pIDirect3DRMAnimationSet->Load(lpvObjSource, lpvObjID, 
      d3drmLOFlags, 
      d3drmLoadTextureProc, lpArgLTP, 
      lpParentFrame) );
}

HRESULT CADirect3DRMAnimationSet::SetTime(D3DVALUE rvTime)
{
  return Check( m_pIDirect3DRMAnimationSet->SetTime(rvTime) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMDevice::CADirect3DRMDevice( CADirect3DRM *pD3DRM, CADirectDraw *pDDRM, LPDIRECTDRAWSURFACE lpDDSBack )
  : CADirect3DRMObject()
{
  D3DCOLORMODEL cm = D3DCOLOR_RGB;

  // get immediate-mode interfaces
  LPDIRECTDRAW pDD;
  if (DirectDrawCreate(NULL, &pDD, NULL) != DD_OK)
    return;

  LPDIRECT3D pD3D;
  if (pDD->QueryInterface(IID_IDirect3D, (void**) &pD3D) != DD_OK)
  {
    pDD->Release();
    return;
  }

  HDC hdc = GetDC(NULL);
  int bpp = GetDeviceCaps(hdc, BITSPIXEL);
  ReleaseDC(NULL, hdc);

  D3DFINDDEVICESEARCH search;
  memset(&search, 0, sizeof search);
  search.dwSize = sizeof search;
  search.dpcPrimCaps.dwSize = sizeof search.dpcPrimCaps;
  search.dwFlags = D3DFDS_COLORMODEL;
  search.dcmColorModel = (cm == D3DCOLOR_MONO) ? D3DCOLOR_MONO : D3DCOLOR_RGB;

  if (0) //(bSoftwareOnly)
  {
	  search.dwFlags |= D3DFDS_HARDWARE;
	  search.bHardware = FALSE;
  }

  static D3DFINDDEVICERESULT result;
  memset(&result, 0, sizeof result);
  result.dwSize = sizeof result;
  result.ddHwDesc.dwSize = sizeof result.ddHwDesc;
  result.ddSwDesc.dwSize = sizeof result.ddSwDesc;

  HRESULT error = pD3D->FindDevice( &search, &result );
  if (error == DD_OK)
  {
	  /*
	   * If the device found is hardware but cannot support the current
	   * bit depth, then fall back to software rendering.
	   */
	  if (result.ddHwDesc.dwFlags &&
	      !(result.ddHwDesc.dwDeviceRenderBitDepth & bppToddbd(bpp)))
    {

      search.dwFlags |= D3DFDS_HARDWARE;
	    search.bHardware = FALSE;
	    memset(&result, 0, sizeof result);
	    result.dwSize = sizeof result;
	    error = pD3D->FindDevice(&search, &result);
	  }
  }

  pD3D->Release();
  pDD->Release();

  GUID *pGuid = NULL;
  if (error == DD_OK)
	  pGuid = &result.guid;

  pD3DRM->CreateDeviceFromSurface( pGuid, pDDRM->I(), lpDDSBack, &m_pIDirect3DRMDevice );

  SetBaseInterface();
}

CADirect3DRMDevice::CADirect3DRMDevice( LPDIRECT3DRMDEVICE pIDev )
  : CADirect3DRMObject()
{
  m_pIDirect3DRMDevice = pIDev;

  SetBaseInterface();
}

CADirect3DRMDevice::~CADirect3DRMDevice()
{
  if (m_pIDirect3DRMDevice) m_pIDirect3DRMDevice->Release();
}

void CADirect3DRMDevice::SetBaseInterface()
{
  Check( m_pIDirect3DRMDevice->QueryInterface( IID_IDirect3DRMObject, (void **) &m_pIDirect3DRMObject ));
}


HRESULT CADirect3DRMDevice::AddUpdateCallback(D3DRMUPDATECALLBACK d3drmUpdateProc, LPVOID arg)
{
  return Check( m_pIDirect3DRMDevice->AddUpdateCallback( d3drmUpdateProc, arg) );
}

HRESULT CADirect3DRMDevice::DeleteUpdateCallback(D3DRMUPDATECALLBACK d3drmUpdateProc,
      LPVOID arg)
{
  return Check( m_pIDirect3DRMDevice->DeleteUpdateCallback( d3drmUpdateProc, arg) );
}

DWORD CADirect3DRMDevice::GetBufferCount()
{
  return Check( m_pIDirect3DRMDevice->GetBufferCount() );
}

D3DCOLORMODEL CADirect3DRMDevice::GetColorModel()
{
  return m_pIDirect3DRMDevice->GetColorModel();
}

HRESULT CADirect3DRMDevice::GetDirect3DDevice(LPDIRECT3DDEVICE * lplpD3DDevice)
{
  return Check( m_pIDirect3DRMDevice->GetDirect3DDevice(lplpD3DDevice) );
}

BOOL CADirect3DRMDevice::GetDither()
{
  return m_pIDirect3DRMDevice->GetDither();
}

DWORD CADirect3DRMDevice::GetHeight()
{
  return m_pIDirect3DRMDevice->GetHeight();
}

DWORD CADirect3DRMDevice::GetTrianglesDrawn()
{
  return m_pIDirect3DRMDevice->GetTrianglesDrawn();
}

D3DRMRENDERQUALITY CADirect3DRMDevice::GetQuality()
{
  return m_pIDirect3DRMDevice->GetQuality();
}

DWORD CADirect3DRMDevice::GetShades()
{
  return m_pIDirect3DRMDevice->GetShades();
}

D3DRMTEXTUREQUALITY CADirect3DRMDevice::GetTextureQuality()
{
  return m_pIDirect3DRMDevice->GetTextureQuality();
}

HRESULT CADirect3DRMDevice::GetViewports(LPDIRECT3DRMVIEWPORTARRAY* lplpViewports)
{
  return Check( m_pIDirect3DRMDevice->GetViewports(lplpViewports) );
}

DWORD CADirect3DRMDevice::GetWidth()
{
  return m_pIDirect3DRMDevice->GetWidth();
}

DWORD CADirect3DRMDevice::GetWireframeOptions()
{
  return m_pIDirect3DRMDevice->GetWireframeOptions();
}

HRESULT CADirect3DRMDevice::Init(ULONG width, ULONG height)
{
  return m_pIDirect3DRMDevice->Init(width, height);
}

HRESULT CADirect3DRMDevice::InitFromClipper(LPDIRECTDRAWCLIPPER lpDDClipper, 
      LPGUID lpGUID, int width, int height)
{
  return Check( m_pIDirect3DRMDevice->InitFromClipper( lpDDClipper, 
      lpGUID, width, height) );
}

HRESULT CADirect3DRMDevice::InitFromD3D(LPDIRECT3D lpD3D, LPDIRECT3DDEVICE lpD3DIMDev)
{
  return Check( m_pIDirect3DRMDevice->InitFromD3D(lpD3D, lpD3DIMDev) );
}

HRESULT CADirect3DRMDevice::SetBufferCount(DWORD dwCount)
{
  return Check( m_pIDirect3DRMDevice->SetBufferCount(dwCount) );
}

HRESULT CADirect3DRMDevice::SetDither(BOOL bDither)
{
  return Check( m_pIDirect3DRMDevice->SetDither(bDither) );
}

HRESULT CADirect3DRMDevice::SetQuality (D3DRMRENDERQUALITY rqQuality)
{
  return Check( m_pIDirect3DRMDevice->SetQuality (rqQuality) );
}

HRESULT CADirect3DRMDevice::SetShades(DWORD ulShades)
{
  return Check( m_pIDirect3DRMDevice->SetShades(ulShades) );
}

HRESULT CADirect3DRMDevice::SetTextureQuality(D3DRMTEXTUREQUALITY tqTextureQuality)
{
  return Check( m_pIDirect3DRMDevice->SetTextureQuality(tqTextureQuality) );
}

HRESULT CADirect3DRMDevice::Update()
{
  return Check( m_pIDirect3DRMDevice->Update() );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMFace::CADirect3DRMFace( CADirect3DRM *pD3D )
  : CADirect3DRMObject()
{
  pD3D->CreateFace( &m_pIDirect3DRMFace );

  SetBaseInterface();
}

CADirect3DRMFace::~CADirect3DRMFace()
{
  if (m_pIDirect3DRMFace) m_pIDirect3DRMFace->Release();
}

void CADirect3DRMFace::SetBaseInterface()
{
  Check( m_pIDirect3DRMFace->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}


HRESULT CADirect3DRMFace::AddVertex(D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
  return Check( m_pIDirect3DRMFace->AddVertex( x, y, z) );
}

HRESULT CADirect3DRMFace::AddVertexAndNormalIndexed(DWORD vertex, DWORD normal)
{
  return Check( m_pIDirect3DRMFace->AddVertexAndNormalIndexed(vertex, normal) );
}

D3DCOLOR CADirect3DRMFace::GetColor()
{
  return m_pIDirect3DRMFace->GetColor();
}

HRESULT CADirect3DRMFace::GetMaterial(LPDIRECT3DRMMATERIAL* lplpMaterial)
{
  return Check( m_pIDirect3DRMFace->GetMaterial(lplpMaterial) );
}

HRESULT CADirect3DRMFace::GetNormal(D3DVECTOR *lpNormal)
{
  return Check( m_pIDirect3DRMFace->GetNormal(lpNormal) );
}

HRESULT CADirect3DRMFace::GetTexture(LPDIRECT3DRMTEXTURE* lplpTexture)
{
  return Check( m_pIDirect3DRMFace->GetTexture(lplpTexture) );
}

int CADirect3DRMFace::GetTextureCoordinateIndex(DWORD dwIndex)
{
  return m_pIDirect3DRMFace->GetTextureCoordinateIndex(dwIndex);
}

HRESULT CADirect3DRMFace::GetTextureCoordinates(DWORD index, D3DVALUE *lpU,
      D3DVALUE *lpV)
{
  return Check( m_pIDirect3DRMFace->GetTextureCoordinates(index, lpU, lpV) );
}

HRESULT CADirect3DRMFace::GetTextureTopology(BOOL *lpU, BOOL *lpV)
{
  return Check( m_pIDirect3DRMFace->GetTextureTopology(lpU, lpV) );
}

HRESULT CADirect3DRMFace::GetVertex(DWORD index, D3DVECTOR *lpPosition,
      D3DVECTOR *lpNormal)
{
  return Check( m_pIDirect3DRMFace->GetVertex( index, lpPosition, lpNormal) );
}

int CADirect3DRMFace::GetVertexCount()
{
  return m_pIDirect3DRMFace->GetVertexCount();
}

int CADirect3DRMFace::GetVertexIndex (DWORD dwIndex)
{
  return m_pIDirect3DRMFace->GetVertexIndex (dwIndex);
}

HRESULT CADirect3DRMFace::GetVertices(DWORD *lpdwVertexCount, D3DVECTOR *lpPosition,
      D3DVECTOR *lpNormal)
{
  return Check( m_pIDirect3DRMFace->GetVertices(lpdwVertexCount, lpPosition, lpNormal) );
}

HRESULT CADirect3DRMFace::SetColor(D3DCOLOR color)
{
  return Check( m_pIDirect3DRMFace->SetColor(color) );
}

HRESULT CADirect3DRMFace::SetColorRGB(D3DVALUE red, D3DVALUE green, D3DVALUE blue)
{
  return Check( m_pIDirect3DRMFace->SetColorRGB(red, green, blue) );
}

HRESULT CADirect3DRMFace::SetMaterial(LPDIRECT3DRMMATERIAL lpD3DRMMaterial)
{
  return Check( m_pIDirect3DRMFace->SetMaterial(lpD3DRMMaterial) );
}

HRESULT CADirect3DRMFace::SetTexture(LPDIRECT3DRMTEXTURE lpD3DRMTexture)
{
  return Check( m_pIDirect3DRMFace->SetTexture(lpD3DRMTexture) );
}

HRESULT CADirect3DRMFace::SetTextureCoordinates(DWORD vertex, D3DVALUE u, D3DVALUE v)
{
  return Check( m_pIDirect3DRMFace->SetTextureCoordinates( vertex, u, v) );
}

HRESULT CADirect3DRMFace::SetTextureTopology(BOOL cylU, BOOL cylV)
{
  return Check( m_pIDirect3DRMFace->SetTextureTopology(cylU, cylV) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMFrame::CADirect3DRMFrame( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME lpD3DRMFrame )
  : CADirect3DRMObject()
{
  pD3D->CreateFrame( lpD3DRMFrame, &m_pIDirect3DRMFrame );

  SetBaseInterface();
}

CADirect3DRMFrame::~CADirect3DRMFrame()
{
  if (m_pIDirect3DRMFrame) m_pIDirect3DRMFrame->Release();
}

void CADirect3DRMFrame::SetBaseInterface()
{
  Check( m_pIDirect3DRMFrame->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMFrame::AddChild(LPDIRECT3DRMFRAME lpD3DRMFrameChild)
{
  return Check( m_pIDirect3DRMFrame->AddChild( lpD3DRMFrameChild) );
}

HRESULT CADirect3DRMFrame::AddLight(LPDIRECT3DRMLIGHT lpD3DRMLight)
{
  return Check( m_pIDirect3DRMFrame->AddLight( lpD3DRMLight) );
}

HRESULT CADirect3DRMFrame::AddMoveCallback(D3DRMFRAMEMOVECALLBACK d3drmFMC, VOID * lpArg)
{
  return Check( m_pIDirect3DRMFrame->AddMoveCallback( d3drmFMC, lpArg) );
}

HRESULT CADirect3DRMFrame::AddRotation(D3DRMCOMBINETYPE rctCombine, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ, D3DVALUE rvTheta)
{
  return Check( m_pIDirect3DRMFrame->AddRotation( rctCombine, rvX,
      rvY, rvZ, rvTheta) );
}

HRESULT CADirect3DRMFrame::AddScale(D3DRMCOMBINETYPE rctCombine, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ)
{
  return Check( m_pIDirect3DRMFrame->AddScale( rctCombine, rvX, rvY, rvZ) );
}

HRESULT CADirect3DRMFrame::AddTransform(D3DRMCOMBINETYPE rctCombine,
      D3DRMMATRIX4D rmMatrix)
{
  return Check( m_pIDirect3DRMFrame->AddTransform( rctCombine, rmMatrix) );
}

HRESULT CADirect3DRMFrame::AddTranslation(D3DRMCOMBINETYPE rctCombine, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ)
{
  return Check( m_pIDirect3DRMFrame->AddTranslation( rctCombine, rvX, rvY, rvZ) );
}

HRESULT CADirect3DRMFrame::AddVisual(LPDIRECT3DRMVISUAL lpD3DRMVisual)
{
  return Check( m_pIDirect3DRMFrame->AddVisual( lpD3DRMVisual) );
}

HRESULT CADirect3DRMFrame::DeleteChild(LPDIRECT3DRMFRAME lpChild)
{
  return Check( m_pIDirect3DRMFrame->DeleteChild( lpChild) );
}

HRESULT CADirect3DRMFrame::DeleteLight(LPDIRECT3DRMLIGHT lpD3DRMLight)
{
  return Check( m_pIDirect3DRMFrame->DeleteLight( lpD3DRMLight) );
}

HRESULT CADirect3DRMFrame::DeleteMoveCallback(D3DRMFRAMEMOVECALLBACK d3drmFMC, 
      VOID * lpArg)
{
  return Check( m_pIDirect3DRMFrame->DeleteMoveCallback( d3drmFMC, lpArg) );
}

HRESULT CADirect3DRMFrame::DeleteVisual(LPDIRECT3DRMVISUAL lpD3DRMVisual)
{
  return Check( m_pIDirect3DRMFrame->DeleteVisual( lpD3DRMVisual) );
}

HRESULT CADirect3DRMFrame::GetChildren(LPDIRECT3DRMFRAMEARRAY* lplpChildren)
{
  return Check( m_pIDirect3DRMFrame->GetChildren( lplpChildren) );
}

D3DCOLOR CADirect3DRMFrame::GetColor()
{
  return m_pIDirect3DRMFrame->GetColor();
}

HRESULT CADirect3DRMFrame::GetLights(LPDIRECT3DRMLIGHTARRAY* lplpLights)
{
  return Check( m_pIDirect3DRMFrame->GetLights( lplpLights) );
}

D3DRMMATERIALMODE CADirect3DRMFrame::GetMaterialMode()
{
  return m_pIDirect3DRMFrame->GetMaterialMode();
}

HRESULT CADirect3DRMFrame::GetOrientation(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvDir,
      LPD3DVECTOR lprvUp)
{
  return Check( m_pIDirect3DRMFrame->GetOrientation( lpRef, lprvDir, lprvUp) );
}

HRESULT CADirect3DRMFrame::GetParent(LPDIRECT3DRMFRAME* lplpParent)
{
  return Check( m_pIDirect3DRMFrame->GetParent( lplpParent) );
}

HRESULT CADirect3DRMFrame::GetPosition(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvPos)
{
  return Check( m_pIDirect3DRMFrame->GetPosition( lpRef, lprvPos) );
}

HRESULT CADirect3DRMFrame::GetRotation(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvAxis,
      LPD3DVALUE lprvTheta)
{
  return Check( m_pIDirect3DRMFrame->GetRotation( lpRef, lprvAxis, lprvTheta) );
}

HRESULT CADirect3DRMFrame::GetScene(LPDIRECT3DRMFRAME* lplpRoot)
{
  return Check( m_pIDirect3DRMFrame->GetScene( lplpRoot) );
}

D3DCOLOR CADirect3DRMFrame::GetSceneBackground()
{
  return m_pIDirect3DRMFrame->GetSceneBackground();
}

HRESULT CADirect3DRMFrame::GetSceneBackgroundDepth(
      LPDIRECTDRAWSURFACE * lplpDDSurface)
{
  return Check( m_pIDirect3DRMFrame->GetSceneBackgroundDepth( lplpDDSurface) );
}

D3DCOLOR CADirect3DRMFrame::GetSceneFogColor()
{
  return m_pIDirect3DRMFrame->GetSceneFogColor();
}

BOOL CADirect3DRMFrame::GetSceneFogEnable()
{
  return m_pIDirect3DRMFrame->GetSceneFogEnable();
}

D3DRMFOGMODE CADirect3DRMFrame::GetSceneFogMode()
{
  return m_pIDirect3DRMFrame->GetSceneFogMode();
}

HRESULT CADirect3DRMFrame::GetSceneFogParams(D3DVALUE * lprvStart, D3DVALUE * lprvEnd,
      D3DVALUE * lprvDensity)
{
  return Check( m_pIDirect3DRMFrame->GetSceneFogParams( lprvStart, lprvEnd, lprvDensity) );
}

D3DRMSORTMODE CADirect3DRMFrame::GetSortMode()
{
  return m_pIDirect3DRMFrame->GetSortMode();
}

HRESULT CADirect3DRMFrame::GetTexture(LPDIRECT3DRMTEXTURE* lplpTexture)
{
  return Check( m_pIDirect3DRMFrame->GetTexture( lplpTexture) );
}

HRESULT CADirect3DRMFrame::GetTextureTopology(BOOL * lpbWrap_u, BOOL * lpbWrap_v)
{
  return Check( m_pIDirect3DRMFrame->GetTextureTopology( lpbWrap_u, lpbWrap_v) );
}

HRESULT CADirect3DRMFrame::GetTransform(D3DRMMATRIX4D rmMatrix)
{
  return Check( m_pIDirect3DRMFrame->GetTransform( rmMatrix) );
}

HRESULT CADirect3DRMFrame::GetVelocity(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvVel,
      BOOL fRotVel)
{
  return Check( m_pIDirect3DRMFrame->GetVelocity( lpRef, lprvVel, fRotVel) );
}

HRESULT CADirect3DRMFrame::GetVisuals(LPDIRECT3DRMVISUALARRAY* lplpVisuals)
{
  return Check( m_pIDirect3DRMFrame->GetVisuals( lplpVisuals) );
}

D3DRMZBUFFERMODE CADirect3DRMFrame::GetZbufferMode()
{
  return m_pIDirect3DRMFrame->GetZbufferMode();
}

HRESULT CADirect3DRMFrame::InverseTransform(D3DVECTOR *lprvDst, D3DVECTOR *lprvSrc)
{
  return Check( m_pIDirect3DRMFrame->InverseTransform( lprvDst, lprvSrc) );
}

HRESULT CADirect3DRMFrame::Load(LPVOID lpvObjSource, LPVOID lpvObjID,
      D3DRMLOADOPTIONS d3drmLOFlags,
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpArgLTP)
{
  return Check( m_pIDirect3DRMFrame->Load( lpvObjSource,  lpvObjID,
      d3drmLOFlags, d3drmLoadTextureProc, lpArgLTP) );
}

HRESULT CADirect3DRMFrame::LookAt(LPDIRECT3DRMFRAME lpTarget, LPDIRECT3DRMFRAME lpRef,
      D3DRMFRAMECONSTRAINT rfcConstraint)
{
  return Check( m_pIDirect3DRMFrame->LookAt( lpTarget, lpRef, rfcConstraint) );
}

HRESULT CADirect3DRMFrame::Move(D3DVALUE delta)
{
  return Check( m_pIDirect3DRMFrame->Move( delta) );
}

HRESULT CADirect3DRMFrame::SetColor(D3DCOLOR rcColor)
{
  return Check( m_pIDirect3DRMFrame->SetColor( rcColor) );
}

HRESULT CADirect3DRMFrame::SetColorRGB(D3DVALUE rvRed, D3DVALUE rvGreen,
      D3DVALUE rvBlue)
{
  return Check( m_pIDirect3DRMFrame->SetColorRGB( rvRed, rvGreen, rvBlue) );
}

HRESULT CADirect3DRMFrame::SetMaterialMode(D3DRMMATERIALMODE rmmMode)
{
  return Check( m_pIDirect3DRMFrame->SetMaterialMode( rmmMode) );
}

HRESULT CADirect3DRMFrame::SetOrientation(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvDx,
      D3DVALUE rvDy, D3DVALUE rvDz, D3DVALUE rvUx, D3DVALUE rvUy,
      D3DVALUE rvUz)
{
  return Check( m_pIDirect3DRMFrame->SetOrientation( lpRef, rvDx,
      rvDy, rvDz, rvUx, rvUy, rvUz) );
}

HRESULT CADirect3DRMFrame::SetPosition(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ)
{
  return Check( m_pIDirect3DRMFrame->SetPosition( lpRef, rvX, rvY, rvZ) );
}

HRESULT CADirect3DRMFrame::SetRotation(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ, D3DVALUE rvTheta)
{
  return Check( m_pIDirect3DRMFrame->SetRotation( lpRef, rvX, rvY, rvZ, rvTheta) );
}

HRESULT CADirect3DRMFrame::SetSceneBackground(D3DCOLOR rcColor)
{
  return Check( m_pIDirect3DRMFrame->SetSceneBackground( rcColor) );
}

HRESULT CADirect3DRMFrame::SetSceneBackgroundDepth(LPDIRECTDRAWSURFACE lpImage)
{
  return Check( m_pIDirect3DRMFrame->SetSceneBackgroundDepth( lpImage) );
}

HRESULT CADirect3DRMFrame::SetSceneBackgroundImage(LPDIRECT3DRMTEXTURE lpTexture)
{
  return Check( m_pIDirect3DRMFrame->SetSceneBackgroundImage( lpTexture) );
}

HRESULT CADirect3DRMFrame::SetSceneBackgroundRGB(D3DVALUE rvRed, D3DVALUE rvGreen,
      D3DVALUE rvBlue)
{
  return Check( m_pIDirect3DRMFrame->SetSceneBackgroundRGB( rvRed, rvGreen, rvBlue) );
}

HRESULT CADirect3DRMFrame::SetSceneFogColor(D3DCOLOR rcColor)
{
  return Check( m_pIDirect3DRMFrame->SetSceneFogColor( rcColor) );
}

HRESULT CADirect3DRMFrame::SetSceneFogEnable(BOOL bEnable)
{
  return Check( m_pIDirect3DRMFrame->SetSceneFogEnable( bEnable) );
}

HRESULT CADirect3DRMFrame::SetSceneFogMode(D3DRMFOGMODE rfMode)
{
  return Check( m_pIDirect3DRMFrame->SetSceneFogMode( rfMode) );
}

HRESULT CADirect3DRMFrame::SetSceneFogParams(D3DVALUE rvStart, D3DVALUE rvEnd,
      D3DVALUE rvDensity)
{
  return Check( m_pIDirect3DRMFrame->SetSceneFogParams( rvStart, rvEnd, rvDensity) );
}

HRESULT CADirect3DRMFrame::SetSortMode(D3DRMSORTMODE d3drmSM)
{
  return Check( m_pIDirect3DRMFrame->SetSortMode( d3drmSM) );
}

HRESULT CADirect3DRMFrame::SetTexture(LPDIRECT3DRMTEXTURE lpD3DRMTexture)
{
  return Check( m_pIDirect3DRMFrame->SetTexture( lpD3DRMTexture) );
}

HRESULT CADirect3DRMFrame::SetTextureTopology(BOOL bWrap_u, BOOL bWrap_v)
{
  return Check( m_pIDirect3DRMFrame->SetTextureTopology( bWrap_u, bWrap_v) );
}

HRESULT CADirect3DRMFrame::SetVelocity(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, 
      D3DVALUE rvY, D3DVALUE rvZ, BOOL fRotVel)
{
  return Check( m_pIDirect3DRMFrame->SetVelocity( lpRef, rvX, rvY, rvZ, fRotVel) );
}

HRESULT CADirect3DRMFrame::SetZbufferMode(D3DRMZBUFFERMODE d3drmZBM)
{
  return Check( m_pIDirect3DRMFrame->SetZbufferMode( d3drmZBM) );
}

HRESULT CADirect3DRMFrame::Transform(D3DVECTOR *lpd3dVDst, D3DVECTOR *lpd3dVSrc)
{
  return Check( m_pIDirect3DRMFrame->Transform( lpd3dVDst, lpd3dVSrc) );
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMLight::CADirect3DRMLight( CADirect3DRM *pD3D,
   D3DRMLIGHTTYPE d3drmltLightType, D3DCOLOR cColor )
  : CADirect3DRMObject()
{
  pD3D->CreateLight( d3drmltLightType, cColor, &m_pIDirect3DRMLight );

  SetBaseInterface();

  m_pFrame = new CADirect3DRMFrame( pD3D, NULL );
  m_pFrame->AddLight( I() );
}

CADirect3DRMLight::~CADirect3DRMLight()
{
  delete m_pFrame;
  if (m_pIDirect3DRMLight) m_pIDirect3DRMLight->Release();
}

void CADirect3DRMLight::SetBaseInterface()
{
  Check( m_pIDirect3DRMLight->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



D3DCOLOR CADirect3DRMLight::GetColor()
{
  return m_pIDirect3DRMLight->GetColor();
}

D3DVALUE CADirect3DRMLight::GetConstantAttenuation()
{
  return m_pIDirect3DRMLight->GetConstantAttenuation();
}

HRESULT CADirect3DRMLight::GetEnableFrame(LPDIRECT3DRMFRAME * lplpEnableFrame)
{
  return Check( m_pIDirect3DRMLight->GetEnableFrame( lplpEnableFrame) );
}

D3DVALUE CADirect3DRMLight::GetLinearAttenuation()
{
  return m_pIDirect3DRMLight->GetLinearAttenuation();
}

D3DVALUE CADirect3DRMLight::GetPenumbra()
{
  return m_pIDirect3DRMLight->GetPenumbra();
}

D3DVALUE CADirect3DRMLight::GetQuadraticAttenuation()
{
  return m_pIDirect3DRMLight->GetQuadraticAttenuation();
}

D3DVALUE CADirect3DRMLight::GetRange()
{
  return m_pIDirect3DRMLight->GetRange();
}

D3DRMLIGHTTYPE CADirect3DRMLight::GetType()
{
  return m_pIDirect3DRMLight->GetType();
}

D3DVALUE CADirect3DRMLight::GetUmbra()
{
  return m_pIDirect3DRMLight->GetUmbra();
}

HRESULT CADirect3DRMLight::SetColor(D3DCOLOR rcColor)
{
  return Check( m_pIDirect3DRMLight->SetColor( rcColor) );
}

HRESULT CADirect3DRMLight::SetColorRGB(D3DVALUE rvRed, D3DVALUE rvGreen,
      D3DVALUE rvBlue)
{
  return Check( m_pIDirect3DRMLight->SetColorRGB( rvRed, rvGreen, rvBlue) );
}

HRESULT CADirect3DRMLight::SetConstantAttenuation(D3DVALUE rvAtt)
{
  return Check( m_pIDirect3DRMLight->SetConstantAttenuation( rvAtt) );
}

HRESULT CADirect3DRMLight::SetEnableFrame(LPDIRECT3DRMFRAME lpEnableFrame)
{
  return Check( m_pIDirect3DRMLight->SetEnableFrame( lpEnableFrame) );
}

HRESULT CADirect3DRMLight::SetLinearAttenuation(D3DVALUE rvAtt)
{
  return Check( m_pIDirect3DRMLight->SetLinearAttenuation( rvAtt) );
}

HRESULT CADirect3DRMLight::SetPenumbra(D3DVALUE rvAngle)
{
  return Check( m_pIDirect3DRMLight->SetPenumbra( rvAngle) );
}

HRESULT CADirect3DRMLight::SetQuadraticAttenuation(D3DVALUE rvAtt)
{
  return Check( m_pIDirect3DRMLight->SetQuadraticAttenuation( rvAtt) );
}

HRESULT CADirect3DRMLight::SetRange(D3DVALUE rvRange)
{
  return Check( m_pIDirect3DRMLight->SetRange( rvRange) );
}

HRESULT CADirect3DRMLight::SetType(D3DRMLIGHTTYPE d3drmtType)
{
  return Check( m_pIDirect3DRMLight->SetType( d3drmtType) );
}

HRESULT CADirect3DRMLight::SetUmbra(D3DVALUE rvAngle)
{
  return Check( m_pIDirect3DRMLight->SetUmbra( rvAngle) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMMaterial::CADirect3DRMMaterial( CADirect3DRM *pD3D, D3DVALUE vPower)
  : CADirect3DRMObject()
{
  pD3D->CreateMaterial( vPower, &m_pIDirect3DRMMaterial );

  SetBaseInterface();
}

CADirect3DRMMaterial::~CADirect3DRMMaterial()
{
  if (m_pIDirect3DRMMaterial) m_pIDirect3DRMMaterial->Release();
}

void CADirect3DRMMaterial::SetBaseInterface()
{
  Check( m_pIDirect3DRMMaterial->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMMaterial::GetEmissive(D3DVALUE *lpr, D3DVALUE *lpg, D3DVALUE *lpb)
{
  return Check( m_pIDirect3DRMMaterial->GetEmissive(lpr, lpg, lpb) );
}

D3DVALUE CADirect3DRMMaterial::GetPower()
{
  return m_pIDirect3DRMMaterial->GetPower();
}

HRESULT CADirect3DRMMaterial::GetSpecular(D3DVALUE *lpr, D3DVALUE *lpg, D3DVALUE *lpb)
{
  return Check( m_pIDirect3DRMMaterial->GetSpecular( lpr, lpg, lpb) );
}

HRESULT CADirect3DRMMaterial::SetEmissive(D3DVALUE r, D3DVALUE g, D3DVALUE b)
{
  return Check( m_pIDirect3DRMMaterial->SetEmissive( r, g, b) );
}

HRESULT CADirect3DRMMaterial::SetPower(D3DVALUE rvPower)
{
  return Check( m_pIDirect3DRMMaterial->SetPower( rvPower) );
}

HRESULT CADirect3DRMMaterial::SetSpecular(D3DVALUE r, D3DVALUE g, D3DVALUE b)
{
  return Check( m_pIDirect3DRMMaterial->SetSpecular( r, g, b) );
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMMesh::CADirect3DRMMesh( CADirect3DRM *pD3D )
  : CADirect3DRMObject()
{
  pD3D->CreateMesh( &m_pIDirect3DRMMesh );

  SetBaseInterface();
}

CADirect3DRMMesh::~CADirect3DRMMesh()
{
  if (m_pIDirect3DRMMesh) m_pIDirect3DRMMesh->Release();
}

void CADirect3DRMMesh::SetBaseInterface()
{
  Check( m_pIDirect3DRMMesh->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMMesh::AddGroup(unsigned vCount, unsigned fCount, 
      unsigned vPerFace, unsigned *fData, D3DRMGROUPINDEX *returnId)
{
  return Check( m_pIDirect3DRMMesh->AddGroup( vCount,  fCount, 
      vPerFace, fData, returnId) );
}

HRESULT CADirect3DRMMesh::GetBox(D3DRMBOX * lpD3DRMBox)
{
  return Check( m_pIDirect3DRMMesh->GetBox( lpD3DRMBox) );
}

HRESULT CADirect3DRMMesh::GetGroup(D3DRMGROUPINDEX id, unsigned *vCount, 
      unsigned *fCount, unsigned *vPerFace, DWORD *fDataSize,
      unsigned *fData)
{
  return Check( m_pIDirect3DRMMesh->GetGroup( id, vCount, fCount, vPerFace, fDataSize, fData) );
}

D3DCOLOR CADirect3DRMMesh::GetGroupColor(D3DRMGROUPINDEX id)
{
  return m_pIDirect3DRMMesh->GetGroupColor( id);
}

unsigned CADirect3DRMMesh::GetGroupCount()
{
  return m_pIDirect3DRMMesh->GetGroupCount();
}

D3DRMMAPPING CADirect3DRMMesh::GetGroupMapping(D3DRMGROUPINDEX id)
{
  return m_pIDirect3DRMMesh->GetGroupMapping( id);
}

HRESULT CADirect3DRMMesh::GetGroupMaterial(D3DRMGROUPINDEX id, 
      LPDIRECT3DRMMATERIAL *returnPtr)
{
  return Check( m_pIDirect3DRMMesh->GetGroupMaterial( id, returnPtr) );
}

D3DRMRENDERQUALITY CADirect3DRMMesh::GetGroupQuality(D3DRMGROUPINDEX id)
{
  return m_pIDirect3DRMMesh->GetGroupQuality( id);
}

HRESULT CADirect3DRMMesh::GetGroupTexture(D3DRMGROUPINDEX id, 
      LPDIRECT3DRMTEXTURE *returnPtr)
{
  return m_pIDirect3DRMMesh->GetGroupTexture( id, returnPtr);
}

HRESULT CADirect3DRMMesh::GetVertices(D3DRMGROUPINDEX id, DWORD index, 
      DWORD count, D3DRMVERTEX *returnPtr)
{
  return Check( m_pIDirect3DRMMesh->GetVertices( id, index, count, returnPtr) );
}

HRESULT CADirect3DRMMesh::Scale(D3DVALUE sx, D3DVALUE sy, D3DVALUE sz)
{
  return Check( m_pIDirect3DRMMesh->Scale( sx, sy, sz) );
}

HRESULT CADirect3DRMMesh::SetGroupColor(D3DRMGROUPINDEX id, D3DCOLOR value)
{
  return Check( m_pIDirect3DRMMesh->SetGroupColor( id, value) );
}

HRESULT CADirect3DRMMesh::SetGroupColorRGB(D3DRMGROUPINDEX id, D3DVALUE red, 
      D3DVALUE green, D3DVALUE blue)
{
  return Check( m_pIDirect3DRMMesh->SetGroupColorRGB( id, red, green, blue) );
}

HRESULT CADirect3DRMMesh::SetGroupMapping(D3DRMGROUPINDEX id, D3DRMMAPPING value)
{
  return Check( m_pIDirect3DRMMesh->SetGroupMapping( id, value) );
}

HRESULT CADirect3DRMMesh::SetGroupMaterial(D3DRMGROUPINDEX id, LPDIRECT3DRMMATERIAL value)
{
  return Check( m_pIDirect3DRMMesh->SetGroupMaterial( id, value) );
}

HRESULT CADirect3DRMMesh::SetGroupQuality(D3DRMGROUPINDEX id, D3DRMRENDERQUALITY value)
{
  return Check( m_pIDirect3DRMMesh->SetGroupQuality( id, value) );
}

HRESULT CADirect3DRMMesh::SetGroupTexture(D3DRMGROUPINDEX id, LPDIRECT3DRMTEXTURE value)
{
  return Check( m_pIDirect3DRMMesh->SetGroupTexture( id, value) );
}

HRESULT CADirect3DRMMesh::SetVertices(D3DRMGROUPINDEX id, unsigned index, 
      unsigned count, D3DRMVERTEX *values)
{
  return Check( m_pIDirect3DRMMesh->SetVertices( id, index, count, values) );
}

HRESULT CADirect3DRMMesh::Translate(D3DVALUE tx, D3DVALUE ty, D3DVALUE tz)
{
  return Check( m_pIDirect3DRMMesh->Translate( tx, ty, tz) );
}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMMeshBuilder::CADirect3DRMMeshBuilder( CADirect3DRM *pD3D )
  : CADirect3DRMObject()
{
  pD3D->CreateMeshBuilder( &m_pIDirect3DRMMeshBuilder );

  SetBaseInterface();
}

CADirect3DRMMeshBuilder::~CADirect3DRMMeshBuilder()
{
  if (m_pIDirect3DRMMeshBuilder) m_pIDirect3DRMMeshBuilder->Release();
}

void CADirect3DRMMeshBuilder::SetBaseInterface()
{
  Check( m_pIDirect3DRMMeshBuilder->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMMeshBuilder::AddFace(LPDIRECT3DRMFACE lpD3DRMFace)
{
  return Check( m_pIDirect3DRMMeshBuilder->AddFace( lpD3DRMFace) );
}

HRESULT CADirect3DRMMeshBuilder::AddFaces(DWORD dwVertexCount, D3DVECTOR * lpD3DVertices,
      DWORD normalCount, D3DVECTOR *lpNormals, DWORD *lpFaceData,
      LPDIRECT3DRMFACEARRAY* lplpD3DRMFaceArray)
{
  return Check( m_pIDirect3DRMMeshBuilder->AddFaces( dwVertexCount,  lpD3DVertices,
      normalCount, lpNormals, lpFaceData, lplpD3DRMFaceArray) );
}

HRESULT CADirect3DRMMeshBuilder::AddFrame(LPDIRECT3DRMFRAME lpD3DRMFrame)
{
  return Check( m_pIDirect3DRMMeshBuilder->AddFrame( lpD3DRMFrame) );
}

HRESULT CADirect3DRMMeshBuilder::AddMesh(LPDIRECT3DRMMESH lpD3DRMMesh)
{
  return Check( m_pIDirect3DRMMeshBuilder->AddMesh( lpD3DRMMesh) );
}

HRESULT CADirect3DRMMeshBuilder::AddMeshBuilder(LPDIRECT3DRMMESHBUILDER lpD3DRMMeshBuild)
{
  return Check( m_pIDirect3DRMMeshBuilder->AddMeshBuilder( lpD3DRMMeshBuild) );
}

int CADirect3DRMMeshBuilder::AddNormal(D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
  return m_pIDirect3DRMMeshBuilder->AddNormal( x, y, z);
}

int CADirect3DRMMeshBuilder::AddVertex(D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
  return m_pIDirect3DRMMeshBuilder->AddVertex( x, y, z);
}

HRESULT CADirect3DRMMeshBuilder::CreateFace(LPDIRECT3DRMFACE* lplpD3DRMFace)
{
  return Check( m_pIDirect3DRMMeshBuilder->CreateFace( lplpD3DRMFace) );
}

HRESULT CADirect3DRMMeshBuilder::CreateMesh(LPDIRECT3DRMMESH* lplpD3DRMMesh)
{
  return Check( m_pIDirect3DRMMeshBuilder->CreateMesh( lplpD3DRMMesh) );
}

HRESULT CADirect3DRMMeshBuilder::GenerateNormals()
{
  return Check( m_pIDirect3DRMMeshBuilder->GenerateNormals() );
}

HRESULT CADirect3DRMMeshBuilder::GetBox(D3DRMBOX *lpD3DRMBox)
{
  return Check( m_pIDirect3DRMMeshBuilder->GetBox( lpD3DRMBox) );
}

D3DRMCOLORSOURCE CADirect3DRMMeshBuilder::GetColorSource()
{
  return m_pIDirect3DRMMeshBuilder->GetColorSource();
}

int CADirect3DRMMeshBuilder::GetFaceCount()
{
  return m_pIDirect3DRMMeshBuilder->GetFaceCount();
}

HRESULT CADirect3DRMMeshBuilder::GetFaces(LPDIRECT3DRMFACEARRAY* lplpD3DRMFaceArray)
{
  return Check( m_pIDirect3DRMMeshBuilder->GetFaces( lplpD3DRMFaceArray) );
}

BOOL CADirect3DRMMeshBuilder::GetPerspective()
{
  return m_pIDirect3DRMMeshBuilder->GetPerspective();
}

D3DRMRENDERQUALITY CADirect3DRMMeshBuilder::GetQuality()
{
  return m_pIDirect3DRMMeshBuilder->GetQuality();
}

HRESULT CADirect3DRMMeshBuilder::GetTextureCoordinates(DWORD index, D3DVALUE *lpU,
      D3DVALUE *lpV)
{
  return Check( m_pIDirect3DRMMeshBuilder->GetTextureCoordinates( index, lpU, lpV) );
}

D3DCOLOR CADirect3DRMMeshBuilder::GetVertexColor(DWORD index)
{
  return m_pIDirect3DRMMeshBuilder->GetVertexColor( index);
}

int CADirect3DRMMeshBuilder::GetVertexCount()
{
  return m_pIDirect3DRMMeshBuilder->GetVertexCount();
}

HRESULT CADirect3DRMMeshBuilder::GetVertices(DWORD *vcount, D3DVECTOR *vertices, 
      DWORD *ncount, D3DVECTOR *normals, DWORD *face_data_size, 
      DWORD *face_data)
{
  return Check( m_pIDirect3DRMMeshBuilder->GetVertices(vcount, vertices, 
      ncount, normals, face_data_size, face_data) );
}

HRESULT CADirect3DRMMeshBuilder::Load(LPVOID lpvObjSource, LPVOID lpvObjID, 
      D3DRMLOADOPTIONS d3drmLOFlags,
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpvArg)
{
  return Check( m_pIDirect3DRMMeshBuilder->Load( lpvObjSource, lpvObjID, 
      d3drmLOFlags, d3drmLoadTextureProc, lpvArg) );
}

HRESULT CADirect3DRMMeshBuilder::ReserveSpace(DWORD vertexCount, DWORD normalCount,
      DWORD faceCount)
{
  return Check( m_pIDirect3DRMMeshBuilder->ReserveSpace( vertexCount, normalCount,
      faceCount) );
}

HRESULT CADirect3DRMMeshBuilder::Save(const char * lpFilename, 
      D3DRMXOFFORMAT d3drmXOFFormat, D3DRMSAVEOPTIONS d3drmSOContents)
{
  return Check( m_pIDirect3DRMMeshBuilder->Save( lpFilename, 
      d3drmXOFFormat, d3drmSOContents) );
}

HRESULT CADirect3DRMMeshBuilder::Scale(D3DVALUE sx, D3DVALUE sy, D3DVALUE sz)
{
  return Check( m_pIDirect3DRMMeshBuilder->Scale( sx, sy, sz) );
}

HRESULT CADirect3DRMMeshBuilder::SetColor(D3DCOLOR color)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetColor( color) );
}

HRESULT CADirect3DRMMeshBuilder::SetColorRGB(D3DVALUE red, D3DVALUE green, D3DVALUE blue)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetColorRGB( red, green, blue) );
}

HRESULT CADirect3DRMMeshBuilder::SetColorSource(D3DRMCOLORSOURCE source)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetColorSource( source) );
}

HRESULT CADirect3DRMMeshBuilder::SetMaterial(LPDIRECT3DRMMATERIAL lpIDirect3DRMmaterial)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetMaterial( lpIDirect3DRMmaterial) );
}

HRESULT CADirect3DRMMeshBuilder::SetNormal(DWORD index, D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetNormal( index, x, y, z) );
}

HRESULT CADirect3DRMMeshBuilder::SetPerspective(BOOL perspective)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetPerspective( perspective) );
}

HRESULT CADirect3DRMMeshBuilder::SetQuality(D3DRMRENDERQUALITY quality)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetQuality( quality) );
}

HRESULT CADirect3DRMMeshBuilder::SetTexture(LPDIRECT3DRMTEXTURE lpD3DRMTexture)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetTexture( lpD3DRMTexture) );
}

HRESULT CADirect3DRMMeshBuilder::SetTextureCoordinates(DWORD index, D3DVALUE u, D3DVALUE v)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetTextureCoordinates( index, u, v) );
}

HRESULT CADirect3DRMMeshBuilder::SetTextureTopology(BOOL cylU, BOOL cylV)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetTextureTopology( cylU, cylV) );
}

HRESULT CADirect3DRMMeshBuilder::SetVertex(DWORD index, D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetVertex( index, x, y, z) );
}

HRESULT CADirect3DRMMeshBuilder::SetVertexColor(DWORD index, D3DCOLOR color)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetVertexColor( index, color) );
}

HRESULT CADirect3DRMMeshBuilder::SetVertexColorRGB(DWORD index, D3DVALUE red, 
      D3DVALUE green, D3DVALUE blue)
{
  return Check( m_pIDirect3DRMMeshBuilder->SetVertexColorRGB( index, red, green, blue) );
}

HRESULT CADirect3DRMMeshBuilder::Translate(D3DVALUE tx, D3DVALUE ty, D3DVALUE tz)
{
  return Check( m_pIDirect3DRMMeshBuilder->Translate( tx, ty, tz) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMTexture::CADirect3DRMTexture( CADirect3DRM *pD3D, LPD3DRMIMAGE lpImage )
  : CADirect3DRMObject()
{
  pD3D->CreateTexture( lpImage, &m_pIDirect3DRMTexture );

  SetBaseInterface();
}

CADirect3DRMTexture::CADirect3DRMTexture( CADirect3DRM *pD3D, LPDIRECTDRAWSURFACE lpSurface )
  : CADirect3DRMObject()
{
  pD3D->CreateTextureFromSurface( lpSurface, &m_pIDirect3DRMTexture );

  SetBaseInterface();
}

CADirect3DRMTexture::CADirect3DRMTexture( CADirect3DRM *pD3D, const char *pFilename )
  : CADirect3DRMObject()
{
  pD3D->LoadTexture( pFilename, &m_pIDirect3DRMTexture );

  SetBaseInterface();
}

CADirect3DRMTexture::CADirect3DRMTexture( CADirect3DRM *pD3D, HRSRC rc )
  : CADirect3DRMObject()
{
  pD3D->LoadTextureFromResource( rc, &m_pIDirect3DRMTexture );

  SetBaseInterface();
}

CADirect3DRMTexture::~CADirect3DRMTexture()
{
  if (m_pIDirect3DRMTexture) m_pIDirect3DRMTexture->Release();
}

void CADirect3DRMTexture::SetBaseInterface()
{
  Check( m_pIDirect3DRMTexture->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMTexture::Changed(BOOL bPixels, BOOL bPalette)
{
  return Check( m_pIDirect3DRMTexture->Changed( bPixels, bPalette) );
}

DWORD CADirect3DRMTexture::GetColors()
{
  return m_pIDirect3DRMTexture->GetColors();
}

HRESULT CADirect3DRMTexture::GetDecalOrigin(LONG * lplX, LONG * lplY)
{
  return Check( m_pIDirect3DRMTexture->GetDecalOrigin( lplX, lplY) );
}

DWORD CADirect3DRMTexture::GetDecalScale()
{
  return m_pIDirect3DRMTexture->GetDecalScale();
}

HRESULT CADirect3DRMTexture::GetDecalSize(D3DVALUE *lprvWidth, D3DVALUE *lprvHeight)
{
  return Check( m_pIDirect3DRMTexture->GetDecalSize(lprvWidth, lprvHeight) );
}

BOOL CADirect3DRMTexture::GetDecalTransparency()
{
  return m_pIDirect3DRMTexture->GetDecalTransparency();
}

D3DCOLOR CADirect3DRMTexture::GetDecalTransparentColor()
{
  return m_pIDirect3DRMTexture->GetDecalTransparentColor();
}

D3DRMIMAGE *CADirect3DRMTexture::GetImage()
{
  return m_pIDirect3DRMTexture->GetImage();
}

DWORD CADirect3DRMTexture::GetShades()
{
  return m_pIDirect3DRMTexture->GetShades();
}

HRESULT CADirect3DRMTexture::InitFromFile(const char *filename)
{
  return Check( m_pIDirect3DRMTexture->InitFromFile(filename) );
}

HRESULT CADirect3DRMTexture::InitFromResource(HRSRC rs)
{
  return Check( m_pIDirect3DRMTexture->InitFromResource( rs) );
}

HRESULT CADirect3DRMTexture::InitFromSurface(LPDIRECTDRAWSURFACE lpDDS)
{
  return Check( m_pIDirect3DRMTexture->InitFromSurface( lpDDS) );
}

HRESULT CADirect3DRMTexture::SetColors(DWORD ulColors)
{
  return Check( m_pIDirect3DRMTexture->SetColors( ulColors) );
}

HRESULT CADirect3DRMTexture::SetDecalOrigin(LONG lX, LONG lY)
{
  return Check( m_pIDirect3DRMTexture->SetDecalOrigin( lX, lY) );
}

HRESULT CADirect3DRMTexture::SetDecalScale(DWORD dwScale)
{
  return Check( m_pIDirect3DRMTexture->SetDecalScale( dwScale) );
}

HRESULT CADirect3DRMTexture::SetDecalSize(D3DVALUE rvWidth, D3DVALUE rvHeight)
{
  return Check( m_pIDirect3DRMTexture->SetDecalSize( rvWidth, rvHeight) );
}

HRESULT CADirect3DRMTexture::SetDecalTransparency(BOOL bTransp)
{
  return Check( m_pIDirect3DRMTexture->SetDecalTransparency( bTransp) );
}

HRESULT CADirect3DRMTexture::SetDecalTransparentColor(D3DCOLOR rcTransp)
{
  return Check( m_pIDirect3DRMTexture->SetDecalTransparentColor( rcTransp) );
}

HRESULT CADirect3DRMTexture::SetShades(DWORD ulShades)
{
  return Check( m_pIDirect3DRMTexture->SetShades( ulShades) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMUserVisual::CADirect3DRMUserVisual( CADirect3DRM *pD3D, D3DRMUSERVISUALCALLBACK fn,
    LPVOID  lpArg )
  : CADirect3DRMObject()
{
  pD3D->CreateUserVisual( fn, lpArg, &m_pIDirect3DRMUserVisual );

  SetBaseInterface();
}

CADirect3DRMUserVisual::~CADirect3DRMUserVisual()
{
  if (m_pIDirect3DRMUserVisual) m_pIDirect3DRMUserVisual->Release();
}

void CADirect3DRMUserVisual::SetBaseInterface()
{
  Check( m_pIDirect3DRMUserVisual->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMUserVisual::Init(D3DRMUSERVISUALCALLBACK d3drmUVProc, void * lpArg)
{
  return Check( m_pIDirect3DRMUserVisual->Init( d3drmUVProc, lpArg) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMViewport::CADirect3DRMViewport( CADirect3DRM *pD3D, LPDIRECT3DRMDEVICE lpDev,
    LPDIRECT3DRMFRAME lpCamera, DWORD dwXPos,
    DWORD dwYPos, DWORD dwWidth, DWORD dwHeight)
  : CADirect3DRMObject()
{
  pD3D->CreateViewport( lpDev, lpCamera, dwXPos, dwYPos, dwWidth, dwHeight, &m_pIDirect3DRMViewport );

  SetBaseInterface();
}

CADirect3DRMViewport::CADirect3DRMViewport( LPDIRECT3DRMVIEWPORT pIViewport )
  : CADirect3DRMObject()
{
  m_pIDirect3DRMViewport = pIViewport;

  SetBaseInterface();
}

CADirect3DRMViewport::~CADirect3DRMViewport()
{
  if (m_pIDirect3DRMViewport) m_pIDirect3DRMViewport->Release();
}

void CADirect3DRMViewport::SetBaseInterface()
{
  Check( m_pIDirect3DRMViewport->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMViewport::Clear()
{
  return Check( m_pIDirect3DRMViewport->Clear() );
}

HRESULT CADirect3DRMViewport::Configure(LONG lX, LONG lY, DWORD dwWidth, DWORD dwHeight)
{
  return Check( m_pIDirect3DRMViewport->Configure( lX, lY, dwWidth, dwHeight) );
}

HRESULT CADirect3DRMViewport::ForceUpdate(DWORD dwX1, DWORD dwY1, DWORD dwX2,
      DWORD dwY2)
{
  return Check( m_pIDirect3DRMViewport->ForceUpdate( dwX1, dwY1, dwX2, dwY2) );
}

D3DVALUE CADirect3DRMViewport::GetBack()
{
  return m_pIDirect3DRMViewport->GetBack();
}

HRESULT CADirect3DRMViewport::GetCamera(LPDIRECT3DRMFRAME *lpCamera)
{
  return Check( m_pIDirect3DRMViewport->GetCamera(lpCamera) );
}

HRESULT CADirect3DRMViewport::GetDevice(LPDIRECT3DRMDEVICE *lpD3DRMDevice)
{
  return Check( m_pIDirect3DRMViewport->GetDevice(lpD3DRMDevice) );
}

HRESULT CADirect3DRMViewport::GetDirect3DViewport(LPDIRECT3DVIEWPORT * lplpD3DViewport)
{
  return Check( m_pIDirect3DRMViewport->GetDirect3DViewport( lplpD3DViewport) );
}

D3DVALUE CADirect3DRMViewport::GetField()
{
  return m_pIDirect3DRMViewport->GetField();
}

D3DVALUE CADirect3DRMViewport::GetFront()
{
  return m_pIDirect3DRMViewport->GetFront();
}

DWORD CADirect3DRMViewport::GetHeight()
{
  return m_pIDirect3DRMViewport->GetHeight();
}

HRESULT CADirect3DRMViewport::GetPlane(D3DVALUE *lpd3dvLeft, D3DVALUE *lpd3dvRight,
      D3DVALUE *lpd3dvBottom, D3DVALUE *lpd3dvTop)
{
  return Check( m_pIDirect3DRMViewport->GetPlane(lpd3dvLeft, lpd3dvRight,
      lpd3dvBottom, lpd3dvTop) );
}

D3DRMPROJECTIONTYPE CADirect3DRMViewport::GetProjection()
{
  return m_pIDirect3DRMViewport->GetProjection();
}

BOOL CADirect3DRMViewport::GetUniformScaling()
{
  return m_pIDirect3DRMViewport->GetUniformScaling();
}

DWORD CADirect3DRMViewport::GetWidth()
{
  return m_pIDirect3DRMViewport->GetWidth();
}

LONG CADirect3DRMViewport::GetX()
{
  return m_pIDirect3DRMViewport->GetX();
}

LONG CADirect3DRMViewport::GetY()
{
  return m_pIDirect3DRMViewport->GetY();
}

HRESULT CADirect3DRMViewport::Init(LPDIRECT3DRMDEVICE lpD3DRMDevice, 
      LPDIRECT3DRMFRAME lpD3DRMFrameCamera, DWORD xpos, DWORD ypos, 
      DWORD width, DWORD height)
{
  return Check( m_pIDirect3DRMViewport->Init( lpD3DRMDevice, 
      lpD3DRMFrameCamera, xpos, ypos, width, height) );
}

HRESULT CADirect3DRMViewport::InverseTransform(D3DVECTOR * lprvDst, D3DRMVECTOR4D * lprvSrc)
{
  return Check( m_pIDirect3DRMViewport->InverseTransform( lprvDst, lprvSrc) );
}

HRESULT CADirect3DRMViewport::Pick(LONG lX, LONG lY, 
      LPDIRECT3DRMPICKEDARRAY* lplpVisuals)
{
  return Check( m_pIDirect3DRMViewport->Pick( lX, lY, lplpVisuals) );
}

HRESULT CADirect3DRMViewport::Render(LPDIRECT3DRMFRAME lpD3DRMFrame)
{
  return Check( m_pIDirect3DRMViewport->Render( lpD3DRMFrame) );
}

HRESULT CADirect3DRMViewport::SetBack(D3DVALUE rvBack)
{
  return Check( m_pIDirect3DRMViewport->SetBack( rvBack) );
}

HRESULT CADirect3DRMViewport::SetCamera(LPDIRECT3DRMFRAME lpCamera)
{
  return Check( m_pIDirect3DRMViewport->SetCamera( lpCamera) );
}

HRESULT CADirect3DRMViewport::SetField(D3DVALUE rvField)
{
  return Check( m_pIDirect3DRMViewport->SetField( rvField) );
}

HRESULT CADirect3DRMViewport::SetFront(D3DVALUE rvFront)
{
  return Check( m_pIDirect3DRMViewport->SetFront( rvFront) );
}

HRESULT CADirect3DRMViewport::SetPlane(D3DVALUE rvLeft, D3DVALUE rvRight, D3DVALUE rvBottom,
      D3DVALUE rvTop)
{
  return Check( m_pIDirect3DRMViewport->SetPlane( rvLeft, rvRight, rvBottom, rvTop) );
}

HRESULT CADirect3DRMViewport::SetProjection(D3DRMPROJECTIONTYPE rptType)
{
  return Check( m_pIDirect3DRMViewport->SetProjection( rptType) );
}

HRESULT CADirect3DRMViewport::SetUniformScaling(BOOL bScale)
{
  return Check( m_pIDirect3DRMViewport->SetUniformScaling( bScale) );
}

HRESULT CADirect3DRMViewport::Transform(D3DRMVECTOR4D * lprvDst, D3DVECTOR * lprvSrc)
{
  return Check( m_pIDirect3DRMViewport->Transform( lprvDst, lprvSrc) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMWinDevice::CADirect3DRMWinDevice( CADirect3DRM *pD3D,
  CADirectDraw *pDD, LPDIRECTDRAWSURFACE lpDDSBack )
  : CADirect3DRMDevice( pD3D, pDD, lpDDSBack )
{
  Check( pD3D->I()->QueryInterface(
    IID_IDirect3DRMWinDevice, (void**)&m_pIDirect3DRMWinDevice ));

  SetBaseInterface();
}

CADirect3DRMWinDevice::~CADirect3DRMWinDevice()
{
  if (m_pIDirect3DRMWinDevice) m_pIDirect3DRMWinDevice->Release();
}

void CADirect3DRMWinDevice::SetBaseInterface()
{
  Check( m_pIDirect3DRMWinDevice->QueryInterface( IID_IDirect3DRMDevice, (void**)&m_pIDirect3DRMDevice ));
}


HRESULT CADirect3DRMWinDevice::HandleActivate(WORD wParam)
{
  return Check( m_pIDirect3DRMWinDevice->HandleActivate( wParam) );
}

HRESULT CADirect3DRMWinDevice::HandlePaint(HDC hDC)
{
  return Check( m_pIDirect3DRMWinDevice->HandlePaint( hDC) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirect3DRMWrap::CADirect3DRMWrap( CADirect3DRM *pD3D,
    D3DRMWRAPTYPE type, LPDIRECT3DRMFRAME lpRef, 
    D3DVALUE ox, D3DVALUE oy, D3DVALUE oz, D3DVALUE dx, D3DVALUE dy, 
    D3DVALUE dz, D3DVALUE ux, D3DVALUE uy, D3DVALUE uz, D3DVALUE ou, 
    D3DVALUE ov, D3DVALUE su, D3DVALUE sv)
  : CADirect3DRMObject()
{
  pD3D->CreateWrap( type, lpRef,
                    ox, oy, oz, dx, dy, 
                    dz, ux, uy, uz, ou, 
                    ov, su, sv, 
                    &m_pIDirect3DRMWrap );

  SetBaseInterface();
}

CADirect3DRMWrap::~CADirect3DRMWrap()
{
  if (m_pIDirect3DRMWrap) m_pIDirect3DRMWrap->Release();
}

void CADirect3DRMWrap::SetBaseInterface()
{
  Check( m_pIDirect3DRMWrap->QueryInterface( IID_IDirect3DRMObject, (void**)&m_pIDirect3DRMObject ));
}



HRESULT CADirect3DRMWrap::Apply(LPDIRECT3DRMOBJECT lpObject)
{
  return Check( m_pIDirect3DRMWrap->Apply( lpObject) );
}

HRESULT CADirect3DRMWrap::ApplyRelative(LPDIRECT3DRMFRAME frame,
      LPDIRECT3DRMOBJECT mesh)
{
  return Check( m_pIDirect3DRMWrap->ApplyRelative( frame, mesh) );
}

HRESULT CADirect3DRMWrap::Init(D3DRMWRAPTYPE d3drmwt, LPDIRECT3DRMFRAME lpd3drmfRef, 
      D3DVALUE ox, D3DVALUE oy, D3DVALUE oz, 
      D3DVALUE dx, D3DVALUE dy, D3DVALUE dz, 
      D3DVALUE ux, D3DVALUE uy, D3DVALUE uz, 
      D3DVALUE ou, D3DVALUE ov, D3DVALUE su, D3DVALUE sv)
{
  return Check( m_pIDirect3DRMWrap->Init( d3drmwt, lpd3drmfRef, 
      ox, oy, oz, 
      dx, dy, dz, 
      ux, uy, uz, 
      ou, ov, su, sv) );
}

