//
// ds3dvirtualbuffer.cpp
//
// Classes representing virtual DirectSound3D buffers that may or may not have
// a real DirectSoundBuffer associated with them at any given moment.
//

#include "soundbase.h"

#include <algorithm>

#include "ds3dutil.h"
#include "ds3dbuffer.h"
#include "ds3dvirtualbuffer.h"

namespace SoundEngine {


/////////////////////////////////////////////////////////////////////////////
//
// DS3DBufferPositionTracker
//
/////////////////////////////////////////////////////////////////////////////

// Create a position tracker for a sound which plays until it reaches the 
// end (for non-looping sounds) or has stop called upon it.  
BufferPositionTracker::BufferPositionTracker(ISoundPCMData* pdata, bool bLooping) :
    m_dwBytesPerSec(pdata->GetBytesPerSec()), 
    m_dwLength(pdata->GetSize()), 
    m_dwPositionMask(~(pdata->GetBytesPerSample() - 1)),
    m_dwPosition(0), 
    m_bLooping(bLooping), 
    m_bEnding(false), 
    m_bASR(false)
{
};


// Creates a position tracker for an ASR buffer.
BufferPositionTracker::BufferPositionTracker(ISoundPCMData* pdata, 
        DWORD dwLoopStart, DWORD dwLoopLength) :
    m_dwBytesPerSec(pdata->GetBytesPerSec()), 
    m_dwLength(pdata->GetSize()), 
    m_dwPositionMask(~(pdata->GetBytesPerSample() - 1)),
    m_dwPosition(0), 
    m_bASR(true), 
    m_bEnding(false), 
    m_dwLoopStart(dwLoopStart), 
    m_dwLoopLength(dwLoopLength) 
{
    // make sure the loop is contained in the buffer
    ZAssert(m_dwLoopStart + m_dwLoopLength <= m_dwLength);
};


// the sound has been given a stop.  
void BufferPositionTracker::Stop(bool bForceNow)
{
    if (!m_bASR || bForceNow)
    {
        // stop it now.  
        m_dwPosition = m_dwLoopLength;
    }

    m_bEnding = true;
};


// the given number of miliseconds have elapsed
void BufferPositionTracker::AddTime(DWORD dwElapsedTime)
{
    DWORD dwElapsedBytes = 
        (DWORD)(dwElapsedTime * (int64_t)m_dwBytesPerSec / 1000);

    if (!m_bASR)
    {
        m_dwPosition += dwElapsedBytes;
        
        // if we've passed the end...
        if (m_dwPosition > m_dwLength)
        {
            // loop for a looping sound
            if (m_bLooping && !m_bEnding)
            {
                m_dwPosition %= m_dwLength;
            }
            else
            {
                // stop the position at the end of the sound.
                m_dwPosition = m_dwLength;
            }
        }
    }
    else // m_bASR
    {
        if (m_bEnding)
        {
            bool bWasInAttack = m_dwPosition < m_dwLoopStart;

            m_dwPosition += dwElapsedBytes;

            // if we were in the attack portion, make sure we jump
            // directly to the release portion of the wave.  
            if (bWasInAttack && (m_dwPosition > m_dwLoopStart))
            {
                m_dwPosition += m_dwLoopLength;
            }

            // stop the position at the end of the sound.
            if (m_dwPosition > m_dwLength)
            {
                m_dwPosition = m_dwLength;
            }
        }
        else
        {
            m_dwPosition += dwElapsedBytes;

            // loop if appropriate
            if (m_dwPosition >= m_dwLoopStart + m_dwLoopLength)
            {
                m_dwPosition = 
                    ((m_dwPosition - m_dwLoopStart) % m_dwLoopLength) 
                        + m_dwLoopStart;
            }
        }
    }
}


// Resets the position to the given one, presumably retrieved from a 
// playing buffer.
void BufferPositionTracker::SetPosition(DWORD dwPosition)
{
    m_dwPosition = std::min(dwPosition, m_dwLength);
}



/////////////////////////////////////////////////////////////////////////////
//
// DSVirtualSoundBuffer
//
/////////////////////////////////////////////////////////////////////////////


// prepare a sound buffer with the given quality and 3D support using the
// direct sound object pointed to by pDirectSound.
// mdvalley: DirectSound8
HRESULT DSVirtualSoundBuffer::PrepareBuffer8(IDirectSound8* pDirectSound, 
    ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D)
{
    HRESULT hr;

    if (m_pds3dbuffer == nullptr)
    {
        if (m_bufferPositionTracker.IsASR())
        {
            DS3DASRSoundBuffer* pds3dASRBuffer = new DS3DASRSoundBuffer();

            // initialize the buffer.
            hr = pds3dASRBuffer->Init8(pDirectSound, m_pdata, 
                m_bufferPositionTracker.GetLoopOffset(), 
                m_bufferPositionTracker.GetLoopLength(),
                bSupport3D, quality, bAllowHardware);
            if (ZFailed(hr)) return hr;

            m_pds3dbuffer = pds3dASRBuffer;
        }
        // stream sounds that are larger than 1 MB or so.
        else if (m_pdata->GetSize() > 1000000)
		// else if (m_pdata->GetSize() > 100000)		// mdvalley: lower limit to 100k for debugging
        {
            DS3DStreamingSoundBuffer* pds3DStreamingBuffer = new DS3DStreamingSoundBuffer();

            // initialize the buffer.
            hr = pds3DStreamingBuffer->Init8(pDirectSound, m_pdata, 
                m_bufferPositionTracker.IsLooping(), bSupport3D, quality, bAllowHardware);
            if (ZFailed(hr)) return hr;

            m_pds3dbuffer = pds3DStreamingBuffer;
        }
        else
        {
            DS3DStaticSoundBuffer* pds3DStaticBuffer = new DS3DStaticSoundBuffer();

            // initialize the buffer.
            hr = pds3DStaticBuffer->Init8(pDirectSound, m_pdata, 
                m_bufferPositionTracker.IsLooping(), bSupport3D, quality, bAllowHardware);
            if (FAILED(hr)) return hr;

            m_pds3dbuffer = pds3DStaticBuffer;
        }
    }

    m_quality = quality;
    m_bAllowHardware = bAllowHardware;

    // update the buffers's volume and pitch as needed.
    hr = m_pds3dbuffer->UpdateState(m_fGain, m_fPitch, false);
    if (ZFailed(hr)) return hr;

    return S_OK;
}

HRESULT DSVirtualSoundBuffer::PrepareBuffer(IDirectSound* pDirectSound, 
    ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D)
{
    HRESULT hr;

    if (m_pds3dbuffer == nullptr)
    {
        if (m_bufferPositionTracker.IsASR())
        {
            DS3DASRSoundBuffer* pds3dASRBuffer = new DS3DASRSoundBuffer();

            // initialize the buffer.
            hr = pds3dASRBuffer->Init(pDirectSound, m_pdata, 
                m_bufferPositionTracker.GetLoopOffset(), 
                m_bufferPositionTracker.GetLoopLength(),
                bSupport3D, quality, bAllowHardware);
            if (ZFailed(hr)) return hr;

            m_pds3dbuffer = pds3dASRBuffer;
        }
        // stream sounds that are larger than 1 MB or so.
        else if (m_pdata->GetSize() > 1000000)
		// else if (m_pdata->GetSize() > 100000)		// mdvalley: lower limit to 100k for debugging
        {
            DS3DStreamingSoundBuffer* pds3DStreamingBuffer = new DS3DStreamingSoundBuffer();

            // initialize the buffer.
            hr = pds3DStreamingBuffer->Init(pDirectSound, m_pdata, 
                m_bufferPositionTracker.IsLooping(), bSupport3D, quality, bAllowHardware);
            if (ZFailed(hr)) return hr;

            m_pds3dbuffer = pds3DStreamingBuffer;
        }
        else
        {
            DS3DStaticSoundBuffer* pds3DStaticBuffer = new DS3DStaticSoundBuffer();

            // initialize the buffer.
            hr = pds3DStaticBuffer->Init(pDirectSound, m_pdata, 
                m_bufferPositionTracker.IsLooping(), bSupport3D, quality, bAllowHardware);
            if (FAILED(hr)) return hr;

            m_pds3dbuffer = pds3DStaticBuffer;
        }
    }

    m_quality = quality;
    m_bAllowHardware = bAllowHardware;

    // update the buffers's volume and pitch as needed.
    hr = m_pds3dbuffer->UpdateState(m_fGain, m_fPitch, false);
    if (ZFailed(hr)) return hr;

    return S_OK;
}

// Creates a virtual sound buffer for a normal sound, possibly looping
DSVirtualSoundBuffer::DSVirtualSoundBuffer(ISoundPCMData* pdata, bool bLooping) : 
    m_bufferPositionTracker(pdata, bLooping),
    m_pdata(pdata),

