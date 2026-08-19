#include <stdafx.h>
#include "dxasound.h"

extern CADirectSound *g_pDirectSound;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectSound::CADirectSound() : CADirectXObject()
{
  Check( DirectSoundCreate( NULL, &m_pIDirectSound, NULL) );
}

CADirectSound::~CADirectSound()
{
  if (m_pIDirectSound) m_pIDirectSound->Release();
}

HRESULT CADirectSound::Compact()
{
  return Check( m_pIDirectSound->Compact() );
}

HRESULT CADirectSound::CreateSoundBuffer(LPDSBUFFERDESC lpDSBufferDesc,
      LPLPDIRECTSOUNDBUFFER lplpDirectSoundBuffer,
      IUnknown FAR * pUnkOuter)
{
  return Check( m_pIDirectSound->CreateSoundBuffer( lpDSBufferDesc,
       lplpDirectSoundBuffer,
       pUnkOuter) );
}

HRESULT CADirectSound::DuplicateSoundBuffer(
      LPDIRECTSOUNDBUFFER lpDsbOriginal,
      LPLPDIRECTSOUNDBUFFER lplpDsbDuplicate)
{
  return Check( m_pIDirectSound->DuplicateSoundBuffer(
       lpDsbOriginal,
       lplpDsbDuplicate) );
}

HRESULT CADirectSound::GetCaps(LPDSCAPS lpDSCaps)
{
  return Check( m_pIDirectSound->GetCaps( lpDSCaps) );
}

HRESULT CADirectSound::GetSpeakerConfig(LPDWORD lpdwSpeakerConfig)
{
  return Check( m_pIDirectSound->GetSpeakerConfig( lpdwSpeakerConfig) );
}

HRESULT CADirectSound::Initialize(GUID FAR * lpGuid)
{
  return Check( m_pIDirectSound->Initialize( lpGuid) );
}

HRESULT CADirectSound::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
  return Check( m_pIDirectSound->SetCooperativeLevel( hwnd,  dwLevel) );
}

