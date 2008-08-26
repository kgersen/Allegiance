#include    "pch.h"
#include <dsound.h>
#include "sound.h"


//////////////////////////////////////////////////////////////////////////////
//
//  Wave Structures
//
//////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct WaveHeader
{
    BYTE        RIFF[4];          // "RIFF"
    DWORD       dwSize;           // Size of data to follow
    BYTE        WAVE[4];          // "WAVE"
    BYTE        fmt_[4];          // "fmt "
    DWORD       dw16;             // 16
    WORD        wOne_0;           // 1
    WORD        wChnls;           // Number of Channels
    DWORD       dwSRate;          // Sample Rate
    DWORD       BytesPerSec;      // Sample Rate
    WORD        wBlkAlign;        // 1
    WORD        BitsPerSample;    // Sample size
    DWORD       DATAID;           // "DATA"
    DWORD       dwDSize;          // Number of Samples
};

struct WaveHeader1
{
    BYTE        RIFF[4];          // "RIFF"
    DWORD       dwSize;           // Size of data to follow
    BYTE        WAVE[4];          // "WAVE"
    BYTE        fmt_[4];          // "fmt "
    DWORD       dw16;             // 16
    WORD        wOne_0;           // 1
    WORD        wChnls;           // Number of Channels
    DWORD       dwSRate;          // Sample Rate
    DWORD       BytesPerSec;      // Sample Rate
    WORD        wBlkAlign;        // 1
    WORD        BitsPerSample;    // Sample size
	DWORD		FACTID;			  // "fact"
	DWORD		dw4;			  // 4
	DWORD		dummy;			  // ??
    DWORD       DATAID;           // "DATA"
    DWORD       dwDSize;          // Number of Samples
};

struct WaveHeader2
{
    BYTE        RIFF[4];          // "RIFF"
    DWORD       dwSize;           // Size of data to follow
    BYTE        WAVE[4];          // "WAVE"
    BYTE        fmt_[4];          // "fmt "
    DWORD       dw18;             // 18
    WORD        wOne_0;           // 1
    WORD        wChnls;           // Number of Channels
    DWORD       dwSRate;          // Sample Rate
    DWORD       BytesPerSec;      // Sample Rate
    WORD        wBlkAlign;        // 1
    WORD        BitsPerSample;    // Sample size
	WORD		dummy1;			  // ??
	DWORD		FACTID;		      // "fact"
	DWORD		dw4;			  // 4
	DWORD		dummy;			  // ??
    DWORD       DATAID;          // "DATA"
    DWORD       dwDSize;          // Number of Samples
};
#pragma pack()

class SoundBuffer;

class SoundEngine : public IObject
{
public:
    virtual IDirectSound* GetDS() = 0;
    virtual bool RegisterPlayingSoundBuffer(SoundBuffer* pBuffer, HANDLE& eventUpdate, HANDLE& eventStop) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
//  SoundBuffer
//
//////////////////////////////////////////////////////////////////////////////

int g_nTotalDupedBuffers = 0;
int g_nAllowableDupedBuffers = 1000;

class   SoundBuffer : public IObject
{
private:
    TRef<IDirectSoundBuffer>    m_pDSBuffer;
    TRef<IDirectSound3DBuffer>  m_pDS3DBuffer;
    TRef<SoundEngine>           m_pSoundEngine;
    TRef<SoundSource>           m_pSoundSource;
    int                         m_nPriority;
    int                         m_nVolume;
    bool                        m_b3D;
    bool                        m_bLooping;
    bool                        m_bPlayingLoop;
    ZString                     m_strWave;
    int                         m_nStreamingBufferSize;
    
