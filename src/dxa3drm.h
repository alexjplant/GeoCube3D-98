#ifndef __DXA3DRM_H__
#define __DXA3DRM_H__

#include "dxaobj.h"

#include "d3drm.h"      // DirectX
#include "d3drmwin.h"

class CADirectDraw;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRM : public CADirectXObject
{
public:               
  CADirect3DRM();
  ~CADirect3DRM();

  IDirect3DRM *I() { return m_pIDirect3DRM; };

  HRESULT AddSearchPath(LPCSTR lpPath);

  HRESULT CreateAnimation(LPDIRECT3DRMANIMATION * lplpD3DRMAnimation);

  HRESULT CreateAnimationSet (LPDIRECT3DRMANIMATIONSET * lplpD3DRMAnimationSet);

  HRESULT CreateDevice(DWORD dwWidth, DWORD dwHeight,
      LPDIRECT3DRMDEVICE* lplpD3DRMDevice);

  HRESULT CreateDeviceFromClipper(LPDIRECTDRAWCLIPPER lpDDClipper, 
      LPGUID lpGUID, int width, int height, 
      LPDIRECT3DRMDEVICE * lplpD3DRMDevice);

  HRESULT CreateDeviceFromD3D(LPDIRECT3D lpD3D, 
      LPDIRECT3DDEVICE lpD3DDev, LPDIRECT3DRMDEVICE * lplpD3DRMDevice);

  HRESULT CreateDeviceFromSurface(LPGUID lpGUID, LPDIRECTDRAW lpDD,
      LPDIRECTDRAWSURFACE lpDDSBack,
      LPDIRECT3DRMDEVICE * lplpD3DRMDevice);

  HRESULT CreateFace(LPDIRECT3DRMFACE * lplpd3drmFace);

  HRESULT CreateFrame(LPDIRECT3DRMFRAME lpD3DRMFrame,
      LPDIRECT3DRMFRAME* lplpD3DRMFrame);

  HRESULT CreateLight(D3DRMLIGHTTYPE d3drmltLightType, 
      D3DCOLOR cColor, LPDIRECT3DRMLIGHT* lplpD3DRMLight);

  HRESULT CreateLightRGB(D3DRMLIGHTTYPE ltLightType, D3DVALUE vRed,
      D3DVALUE vGreen, D3DVALUE vBlue, LPDIRECT3DRMLIGHT* lplpD3DRMLight);

  HRESULT CreateMaterial(D3DVALUE vPower,
      LPDIRECT3DRMMATERIAL * lplpD3DRMMaterial);

  HRESULT CreateMesh(LPDIRECT3DRMMESH* lplpD3DRMMesh);

  HRESULT CreateMeshBuilder(LPDIRECT3DRMMESHBUILDER* lplpD3DRMMeshBuilder);

  HRESULT CreateObject(REFCLSID rclsid, LPUNKNOWN pUnkOuter, 
      REFIID riid, LPVOID FAR* ppv);

  HRESULT CreateShadow(LPDIRECT3DRMVISUAL lpVisual, 
      LPDIRECT3DRMLIGHT lpLight, D3DVALUE px, D3DVALUE py, D3DVALUE pz, 
      D3DVALUE nx, D3DVALUE ny, D3DVALUE nz, 
      LPDIRECT3DRMVISUAL * lplpShadow);

  HRESULT CreateTexture(LPD3DRMIMAGE lpImage,
      LPDIRECT3DRMTEXTURE* lplpD3DRMTexture);

  HRESULT CreateTextureFromSurface(LPDIRECTDRAWSURFACE lpDDS,
      LPDIRECT3DRMTEXTURE * lplpD3DRMTexture);

  HRESULT CreateUserVisual(D3DRMUSERVISUALCALLBACK fn,
      LPVOID  lpArg, LPDIRECT3DRMUSERVISUAL * lplpD3DRMUV);

  HRESULT CreateViewport(LPDIRECT3DRMDEVICE lpDev,
      LPDIRECT3DRMFRAME lpCamera, DWORD dwXPos,
      DWORD dwYPos, DWORD dwWidth, DWORD dwHeight,
      LPDIRECT3DRMVIEWPORT* lplpD3DRMViewport);

  HRESULT CreateWrap(D3DRMWRAPTYPE type, LPDIRECT3DRMFRAME lpRef, 
      D3DVALUE ox, D3DVALUE oy, D3DVALUE oz, D3DVALUE dx, D3DVALUE dy, 
      D3DVALUE dz, D3DVALUE ux, D3DVALUE uy, D3DVALUE uz, D3DVALUE ou, 
      D3DVALUE ov, D3DVALUE su, D3DVALUE sv, 
      LPDIRECT3DRMWRAP* lplpD3DRMWrap);

