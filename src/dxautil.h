#ifndef __DXAUTIL_H__
#define __DXAUTIL_H__


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAVector	: public _D3DVECTOR
{
public:
	CAVector();
	virtual ~CAVector();
	CAVector(const D3DVECTOR& r);
	CAVector(D3DVALUE x, D3DVALUE y, D3DVALUE z);
  CAVector& operator = (const D3DVECTOR& r);
  friend CAVector operator + (const D3DVECTOR& a, const D3DVECTOR& b);
  CAVector& operator += (const D3DVECTOR& r);
  friend CAVector operator - (const D3DVECTOR& a, const D3DVECTOR& b);
  CAVector& operator -= (const D3DVECTOR& r);
  friend CAVector operator * (const D3DVECTOR& a, const D3DVECTOR& b);
  CAVector& operator *= (const D3DVECTOR& r);
  friend CAVector operator * (const D3DVECTOR& a, const D3DVALUE s);
  CAVector& operator *= (const D3DVALUE s);
  CAVector& operator + (const D3DVALUE t);
	CAVector operator - ();
  BOOL operator == (const D3DVECTOR& r);
  BOOL operator != (const D3DVECTOR& r);
	D3DVALUE Dot(const D3DVECTOR& a);
	D3DVALUE Mag();
	BOOL Normalize();
	BOOL Coincident(const D3DVECTOR& r);
	CAVector GenerateUp();
	BOOL IsNull();
  BOOL Reflect(const D3DVECTOR&rNorm);
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAMatrix
{
public:
  CAMatrix();
  virtual ~CAMatrix();
  CAMatrix(const CAMatrix& r);
  CAMatrix(D3DVALUE v00, D3DVALUE v01, D3DVALUE v02, D3DVALUE v03,
           D3DVALUE v10, D3DVALUE v11, D3DVALUE v12, D3DVALUE v13,
           D3DVALUE v20, D3DVALUE v21, D3DVALUE v22, D3DVALUE v23,
           D3DVALUE v30, D3DVALUE v31, D3DVALUE v32, D3DVALUE v33);
  CAMatrix& operator = (const CAMatrix& r);
  friend CAMatrix operator + (const CAMatrix& a, const CAMatrix& b);
  CAMatrix& operator += (const CAMatrix& r);
  friend CAMatrix operator * (const CAMatrix& a, const CAMatrix& b);
  CAMatrix& operator *= (const CAMatrix& r);
  friend D3DVECTOR operator * (const CAMatrix& m, const D3DVECTOR& v);
  void Rotate(D3DVALUE rx, D3DVALUE ry, D3DVALUE rz, BOOL bRadians = FALSE);
  void Translate(D3DVALUE dx, D3DVALUE dy, D3DVALUE dz);
	void Scale(D3DVALUE sx, D3DVALUE sy, D3DVALUE sz);
	void Scale(D3DVALUE s) {Scale(s, s, s);}
	void MakeUnit();
	void Initialize(D3DRMMATRIX4D& rlm);

	// elements
	D3DVALUE m_00, m_01, m_02, m_03;
	D3DVALUE m_10, m_11, m_12, m_13;
	D3DVALUE m_20, m_21, m_22, m_23;
	D3DVALUE m_30, m_31, m_32, m_33;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAControllerData
{
public:               
  CAControllerData();
  ~CAControllerData();

  void Reset();

  LONG m_dX;      
  LONG m_dY;      
  LONG m_dZ;      
  LONG m_dRudder; 
  LONG m_dHeadUpDown; 
  LONG m_dHeadTilt; 
  LONG m_iThrust; 
  BOOL m_bFire;
  BOOL m_bJump;
  CAKey m_aKeys[256];
};

class CAController
{
public:               
  CAController( CADirectInput *pInput );
  ~CAController();

  BOOL Update();
  BOOL Debounce();

  CAControllerData m_data;

  CEvent eKill;
  HANDLE ahEvents[3];

protected:
  CADirectInput *m_pInput;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAScene : public CADirect3DRMFrame
{
public:               
  CAScene( CADirect3DRM *pD3D );
  ~CAScene();

protected:

};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#define MAX_VELOCITY (100 * 1000 / 60)    // meters/second (100kph)           
#define BOUNDARY       1000
#define BOUNDARY_STEP  100

typedef D3DVALUE (* SURFHTFN)(D3DVALUE x, D3DVALUE z, void* pArg);
typedef D3DVALUE (* SOLIDRFN)(D3DVALUE z, void* pArg);

typedef struct
{
  DWORD size;
  CAVector vPos;
  CAVector vDir;
  CAVector vUp;
}
SAShapeData;

class CAShape;
typedef CTypedPtrList< CPtrList, CAShape* > CAShapeList;

class CAShape : public CADirect3DRMFrame
{
public:               
  CAShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef );
  ~CAShape();

	void New();

  HRESULT SetPosition(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvX, D3DVALUE rvY, D3DVALUE rvZ);
  HRESULT SetOrientation(LPDIRECT3DRMFRAME lpRef, D3DVALUE rvDx,
      D3DVALUE rvDy, D3DVALUE rvDz, D3DVALUE rvUx, D3DVALUE rvUy, D3DVALUE rvUz);

  BOOL Create(D3DVECTOR* pVectors, int iVectors,
				D3DVECTOR* pNormals, int iNormals,
				int* pFaceData, BOOL bAutoGen = FALSE);
  BOOL Create(D3DVECTOR* pVectors, int iVectors, int* pFaceData, BOOL bAutoGen = FALSE)
    {  return Create(pVectors, iVectors, NULL, 0, pFaceData, bAutoGen);  }
	BOOL Load(const char* pszFileName);
	BOOL Load(UINT uiResid);

  BOOL AddFaces(D3DVECTOR* pVectors, int iVectors,
      				  D3DVECTOR* pNormals, int iNormals,
			      	  int* pFaceData);
	BOOL CreateCuboid(D3DVALUE x, D3DVALUE y, D3DVALUE z);
  BOOL CreateCube(D3DVALUE side)
    {  return CreateCuboid(side, side, side);  }
	BOOL CreateSphere(D3DVALUE r, int nBands = 16);
	BOOL CreateRock(D3DVALUE r, int nBands = 16);
  BOOL CreateMesh(D3DVALUE x1, D3DVALUE x2, D3DVALUE dx,
                  D3DVALUE z1, D3DVALUE z2, D3DVALUE dz,
                  SURFHTFN pfnHeight, void* pArg);
	BOOL CreateRSolid(D3DVALUE z1, D3DVALUE z2, D3DVALUE dz,
					  BOOL bClosed1, BOOL bClosed2,
					  SOLIDRFN pfnRad, void* pArg,
					  int nFacets = 8);
	BOOL CreateCone(D3DVALUE x1, D3DVALUE y1, D3DVALUE z1,
					D3DVALUE r1, BOOL bClosed1,
					D3DVALUE x2, D3DVALUE y2, D3DVALUE z2,
					D3DVALUE r2, BOOL bClosed2,
					int nFacets = 8);
	BOOL CreateRod(D3DVALUE x1, D3DVALUE y1, D3DVALUE z1,
					D3DVALUE x2, D3DVALUE y2, D3DVALUE z2,
					D3DVALUE r, int nFacets = 8)
		{  return CreateCone(x1, y1, z1, r, TRUE, x2, y2, z2, r, TRUE, nFacets);  }

  void SetBoundary( D3DVECTOR vPos, D3DVALUE height, D3DVALUE radius );

  CAShape *Collide( D3DVECTOR vPos, CAShapeList *pBoundaryList );

  void ApplyGravity( CAControllerData &data );

  BOOL TestBorders( CAControllerData &data );

  void Tick() {};

  static BOOL SpheresIntersect( CAVector *pPosS1, D3DVALUE radiusS1, CAVector *pPosS2, D3DVALUE radiusS2 );

  CADirect3DRMMeshBuilder *m_pMeshBuilder;

  SAShapeData m_data;
  CAVector m_vVelocity;
  D3DVALUE m_mass;
  D3DVALUE m_boundaryHeight;
  D3DVALUE m_boundaryRadius;

protected:

};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADecal;
typedef CTypedPtrList< CPtrList, CADecal* > CADecalList;

class CADecal : public CAShape
{
public:               
  CADecal( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef, DWORD dwWidth = 256, DWORD dwHeight = 256 );
  ~CADecal();

  BOOL SetTextLabel( LPCTSTR pText );
  BOOL SetBitmap( LPCTSTR szBitmap );

  CADirectDrawSurface *m_pSurface;
  CADirect3DRMTexture *m_pTexture;

  DWORD m_dwWidth;
  DWORD m_dwHeight;
  CRect m_rText;
};

#endif