    // valid for streaming audio only
    TRef<ZFile>                 m_pfile;
    BYTE*                       m_pWaveData;
    int                         m_nWaveDataSize;
    int                         m_nCurrentReadPosition;
    int                         m_nCurrentWritePosition;

public:
    SoundBuffer(const char* szWave, bool b3D, bool bLooping, SoundEngine* pSoundEngine) :
        m_pSoundEngine(pSoundEngine),
        m_b3D(b3D),
        m_nPriority(1),
        m_nVolume(100),
        m_bLooping(bLooping),
        m_bPlayingLoop(false),
        m_strWave(szWave),
        m_nStreamingBufferSize(0),
        m_pWaveData(NULL),
        m_nWaveDataSize(0),
        m_nCurrentReadPosition(0),
        m_nCurrentWritePosition(0)
    {
        LoadWaveFile();
        if (m_b3D && m_pDSBuffer)
            m_pDSBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)(IDirectSound3DBuffer*)&m_pDS3DBuffer);
    }

    SoundBuffer(SoundBuffer* pSoundBuffer) :
        m_pSoundEngine(pSoundBuffer->m_pSoundEngine),
        m_b3D(pSoundBuffer->m_b3D),
        m_nPriority(pSoundBuffer->m_nPriority),
        m_nVolume(pSoundBuffer->m_nVolume), 
        m_bLooping(pSoundBuffer->m_bLooping),
        m_bPlayingLoop(false),
        m_strWave(pSoundBuffer->m_strWave),
        m_nStreamingBufferSize(pSoundBuffer->m_nStreamingBufferSize),
        m_pfile(pSoundBuffer->m_pfile),
        m_pWaveData(pSoundBuffer->m_pWaveData),
        m_nWaveDataSize(pSoundBuffer->m_nWaveDataSize),
        m_nCurrentReadPosition(0),
        m_nCurrentWritePosition(0)
    {
        HRESULT hr = m_pSoundEngine->GetDS()->DuplicateSoundBuffer(pSoundBuffer->m_pDSBuffer, &m_pDSBuffer);

        // we may fail when trying to duplicate a hardware buffer, so try to reload it to get a 
        // software buffer.
        if ((hr == DSERR_ALLOCATED) || (hr == DSERR_OUTOFMEMORY))
            LoadWaveFile();

        if (m_b3D && m_pDSBuffer)
            m_pDSBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)(IDirectSound3DBuffer*)&m_pDS3DBuffer);
    }

    ~SoundBuffer()
    {
    }

    bool IsValid()
    {
        return (m_pDSBuffer != NULL) && (!m_b3D || m_pDS3DBuffer);
    }
    
    void Play(int nPriority, int nVolume, SoundSource* pSoundSource)
    {
        Vector po, vel;

        if (m_b3D)
        {
            Play(nPriority, nVolume, pSoundSource->GetPosition(), 
                pSoundSource->GetVelocity());
        }
        else
        {
            Play(nPriority, nVolume, Vector(0, 0, 0), Vector(0, 0, 0));
        }
    }

    void Play(int nPriority, int nVolume, const Vector& pos, const Vector& vel)
    {
        if (!m_bLooping || !m_bPlayingLoop)
            {
            if (m_b3D)
                {
                ZAssert(m_pDS3DBuffer);
                m_pDS3DBuffer->SetPosition(pos.X(),pos.Y(),pos.Z(),DS3D_IMMEDIATE);
                m_pDS3DBuffer->SetVelocity(vel.X(),vel.Y(),vel.Z(),DS3D_IMMEDIATE);
                m_pDS3DBuffer->SetMinDistance(1000, DS3D_IMMEDIATE);
                }

            if (m_nStreamingBufferSize == 0)
                {
                m_pDSBuffer->SetCurrentPosition(0);
                m_pDSBuffer->Play(0, 0, m_bLooping ? DSBPLAY_LOOPING : 0);
                }
            else
                {
                HANDLE eventUpdate, eventStop;
                if (m_pSoundEngine->RegisterPlayingSoundBuffer(this, eventUpdate, eventStop))
                    {
                    // Set up notifications for the middle, end, and stopping playing of the buffer
                    TRef<IDirectSoundNotify> directSoundNotify;
                    m_pDSBuffer->QueryInterface(IID_IDirectSoundNotify, 
                        (void**)(IDirectSoundNotify*)&directSoundNotify);
                    
                    DSBPOSITIONNOTIFY dsbPos[3];
                    dsbPos[0].dwOffset = m_nStreamingBufferSize/2 - 1;
                    dsbPos[0].hEventNotify = eventUpdate;
                    dsbPos[1].dwOffset = m_nStreamingBufferSize - 1;
                    dsbPos[1].hEventNotify = eventUpdate;
                    dsbPos[2].dwOffset = DSBPN_OFFSETSTOP;
                    dsbPos[2].hEventNotify = eventStop;
                    directSoundNotify->SetNotificationPositions(3, dsbPos);

                    // reset the current read and write positions
                    m_nCurrentReadPosition = 0;
                    m_nCurrentWritePosition = 0;

                    // initialize the buffer with the first two updates
                    DoUpdate();
                    DoUpdate();

                    // start the buffer playing
                    m_pDSBuffer->SetCurrentPosition(0);
                    m_pDSBuffer->Play(0, 0, DSBPLAY_LOOPING);
                    }
                }

            m_bPlayingLoop = m_bLooping;
            }
    }

    // CAUTION: This method must be reenterant, since it can be called by the 
    // streaming thread.
    void Stop()
    {
        if (!m_bLooping || m_bPlayingLoop)
            {
            m_pDSBuffer->Stop();
            m_bPlayingLoop = false;
            }
    }

    bool IsAvailable()
    {
        ZAssert(m_pDSBuffer);
        if (m_bLooping)
            return false;

        DWORD   status;
        m_pDSBuffer->GetStatus(&status);
        return (status != DSBSTATUS_PLAYING);
    }

    SoundSource* GetSoundSource()
    {
        return m_pSoundSource;
    }

    void DoUpdate()
    {
        int nUpdateSize = m_nStreamingBufferSize / 2;
        int nFirstWriteSize = 0;

        ZAssert(nUpdateSize * 2 == m_nStreamingBufferSize);

        // if we are done playing the sound, stop the buffer
        if (!m_bLooping && m_nCurrentReadPosition >= m_nWaveDataSize + nUpdateSize)
            {
            Stop();
            return;
            }

        // read in the next chunk of data
        if (m_nCurrentReadPosition < m_nWaveDataSize)
            {
            nFirstWriteSize = min(m_nWaveDataSize - m_nCurrentReadPosition, nUpdateSize);

            ReadData(m_pWaveData + m_nCurrentReadPosition, nFirstWriteSize, m_nCurrentWritePosition);            
            m_nCurrentReadPosition += nFirstWriteSize;
            m_nCurrentWritePosition = (m_nCurrentWritePosition + nFirstWriteSize) % m_nStreamingBufferSize;
            }

        // if that was not a full update's worth of data...
        if (nFirstWriteSize < nUpdateSize)
        {
            int nSecondWriteSize = nUpdateSize - nFirstWriteSize;
            
            // if this is a looping sound
            if (m_bLooping)
            {
                // reset the read position and start over
                ReadData(m_pWaveData, nSecondWriteSize, m_nCurrentWritePosition);            
                m_nCurrentReadPosition = nSecondWriteSize;
            }
            else
            {
                // fill in the rest with silence (we will stop the sound after two updates like this)
                FillData(0, nSecondWriteSize, m_nCurrentWritePosition);
                m_nCurrentReadPosition += nSecondWriteSize;
            }

            m_nCurrentWritePosition = (m_nCurrentWritePosition + nSecondWriteSize) % m_nStreamingBufferSize;
        }

        ZAssert(m_nCurrentWritePosition == 0 || m_nCurrentWritePosition == nUpdateSize);
    }