    m_bStarted(false),
    m_bStopped(false),
    m_bRetryStop(false),
    m_bBufferPlaying(false),
    m_bAudible(false),
    m_fAge(0),
    m_fPriority(1.0f),
    m_fGain(0.0f),
    m_fPitch(1.0f),
    m_fDynamicPriority(0.0f)
{
};


// Creates a virtual sound buffer for an ASR sound
DSVirtualSoundBuffer::DSVirtualSoundBuffer(ISoundPCMData* pdata, DWORD dwLoopOffset, DWORD dwLoopLength) : 
    m_bufferPositionTracker(pdata, dwLoopOffset, dwLoopLength),
    m_pdata(pdata),

    m_bStarted(false),
    m_bStopped(false),
    m_bRetryStop(false),
    m_bBufferPlaying(false),
    m_bAudible(false),
    m_fAge(0),
    m_fPriority(1.0f),
    m_fGain(0.0f),
    m_fPitch(1.0f),
    m_fDynamicPriority(0.0f)
{
};


// Recalculates the sounds position, loudness, whether it's playing, etc..
HRESULT DSVirtualSoundBuffer::Update(DWORD dwTimeElapsed, 
        const Vector& vectListenerPosition, float fRolloffFactor)
{
    HRESULT hr;
    bool bWasStopped = m_bStopped;

    // if we tried to stop the sound and failed, try again now
    if (m_bRetryStop && m_bBufferPlaying)
        m_pds3dbuffer->Stop(true);
    
    // Start the sound and calculate the end time, if we have not done so 
    // yet.
    if (!m_bStarted)
    {
        m_bStarted = true;
    }
    else
    {
        // update the play position
        m_bufferPositionTracker.AddTime((DWORD)(dwTimeElapsed * m_fPitch));
        m_fAge += dwTimeElapsed/1000.0f;
    }

    // if the buffer is playing, try to figure out its state.
    if (m_bBufferPlaying)
    {
        bool bPlaying, bBufferLost;

        hr = m_pds3dbuffer->GetStatus(bPlaying, bBufferLost);
        if (ZFailed(hr)) return hr;

        if (bBufferLost)
        {
            // we are obviously no longer playing
            m_bBufferPlaying = false;
        }
        else
        {
            // otherwise, the buffer knows if we are still playing
            m_bStopped = !bPlaying;
        }
    }

    // if we don't have a buffer playing, try to calculate if this sound 
    // would still be playing if it were audible.
    if (!m_bBufferPlaying)
    {
        m_bStopped = m_bufferPositionTracker.IsStopped();
    }
    else
    {
        // update the playing sound's volume and pitch, as needed.
        hr = m_pds3dbuffer->UpdateState(m_fGain, m_fPitch);
        if (ZFailed(hr)) return hr;
    }

    // if this has just stopped
    if (m_bStopped && !bWasStopped)
    {
        // free the sound buffer to conserve resources
        m_pds3dbuffer = nullptr;
        m_bBufferPlaying = FALSE;

        // if there is an event source for the sound finishing, trigger it.
        if (m_peventsourceFinished != nullptr)
        {
            m_peventsourceFinished->Trigger();
        }
    }

    // set this sound to be audible if it is playing and has an attenuation
    // of less than 90dB.
    m_bAudible = !m_bStopped && (m_fGain > c_fMinAudible);

    // set the dynamic priority to be the aproximate volume plus the static
    // priority, with a 0.1 dB priority boost for physically playing sounds
    // and up to a 0.2 dB boost for sounds less than 2 seconds old.  This 
    // should help insure that, all other things being equal, a playing 
    // sound will continue playing and a younger sound will replace an 
    // identical older sound.  
    //
    // Repeating weapon sounds is an example of where this behavior is most
    // important.
    m_fDynamicPriority = m_fGain + m_fPriority
        + (m_bBufferPlaying ? 0.1f : 0)
        + std::max(0.0f, 1.0f * (2 - m_fAge));

    return S_OK;
};


// Creates and starts a real dsound buffer for this sound
// mdvalley: DirectSound8
HRESULT DSVirtualSoundBuffer::StartBuffer8(IDirectSound8* pDirectSound, 
    ISoundEngine::Quality quality, bool bAllowHardware)
{
    HRESULT hr;

    // if we have a buffer playing, we have nothing more to do
    if (m_bBufferPlaying)
    {
        ZAssert(false);  // I want to know about this
        return S_FALSE;
    }

    // Create a sound buffer with the given quality (or reuse the one we 
    // have, if appropriate).
    hr = PrepareBuffer8(pDirectSound, quality, bAllowHardware, false);
    if (FAILED(hr)) return hr;

    // start the sound.
    hr = m_pds3dbuffer->Start(m_bufferPositionTracker.GetPosition(), 
        m_bufferPositionTracker.IsStopping());
    if (FAILED(hr)) return hr;

    m_bBufferPlaying = true;

    return S_OK;
};

HRESULT DSVirtualSoundBuffer::StartBuffer(IDirectSound* pDirectSound, 
    ISoundEngine::Quality quality, bool bAllowHardware)
{
    HRESULT hr;

    // if we have a buffer playing, we have nothing more to do
    if (m_bBufferPlaying)
    {
        ZAssert(false);  // I want to know about this
        return S_FALSE;
    }

    // Create a sound buffer with the given quality (or reuse the one we 
    // have, if appropriate).
    hr = PrepareBuffer(pDirectSound, quality, bAllowHardware, false);
    if (FAILED(hr)) return hr;

    // start the sound.
    hr = m_pds3dbuffer->Start(m_bufferPositionTracker.GetPosition(), 
        m_bufferPositionTracker.IsStopping());
    if (FAILED(hr)) return hr;

    m_bBufferPlaying = true;

    return S_OK;
};

// forcibly stops the given buffer.
HRESULT DSVirtualSoundBuffer::StopBuffer()
{
    HRESULT hr;

    // if we don't have a buffer playing, we have nothing to do
    if (!m_bBufferPlaying)
    {
        ZAssert(false);  // I want to know about this
        return S_FALSE;
    }
    
    // try to stop the sound...
    hr = m_pds3dbuffer->Stop(true);
    if (ZFailed(hr)) return hr;

    // syncronize our buffer tracker with the final position of the buffer.
    DWORD dwPosition;
    hr = m_pds3dbuffer->GetPosition(dwPosition);
    if (ZFailed(hr)) return hr;
    if (dwPosition != 0) // 0 could be a wraparound for a static buffer
        m_bufferPositionTracker.SetPosition(dwPosition);

    m_bBufferPlaying = false;

    return S_OK;
}


// Sets the gain, from 0 to -100 dB
HRESULT DSVirtualSoundBuffer::SetGain(float fGain)
{
    if (fGain > 0)
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    m_fGain = std::max(fGain, -100.0f);

    return S_OK;
};


// Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
// and 2.0 is twice normal speed.  
HRESULT DSVirtualSoundBuffer::SetPitch(float fPitch)
{
    fPitch = std::max<float>(fPitch, DSBFREQUENCY_MIN / m_pdata->GetSampleRate());
    fPitch = std::min<float>(fPitch, DSBFREQUENCY_MAX / m_pdata->GetSampleRate());

    m_fPitch = fPitch;

    return S_OK;
};

// sets the priority - used as a addition to volume when choosing which 
// sounds are most important to play.
HRESULT DSVirtualSoundBuffer::SetPriority(float fPriority) 
{
    m_fPriority = fPriority;
    return S_OK;
};


// Stops the sound.  If bForceNow is true the sound will stop ASAP, 
// possibly popping.  If it is false some sounds may play a trail-off 
// sound or fade away.  
HRESULT DSVirtualSoundBuffer::Stop(bool bForceNow)
{
    HRESULT hr;

    if (m_bBufferPlaying)
    {
        // REVIEW: fade out at higher quality settings?
        hr = m_pds3dbuffer->Stop(bForceNow);
        if (ZFailed(hr)) 
        {
            m_bRetryStop = true;
            return hr;
        }
    }

    m_bufferPositionTracker.Stop(bForceNow);

    return S_OK;
};


// returns S_OK if the sound is currently playing, S_FALSE otherwise.
HRESULT DSVirtualSoundBuffer::IsPlaying()
{
    return m_bStopped ? S_FALSE : S_OK;
};


// Gets an event which fires when the sound finishes playing (for any 
// reason)
IEventSource* DSVirtualSoundBuffer::GetFinishEventSource()
{
    // if we don't have an event source, create one.
    if (m_peventsourceFinished == nullptr)
    {
        m_peventsourceFinished = new EventSourceImpl();
    }

    return m_peventsourceFinished;
};


// Gets an interface for tweaking the sound, if supported, nullptr otherwise.
TRef<ISoundTweakable> DSVirtualSoundBuffer::GetISoundTweakable()
{
    return this;
};


// Gets an interface for tweaking the sound, if supported, nullptr otherwise.
TRef<ISoundTweakable3D> DSVirtualSoundBuffer::GetISoundTweakable3D()
{
    return nullptr;
};


#ifdef _DEBUG
// return a human-readable description of the object, prepending
// strIndent to the beginning of each line.  
ZString DSVirtualSoundBuffer::DebugDump(const ZString& strIndent)
{
    return strIndent + "DSVirtualSoundBuffer: Priority " + ZString(m_fDynamicPriority)
        + ", Age " + ZString(m_fAge) 
        + ", gain " + ZString(m_fGain) 
        + ", pitch " + ZString(m_fPitch) + "\n"
        + SoundDebugDump(m_pdata, strIndent + "  ")
        + ((m_pds3dbuffer) ? SoundDebugDump(m_pds3dbuffer, strIndent + "  ") : "");
}
#endif



/////////////////////////////////////////////////////////////////////////////
//
// DS3DVirtualSoundBuffer
//
/////////////////////////////////////////////////////////////////////////////

// constructs a simple 3D capable sound
DS3DVirtualSoundBuffer::DS3DVirtualSoundBuffer(ISoundPCMData* pdata, bool bLooping, 
        TRef<ISoundPositionSource> psource) :
    DSVirtualSoundBuffer(pdata, bLooping),
    m_pposSource(psource),

