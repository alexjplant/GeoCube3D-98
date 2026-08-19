#ifndef __DXASOUND_H__
#define __DXASOUND_H__

#include "dxaobj.h"

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectSound : public CADirectXObject
{
public:               
  CADirectSound();
  ~CADirectSound();

  IDirectSound *I() { return m_pIDirectSound; };

  HRESULT Compact();

  HRESULT CreateSoundBuffer(LPDSBUFFERDESC lpDSBufferDesc,
      LPLPDIRECTSOUNDBUFFER lplpDirectSoundBuffer,
      IUnknown FAR * pUnkOuter);

  HRESULT DuplicateSoundBuffer(
      LPDIRECTSOUNDBUFFER lpDsbOriginal,
      LPLPDIRECTSOUNDBUFFER lplpDsbDuplicate);

  HRESULT GetCaps(LPDSCAPS lpDSCaps);

  HRESULT GetSpeakerConfig(LPDWORD lpdwSpeakerConfig);

  HRESULT Initialize(GUID FAR * lpGuid);

  HRESULT SetCooperativeLevel(HWND hwnd, DWORD dwLevel);

  HRESULT SetSpeakerConfig(DWORD dwSpeakerConfig);


protected:

  IDirectSound *m_pIDirectSound;
};



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectSoundBuffer : public CADirectXObject
{
public:               
  CADirectSoundBuffer( LPCTSTR pFileName, DWORD dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLDEFAULT );
  CADirectSoundBuffer(CADirectSound *pDirectSound, DSBUFFERDESC *pDesc);
  CADirectSoundBuffer(DWORD dwFlags = DSBCAPS_PRIMARYBUFFER );
  ~CADirectSoundBuffer();

  IDirectSoundBuffer *I() { return m_pIDirectSoundBuffer; };

  HRESULT GetCaps(LPDSBCAPS lpDSBufferCaps);

  HRESULT GetCurrentPosition(LPDWORD lpdwCurrentPlayCursor, 
      LPDWORD lpdwCurrentWriteCursor);

  HRESULT GetFormat(LPWAVEFORMATEX lpwfxFormat, 
      DWORD dwSizeAllocated, LPDWORD lpdwSizeWritten);

  HRESULT GetFrequency(LPDWORD lpdwFrequency);

  HRESULT GetPan(LPLONG lplPan);

  HRESULT GetStatus(LPDWORD lpdwStatus);

  HRESULT GetVolume(LPLONG lplVolume);     

  HRESULT Initialize(LPDIRECTSOUND lpDirectSound,
      LPDSBUFFERDESC lpDSBufferDesc);

  HRESULT Lock(DWORD dwWriteCursor, DWORD dwWriteBytes,
      LPVOID *lplpvAudioPtr1, LPDWORD lpdwAudioBytes1,
      LPVOID *lplpvAudioPtr2, LPDWORD lpdwAudioBytes2, 
      DWORD dwFlags);

  HRESULT Play(DWORD dwReserved1, DWORD dwReserved2, 
      DWORD dwFlags);

  HRESULT Restore();

  HRESULT SetCurrentPosition(DWORD dwNewPosition);

  HRESULT SetFormat(LPWAVEFORMATEX lpfxFormat);

  HRESULT SetFrequency(DWORD dwFrequency);

  HRESULT SetPan(LONG lPan);

  HRESULT SetVolume(LONG lVolume);

  HRESULT Stop();

  HRESULT Unlock(LPVOID lpvAudioPtr1, DWORD dwAudioBytes1, 
      LPVOID lpvAudioPtr2, DWORD dwAudioBytes2);


protected:

  IDirectSoundBuffer *m_pIDirectSoundBuffer;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectSound3DBuffer : public CADirectSoundBuffer
{
public:               
  CADirectSound3DBuffer( LPCTSTR pFileName, DWORD dwFlags = DSBCAPS_CTRLDEFAULT );
  ~CADirectSound3DBuffer();

  IDirectSound3DBuffer *I() { return m_pIDirectSound3DBuffer; };

  HRESULT GetAllParameters(LPDS3DBUFFER lpDs3dBuffer);

  HRESULT GetConeAngles(
      LPDWORD lpdwInsideConeAngle, LPDWORD lpdwOutsideConeAngle);

  HRESULT GetConeOrientation(LPD3DVECTOR lpvOrientation);

  HRESULT GetConeOutsideVolume(LPLONG lplConeOutsideVolume);

  HRESULT GetMaxDistance(LPD3DVALUE lpflMaxDistance);

  HRESULT GetMinDistance(LPD3DVALUE lpflMinDistance);

  HRESULT GetMode(LPDWORD lpdwMode);

  HRESULT GetPosition(LPD3DVECTOR lpvPosition);

  HRESULT GetVelocity(LPD3DVECTOR lpvVelocity);

  HRESULT SetAllParameters(
      LPDS3DBUFFER lpDs3dBuffer, DWORD dwApply);

  HRESULT SetConeAngles(
      DWORD dwInsideConeAngle,
      DWORD dwOutsideConeAngle, DWORD dwApply);

  HRESULT SetConeOrientation(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply);

  HRESULT SetConeOutsideVolume(
      LONG lConeOutsideVolume, DWORD dwApply);

  HRESULT SetMaxDistance(
      D3DVALUE flMaxDistance, DWORD dwApply);

  HRESULT SetMinDistance(
      D3DVALUE flMinDistance, DWORD dwApply);

  HRESULT SetMode(
      DWORD dwMode, DWORD dwApply);

  HRESULT SetPosition(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply);

  HRESULT SetVelocity(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply);


protected:

  IDirectSound3DBuffer *m_pIDirectSound3DBuffer;
};


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
class CADirectSound3DListener : public CADirectXObject
{
public:               
  CADirectSound3DListener( CADirectSoundBuffer *pDsbPrimary );
  ~CADirectSound3DListener();

  IDirectSound3DListener *I() { return m_pIDirectSound3DListener; };

  HRESULT CommitDeferredSettings();

  HRESULT GetAllParameters(LPDS3DLISTENER lpListener);

  HRESULT GetDistanceFactor(LPD3DVALUE lpflDistanceFactor);

  HRESULT GetDopplerFactor(LPD3DVALUE lpflDopplerFactor);

  HRESULT GetOrientation(
      LPD3DVECTOR lpvOrientFront,
      LPD3DVECTOR lpvOrientTop);

  HRESULT GetPosition(LPD3DVECTOR lpvPosition);

  HRESULT GetRolloffFactor(LPD3DVALUE lpflRolloffFactor);

  HRESULT GetVelocity(LPD3DVECTOR lpvVelocity);

  HRESULT SetAllParameters(
      LPDS3DLISTENER lpListener, DWORD dwApply);

  HRESULT SetDistanceFactor(
      D3DVALUE flDistanceFactor, DWORD dwApply);

  HRESULT SetDopplerFactor(
      D3DVALUE flDopplerFactor, DWORD dwApply);

  HRESULT SetOrientation(D3DVALUE xFront,
      D3DVALUE yFront, D3DVALUE zFront,
      D3DVALUE xTop, D3DVALUE yTop,
      D3DVALUE zTop, DWORD    dwApply);

  HRESULT SetPosition(D3DVALUE x, D3DVALUE y,
      D3DVALUE z, DWORD dwApply);

  HRESULT SetRolloffFactor(
      D3DVALUE flRolloffFactor, DWORD dwApply);

  HRESULT SetVelocity(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply);


protected:

  IDirectSound3DListener *m_pIDirectSound3DListener;
};



#endif