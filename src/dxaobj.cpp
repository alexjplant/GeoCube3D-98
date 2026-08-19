#include <stdafx.h>
#include "dxaobj.h"
#include "dxerror.h"


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
CADirectXObject::CADirectXObject()
{
}

CADirectXObject::~CADirectXObject()
{
}

HRESULT CADirectXObject::Check( HRESULT hr )
{
  m_hr = hr;
  if (hr)
  {
    DxErrorHandler eh(hr);

    char szBuff[1024];
    szBuff[0] = '\0';
    eh.GetErrorText(szBuff);
    TRACE1("CADirectXObject::Check() - %s\n", szBuff);

    eh.HandleDxError( hr );

//    throw DxError(hr);

  }
  return hr;
}