HRESULT CADirectSound::SetSpeakerConfig(DWORD dwSpeakerConfig)
{
  return Check( m_pIDirectSound->SetSpeakerConfig( dwSpeakerConfig) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectSound3DBuffer::CADirectSound3DBuffer( LPCTSTR pFileName, DWORD dwFlags ) :
  CADirectSoundBuffer( pFileName, dwFlags | DSBCAPS_CTRL3D )
{
  Check( m_pIDirectSoundBuffer->QueryInterface(
    IID_IDirectSound3DBuffer, (void **) &m_pIDirectSound3DBuffer) );
}

CADirectSound3DBuffer::~CADirectSound3DBuffer()
{
  if (m_pIDirectSound3DBuffer) m_pIDirectSound3DBuffer->Release();
}


HRESULT CADirectSound3DBuffer::GetAllParameters(LPDS3DBUFFER lpDs3dBuffer)
{
  return Check( m_pIDirectSound3DBuffer->GetAllParameters( lpDs3dBuffer) );
}

HRESULT CADirectSound3DBuffer::GetConeAngles(
      LPDWORD lpdwInsideConeAngle, LPDWORD lpdwOutsideConeAngle)
{
  return Check( m_pIDirectSound3DBuffer->GetConeAngles(
       lpdwInsideConeAngle,  lpdwOutsideConeAngle) );
}

HRESULT CADirectSound3DBuffer::GetConeOrientation(LPD3DVECTOR lpvOrientation)
{
  return Check( m_pIDirectSound3DBuffer->GetConeOrientation( lpvOrientation) );
}

HRESULT CADirectSound3DBuffer::GetConeOutsideVolume(LPLONG lplConeOutsideVolume)
{
  return Check( m_pIDirectSound3DBuffer->GetConeOutsideVolume( lplConeOutsideVolume) );
}

HRESULT CADirectSound3DBuffer::GetMaxDistance(LPD3DVALUE lpflMaxDistance)
{
  return Check( m_pIDirectSound3DBuffer->GetMaxDistance( lpflMaxDistance) );
}

HRESULT CADirectSound3DBuffer::GetMinDistance(LPD3DVALUE lpflMinDistance)
{
  return Check( m_pIDirectSound3DBuffer->GetMinDistance( lpflMinDistance) );
}

HRESULT CADirectSound3DBuffer::GetMode(LPDWORD lpdwMode)
{
  return Check( m_pIDirectSound3DBuffer->GetMode( lpdwMode) );
}

HRESULT CADirectSound3DBuffer::GetPosition(LPD3DVECTOR lpvPosition)
{
  return Check( m_pIDirectSound3DBuffer->GetPosition( lpvPosition) );
}

HRESULT CADirectSound3DBuffer::GetVelocity(LPD3DVECTOR lpvVelocity)
{
  return Check( m_pIDirectSound3DBuffer->GetVelocity( lpvVelocity) );
}

HRESULT CADirectSound3DBuffer::SetAllParameters(
      LPDS3DBUFFER lpDs3dBuffer, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetAllParameters(
       lpDs3dBuffer,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetConeAngles(
      DWORD dwInsideConeAngle,
      DWORD dwOutsideConeAngle, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetConeAngles(
       dwInsideConeAngle,
       dwOutsideConeAngle,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetConeOrientation(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetConeOrientation( x,
       y,  z,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetConeOutsideVolume(
      LONG lConeOutsideVolume, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetConeOutsideVolume(
       lConeOutsideVolume,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetMaxDistance(
      D3DVALUE flMaxDistance, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetMaxDistance(
       flMaxDistance,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetMinDistance(
      D3DVALUE flMinDistance, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetMinDistance(
       flMinDistance,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetMode(
      DWORD dwMode, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetMode(
       dwMode,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetPosition(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetPosition( x,
       y,  z,  dwApply) );
}

HRESULT CADirectSound3DBuffer::SetVelocity(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply)
{
  return Check( m_pIDirectSound3DBuffer->SetVelocity( x,
       y,  z,  dwApply) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectSound3DListener::CADirectSound3DListener( CADirectSoundBuffer *pDsbPrimary ) : CADirectXObject()
{
  Check( pDsbPrimary->I()->QueryInterface(
    IID_IDirectSound3DListener, (void **)&m_pIDirectSound3DListener) );
}

CADirectSound3DListener::~CADirectSound3DListener()
{
  if (m_pIDirectSound3DListener) m_pIDirectSound3DListener->Release();
}


HRESULT CADirectSound3DListener::CommitDeferredSettings()
{
  return Check( m_pIDirectSound3DListener->CommitDeferredSettings() );
}

HRESULT CADirectSound3DListener::GetAllParameters(LPDS3DLISTENER lpListener)
{
  return Check( m_pIDirectSound3DListener->GetAllParameters( lpListener) );
}

HRESULT CADirectSound3DListener::GetDistanceFactor(LPD3DVALUE lpflDistanceFactor)
{
  return Check( m_pIDirectSound3DListener->GetDistanceFactor( lpflDistanceFactor) );
}

HRESULT CADirectSound3DListener::GetDopplerFactor(LPD3DVALUE lpflDopplerFactor)
{
  return Check( m_pIDirectSound3DListener->GetDopplerFactor( lpflDopplerFactor) );
}

HRESULT CADirectSound3DListener::GetOrientation(
      LPD3DVECTOR lpvOrientFront,
      LPD3DVECTOR lpvOrientTop)
{
  return Check( m_pIDirectSound3DListener->GetOrientation(
       lpvOrientFront,
       lpvOrientTop) );
}

HRESULT CADirectSound3DListener::GetPosition(LPD3DVECTOR lpvPosition)
{
  return Check( m_pIDirectSound3DListener->GetPosition( lpvPosition) );
}

HRESULT CADirectSound3DListener::GetRolloffFactor(LPD3DVALUE lpflRolloffFactor)
{
  return Check( m_pIDirectSound3DListener->GetRolloffFactor( lpflRolloffFactor) );
}

HRESULT CADirectSound3DListener::GetVelocity(LPD3DVECTOR lpvVelocity)
{
  return Check( m_pIDirectSound3DListener->GetVelocity( lpvVelocity) );
}

HRESULT CADirectSound3DListener::SetAllParameters(
      LPDS3DLISTENER lpListener, DWORD dwApply)
{
  return Check( m_pIDirectSound3DListener->SetAllParameters(
       lpListener,  dwApply) );
}

HRESULT CADirectSound3DListener::SetDistanceFactor(
      D3DVALUE flDistanceFactor, DWORD dwApply)
{
  return Check( m_pIDirectSound3DListener->SetDistanceFactor(
       flDistanceFactor,  dwApply) );
}

HRESULT CADirectSound3DListener::SetDopplerFactor(
      D3DVALUE flDopplerFactor, DWORD dwApply)
{
  return Check( m_pIDirectSound3DListener->SetDopplerFactor(
       flDopplerFactor,  dwApply) );
}

HRESULT CADirectSound3DListener::SetOrientation(D3DVALUE xFront,
      D3DVALUE yFront, D3DVALUE zFront,
      D3DVALUE xTop, D3DVALUE yTop,
      D3DVALUE zTop, DWORD    dwApply)
{
  return Check( m_pIDirectSound3DListener->SetOrientation( xFront,
       yFront,  zFront,
       xTop,    yTop,
       zTop,    dwApply) );
}

HRESULT CADirectSound3DListener::SetPosition(D3DVALUE x, D3DVALUE y,
      D3DVALUE z, DWORD dwApply)
{
  return Check( m_pIDirectSound3DListener->SetPosition( x,  y,
       z,  dwApply) );
}

HRESULT CADirectSound3DListener::SetRolloffFactor(
      D3DVALUE flRolloffFactor, DWORD dwApply)
{
  return Check( m_pIDirectSound3DListener->SetRolloffFactor(
       flRolloffFactor,  dwApply) );
}

HRESULT CADirectSound3DListener::SetVelocity(D3DVALUE x,
      D3DVALUE y, D3DVALUE z, DWORD dwApply)
{
  return Check( m_pIDirectSound3DListener->SetVelocity( x,
       y,  z,  dwApply) );
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
CADirectSoundBuffer::CADirectSoundBuffer(DWORD dwFlags) : CADirectXObject()
{
  DSBUFFERDESC dsbd;
  memset(&dsbd, 0, sizeof(DSBUFFERDESC));
  dsbd.dwSize = sizeof(DSBUFFERDESC);
  dsbd.dwFlags = dwFlags;

  g_pDirectSound->CreateSoundBuffer( &dsbd, &m_pIDirectSoundBuffer, NULL );
}

CADirectSoundBuffer::CADirectSoundBuffer( CADirectSound *pDirectSound, DSBUFFERDESC *pDesc) : CADirectXObject()
{
  pDirectSound->CreateSoundBuffer( pDesc, &m_pIDirectSoundBuffer, NULL );
}

CADirectSoundBuffer::~CADirectSoundBuffer()
{
  if (m_pIDirectSoundBuffer) m_pIDirectSoundBuffer->Release();
}


HRESULT CADirectSoundBuffer::GetCaps(LPDSBCAPS lpDSBufferCaps)
{
  return Check( m_pIDirectSoundBuffer->GetCaps( lpDSBufferCaps) );
}

HRESULT CADirectSoundBuffer::GetCurrentPosition(LPDWORD lpdwCurrentPlayCursor, 
      LPDWORD lpdwCurrentWriteCursor)
{
  return Check( m_pIDirectSoundBuffer->GetCurrentPosition( lpdwCurrentPlayCursor, 
       lpdwCurrentWriteCursor) );
}

HRESULT CADirectSoundBuffer::GetFormat(LPWAVEFORMATEX lpwfxFormat, 
      DWORD dwSizeAllocated, LPDWORD lpdwSizeWritten)
{
  return Check( m_pIDirectSoundBuffer->GetFormat( lpwfxFormat, 
       dwSizeAllocated,  lpdwSizeWritten) );
}

HRESULT CADirectSoundBuffer::GetFrequency(LPDWORD lpdwFrequency)
{
  return Check( m_pIDirectSoundBuffer->GetFrequency( lpdwFrequency) );
}

HRESULT CADirectSoundBuffer::GetPan(LPLONG lplPan)
{
  return Check( m_pIDirectSoundBuffer->GetPan( lplPan) );
}

HRESULT CADirectSoundBuffer::GetStatus(LPDWORD lpdwStatus)
{
  return Check( m_pIDirectSoundBuffer->GetStatus( lpdwStatus) );
}

HRESULT CADirectSoundBuffer::GetVolume(LPLONG lplVolume)
{
  return Check( m_pIDirectSoundBuffer->GetVolume( lplVolume) );
}

HRESULT CADirectSoundBuffer::Initialize(LPDIRECTSOUND lpDirectSound,
      LPDSBUFFERDESC lpDSBufferDesc)
{
  return Check( m_pIDirectSoundBuffer->Initialize( lpDirectSound,
       lpDSBufferDesc) );
}

HRESULT CADirectSoundBuffer::Lock(DWORD dwWriteCursor, DWORD dwWriteBytes,
      LPVOID *lplpvAudioPtr1, LPDWORD lpdwAudioBytes1,
      LPVOID *lplpvAudioPtr2, LPDWORD lpdwAudioBytes2, 
      DWORD dwFlags)
{
  return Check( m_pIDirectSoundBuffer->Lock( dwWriteCursor,  dwWriteBytes,
       lplpvAudioPtr1,  lpdwAudioBytes1,
       lplpvAudioPtr2,  lpdwAudioBytes2, 
       dwFlags) );
}

HRESULT CADirectSoundBuffer::Play(DWORD dwReserved1, DWORD dwReserved2, 
      DWORD dwFlags)
{
  return Check( m_pIDirectSoundBuffer->Play( dwReserved1,  dwReserved2, 
       dwFlags) );
}

HRESULT CADirectSoundBuffer::Restore()
{
  return Check( m_pIDirectSoundBuffer->Restore() );
}

HRESULT CADirectSoundBuffer::SetCurrentPosition(DWORD dwNewPosition)
{
  return Check( m_pIDirectSoundBuffer->SetCurrentPosition( dwNewPosition) );
}

HRESULT CADirectSoundBuffer::SetFormat(LPWAVEFORMATEX lpfxFormat)
{
  return Check( m_pIDirectSoundBuffer->SetFormat( lpfxFormat) );
}

HRESULT CADirectSoundBuffer::SetFrequency(DWORD dwFrequency)
{
  return Check( m_pIDirectSoundBuffer->SetFrequency( dwFrequency) );
}

HRESULT CADirectSoundBuffer::SetPan(LONG lPan)
{
  return Check( m_pIDirectSoundBuffer->SetPan( lPan) );
}

HRESULT CADirectSoundBuffer::SetVolume(LONG lVolume)
{
  return Check( m_pIDirectSoundBuffer->SetVolume( lVolume) );
}

HRESULT CADirectSoundBuffer::Stop()
{
  return Check( m_pIDirectSoundBuffer->Stop() );
}

HRESULT CADirectSoundBuffer::Unlock(LPVOID lpvAudioPtr1, DWORD dwAudioBytes1, 
      LPVOID lpvAudioPtr2, DWORD dwAudioBytes2)
{
  return Check( m_pIDirectSoundBuffer->Unlock( lpvAudioPtr1,  dwAudioBytes1, 
       lpvAudioPtr2,  dwAudioBytes2) );
}

/////// Helper members ////////

CADirectSoundBuffer::CADirectSoundBuffer( LPCTSTR pFileName, DWORD dwFlags )
{
  HMMIO       hmmio;              // file handle for open file 
  MMCKINFO    mmckinfoParent;     // parent chunk information 
  MMCKINFO    mmckinfoSubchunk;   // subchunk information structure 
  DWORD       dwFmtSize;          // size of "FMT" chunk 
  DWORD       dwDataSize;         // size of "DATA" chunk 
  WAVEFORMATEX *pFormat;           // address of "FMT" chunk 
  HPSTR       lpData;             // address of "DATA" chunk 

  // Open the file for reading with buffered I/O 
  // by using the default internal buffer 
  if(!(hmmio = mmioOpen((LPSTR)pFileName, NULL, MMIO_READ | MMIO_ALLOCBUF))) 
    return;

  // Locate a "RIFF" chunk with a "WAVE" form type to make 
  // sure the file is a waveform-audio file. 
  mmckinfoParent.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' ); 
  if (mmioDescend( hmmio, (LPMMCKINFO) &mmckinfoParent, NULL,  MMIO_FINDRIFF)) 
  { 
    mmioClose(hmmio, 0); 
    return;
  } 

  // Find the "FMT" chunk (form type "FMT"); it must be 
  // a subchunk of the "RIFF" chunk. 
  mmckinfoSubchunk.ckid = mmioFOURCC( 'f', 'm', 't', ' ' ); 
  if (mmioDescend( hmmio, &mmckinfoSubchunk, &mmckinfoParent, MMIO_FINDCHUNK)) 
  { 
    mmioClose(hmmio, 0); 
    return;
  } 

  // Get the size of the "FMT" chunk. Allocate 
  // and lock memory for it. 
  dwFmtSize = mmckinfoSubchunk.cksize; 
  pFormat = (WAVEFORMATEX *) new BYTE[ dwFmtSize ];

  // Read the "FMT" chunk. 
  if (mmioRead( hmmio, (HPSTR) pFormat, dwFmtSize) != dwFmtSize)
  { 
    mmioClose(hmmio, 0); 
    return;
  } 

  // Ascend out of the "FMT" subchunk. 
  mmioAscend( hmmio, &mmckinfoSubchunk, 0 ); 

  // Find the data subchunk. The current file position should be at 
  // the beginning of the data chunk; however, you should not make 
  // this assumption. Use mmioDescend to locate the data chunk. 
  mmckinfoSubchunk.ckid = mmioFOURCC( 'd', 'a', 't', 'a' ); 
  if (mmioDescend( hmmio, &mmckinfoSubchunk, &mmckinfoParent, MMIO_FINDCHUNK)) 
  { 
    mmioClose(hmmio, 0); 
    return;
  } 

  // Get the size of the data subchunk. 
  dwDataSize = mmckinfoSubchunk.cksize; 
  if (dwDataSize == 0L)
  { 
    mmioClose(hmmio, 0); 
    return;
  } 

  // Allocate memory for the waveform-audio data. 
  BYTE *pbSoundData = new BYTE[ dwDataSize ];

  // Read the waveform-audio data subchunk. 
  if (mmioRead( hmmio, (HPSTR) pbSoundData, dwDataSize) != dwDataSize)
  { 
    mmioClose(hmmio, 0); 
    return;
  } 

  // Close the file. 
  mmioClose(hmmio, 0); 

  //create a new buffer of the right format & size ...
  DSBUFFERDESC dsbdesc;
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
  dsbdesc.dwSize = sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags = dwFlags; 
  dsbdesc.dwBufferBytes = (dwFlags & DSBCAPS_PRIMARYBUFFER) ? 0 : dwDataSize;
  dsbdesc.lpwfxFormat = (dwFlags & DSBCAPS_PRIMARYBUFFER) ? NULL : pFormat;

  // Create buffer.
  g_pDirectSound->CreateSoundBuffer( &dsbdesc, &m_pIDirectSoundBuffer, NULL );

  // write the data to the buffer
  if (!(dwFlags & DSBCAPS_PRIMARYBUFFER))
  {
    LPVOID lpvPtr1;
    DWORD dwBytes1; 
    LPVOID lpvPtr2;
    DWORD dwBytes2;
    HRESULT hr;
    // Obtain write pointer.
    hr = Lock(0, dwDataSize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);

    if (hr == DSERR_BUFFERLOST)
    {
      Restore();
      hr = Lock(0, dwDataSize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
    }

    if (hr == DS_OK)
    {
      // Write to pointers.
      CopyMemory(lpvPtr1, pbSoundData, dwBytes1);

      if (lpvPtr2 != NULL)
        CopyMemory(lpvPtr2, pbSoundData + dwBytes1, dwBytes2);

      // Release the data back to DirectSound.
      hr = Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
    }
  }

  delete pFormat;       
  delete pbSoundData;       
}

