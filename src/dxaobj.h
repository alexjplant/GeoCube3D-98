#ifndef __DXAOBJ_H__
#define __DXAOBJ_H__

#include <windows.h>

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectXObject
{
public:               
  CADirectXObject();
  ~CADirectXObject();

  HRESULT GetResult() { return m_hr; }

protected:

  HRESULT Check( HRESULT hr );

  HRESULT m_hr;
};


#endif