protected:

    
    #define SEC_TO_STREAM 2

    bool LoadWaveFile()
    {
        TRef<ZFile> pfile = GetWindow()->GetModeler()->GetFile(m_strWave, "wav");
        assert (pfile);
        void* pdata = pfile->GetPointer();
        
        // Read in the wave header          
    	DWORD dwSize;
    	WaveHeader wavHdr;
        memcpy(&wavHdr, pdata, sizeof(wavHdr));
    	if (wavHdr.DATAID == mmioFOURCC('d','a','t','a'))
    		dwSize = wavHdr.dwDSize;
    	else 
            {
    	    if (wavHdr.dw16 == 18) 
        	    {
        		WaveHeader2 wav2;
        		memcpy(&wav2, pdata, sizeof(wav2));
        		dwSize = wav2.dwDSize;
            	} 
            else 
                {
        		WaveHeader1 wav1;
                memcpy(&wav1, pdata, sizeof(wav1));
        		dwSize = wav1.dwDSize;
                }
        	}
        
        // Is this a stereo or mono file?
        BOOL bStereo = wavHdr.wChnls > 1 ? TRUE : FALSE;

        // if it's over SEC_TO_STREAM long...
        if (dwSize / (wavHdr.wBlkAlign * wavHdr.dwSRate) >= SEC_TO_STREAM) 
        {
            // Create a streamed version of the sound
            m_nStreamingBufferSize = SEC_TO_STREAM * wavHdr.wBlkAlign * wavHdr.dwSRate;
            m_pfile = pfile;
            m_pWaveData = (BYTE*)pdata + sizeof(wavHdr);
            m_nWaveDataSize = dwSize;

            // Create the sound buffer for streaming
            if (!CreateSoundBuffer(m_nStreamingBufferSize, wavHdr.dwSRate, wavHdr.BitsPerSample, 
                wavHdr.wBlkAlign, bStereo))
                
                return false;
        }
        else
        {
            // Create the sound buffer for the wave file
            if (!CreateSoundBuffer(dwSize, wavHdr.dwSRate, wavHdr.BitsPerSample, wavHdr.wBlkAlign, bStereo))
                return false;
            
            // Read the data for the wave file into the sound buffer
            if (!ReadData((char*)pdata + sizeof(wavHdr), dwSize)) 
                {
                m_pDSBuffer = NULL;
                return false;
                }
        }

    	m_pDSBuffer->SetCurrentPosition(0);

        return true;
    }


    BOOL CreateSoundBuffer(DWORD dwBufSize, DWORD dwFreq, DWORD dwBitsPerSample, DWORD dwBlkAlign, BOOL bStereo)
    {
        PCMWAVEFORMAT pcmwf;
        DSBUFFERDESC dsbdesc;
		IDirectSound* pdirectsound;

    	ZAssert(!m_pDSBuffer);
        
        // Set up wave format structure.
        memset( &pcmwf, 0, sizeof(PCMWAVEFORMAT) );
        pcmwf.wf.wFormatTag         = WAVE_FORMAT_PCM;      
        pcmwf.wf.nChannels          = bStereo ? 2 : 1;
        pcmwf.wf.nSamplesPerSec     = dwFreq;
        pcmwf.wf.nBlockAlign        = (WORD)dwBlkAlign;
        pcmwf.wf.nAvgBytesPerSec    = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
        pcmwf.wBitsPerSample        = (WORD)dwBitsPerSample;

        // Set up DSBUFFERDESC structure.
        memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));  // Zero it out. 
        dsbdesc.dwSize              = sizeof(DSBUFFERDESC);
        dsbdesc.dwFlags             = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;

        if (m_b3D)
            {
            dsbdesc.dwFlags |= DSBCAPS_CTRL3D;
            }

        if (m_nStreamingBufferSize == 0)
            {
            dsbdesc.dwFlags |= DSBCAPS_STATIC;
            debugf("Not streaming sound.\r\n");
            }
        else
            {
            dsbdesc.dwFlags |= DSBCAPS_CTRLPOSITIONNOTIFY;
            debugf("Streaming sound.\r\n");
            }

        dsbdesc.dwBufferBytes       = dwBufSize; 
        dsbdesc.lpwfxFormat         = (LPWAVEFORMATEX)&pcmwf;

		pdirectsound = m_pSoundEngine->GetDS();

		if (m_pSoundEngine->GetDS())
			return (DS_OK == pdirectsound->CreateSoundBuffer(&dsbdesc, &m_pDSBuffer, NULL));
		else
			return false;
    }

    bool ReadData(void* pdata, DWORD dwSize, DWORD dwOffset = 0) 
    {
        // Lock data in buffer for writing
        LPVOID pData1;
        DWORD  dwData1Size;
        LPVOID pData2;
        DWORD  dwData2Size;

        if (DS_OK != m_pDSBuffer->Lock(dwOffset, dwSize, &pData1, &dwData1Size, &pData2, &dwData2Size, 0))
            return false;

        // Read in first chunk of data
        if (dwData1Size > 0) 
            memcpy(pData1, pdata, dwData1Size);

        // read in second chunk if necessary
        if (dwData2Size > 0) 
            memcpy(pData2, (char*)pdata + dwData1Size, dwData2Size);

        // Unlock data in buffer
        return (DS_OK == m_pDSBuffer->Unlock(pData1, dwData1Size, pData2, dwData2Size));
    }

    bool FillData(BYTE bValue, DWORD dwSize, DWORD dwOffset = 0) 
    {
        // Lock data in buffer for writing
        LPVOID pData1;
        DWORD  dwData1Size;
        LPVOID pData2;
        DWORD  dwData2Size;

        if (DS_OK != m_pDSBuffer->Lock(dwOffset, dwSize, &pData1, &dwData1Size, &pData2, &dwData2Size, 0))
            return false;

        // set the first chunk of data
        if (dwData1Size > 0) 
            memset(pData1, bValue, dwData1Size);

        // read in second chunk if necessary
        if (dwData2Size > 0) 
            memset(pData2, bValue, dwData2Size);

        // Unlock data in buffer
        return (DS_OK == m_pDSBuffer->Unlock(pData1, dwData1Size, pData2, dwData2Size));
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//  BaseSound
//
//////////////////////////////////////////////////////////////////////////////

class   BaseSound : public IObject
{
protected:
    TRef<SoundEngine>           m_pSoundEngine;
    TList<TRef<SoundBuffer> >   m_lstSoundBuffers;
    ZString                     m_strWave;
    bool                        m_b3D;
    int                         m_nMaxBuffers;
    bool                        m_bLooping;
    
public:
    BaseSound(const char* szWave, bool b3D, bool bLooping, SoundEngine* pSoundEngine) :
        m_strWave(szWave),
        m_b3D(b3D),
        m_pSoundEngine(pSoundEngine),
        m_nMaxBuffers(1000),
        m_bLooping(bLooping)
    {
        TRef<SoundBuffer> pSoundBuffer = new SoundBuffer(szWave, b3D, bLooping, pSoundEngine);
        if (pSoundBuffer->IsValid())
            m_lstSoundBuffers.PushEnd(pSoundBuffer);
    }
    
    ~BaseSound()
    {
    }

    virtual void Play(int nPriority, int nVolume, SoundSource* pSoundSource)
    {
        // Get a buffer
        SoundBuffer* pSoundBuffer = GetBuffer(nPriority, nVolume, pSoundSource);
        // and play the sound
        if (pSoundBuffer)
            pSoundBuffer->Play(nPriority, nVolume, pSoundSource);
    }
    
    virtual void Play(int nPriority, int nVolume, const Vector& pos, const Vector& vel)
    {
        // Get a buffer
        SoundBuffer* pSoundBuffer = GetBuffer(nPriority, nVolume, NULL);
        // and play the sound
        if (pSoundBuffer)
            pSoundBuffer->Play(nPriority, nVolume, pos, vel);
    }

    virtual void Stop(SoundSource* pSoundSource)
    {
        // Find and stop all buffers for this source
        TList<TRef<SoundBuffer> >::Iterator iterBuffers(m_lstSoundBuffers);
        while (!iterBuffers.End()) 
            {
            SoundBuffer* pSoundBuffer = iterBuffers.Value();
            if (pSoundBuffer->GetSoundSource() == pSoundSource)
                pSoundBuffer->Stop();
            iterBuffers.Next();                
            }
    }

protected:

    SoundBuffer* GetBuffer(int nPriority, int nVolume, SoundSource* pSoundSource)
    {
		// if we do not have any buffers, it's a sign that we can't create them
		// for some reason - for example, if there is not a DSoundDevice
		if (m_lstSoundBuffers.IsEmpty())
			return NULL;

        if (m_bLooping) // for now looping sounds are single instance
            return m_lstSoundBuffers.GetFront();

        // Find an available buffer
        TList<TRef<SoundBuffer> >::Iterator iterBuffers(m_lstSoundBuffers);
        while (!iterBuffers.End()) 
            {
            SoundBuffer* pSoundBuffer = iterBuffers.Value();
            if (pSoundBuffer->IsAvailable())
                return pSoundBuffer;
            iterBuffers.Next();                
            }

        // Create a duplicate if we can
        if (m_nMaxBuffers+1 > m_lstSoundBuffers.GetCount() &&
                g_nTotalDupedBuffers <= g_nAllowableDupedBuffers)
            {
            TRef<SoundBuffer> pSoundBuffer = new SoundBuffer(m_lstSoundBuffers.GetFront());
            if (pSoundBuffer->IsValid())
                {
                g_nTotalDupedBuffers ++;
                m_lstSoundBuffers.PushEnd(pSoundBuffer);
                debugf("Allocated duplicate sound %s - # for this sound: %d/%d  # total: %d/%d\r\n", (LPCSTR)m_strWave, m_lstSoundBuffers.GetCount()-1, m_nMaxBuffers, g_nTotalDupedBuffers, g_nAllowableDupedBuffers);
                return pSoundBuffer;
                }
            else
                return NULL;
            }
        else
            {
            debugf("Dropped duplicate sound %s - # for this sound: %d/%d  # total: %d/%d\r\n", (LPCSTR)m_strWave, m_lstSoundBuffers.GetCount()-1, m_nMaxBuffers, g_nTotalDupedBuffers, g_nAllowableDupedBuffers);
            }

        // todo: find lower priority
        return NULL;
    }
    
    
};

//////////////////////////////////////////////////////////////////////////////
//
//  LoopingSound
//
//////////////////////////////////////////////////////////////////////////////

class   LoopingSound : public BaseSound
{
public:
    LoopingSound(const char* szWave, bool b3D, SoundEngine* pSoundEngine) :
       BaseSound(szWave, b3D, true, pSoundEngine)
    {
    }
    
};

//////////////////////////////////////////////////////////////////////////////
//
//  DiscreteSound
//
//////////////////////////////////////////////////////////////////////////////

class DiscreteSound : public BaseSound
{
public:
    DiscreteSound(const char* szWave, bool b3D, SoundEngine* pSoundEngine) :
       BaseSound(szWave, b3D, false, pSoundEngine)
    {
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//  AmbientSound
//
//////////////////////////////////////////////////////////////////////////////

/*
class AmbientSoundBit
{
public:
	AmbientSoundBit(SoundID idDiscrete, short nPlayOdds, 
		float fMinDelay, float fMaxDelay);
	virtual ~AmbientSoundBit();

	void 	Initialize();
	bool 	ShouldPlay();
	float 	Delay();
	SoundID	ID();

private:
	
	SoundID	m_idDiscrete;
	short	m_nPlayOdds;
	float 	m_fMinDelay;
	float 	m_fMaxDelay;
};


class   AmbientSound : public Sound
{
private:

    bool 					m_fPlaying;
	SoundID					m_idLooping;
    Time					m_timeNextBit;
	AmbientSoundBitList		m_listSoundBits;
	AmbientSoundBitLink*	m_pCurSoundBitLink;

public:

    AmbientSound(SoundID idLooping); 
	virtual ~AmbientSound(void);

    void AddSoundBit(SoundID idDiscrete, short nPlayOdds, 
		float fMinDelay, float fMaxDelay);
    void Update(Time time);

    AmbientSoundBit::AmbientSoundBit(SoundID idDiscrete, short nPlayOdds, 
    			float fMinDelay, float fMaxDelay)
    {
    	m_idDiscrete = idDiscrete;
    	m_nPlayOdds = nPlayOdds;
    	m_fMinDelay = fMinDelay;
    	m_fMaxDelay = fMaxDelay;
    }

    AmbientSoundBit::~AmbientSoundBit()
    {
    }

    bool AmbientSoundBit::ShouldPlay()
    {
    	return m_nPlayOdds >= (int)random(1 , 100);
    }

    float AmbientSoundBit::Delay()
    {
    	return random(m_fMinDelay, m_fMaxDelay);
    }

    SoundID AmbientSoundBit::ID()
    {
    	return m_idDiscrete;
    }


    AmbientSound::AmbientSound(SoundID idLooping)
    {
    	m_idLooping = idLooping;
    	m_fPlaying = false;
    	m_timeNextBit = 0;
    	m_pCurSoundBitLink = NULL;
    }

    AmbientSound::~AmbientSound(void)
    {
    	stop();
    	for (AmbientSoundBitLink* pLink = m_listSoundBits.first();
    			pLink != NULL; pLink = pLink->next())
    		delete pLink->data();
    }

    void AmbientSound::AddSoundBit(SoundID idDiscrete, short nPlayOdds, 
    			float fMinDelay, float fMaxDelay)
    {
    		AmbientSoundBitLink* pLink = new AmbientSoundBitLink;
    		pLink->data() = new AmbientSoundBit(idDiscrete, nPlayOdds, 
    				fMinDelay, fMaxDelay);
            m_listSoundBits.last(pLink);
    }

    void AmbientSound::Update(Time time)
    {
    	if (m_fPlaying && time >= m_timeNextBit)
    		{
    		AmbientSoundBitLink* pNextSoundBitLink = NULL;

    		if (m_pCurSoundBitLink)
    			pNextSoundBitLink = m_pCurSoundBitLink->next();
    		if (!pNextSoundBitLink)
    			pNextSoundBitLink = m_listSoundBits.first();
    			
    	    while (pNextSoundBitLink != m_pCurSoundBitLink)
    			{
    			if (pNextSoundBitLink->data()->ShouldPlay())
    				{
    				m_pCurSoundBitLink = pNextSoundBitLink;
    				m_timeNextBit = time + m_pCurSoundBitLink->data()->Delay();
    				Sound::PlaySoundEffect(m_pCurSoundBitLink->data()->ID());
    				break;
    				}
    			pNextSoundBitLink = pNextSoundBitLink->next();
    			if (!pNextSoundBitLink)
    				pNextSoundBitLink = m_listSoundBits.first();
    			}	
    		}
    }


    void AmbientSound::play(void)
    {
        if (!m_fPlaying)
    	    {
            m_fPlaying = true;
    		Sound::PlaySoundEffect(m_idLooping);
    		m_timeNextBit = 0;
            m_pCurSoundBitLink = NULL;
        	}
    }

    void AmbientSound::setFrequency(DWORD        frequency)
    {

    }

    void AmbientSound::setVolume(long        volume)
    {

    }

    void AmbientSound::stop(void)
    {
    	m_fPlaying = false;
    	Sound::StopSoundEffect(m_idLooping);
    	if (m_pCurSoundBitLink)
    		{
    		Sound::PlaySoundEffect(m_pCurSoundBitLink->data()->ID());
    		m_pCurSoundBitLink = NULL;
    		}
    }
}
*/

//////////////////////////////////////////////////////////////////////////////
//
//  SoundEngine
//
//////////////////////////////////////////////////////////////////////////////

const int MAX_SOUNDS = 31;
const int MAX_STREAMING_SOUNDS = MAX_SOUNDS;

class SoundEngineImpl : public SoundEngine
{
private:
    TRef<IDirectSound>                  m_pDirectSound;
    TRef<IDirectSound3DListener>        m_pListener;
    bool                                m_bSoundsOn;
    TMap<SoundID, TRef<BaseSound> >     m_mapSounds;

    // streaming sound support
    enum { event_Terminate, numSpecialEvents };
    enum { numEventObjects = numSpecialEvents + 2*MAX_STREAMING_SOUNDS };
    HANDLE                              m_vSoundEvents[numEventObjects];
    TRef<SoundBuffer>                   m_vsoundbuffersPlaying[MAX_STREAMING_SOUNDS];
    HANDLE                              m_threadUpdate;

public:
    SoundEngineImpl() : 
        m_bSoundsOn(true)
    {
    }
    
    void Initialize(HWND hWnd)
    {
        ZAssert(!m_pDirectSound);
        if (SUCCEEDED(DirectSoundCreate(NULL, &m_pDirectSound, NULL))) {
            if (FAILED(m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL))) {
                m_pDirectSound = NULL;
            } else {
                DSBUFFERDESC        dsbd;
                TRef<IDirectSoundBuffer> dsb;
                ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
                dsbd.dwSize = sizeof(DSBUFFERDESC);
                dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
                if SUCCEEDED(m_pDirectSound->CreateSoundBuffer(&dsbd, &dsb, NULL)) {
                    if SUCCEEDED(dsb->QueryInterface(IID_IDirectSound3DListener,(void**)(IDirectSound3DListener*)&m_pListener)) {
                        //m_pListener->SetRolloffFactor(DS3D_MINROLLOFFFACTOR, DS3D_IMMEDIATE);
                    }
                }

                // fill in the array of events for us to use
                for (int i = 0; i < numEventObjects; i++)
                {
                    m_vSoundEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
                    ZAssert(m_vSoundEvents[i] != NULL);
                }

                // create and start an update thread
                
                DWORD threadId;
                m_threadUpdate = CreateThread(NULL, 0, &UpdateThreadStartProc, this, 0, &threadId);
                //SetThreadPriority(m_threadUpdate, THREAD_PRIORITY_HIGHEST);
            }
        }
    }

    void Terminate()
    {
        if (m_pDirectSound)
            {
            int i;

            // kill the sound update thread
            SetEvent(m_vSoundEvents[event_Terminate]);
            WaitForSingleObject(m_threadUpdate, INFINITE);
            CloseHandle(m_threadUpdate);

            // release any playing sound buffers
            for (i = 0; i < MAX_STREAMING_SOUNDS; i++)
                m_vsoundbuffersPlaying[i] = NULL;

            // release all of the events
            for (i = 0; i < numEventObjects; i++)
                CloseHandle(m_vSoundEvents[i]);
            }

        m_mapSounds.SetEmpty();
        m_pListener = NULL;
        m_pDirectSound = NULL;
    }

    IDirectSound* GetDS()
    {
        return m_pDirectSound;
    }

    bool RegisterPlayingSoundBuffer(SoundBuffer* pBuffer, HANDLE& eventUpdate, HANDLE& eventStop)
    {
        // find an unused slot
        int nIndex = 0;
        while (nIndex < MAX_STREAMING_SOUNDS && m_vsoundbuffersPlaying[nIndex] != NULL)
            nIndex++;

        if (nIndex == MAX_STREAMING_SOUNDS)
            return false;

        // put the sound into the empty slot
        m_vsoundbuffersPlaying[nIndex] = pBuffer;

        // return the update and stop events
        eventUpdate = GetUpdateEvent(nIndex);
        eventStop = GetStopEvent(nIndex);

        return true;
    }

    bool GetSoundsOn()
    {
        return m_bSoundsOn;
    }

    void SetSoundsOn(bool bSoundsOn)
    {
        m_bSoundsOn = bSoundsOn;
    }
    
    void DefineDiscreteSound(SoundID id, const char*  szWave, bool fSingleInstance, bool b3D)
    {
        m_mapSounds.Set(id, new DiscreteSound(szWave, b3D, this));
    }
    
    void DefineLoopingSound(SoundID id, const char* szWave, bool b3D)
    {
        m_mapSounds.Set(id, new LoopingSound(szWave, b3D, this));
    }
    
    void DefineAmbientSound(SoundID idAmbient, SoundID idLooping, bool b3D = false)
    {
//        m_mapSounds.Set(idAmbient, new AmbientSound(idLooping, b3D));
    }

    void AddAmbientSoundBit(SoundID idAmbient, SoundID idDescrete,
                        short nPlayOdds, float fMinDelay, float fMaxDelay)
    {
/*        TRef<BaseSound> pBaseSound;
        m_mapPanes.Find(idAmbient, pBaseSound));
        ZAssert(pBaseSound);
        ((AmbientSound*)(BaseSound*)pBaseSound)->AddSoundBit(idDescrete,
                        nPlayOdds, fMinDelay, fMaxDelay);
*/    }

    void Play(SoundID id, int nPriority, int nVolume, SoundSource* pSoundSource = NULL)
    {
        if (m_bSoundsOn)
            {
            TRef<BaseSound> pBaseSound;
            m_mapSounds.Find(id, pBaseSound);
            ZAssert(pBaseSound);
            pBaseSound->Play(nPriority, nVolume, pSoundSource);
            }
    }

    void Stop(SoundID id, SoundSource* pSoundSource = NULL)
    {
        TRef<BaseSound> pBaseSound;
        m_mapSounds.Find(id, pBaseSound);
        ZAssert(pBaseSound);
        pBaseSound->Stop(pSoundSource);
    }
    
    void Play(SoundID id, int nPriority, int nVolume, const Vector& pos, const Vector& vel)
    {
        if (m_bSoundsOn)
            {
            TRef<BaseSound> pBaseSound;
            m_mapSounds.Find(id, pBaseSound);
            ZAssert(pBaseSound);
            pBaseSound->Play(nPriority, nVolume, pos, vel);
            }
    }

    void UpdateListener(const Vector& pos, const Vector& vel, const Orientation& orient)
    {
        if (m_pListener)
            {
            m_pListener->SetPosition(pos.X(),pos.Y(),pos.Z(),DS3D_DEFERRED);
            m_pListener->SetVelocity(vel.X(),vel.Y(),vel.Z(),DS3D_DEFERRED);

            Vector forward = orient.GetForward();
            Vector up = orient.GetUp();
            m_pListener->SetOrientation(forward.X(),forward.Y(),forward.Z(),-up.X(),-up.Y(),-up.Z(),DS3D_DEFERRED);

            m_pListener->CommitDeferredSettings();
            }
    }


    void Update(Time time)
    {

    }

private:

    static bool IsUpdateEvent(int nEvent)
    {
        ZAssert(nEvent < numEventObjects);
        return ((nEvent - numSpecialEvents) % 2 == 0);
    }

    static bool IsStopEvent(int nEvent)
    {
        ZAssert(nEvent < numEventObjects);
        return !IsUpdateEvent(nEvent);
    }

    int GetSoundIndexFromEvent(int nEvent)
    {
        ZAssert(nEvent < numEventObjects);
        int nIndex = (nEvent - numSpecialEvents) / 2;

        ZAssert(nIndex < MAX_STREAMING_SOUNDS);
        return nIndex;
    }

    HANDLE GetUpdateEvent(int nIndex)
    {
        ZAssert(nIndex < MAX_STREAMING_SOUNDS);
        return m_vSoundEvents[numSpecialEvents + nIndex * 2];
    }

    HANDLE GetStopEvent(int nIndex)
    {
        ZAssert(nIndex < MAX_STREAMING_SOUNDS);
        return m_vSoundEvents[numSpecialEvents + nIndex * 2 + 1];
    }

    static unsigned long __stdcall UpdateThreadStartProc(void* pThis)
    {
        ((SoundEngineImpl*)pThis)->UpdateThreadProc();
        return 0;
    }

    void UpdateThreadProc()
    {
        while (1)
            {
            int nEvent = WaitForMultipleObjects(numEventObjects, m_vSoundEvents, FALSE, INFINITE) 
                - WAIT_OBJECT_0;

            ZAssert((nEvent >= 0) && (nEvent < numEventObjects));

            if (nEvent < numSpecialEvents)
                {
                switch (nEvent)
                    {
                    case event_Terminate:
                        // exit the thread
                        return;
                        break;

                    default:
                        // unreached
                        ZAssert(false);
                        break;
                    }
                }
            else if (IsUpdateEvent(nEvent))
                {
                SoundBuffer* pbuffer = m_vsoundbuffersPlaying[GetSoundIndexFromEvent(nEvent)];
                if (pbuffer != NULL)
                    m_vsoundbuffersPlaying[GetSoundIndexFromEvent(nEvent)]->DoUpdate();
                }
            else
                {
                ZAssert(IsStopEvent(nEvent));
                ZAssert(m_vsoundbuffersPlaying[GetSoundIndexFromEvent(nEvent)] != NULL);

                // free up this slot in the playing sound list
                m_vsoundbuffersPlaying[GetSoundIndexFromEvent(nEvent)] = NULL;
                }
            }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//  Old WinTrek API
//
//////////////////////////////////////////////////////////////////////////////

TRef<SoundEngineImpl> s_pSoundEngine;
bool g_bEnableSound = true;

void Sound::Initialize(HWND hWnd)
{
    if (g_bEnableSound) {
        assert (s_pSoundEngine == NULL);
        s_pSoundEngine = new SoundEngineImpl();
        s_pSoundEngine->Initialize(hWnd);
    }
}

void Sound::Terminate()
{
    if (g_bEnableSound) {
        s_pSoundEngine->Terminate();
        s_pSoundEngine = NULL;
    }
}

bool Sound::GetPlaySounds(void)
{
    if (g_bEnableSound) {
        return s_pSoundEngine->GetSoundsOn();
    }

    return false;
}

void Sound::SetPlaySounds(bool    newVal)
{
    if (g_bEnableSound) {
        s_pSoundEngine->SetSoundsOn(newVal);
    }
}

void Sound::DefineDiscreteSoundEffect(SoundID        id,
                                         const char*    szWave,
                                         bool           fSingleInstance,
                                         bool           b3D)
{
    if (g_bEnableSound) {
        s_pSoundEngine->DefineDiscreteSound(id, szWave, fSingleInstance, b3D);
    }
}

void Sound::DefineLoopingSoundEffect(SoundID        id,
                                      const char*   szWave,
                                      bool          b3D)
{
    if (g_bEnableSound) {
        s_pSoundEngine->DefineLoopingSound(id, szWave, b3D);
    }
}

void Sound::DefineAmbientSoundEffect(SoundID idAmbient, SoundID idLooping)
{
    if (g_bEnableSound) {
        s_pSoundEngine->DefineAmbientSound(idAmbient, idLooping);
    }
}

void Sound::AddAmbientSoundBit(SoundID idAmbient, 
					SoundID idDescrete, short nPlayOdds,
					float fMinDelay, float fMaxDelay)
{
    if (g_bEnableSound) {
        s_pSoundEngine->AddAmbientSoundBit(idAmbient, idDescrete, nPlayOdds, fMinDelay, fMaxDelay);
    }
}

void Sound::UpdateAmbientSounds(Time time)
{
    if (g_bEnableSound) {
        s_pSoundEngine->Update(time);
    }
}

void Sound::PlaySoundEffect(SoundID   id,  const Vector& pos, const Vector& vel)
{
    if (g_bEnableSound) {
        s_pSoundEngine->Play(id, 1, 100, pos, vel);
    }
}

void Sound::PlaySoundEffect(SoundID id)
{
    if (g_bEnableSound) {
        s_pSoundEngine->Play(id, 1, 100, NULL);
    }
}

void Sound::StopSoundEffect(SoundID   id)
{
    if (g_bEnableSound) {
        s_pSoundEngine->Stop(id);
    }
}

void Sound::UpdateListener(const Vector& pos, const Vector& vel, const Orientation& orient)
{
    if (g_bEnableSound) {
        s_pSoundEngine->UpdateListener(pos, vel, orient);
    }
}
