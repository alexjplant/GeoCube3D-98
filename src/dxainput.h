#ifndef __DXAINPUT_H__
#define __DXAINPUT_H__

#include "dxaobj.h"

#include "dinput.h"  // DirectX

class CADirectInputDevice;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CAKey
{
public:
  BOOL Down()   { return iState == 1; }
  BOOL Up()     { return iState == 2; }
  BOOL Repeat() { return bRepeat; }
  void Clear()  { iState = 0;  bRepeat = FALSE; }

  int   iState;  // 0-no event, 1-down, 2-up
  BOOL  bRepeat; // TRUE every REPEAT_INTERVAL ms when Down()
  DWORD dwTime;  // Down()-time stamp, Up()-total time pressed
  DWORD dwRepeatTime;  // time until next repeat
};

#define REPEAT_INTERVAL 50

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectInput : public CADirectXObject
{
public:               
  CADirectInput(HINSTANCE hinst);
  ~CADirectInput();

  IDirectInput *I() { return m_pIDirectInput; };

  HRESULT CreateDevice(REFGUID rguid, 
      LPDIRECTINPUTDEVICE *lplpDirectInputDevice,
      LPUNKNOWN pUnkOuter); 

  HRESULT EnumDevices(DWORD dwDevType, 
      LPDIENUMDEVICESCALLBACK lpCallback, LPVOID pvRef,
      DWORD dwFlags );

  HRESULT GetDeviceStatus(REFGUID rguidInstance);

  HRESULT Initialize(HINSTANCE hinst, DWORD dwVersion);

  HRESULT RunControlPanel(HWND  hwndOwner, DWORD dwFlags);

  //  Helper members
  BOOL mCreate(HWND hWnd);
  void mUpdate();

  BOOL                 m_bMouse;
  HANDLE               m_hMouse;
  CADirectInputDevice *m_pMouseDev;
	DIMOUSESTATE         m_MouseState;

  BOOL                 m_bKeyboard;
  HANDLE               m_hKeyboard;
  CADirectInputDevice *m_pKeyboardDev;
  CAKey                m_aKeys[256];

  BOOL                 m_bJoystick;
  JOYINFOEX            m_joyInfo;

protected:

  IDirectInput *m_pIDirectInput;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectInputDevice : public CADirectXObject
{
public:               
  CADirectInputDevice(CADirectInput *pDirectInput, REFGUID rguid);
  ~CADirectInputDevice();

  IDirectInputDevice *I() { return m_pIDirectInputDevice; };

  HRESULT Acquire();

  HRESULT EnumObjects(
      LPDIENUMDEVICEOBJECTSCALLBACK lpCallback,
      LPVOID pvRef, DWORD dwFlags);

  HRESULT GetCapabilities(LPDIDEVCAPS lpDIDevCaps);

  HRESULT GetDeviceData(DWORD cbObjectData, 
      LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut,
      DWORD dwFlags);

  HRESULT GetDeviceInfo(LPDIDEVICEINSTANCE pdidi);

  HRESULT GetDeviceState(DWORD cbData, LPVOID lpvData);

  HRESULT GetObjectInfo(
      LPDIDEVICEOBJECTINSTANCE pdidoi,
      DWORD dwObj, DWORD dwHow);

  HRESULT GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);

  HRESULT Initialize(HINSTANCE hinst, DWORD dwVersion,
      REFGUID rguid);

  HRESULT RunControlPanel(HWND hwndOwner,
      DWORD dwFlags);

  HRESULT SetCooperativeLevel(HWND hwnd, DWORD dwFlags);

  HRESULT SetDataFormat(LPCDIDATAFORMAT lpdf);

  HRESULT SetEventNotification(HANDLE hEvent);

  HRESULT SetProperty(REFGUID rguid, LPCDIPROPHEADER pdiph);

  HRESULT Unacquire();


protected:

  IDirectInputDevice *m_pIDirectInputDevice;
};



#endif