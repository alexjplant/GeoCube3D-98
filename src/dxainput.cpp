#include <stdafx.h>
#include "dxainput.h"

#define DINPUT_BUFFERSIZE  512

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectInput::CADirectInput(HINSTANCE hinst) : CADirectXObject()
{
  Check( DirectInputCreate( hinst, DIRECTINPUT_VERSION,
    &m_pIDirectInput, NULL) );

  m_pMouseDev = NULL;
  m_bMouse = FALSE;
  m_hMouse = 0;

  m_pKeyboardDev = NULL;
  m_bKeyboard = FALSE;
  m_hKeyboard = 0;

	memset(&m_joyInfo, 0, sizeof(m_joyInfo));
  m_bJoystick = FALSE;
}

CADirectInput::~CADirectInput()
{
  if (m_hMouse) CloseHandle( m_hMouse );
  delete m_pMouseDev;

  if (m_hKeyboard) CloseHandle( m_hKeyboard );
  delete m_pKeyboardDev;
}

HRESULT CADirectInput::CreateDevice(REFGUID rguid, 
      LPDIRECTINPUTDEVICE *lplpDirectInputDevice,
      LPUNKNOWN pUnkOuter)
{
  return Check( m_pIDirectInput->CreateDevice( rguid, 
      lplpDirectInputDevice,
      pUnkOuter) );
}

HRESULT CADirectInput::EnumDevices(DWORD dwDevType, 
      LPDIENUMDEVICESCALLBACK lpCallback, LPVOID pvRef,
      DWORD dwFlags )
{
  return Check( m_pIDirectInput->EnumDevices( dwDevType, 
       lpCallback,  pvRef,
       dwFlags ) );
}

HRESULT CADirectInput::GetDeviceStatus(REFGUID rguidInstance)
{
  return Check( m_pIDirectInput->GetDeviceStatus( rguidInstance ));
}

HRESULT CADirectInput::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
  return Check( m_pIDirectInput->Initialize( hinst,  dwVersion) );
}

HRESULT CADirectInput::RunControlPanel(HWND  hwndOwner, DWORD dwFlags)
{
  return Check( m_pIDirectInput->RunControlPanel(  hwndOwner,  dwFlags) );
}

/////////////// Helper members /////////////////
//
BOOL CADirectInput::mCreate(HWND hWnd)
{
  DIPROPDWORD dipdw =
  {
    { 
      sizeof(DIPROPDWORD),        // diph.dwSize
      sizeof(DIPROPHEADER),       // diph.dwHeaderSize
      0,                          // diph.dwObj
      DIPH_DEVICE,                // diph.dwHow
    },
    DINPUT_BUFFERSIZE,            // dwData
  };

	m_pMouseDev = new CADirectInputDevice(this, GUID_SysMouse);
	m_pMouseDev->SetDataFormat(&c_dfDIMouse);
	m_pMouseDev->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
  m_hMouse = CreateEvent( NULL, FALSE, FALSE, NULL );
  m_pMouseDev->SetEventNotification( m_hMouse );
  m_pMouseDev->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
  m_pMouseDev->I()->Acquire();
  memset( &m_MouseState, '\0', sizeof(m_MouseState) );
	m_bMouse = TRUE;

  m_pKeyboardDev = new CADirectInputDevice(this, GUID_SysKeyboard);
	m_pKeyboardDev->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboardDev->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
  m_hKeyboard = CreateEvent( NULL, FALSE, FALSE, NULL );
  m_pKeyboardDev->SetEventNotification( m_hKeyboard );
  m_pKeyboardDev->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
  m_pKeyboardDev->I()->Acquire();
  for (int i = 0; i < 256; i++)
    m_aKeys[i].Clear();

	m_bKeyboard = TRUE;

	m_bJoystick = joyGetNumDevs() > 0 ? TRUE : FALSE;

	memset(&m_joyInfo, 0, sizeof(m_joyInfo));
	m_joyInfo.dwSize = sizeof(m_joyInfo);
	m_joyInfo.dwFlags = JOY_RETURNALL;
	if (joyGetPosEx(0, &m_joyInfo) != JOYERR_NOERROR)
    m_bJoystick = FALSE;

	return TRUE;
}

void CADirectInput::mUpdate(void)
{
  DIDEVICEOBJECTDATA rgod[DINPUT_BUFFERSIZE]; /* Receives buffered data */
  DWORD cod;

  // get buffered input from the keyboard
TRY_KEYBOARD_AGAIN:;
  cod = DINPUT_BUFFERSIZE;
  m_hr = m_pKeyboardDev->I()->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
    rgod, &cod, 0);

  if (m_hr == DI_OK)
  {
    // get the buffered keys
    for (DWORD iod = 0; iod < cod; iod++)
    {
      DWORD off = rgod[iod].dwOfs;
      m_aKeys[ off ].dwRepeatTime = 0;

      if (rgod[iod].dwData & 0x80)
      {
        m_aKeys[ off ].iState = 1;   // down
        m_aKeys[ off ].dwTime = rgod[iod].dwTimeStamp;
        m_aKeys[ off ].bRepeat = TRUE;
      }
      else
      {
        m_aKeys[ off ].iState = 2;  // up
        m_aKeys[ off ].dwTime =
          rgod[iod].dwTimeStamp - m_aKeys[ off ].dwTime;
        m_aKeys[ off ].bRepeat = FALSE;
      }
    }
  }
  else  //  We got an error or we got DI_BUFFEROVERFLOW.
  {
    for (int i = 0; i < 256; i++)
      m_aKeys[i].Clear();

    if ((m_hr == DIERR_INPUTLOST) || (m_hr == DIERR_NOTACQUIRED))
    {
      m_hr = m_pKeyboardDev->I()->Acquire();
                
      if (SUCCEEDED(m_hr)) goto TRY_KEYBOARD_AGAIN;
    }
  }

  // get buffered input from the mouse