  HRESULT EnumerateObjects(D3DRMOBJECTCALLBACK func, LPVOID lpArg);

  HRESULT GetDevices(LPDIRECT3DRMDEVICEARRAY* lplpDevArray);

  HRESULT GetNamedObject(const char * lpName,
      LPDIRECT3DRMOBJECT* lplpD3DRMObject);

  HRESULT GetSearchPath(DWORD * lpdwSize, LPSTR lpszPath);

  HRESULT Load(LPVOID lpvObjSource, LPVOID lpvObjID,
      LPIID * lplpGUIDs, DWORD dwcGUIDs, D3DRMLOADOPTIONS d3drmLOFlags, 
      D3DRMLOADCALLBACK d3drmLoadProc, LPVOID lpArgLP, 
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpArgLTP, 
      LPDIRECT3DRMFRAME lpParentFrame);

  HRESULT LoadTexture(const char * lpFileName,
      LPDIRECT3DRMTEXTURE* lplpD3DRMTexture);

  HRESULT LoadTextureFromResource(HRSRC rs, 
      LPDIRECT3DRMTEXTURE * lplpD3DRMTexture);

  HRESULT SetDefaultTextureColors(DWORD dwColors);

  HRESULT SetDefaultTextureShades(DWORD dwShades);

  HRESULT SetSearchPath(LPCSTR lpPath);

  HRESULT Tick(D3DVALUE d3dvalTick);


protected:

