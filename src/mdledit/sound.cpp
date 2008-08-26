#include "pch.h"
#include "dsound.h"

//////////////////////////////////////////////////////////////////////////////
//
// Stuff duplicated from other files
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
class TZeroFillWithSize : public Type {
protected:
    TZeroFillWithSize()
    {
        Reinitialize();
    }

public:
    void Reinitialize()
    {
        memset(this, 0, sizeof(Type));
        this->dwSize = sizeof(Type);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

typedef IDirectSound       IDirectSoundX;
typedef IDirectSoundBuffer IDirectSoundBufferX;

void DDCall(HRESULT hr)
{
    ZSucceeded(hr);
}

class DSBD : public TZeroFillWithSize<DSBUFFERDESC> {
};

class DSCaps : public TZeroFillWithSize<DSCAPS> {
};

class WaveFormatEX : public TZeroFill<WAVEFORMATEX> {
public:
    WaveFormatEX()
    {
        cbSize = sizeof(WaveFormatEX);    
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class SoundBuffer {
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class SoundEngineImpl : public SoundEngine {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IDirectSoundX>       m_pds;
    DSCaps                    m_dscaps;
    TRef<IDirectSoundBufferX> m_pdsb;

public:
    SoundEngineImpl(HWND hwnd)
    {
        //
        // Create the DSound Device
        //

        DDCall(::DirectSoundCreate(NULL, &m_pds, NULL));
        DDCall(m_pds->SetCooperativeLevel(hwnd, DSSCL_PRIORITY));
        DDCall(m_pds->GetCaps(&m_dscaps));

        //
        // Create a buffer
        //

        WaveFormatEX wf;

        wf.wFormatTag      = WAVE_FORMAT_PCM; 
        wf.nChannels       = 1; 
        wf.nSamplesPerSec  = 22050; 
        wf.wBitsPerSample  = 16; 
        wf.nBlockAlign     = wf.wBitsPerSample / 8 * wf.nChannels;
        wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

        DSBD dsbd;

        dsbd.dwFlags       = DSBCAPS_GETCURRENTPOSITION2;
        dsbd.dwBufferBytes = 20 * wf.nSamplesPerSec * 2;
        dsbd.dwReserved    = 0;
        dsbd.lpwfxFormat   = &wf;

        DDCall(m_pds->CreateSoundBuffer(&dsbd, &m_pdsb, NULL));

        //
        // Fill the buffer
        //

        short* p1;
        DWORD dw1;
        short* p2;
        DWORD dw2;

        DDCall(m_pdsb->Lock(
            0,
            dsbd.dwBufferBytes,
            (void**)&p1,  
            &dw1,
            (void**)&p2,
            &dw2,
            DSBLOCK_ENTIREBUFFER
        ));

        for(DWORD index = 0; index < dw1 / 2; index++) {
            float time  = float(index) / float(wf.nSamplesPerSec);
            float value = Function(time);
            int i = int(32767.0f * value);
            p1[index] = short(i);
        }

        DDCall(m_pdsb->Unlock(p1, dw1, p2, dw2));

        //
        // Play the Buffer
        //

        DDCall(m_pdsb->Play(0, 0, DSBPLAY_LOOPING));
    }
    
    float Function(float time)
    {
        float f  = 880.0f;
        float v  = 0.8f + 0.2f * sin(2.0f * pi * 5.0f * time);

        return v * sin(2.0f * pi * f * time);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Update()
    {
    }
};

TRef<SoundEngine> CreateSoundEngine(HWND hwnd)
{
    return new SoundEngineImpl(hwnd);
}