TRY_MOUSE_AGAIN:;
  cod = DINPUT_BUFFERSIZE;
  m_hr = m_pMouseDev->I()->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
    rgod, &cod, 0);

  if (m_hr == DI_OK)
  {
    m_MouseState.lX = 0;
    m_MouseState.lY = 0;
    m_MouseState.lZ = 0;
    for (DWORD iod = 0; iod < cod; iod++)
    {
      switch (rgod[iod].dwOfs)
      {
      case DIMOFS_X:
        m_MouseState.lX += rgod[iod].dwData;
        break;
      case DIMOFS_Y:
        m_MouseState.lY += rgod[iod].dwData;
        break;
      case DIMOFS_Z:
        m_MouseState.lZ += rgod[iod].dwData;
        break;
      case DIMOFS_BUTTON0:
        m_MouseState.rgbButtons[0] =  rgod[iod].dwData;
        break;
      case DIMOFS_BUTTON1:
        m_MouseState.rgbButtons[1] =  rgod[iod].dwData;
        break;
      case DIMOFS_BUTTON2:
        m_MouseState.rgbButtons[2] =  rgod[iod].dwData;
        break;
      case DIMOFS_BUTTON3:
        m_MouseState.rgbButtons[3] =  rgod[iod].dwData;
        break;
      }
    }
  }
  else  //  We got an error or we got DI_BUFFEROVERFLOW.
  {
    memset( &m_MouseState, '\0', sizeof(m_MouseState) );

    if ((m_hr == DIERR_INPUTLOST) || (m_hr == DIERR_NOTACQUIRED))
    {
      m_hr = m_pMouseDev->I()->Acquire();
                
      if (SUCCEEDED(m_hr)) goto TRY_MOUSE_AGAIN;
    }
  }

  // joystick
	if (m_bJoystick)
	{
		memset(&m_joyInfo, 0, sizeof(m_joyInfo));
		m_joyInfo.dwSize = sizeof(m_joyInfo);
		m_joyInfo.dwFlags = JOY_RETURNALL;

	  if (joyGetPosEx(0, &m_joyInfo) != JOYERR_NOERROR)
      m_bJoystick = FALSE;
	}
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectInputDevice::CADirectInputDevice(CADirectInput *pDirectInput, REFGUID rguid) : CADirectXObject()
{
  pDirectInput->CreateDevice( rguid, &m_pIDirectInputDevice, NULL );
}

CADirectInputDevice::~CADirectInputDevice()
{
}

HRESULT CADirectInputDevice::Acquire()
{
  return Check( m_pIDirectInputDevice->Acquire() );
}

HRESULT CADirectInputDevice::EnumObjects(
      LPDIENUMDEVICEOBJECTSCALLBACK lpCallback,
      LPVOID pvRef, DWORD dwFlags)
{
  return Check( m_pIDirectInputDevice->EnumObjects(
       lpCallback,
       pvRef,  dwFlags) );
}

HRESULT CADirectInputDevice::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
  return Check( m_pIDirectInputDevice->GetCapabilities( lpDIDevCaps) );
}

HRESULT CADirectInputDevice::GetDeviceData(DWORD cbObjectData, 
      LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut,
      DWORD dwFlags)
{
  return Check( m_pIDirectInputDevice->GetDeviceData( cbObjectData, 
       rgdod,  pdwInOut,
       dwFlags) );
}

HRESULT CADirectInputDevice::GetDeviceInfo(LPDIDEVICEINSTANCE pdidi)
{
  return Check( m_pIDirectInputDevice->GetDeviceInfo( pdidi) );
}

HRESULT CADirectInputDevice::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
  m_hr = m_pIDirectInputDevice->GetDeviceState( cbData,  lpvData);

  if ((m_hr == DIERR_INPUTLOST) || (m_hr == DIERR_NOTACQUIRED))
    return m_hr;

  return Check( m_hr );
}

HRESULT CADirectInputDevice::GetObjectInfo(
      LPDIDEVICEOBJECTINSTANCE pdidoi,
      DWORD dwObj, DWORD dwHow)
{
  return Check( m_pIDirectInputDevice->GetObjectInfo(
       pdidoi,
       dwObj,  dwHow) );
}

HRESULT CADirectInputDevice::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
  return Check( m_pIDirectInputDevice->GetProperty( rguidProp,  pdiph) );
}

HRESULT CADirectInputDevice::Initialize(HINSTANCE hinst, DWORD dwVersion,
      REFGUID rguid)
{
  return Check( m_pIDirectInputDevice->Initialize( hinst,  dwVersion,
       rguid) );
}

HRESULT CADirectInputDevice::RunControlPanel(HWND hwndOwner,
      DWORD dwFlags)
{
  return Check( m_pIDirectInputDevice->RunControlPanel( hwndOwner,
       dwFlags) );
}

HRESULT CADirectInputDevice::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
  return Check( m_pIDirectInputDevice->SetCooperativeLevel( hwnd,  dwFlags) );
}

HRESULT CADirectInputDevice::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
  return Check( m_pIDirectInputDevice->SetDataFormat( lpdf) );
}

HRESULT CADirectInputDevice::SetEventNotification(HANDLE hEvent)
{
  return Check( m_pIDirectInputDevice->SetEventNotification( hEvent) );
}

HRESULT CADirectInputDevice::SetProperty(REFGUID rguid, LPCDIPROPHEADER pdiph)
{
  return Check( m_pIDirectInputDevice->SetProperty( rguid,  pdiph) );
}

HRESULT CADirectInputDevice::Unacquire()
{
  return Check( m_pIDirectInputDevice->Unacquire() );
}


