#ifndef __CAROCKSHAPE_H__
#define __CAROCKSHAPE_H__

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CARockShape;
typedef CTypedPtrList< CPtrList, CARockShape* > CARockShapeList;

class CARockShape : public CAShape
{
public:               
  CARockShape( CADirect3DRM *pD3D, LPDIRECT3DRMFRAME pRef );
  ~CARockShape();

  int m_iType;
  int m_nSize;

protected:

};

#define ROCK_SIZE_LARGE  200
#define ROCK_SIZE_MEDIUM 100
#define ROCK_SIZE_SMALL   50

#endif