  IDirect3DRM *m_pIDirect3DRM;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMObject : public CADirectXObject
{
public:               
  CADirect3DRMObject();
  ~CADirect3DRMObject();

  IDirect3DRMObject *I() { return m_pIDirect3DRMObject; };
  virtual void SetBaseInterface() = 0;

  HRESULT AddDestroyCallback(D3DRMOBJECTCALLBACK lpCallback,
      LPVOID lpArg);

  HRESULT Clone(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppvObj);

  HRESULT DeleteDestroyCallback(D3DRMOBJECTCALLBACK d3drmObjProc,
      LPVOID lpArg);

  DWORD GetAppData();

  HRESULT GetClassName(LPDWORD lpdwSize, LPSTR lpName);

  HRESULT GetName(LPDWORD lpdwSize, LPSTR lpName);

  HRESULT SetAppData(DWORD ulData);

  HRESULT SetName(const char * lpName);


protected:

  IDirect3DRMObject *m_pIDirect3DRMObject;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMAnimation : public CADirect3DRMObject
{
public:               
  CADirect3DRMAnimation( CADirect3DRM *pD3D );
  ~CADirect3DRMAnimation();

  IDirect3DRMAnimation *I() { return m_pIDirect3DRMAnimation; };
  void SetBaseInterface();

  HRESULT AddPositionKey(D3DVALUE rvTime, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ);

  HRESULT AddRotateKey(D3DVALUE rvTime, D3DRMQUATERNION *rqQuat);

  HRESULT AddScaleKey(D3DVALUE rvTime, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ);

  HRESULT DeleteKey(D3DVALUE rvTime);

  D3DRMANIMATIONOPTIONS GetOptions();

  HRESULT SetFrame(LPDIRECT3DRMFRAME lpD3DRMFrame);

  HRESULT SetOptions(D3DRMANIMATIONOPTIONS d3drmanimFlags);

  HRESULT SetTime(D3DVALUE rvTime);


protected:

  IDirect3DRMAnimation *m_pIDirect3DRMAnimation;
};




/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMAnimationSet : public CADirect3DRMObject
{
public:               
  CADirect3DRMAnimationSet( CADirect3DRM *pD3D );
  ~CADirect3DRMAnimationSet();

  IDirect3DRMAnimationSet *I() { return m_pIDirect3DRMAnimationSet; };
  void SetBaseInterface();

  HRESULT AddAnimation(LPDIRECT3DRMANIMATION lpD3DRMAnimation);

  HRESULT DeleteAnimation(LPDIRECT3DRMANIMATION lpD3DRMAnimation);

  HRESULT Load(LPVOID lpvObjSource, LPVOID lpvObjID, 
      D3DRMLOADOPTIONS d3drmLOFlags, 
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpArgLTP, 
      LPDIRECT3DRMFRAME lpParentFrame);

  HRESULT SetTime(D3DVALUE rvTime);


protected:

  IDirect3DRMAnimationSet *m_pIDirect3DRMAnimationSet;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMDevice : public CADirect3DRMObject
{
public:               
  CADirect3DRMDevice( CADirect3DRM *pD3D, CADirectDraw *pDD, LPDIRECTDRAWSURFACE lpDDSBack );
  CADirect3DRMDevice( LPDIRECT3DRMDEVICE pIDev );
  ~CADirect3DRMDevice();

  IDirect3DRMDevice *I() { return m_pIDirect3DRMDevice; };
  void SetBaseInterface();

  HRESULT AddUpdateCallback(D3DRMUPDATECALLBACK d3drmUpdateProc, LPVOID arg);

  HRESULT DeleteUpdateCallback(D3DRMUPDATECALLBACK d3drmUpdateProc,
      LPVOID arg);

  DWORD GetBufferCount(); 

  D3DCOLORMODEL GetColorModel();

  HRESULT GetDirect3DDevice(LPDIRECT3DDEVICE * lplpD3DDevice); 

  BOOL GetDither();

  DWORD GetHeight();

  DWORD GetTrianglesDrawn();

  D3DRMRENDERQUALITY GetQuality();

  DWORD GetShades();

  D3DRMTEXTUREQUALITY GetTextureQuality();

  HRESULT GetViewports(LPDIRECT3DRMVIEWPORTARRAY* lplpViewports);

  DWORD GetWidth();

  DWORD GetWireframeOptions();

  HRESULT Init(ULONG width, ULONG height);

  HRESULT InitFromClipper(LPDIRECTDRAWCLIPPER lpDDClipper, 
      LPGUID lpGUID, int width, int height);

  HRESULT InitFromD3D(LPDIRECT3D lpD3D, LPDIRECT3DDEVICE lpD3DIMDev);

  HRESULT SetBufferCount(DWORD dwCount);

  HRESULT SetDither(BOOL bDither);

  HRESULT SetQuality (D3DRMRENDERQUALITY rqQuality);

  HRESULT SetShades(DWORD ulShades);

  HRESULT SetTextureQuality(D3DRMTEXTUREQUALITY tqTextureQuality);

  HRESULT Update();


protected:

  IDirect3DRMDevice *m_pIDirect3DRMDevice;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMFace : public CADirect3DRMObject
{
public:               
  CADirect3DRMFace( CADirect3DRM *pD3D );
  ~CADirect3DRMFace();

  IDirect3DRMFace *I() { return m_pIDirect3DRMFace; };
  void SetBaseInterface();

  HRESULT AddVertex(D3DVALUE x, D3DVALUE y, D3DVALUE z);

  HRESULT AddVertexAndNormalIndexed(DWORD vertex, DWORD normal);

  D3DCOLOR GetColor();

  HRESULT GetMaterial(LPDIRECT3DRMMATERIAL* lplpMaterial);

  HRESULT GetNormal(D3DVECTOR *lpNormal);

  HRESULT GetTexture(LPDIRECT3DRMTEXTURE* lplpTexture);

  int GetTextureCoordinateIndex(DWORD dwIndex);

  HRESULT GetTextureCoordinates(DWORD index, D3DVALUE *lpU,
      D3DVALUE *lpV);

  HRESULT GetTextureTopology(BOOL *lpU, BOOL *lpV);

  HRESULT GetVertex(DWORD index, D3DVECTOR *lpPosition,
      D3DVECTOR *lpNormal);

  int GetVertexCount();

  int GetVertexIndex (DWORD dwIndex);

  HRESULT GetVertices(DWORD *lpdwVertexCount, D3DVECTOR *lpPosition,
      D3DVECTOR *lpNormal);

  HRESULT SetColor(D3DCOLOR color);

  HRESULT SetColorRGB(D3DVALUE red, D3DVALUE green, D3DVALUE blue);

  HRESULT SetMaterial(LPDIRECT3DRMMATERIAL lpD3DRMMaterial);

  HRESULT SetTexture(LPDIRECT3DRMTEXTURE lpD3DRMTexture);

  HRESULT SetTextureCoordinates(DWORD vertex, D3DVALUE u, D3DVALUE v);

  HRESULT SetTextureTopology(BOOL cylU, BOOL cylV);


protected:

  IDirect3DRMFace *m_pIDirect3DRMFace;
};



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMFrame : public CADirect3DRMObject
{
public:               
  CADirect3DRMFrame( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME lpD3DRMFrame );
  ~CADirect3DRMFrame();

  IDirect3DRMFrame *I() { return m_pIDirect3DRMFrame; };
  void SetBaseInterface();

  HRESULT AddChild(LPDIRECT3DRMFRAME lpD3DRMFrameChild);

  HRESULT AddLight(LPDIRECT3DRMLIGHT lpD3DRMLight);

  HRESULT AddMoveCallback(D3DRMFRAMEMOVECALLBACK d3drmFMC, VOID * lpArg);

  HRESULT AddRotation(D3DRMCOMBINETYPE rctCombine, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ, D3DVALUE rvTheta);

  HRESULT AddScale(D3DRMCOMBINETYPE rctCombine, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ);

  HRESULT AddTransform(D3DRMCOMBINETYPE rctCombine,
      D3DRMMATRIX4D rmMatrix);

  HRESULT AddTranslation(D3DRMCOMBINETYPE rctCombine, D3DVALUE rvX,
      D3DVALUE rvY, D3DVALUE rvZ);

  HRESULT AddVisual(LPDIRECT3DRMVISUAL lpD3DRMVisual);

  HRESULT DeleteChild(LPDIRECT3DRMFRAME lpChild);

  HRESULT DeleteLight(LPDIRECT3DRMLIGHT lpD3DRMLight);

  HRESULT DeleteMoveCallback(D3DRMFRAMEMOVECALLBACK d3drmFMC, 
      VOID * lpArg);

  HRESULT DeleteVisual(LPDIRECT3DRMVISUAL lpD3DRMVisual);

  HRESULT GetChildren(LPDIRECT3DRMFRAMEARRAY* lplpChildren);

  D3DCOLOR GetColor();

  HRESULT GetLights(LPDIRECT3DRMLIGHTARRAY* lplpLights);

  D3DRMMATERIALMODE GetMaterialMode();

  HRESULT GetOrientation(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvDir,
      LPD3DVECTOR lprvUp);

  HRESULT GetParent(LPDIRECT3DRMFRAME* lplpParent);

  HRESULT GetPosition(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvPos);

  HRESULT GetRotation(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvAxis,
      LPD3DVALUE lprvTheta);

  HRESULT GetScene(LPDIRECT3DRMFRAME* lplpRoot);

  D3DCOLOR GetSceneBackground();

  HRESULT GetSceneBackgroundDepth(
      LPDIRECTDRAWSURFACE * lplpDDSurface);

  D3DCOLOR GetSceneFogColor();

  BOOL GetSceneFogEnable();

  D3DRMFOGMODE GetSceneFogMode();

  HRESULT GetSceneFogParams(D3DVALUE * lprvStart, D3DVALUE * lprvEnd,
      D3DVALUE * lprvDensity);

  D3DRMSORTMODE GetSortMode();

  HRESULT GetTexture(LPDIRECT3DRMTEXTURE* lplpTexture);

  HRESULT GetTextureTopology(BOOL * lpbWrap_u, BOOL * lpbWrap_v);

  HRESULT GetTransform(D3DRMMATRIX4D rmMatrix);

  HRESULT GetVelocity(LPDIRECT3DRMFRAME lpRef, LPD3DVECTOR lprvVel,
      BOOL fRotVel);

  HRESULT GetVisuals(LPDIRECT3DRMVISUALARRAY* lplpVisuals);

  D3DRMZBUFFERMODE GetZbufferMode();

  HRESULT InverseTransform(D3DVECTOR *lprvDst, D3DVECTOR *lprvSrc);

  HRESULT Load(LPVOID lpvObjSource, LPVOID lpvObjID,
      D3DRMLOADOPTIONS d3drmLOFlags,
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpArgLTP);

  HRESULT LookAt(LPDIRECT3DRMFRAME lpTarget, LPDIRECT3DRMFRAME lpRef,
      D3DRMFRAMECONSTRAINT rfcConstraint);

  HRESULT Move(D3DVALUE delta);

  HRESULT SetColor(D3DCOLOR rcColor);

  HRESULT SetColorRGB(D3DVALUE rvRed, D3DVALUE rvGreen,
      D3DVALUE rvBlue);

  HRESULT SetMaterialMode(D3DRMMATERIALMODE rmmMode);

  HRESULT SetOrientation(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvDx,
      D3DVALUE rvDy, D3DVALUE rvDz, D3DVALUE rvUx, D3DVALUE rvUy,
      D3DVALUE rvUz);

  HRESULT SetPosition(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ);

  HRESULT SetRotation(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, D3DVALUE rvY,
      D3DVALUE rvZ, D3DVALUE rvTheta);

  HRESULT SetSceneBackground(D3DCOLOR rcColor);

  HRESULT SetSceneBackgroundDepth(LPDIRECTDRAWSURFACE lpImage);

  HRESULT SetSceneBackgroundImage(LPDIRECT3DRMTEXTURE lpTexture);

  HRESULT SetSceneBackgroundRGB(D3DVALUE rvRed, D3DVALUE rvGreen,
      D3DVALUE rvBlue);

  HRESULT SetSceneFogColor(D3DCOLOR rcColor);

  HRESULT SetSceneFogEnable(BOOL bEnable);

  HRESULT SetSceneFogMode(D3DRMFOGMODE rfMode);

  HRESULT SetSceneFogParams(D3DVALUE rvStart, D3DVALUE rvEnd,
      D3DVALUE rvDensity);

  HRESULT SetSortMode(D3DRMSORTMODE d3drmSM);

  HRESULT SetTexture(LPDIRECT3DRMTEXTURE lpD3DRMTexture);

  HRESULT SetTextureTopology(BOOL bWrap_u, BOOL bWrap_v);

  HRESULT SetVelocity(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, 
      D3DVALUE rvY, D3DVALUE rvZ, BOOL fRotVel);

  HRESULT SetZbufferMode(D3DRMZBUFFERMODE d3drmZBM);

  HRESULT Transform(D3DVECTOR *lpd3dVDst, D3DVECTOR *lpd3dVSrc);


protected:

  IDirect3DRMFrame *m_pIDirect3DRMFrame;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMLight : public CADirect3DRMObject
{
public:               
  CADirect3DRMLight(CADirect3DRM *pD3D, D3DRMLIGHTTYPE d3drmltLightType, D3DCOLOR cColor);
  ~CADirect3DRMLight();

  IDirect3DRMLight *I() { return m_pIDirect3DRMLight; };
  void SetBaseInterface();

  D3DCOLOR GetColor();

  D3DVALUE GetConstantAttenuation();

  HRESULT GetEnableFrame(LPDIRECT3DRMFRAME * lplpEnableFrame);

  D3DVALUE GetLinearAttenuation();

  D3DVALUE GetPenumbra();

  D3DVALUE GetQuadraticAttenuation();

  D3DVALUE GetRange();

  D3DRMLIGHTTYPE GetType();

  D3DVALUE GetUmbra();

  HRESULT SetColor(D3DCOLOR rcColor);

  HRESULT SetColorRGB(D3DVALUE rvRed, D3DVALUE rvGreen,
      D3DVALUE rvBlue);

  HRESULT SetConstantAttenuation(D3DVALUE rvAtt);

  HRESULT SetEnableFrame(LPDIRECT3DRMFRAME lpEnableFrame);

  HRESULT SetLinearAttenuation(D3DVALUE rvAtt);

  HRESULT SetPenumbra(D3DVALUE rvAngle);

  HRESULT SetQuadraticAttenuation(D3DVALUE rvAtt);

  HRESULT SetRange(D3DVALUE rvRange);

  HRESULT SetType(D3DRMLIGHTTYPE d3drmtType);

  HRESULT SetUmbra(D3DVALUE rvAngle);

  CADirect3DRMFrame *m_pFrame;

protected:

  IDirect3DRMLight *m_pIDirect3DRMLight;
};



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMMaterial : public CADirect3DRMObject
{
public:               
  CADirect3DRMMaterial( CADirect3DRM *pD3D, D3DVALUE vPower);
  ~CADirect3DRMMaterial();

  IDirect3DRMMaterial *I() { return m_pIDirect3DRMMaterial; };
  void SetBaseInterface();

  HRESULT GetEmissive(D3DVALUE *lpr, D3DVALUE *lpg, D3DVALUE *lpb);

  D3DVALUE GetPower();

  HRESULT GetSpecular(D3DVALUE *lpr, D3DVALUE *lpg, D3DVALUE *lpb);

  HRESULT SetEmissive(D3DVALUE r, D3DVALUE g, D3DVALUE b);

  HRESULT SetPower(D3DVALUE rvPower);

  HRESULT SetSpecular(D3DVALUE r, D3DVALUE g, D3DVALUE b);

protected:

  IDirect3DRMMaterial *m_pIDirect3DRMMaterial;
};



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMMesh : public CADirect3DRMObject
{
public:               
  CADirect3DRMMesh( CADirect3DRM *pD3D );
  ~CADirect3DRMMesh();

  IDirect3DRMMesh *I() { return m_pIDirect3DRMMesh; };
  void SetBaseInterface();

  HRESULT AddGroup(unsigned vCount, unsigned fCount, 
      unsigned vPerFace, unsigned *fData, D3DRMGROUPINDEX *returnId);

  HRESULT GetBox(D3DRMBOX * lpD3DRMBox);

  HRESULT GetGroup(D3DRMGROUPINDEX id, unsigned *vCount, 
      unsigned *fCount, unsigned *vPerFace, DWORD *fDataSize,
      unsigned *fData);

  D3DCOLOR GetGroupColor(D3DRMGROUPINDEX id);

  unsigned GetGroupCount();

  D3DRMMAPPING GetGroupMapping(D3DRMGROUPINDEX id); 

  HRESULT GetGroupMaterial(D3DRMGROUPINDEX id, 
      LPDIRECT3DRMMATERIAL *returnPtr); 

  D3DRMRENDERQUALITY GetGroupQuality(D3DRMGROUPINDEX id);

  HRESULT GetGroupTexture(D3DRMGROUPINDEX id, 
      LPDIRECT3DRMTEXTURE *returnPtr);

  HRESULT GetVertices(D3DRMGROUPINDEX id, DWORD index, 
      DWORD count, D3DRMVERTEX *returnPtr);

  HRESULT Scale(D3DVALUE sx, D3DVALUE sy, D3DVALUE sz);

  HRESULT SetGroupColor(D3DRMGROUPINDEX id, D3DCOLOR value);

  HRESULT SetGroupColorRGB(D3DRMGROUPINDEX id, D3DVALUE red, 
      D3DVALUE green, D3DVALUE blue);

  HRESULT SetGroupMapping(D3DRMGROUPINDEX id, D3DRMMAPPING value);

  HRESULT SetGroupMaterial(D3DRMGROUPINDEX id, LPDIRECT3DRMMATERIAL value);

  HRESULT SetGroupQuality(D3DRMGROUPINDEX id, D3DRMRENDERQUALITY value);

  HRESULT SetGroupTexture(D3DRMGROUPINDEX id, LPDIRECT3DRMTEXTURE value);

  HRESULT SetVertices(D3DRMGROUPINDEX id, unsigned index, 
      unsigned count, D3DRMVERTEX *values);

  HRESULT Translate(D3DVALUE tx, D3DVALUE ty, D3DVALUE tz);


protected:

  IDirect3DRMMesh *m_pIDirect3DRMMesh;
};




/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMMeshBuilder : public CADirect3DRMObject
{
public:               
  CADirect3DRMMeshBuilder( CADirect3DRM *pD3D );
  ~CADirect3DRMMeshBuilder();

  IDirect3DRMMeshBuilder *I() { return m_pIDirect3DRMMeshBuilder; };
  void SetBaseInterface();

  HRESULT AddFace(LPDIRECT3DRMFACE lpD3DRMFace);

  HRESULT AddFaces(DWORD dwVertexCount, D3DVECTOR * lpD3DVertices,
      DWORD normalCount, D3DVECTOR *lpNormals, DWORD *lpFaceData,
      LPDIRECT3DRMFACEARRAY* lplpD3DRMFaceArray);

  HRESULT AddFrame(LPDIRECT3DRMFRAME lpD3DRMFrame);

  HRESULT AddMesh(LPDIRECT3DRMMESH lpD3DRMMesh);

  HRESULT AddMeshBuilder(LPDIRECT3DRMMESHBUILDER lpD3DRMMeshBuild);

  int AddNormal(D3DVALUE x, D3DVALUE y, D3DVALUE z);

  int AddVertex(D3DVALUE x, D3DVALUE y, D3DVALUE z);

  HRESULT CreateFace(LPDIRECT3DRMFACE* lplpD3DRMFace);

  HRESULT CreateMesh(LPDIRECT3DRMMESH* lplpD3DRMMesh);

  HRESULT GenerateNormals();

  HRESULT GetBox(D3DRMBOX *lpD3DRMBox);

  D3DRMCOLORSOURCE GetColorSource();

  int GetFaceCount();

  HRESULT GetFaces(LPDIRECT3DRMFACEARRAY* lplpD3DRMFaceArray);

  BOOL GetPerspective();

  D3DRMRENDERQUALITY GetQuality();

  HRESULT GetTextureCoordinates(DWORD index, D3DVALUE *lpU,
      D3DVALUE *lpV);

  D3DCOLOR GetVertexColor(DWORD index);

  int GetVertexCount();

  HRESULT GetVertices(DWORD *vcount, D3DVECTOR *vertices, 
      DWORD *ncount, D3DVECTOR *normals, DWORD *face_data_size, 
      DWORD *face_data);

  HRESULT Load(LPVOID lpvObjSource, LPVOID lpvObjID, 
      D3DRMLOADOPTIONS d3drmLOFlags,
      D3DRMLOADTEXTURECALLBACK d3drmLoadTextureProc, LPVOID lpvArg);

  HRESULT ReserveSpace(DWORD vertexCount, DWORD normalCount,
      DWORD faceCount);

  HRESULT Save(const char * lpFilename, 
      D3DRMXOFFORMAT d3drmXOFFormat, D3DRMSAVEOPTIONS d3drmSOContents);

  HRESULT Scale(D3DVALUE sx, D3DVALUE sy, D3DVALUE sz);

  HRESULT SetColor(D3DCOLOR color);

  HRESULT SetColorRGB(D3DVALUE red, D3DVALUE green, D3DVALUE blue);

  HRESULT SetColorSource(D3DRMCOLORSOURCE source);

  HRESULT SetMaterial(LPDIRECT3DRMMATERIAL lpIDirect3DRMmaterial);

  HRESULT SetNormal(DWORD index, D3DVALUE x, D3DVALUE y, D3DVALUE z);

  HRESULT SetPerspective(BOOL perspective);

  HRESULT SetQuality(D3DRMRENDERQUALITY quality);

  HRESULT SetTexture(LPDIRECT3DRMTEXTURE lpD3DRMTexture);

  HRESULT SetTextureCoordinates(DWORD index, D3DVALUE u, D3DVALUE v);

  HRESULT SetTextureTopology(BOOL cylU, BOOL cylV);

  HRESULT SetVertex(DWORD index, D3DVALUE x, D3DVALUE y, D3DVALUE z);

  HRESULT SetVertexColor(DWORD index, D3DCOLOR color);

  HRESULT SetVertexColorRGB(DWORD index, D3DVALUE red, 
      D3DVALUE green, D3DVALUE blue);

  HRESULT Translate(D3DVALUE tx, D3DVALUE ty, D3DVALUE tz);


protected:

  IDirect3DRMMeshBuilder *m_pIDirect3DRMMeshBuilder;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMTexture : public CADirect3DRMObject
{
public:               
  CADirect3DRMTexture( CADirect3DRM *pD3D, LPDIRECTDRAWSURFACE lpSurface );
  CADirect3DRMTexture( CADirect3DRM *pD3D, LPD3DRMIMAGE lpImage );
  CADirect3DRMTexture( CADirect3DRM *pD3D, const char *pFilename );
  CADirect3DRMTexture( CADirect3DRM *pD3D, HRSRC rc );
  ~CADirect3DRMTexture();

  IDirect3DRMTexture *I() { return m_pIDirect3DRMTexture; };
  void SetBaseInterface();

  HRESULT Changed(BOOL bPixels, BOOL bPalette);

  DWORD GetColors();

  HRESULT GetDecalOrigin(LONG * lplX, LONG * lplY);

  DWORD GetDecalScale();

  HRESULT GetDecalSize(D3DVALUE *lprvWidth, D3DVALUE *lprvHeight);

  BOOL GetDecalTransparency();

  D3DCOLOR GetDecalTransparentColor();

  D3DRMIMAGE * GetImage();

  DWORD GetShades();

  HRESULT InitFromFile(const char *filename);

  HRESULT InitFromResource(HRSRC rs);

  HRESULT InitFromSurface(LPDIRECTDRAWSURFACE lpDDS);

  HRESULT SetColors(DWORD ulColors);

  HRESULT SetDecalOrigin(LONG lX, LONG lY);

  HRESULT SetDecalScale(DWORD dwScale);

  HRESULT SetDecalSize(D3DVALUE rvWidth, D3DVALUE rvHeight);

  HRESULT SetDecalTransparency(BOOL bTransp);

  HRESULT SetDecalTransparentColor(D3DCOLOR rcTransp);

  HRESULT SetShades(DWORD ulShades);


protected:

  IDirect3DRMTexture *m_pIDirect3DRMTexture;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMUserVisual : public CADirect3DRMObject
{
public:               
  CADirect3DRMUserVisual( CADirect3DRM *pD3D, D3DRMUSERVISUALCALLBACK fn, LPVOID  lpArg );
  ~CADirect3DRMUserVisual();

  IDirect3DRMUserVisual *I() { return m_pIDirect3DRMUserVisual; };
  void SetBaseInterface();

  HRESULT Init(D3DRMUSERVISUALCALLBACK d3drmUVProc, void * lpArg);


protected:

  IDirect3DRMUserVisual *m_pIDirect3DRMUserVisual;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMViewport : public CADirect3DRMObject
{
public:               
  CADirect3DRMViewport( CADirect3DRM *pD3D, LPDIRECT3DRMDEVICE lpDev,
    LPDIRECT3DRMFRAME lpCamera, DWORD dwXPos,
    DWORD dwYPos, DWORD dwWidth, DWORD dwHeight);
  CADirect3DRMViewport( LPDIRECT3DRMVIEWPORT pIViewport );
  ~CADirect3DRMViewport();

  IDirect3DRMViewport *I() { return m_pIDirect3DRMViewport; };
  void SetBaseInterface();

  HRESULT Clear();

  HRESULT Configure(LONG lX, LONG lY, DWORD dwWidth, DWORD dwHeight);

  HRESULT ForceUpdate(DWORD dwX1, DWORD dwY1, DWORD dwX2,
      DWORD dwY2);

  D3DVALUE GetBack();

  HRESULT GetCamera(LPDIRECT3DRMFRAME *lpCamera);

  HRESULT GetDevice(LPDIRECT3DRMDEVICE *lpD3DRMDevice);

  HRESULT GetDirect3DViewport(LPDIRECT3DVIEWPORT * lplpD3DViewport);

  D3DVALUE GetField();

  D3DVALUE GetFront();

  DWORD GetHeight();

  HRESULT GetPlane(D3DVALUE *lpd3dvLeft, D3DVALUE *lpd3dvRight,
      D3DVALUE *lpd3dvBottom, D3DVALUE *lpd3dvTop);

  D3DRMPROJECTIONTYPE GetProjection();

  BOOL GetUniformScaling();

  DWORD GetWidth();

  LONG GetX();

  LONG GetY();

  HRESULT Init(LPDIRECT3DRMDEVICE lpD3DRMDevice, 
      LPDIRECT3DRMFRAME lpD3DRMFrameCamera, DWORD xpos, DWORD ypos, 
      DWORD width, DWORD height);

  HRESULT InverseTransform(D3DVECTOR * lprvDst, D3DRMVECTOR4D * lprvSrc);

  HRESULT Pick(LONG lX, LONG lY, 
      LPDIRECT3DRMPICKEDARRAY* lplpVisuals);

  HRESULT Render(LPDIRECT3DRMFRAME lpD3DRMFrame);

  HRESULT SetBack(D3DVALUE rvBack);

  HRESULT SetCamera(LPDIRECT3DRMFRAME lpCamera);

  HRESULT SetField(D3DVALUE rvField);

  HRESULT SetFront(D3DVALUE rvFront);

  HRESULT SetPlane(D3DVALUE rvLeft, D3DVALUE rvRight, D3DVALUE rvBottom,
      D3DVALUE rvTop);

  HRESULT SetProjection(D3DRMPROJECTIONTYPE rptType);

  HRESULT SetUniformScaling(BOOL bScale);

  HRESULT Transform(D3DRMVECTOR4D * lprvDst, D3DVECTOR * lprvSrc);


protected:

  IDirect3DRMViewport *m_pIDirect3DRMViewport;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMWinDevice : public CADirect3DRMDevice
{
public:               
  CADirect3DRMWinDevice( CADirect3DRM *pD3D, CADirectDraw *pDD, LPDIRECTDRAWSURFACE lpDDSBack );
  ~CADirect3DRMWinDevice();

  IDirect3DRMWinDevice *I() { return m_pIDirect3DRMWinDevice; };
  void SetBaseInterface();

  HRESULT HandleActivate(WORD wParam);

  HRESULT HandlePaint(HDC hDC);


protected:

  IDirect3DRMWinDevice *m_pIDirect3DRMWinDevice;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirect3DRMWrap : public CADirect3DRMObject
{
public:               
  CADirect3DRMWrap( CADirect3DRM *pD3D,
    D3DRMWRAPTYPE type, LPDIRECT3DRMFRAME lpRef, 
    D3DVALUE ox, D3DVALUE oy, D3DVALUE oz, D3DVALUE dx, D3DVALUE dy, 
    D3DVALUE dz, D3DVALUE ux, D3DVALUE uy, D3DVALUE uz, D3DVALUE ou, 
    D3DVALUE ov, D3DVALUE su, D3DVALUE sv);
  ~CADirect3DRMWrap();

  IDirect3DRMWrap *I() { return m_pIDirect3DRMWrap; };
  void SetBaseInterface();

  HRESULT Apply(LPDIRECT3DRMOBJECT lpObject);

  HRESULT ApplyRelative(LPDIRECT3DRMFRAME frame,
      LPDIRECT3DRMOBJECT mesh);

  HRESULT Init(D3DRMWRAPTYPE d3drmwt, LPDIRECT3DRMFRAME lpd3drmfRef, 
      D3DVALUE ox, D3DVALUE oy, D3DVALUE oz, 
      D3DVALUE dx, D3DVALUE dy, D3DVALUE dz, 
      D3DVALUE ux, D3DVALUE uy, D3DVALUE uz, 
      D3DVALUE ou, D3DVALUE ov, D3DVALUE su, D3DVALUE sv);


protected:

  IDirect3DRMWrap *m_pIDirect3DRMWrap;
};



#endif