    m_b3D(false),
    m_bListenerRelative(false),
    m_bPlayingIn3D(true),
    m_fMinimumDistance(20.0f),
    m_fInnerAngle(360),
    m_fOuterAngle(360),
    m_fOutsideGain(0.0f),
    m_vectPosition(0,0,0),
    m_vectVelocity(0,0,0),
    m_vectOrientation(0,0,1)
{
};


// constructs an ASR 3D capable sound
DS3DVirtualSoundBuffer::DS3DVirtualSoundBuffer(ISoundPCMData* pdata, DWORD dwLoopOffset, 
        DWORD dwLoopLength, TRef<ISoundPositionSource> psource) :
    DSVirtualSoundBuffer(pdata, dwLoopOffset, dwLoopLength),
    m_pposSource(psource),

    m_b3D(false),
    m_bListenerRelative(false),
    m_bPlayingIn3D(true),
    m_fMinimumDistance(20.0f),
    m_fInnerAngle(360),
    m_fOuterAngle(360),
    m_fOutsideGain(0.0f),
    m_vectPosition(0,0,0),
    m_vectVelocity(0,0,0),
    m_vectOrientation(0,0,1)
{
};


// prepare a sound buffer with the given quality and 3D support using the
// direct sound object pointed to by pDirectSound.
	// mdvalley: DSound8
HRESULT DS3DVirtualSoundBuffer::PrepareBuffer8(IDirectSound8* pDirectSound, 
    ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D)
{
    HRESULT hr;

    // If the old and new quality are not the same, we need a new buffer
    // because the 3D quality is set on a per-buffer basis.
    if (quality != m_quality || m_bAllowHardware != bAllowHardware)
        m_pds3dbuffer = nullptr;

    // do all of the 2D buffer creation stuff.
    hr = DSVirtualSoundBuffer::PrepareBuffer8(pDirectSound, quality, bAllowHardware, true);
    if (FAILED(hr)) return hr;

    // update the playing sound's 3D state, as needed.
    hr = m_pds3dbuffer->UpdateState3D(
        m_vectPosition, m_vectVelocity, m_vectOrientation, 
        m_fMinimumDistance, 
        m_fInnerAngle, m_fOuterAngle, m_fOutsideGain,
        m_bPlayingIn3D,
        m_bListenerRelative,
        false
        );
    if (ZFailed(hr)) return hr;

    return S_OK;
};

HRESULT DS3DVirtualSoundBuffer::PrepareBuffer(IDirectSound* pDirectSound, 
    ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D)
{
    HRESULT hr;

    // If the old and new quality are not the same, we need a new buffer
    // because the 3D quality is set on a per-buffer basis.
    if (quality != m_quality || m_bAllowHardware != bAllowHardware)
        m_pds3dbuffer = nullptr;

    // do all of the 2D buffer creation stuff.
    hr = DSVirtualSoundBuffer::PrepareBuffer(pDirectSound, quality, bAllowHardware, true);
    if (FAILED(hr)) return hr;

    // update the playing sound's 3D state, as needed.
    hr = m_pds3dbuffer->UpdateState3D(
        m_vectPosition, m_vectVelocity, m_vectOrientation, 
        m_fMinimumDistance, 
        m_fInnerAngle, m_fOuterAngle, m_fOutsideGain,
        m_bPlayingIn3D,
        m_bListenerRelative,
        false
        );
    if (ZFailed(hr)) return hr;

    return S_OK;
};


// Recalculates the sounds position, loudness, whether it's playing, etc..
HRESULT DS3DVirtualSoundBuffer::Update(DWORD dwTimeElapsed, 
        const Vector& vectListenerPosition, float fRolloffFactor)
{
    HRESULT hr;

    // if the source no longer exists, stop the sound.
    hr = m_pposSource->IsPlaying();
    if (ZFailed(hr)) return hr;
    bool bSourceIsValid = hr == S_OK;

    if (!bSourceIsValid)
    {
        Stop(true);
    }

    // most of what DSVirtualSoundBuffer does is perfectly fine
    hr = DSVirtualSoundBuffer::Update(dwTimeElapsed, vectListenerPosition,
        fRolloffFactor);
    if (ZFailed(hr)) return hr;

    if (bSourceIsValid)
    {
        // get the position of the source.
        hr = m_pposSource->GetPosition(m_vectPosition);
        if (ZFailed(hr)) return hr;

        // find out whether the position is listener relative
        hr = m_pposSource->IsListenerRelative();
        if (ZFailed(hr)) return hr;
        m_bListenerRelative = hr == S_OK;

        // optimization: don't do 3D processing if the source position is  
        // identical to the listener's position.
        m_bPlayingIn3D = m_b3D 
            && m_vectPosition 
                != (m_bListenerRelative ? Vector(0,0,0) : vectListenerPosition);

        if (m_bPlayingIn3D)
        {
            // get the velocity of the source
            hr = m_pposSource->GetVelocity(m_vectVelocity);
            if (ZFailed(hr)) return hr;

            // get the position of the listener relative to this source
            Vector vectRelListener = m_bListenerRelative 
                ? -m_vectPosition : (vectListenerPosition - m_vectPosition);

            // get the distance to the listener
            float fDistanceSquared = vectRelListener.LengthSquared();
        
            // calculate some useful constants
            float fMinimumDistanceSquared = m_fMinimumDistance * m_fMinimumDistance;
            float fMaximumDistanceSquared = fMinimumDistanceSquared
                * c_fMinToMaxDistanceRatio * c_fMinToMaxDistanceRatio;
        
            // if we are outside of the maximum distance for this sound
            if (fDistanceSquared > fMaximumDistanceSquared || !m_bAudible)
            {
                // this sound is silent
                m_bAudible = false;
                m_fDynamicPriority += -100;
            }
            else
            {
                // calculate how much softer the sound is (in dB) based on distance
                float fDistanceGain = std::min(0.0f, (float)(
                    fRolloffFactor * -10 * log(fDistanceSquared/fMinimumDistanceSquared)
                    ));

                // if this has a sound cone
                float fConeGain = 0;
                if (m_fOutsideGain != 0.0f && m_fInnerAngle != 360
                    && !m_bListenerRelative)  // TODO: HACK: we don't have the info 
                                              // to do the right thing for listener
                                              // relative sounds yet.
                {
                    const float fRadiansToDegrees = 57.2957795f;

                    // get the orientation of the source
                    hr = m_pposSource->GetOrientation(m_vectOrientation);
                    if (ZFailed(hr)) return hr;
                    ZAssertIsUnitVector(m_vectOrientation);

                    // calculate the angle between the source and the listener
                    // (using the identity v1 * v2 = |v1||v2|cos(angle) )
                    float fPhi = (float)acos(m_vectOrientation * vectRelListener 
                        / sqrt(fDistanceSquared)) * fRadiansToDegrees;

                    // adjust the cone attenuation to reflect this.
                    if (fPhi > m_fInnerAngle)
                    {
                        if (fPhi < m_fOuterAngle)
                        {
                            // do a linear fade between the outside and inside 
                            // gains.
                            fConeGain = m_fOutsideGain 
                                * (fPhi - m_fInnerAngle)
                                / (m_fOuterAngle - m_fInnerAngle);
                        }
                        else
                        {
                            fConeGain = m_fOutsideGain;
                        }
                    }
                }

                float fTotalGain = m_fGain + fDistanceGain + fConeGain;

                // update the audibility
                m_bAudible = fTotalGain > c_fMinAudible;

                // add in the distance and cone attenuations to the priority
                m_fDynamicPriority += fDistanceGain + fConeGain;
            }
        }
    };
    
    // if we have a buffer...
    if (m_bBufferPlaying)
    {
        // update the playing sound's 3D state, as needed.
        hr = m_pds3dbuffer->UpdateState3D(
            m_vectPosition, m_vectVelocity, m_vectOrientation, 
            m_fMinimumDistance, 
            m_fInnerAngle, m_fOuterAngle, m_fOutsideGain,
            m_bPlayingIn3D,
            m_bListenerRelative
            );
        if (ZFailed(hr)) return hr;
    }

    return S_OK;
}


// toggles 3D Positioning on and off for the given sound.
HRESULT DS3DVirtualSoundBuffer::Set3D(bool b3D)
{
    m_b3D = b3D;

    return S_OK;
};


// Sets the distance at which the sound will be at max volume.  This
// effects how quickly the sound drops off with distance.  
HRESULT DS3DVirtualSoundBuffer::SetMinimumDistance(float fMinimumDistance)
{
    // Any distance greater than 0 is OK.
    if (fMinimumDistance <= 0)
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    m_fMinimumDistance = fMinimumDistance;
    
    return S_OK;
};


// Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
// normal levels, outside of which it fades down by fOutsideGain 
// (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
HRESULT DS3DVirtualSoundBuffer::SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain)
{
    // check the parameters
    if ((fInnerAngle < 0 || fInnerAngle > 360)
        || (fOuterAngle < 0 || fOuterAngle > 360)
        || (fOuterAngle < fInnerAngle)
        || (fOutsideGain < -100 || fOutsideGain > 0)
        )
    {
        ZAssert(false);
        return E_INVALIDARG;
    };

    m_fInnerAngle = fInnerAngle;
    m_fOuterAngle = fOuterAngle;
    m_fOutsideGain = fOutsideGain;

    return S_OK;
};


// Sets the gain, from 0 to -100 dB
HRESULT DS3DVirtualSoundBuffer::SetGain(float fGain)
{
    return DSVirtualSoundBuffer::SetGain(fGain);
};


// Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
// and 2.0 is twice normal speed.  
HRESULT DS3DVirtualSoundBuffer::SetPitch(float fPitch)
{
    return DSVirtualSoundBuffer::SetPitch(fPitch);
};


// sets the priority - used as a addition to volume when choosing which 
// sounds are most important to play.
HRESULT DS3DVirtualSoundBuffer::SetPriority(float fPriority) 
{
    return DSVirtualSoundBuffer::SetPriority(fPriority);
};


// Gets an interface for tweaking the sound, if supported, nullptr otherwise.
TRef<ISoundTweakable> DS3DVirtualSoundBuffer::GetISoundTweakable()
{
    return DSVirtualSoundBuffer::GetISoundTweakable();
};

// Gets an interface for tweaking the sound, if supported, nullptr otherwise.
TRef<ISoundTweakable3D> DS3DVirtualSoundBuffer::GetISoundTweakable3D()
{
    return this;
};


};
