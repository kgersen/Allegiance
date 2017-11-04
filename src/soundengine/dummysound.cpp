//
// dummysound.cpp
//
// a bunch of dummy implementations of the sound interfaces, for use without a
// sound card.
//

#include "soundbase.h"
#include "soundutil.h"

namespace SoundEngine {


//
// A stubbed version of the ISoundInstance interface
//
class DummySoundInstance : public ISoundInstance, public ISoundTweakable3D
{
    bool m_bLooping;
    bool m_bPlaying;
    TRef<ISoundPositionSource> m_psource;
    TRef<EventSourceImpl> m_peventsourceFinished;

public:
    
    DummySoundInstance(bool bLooping, ISoundPositionSource* psource) 
        :   m_bLooping(bLooping), 
            m_bPlaying(true),
            m_psource(psource) 
    {
        m_peventsourceFinished = new EventSourceImpl();
    };


    // Stops the sound.  If bForceNow is true the sound will stop ASAP, 
    // possibly popping.  If it is false some sounds may play a trail-off 
    // sound or fade away.  
    HRESULT Stop(bool bForceNow = false)
    {
        m_bLooping = false;
        return S_OK;
    };

    // returns S_OK if the sound is currently playing
    HRESULT IsPlaying()
    {
        return m_bPlaying ? S_OK : S_FALSE;
    };

    // Gets an event which fires when the sound finishes playing (for any 
    // reason)
    IEventSource* GetFinishEventSource()
    {
        return m_peventsourceFinished;
    };

    // Gets an interface for tweaking the sound, if supported, nullptr otherwise.
    virtual TRef<ISoundTweakable> GetISoundTweakable()
    {
        return this;
    };

    // Gets an interface for tweaking the sound, if supported, nullptr otherwise.
    virtual TRef<ISoundTweakable3D> GetISoundTweakable3D()
    {
        return this;
    };

    // update the sound, return true if we should keep it around.
    bool Update()
    {
        if (m_bPlaying)
        {
            m_bPlaying = m_bLooping && (!m_psource || m_psource->IsPlaying());

            if (!m_bPlaying)
                m_peventsourceFinished->Trigger();
        }

        return m_bPlaying;
    };


    //
    // ISoundTweakable
    //

    // Sets the gain, from 0 to -100 dB
    HRESULT SetGain(float fGain)
    {
        return S_OK;
    }

    // Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
    // and 2.0 is twice normal speed.  
    HRESULT SetPitch(float fPitch)
    {
        return S_OK;
    }

    // sets the priority - used as a addition to volume when choosing which 
    // sounds are most important to play.
    virtual HRESULT SetPriority(float fPriority)
    {
        return S_OK;
    }


    //
    // ISoundTweakable3D 
    //

    // toggles 3D Positioning on and off for the given sound.
    HRESULT Set3D(bool b3D)
    {
        return S_OK;
    }

    // Sets the distance at which the sound will be at max volume.  This
    // effects how quickly the sound drops off with distance.  
    HRESULT SetMinimumDistance(float fMinimumDistance)
    {
        return S_OK;
    }

    // Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
    // normal levels, outside of which it fades down by fOutsideGain 
    // (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
    HRESULT SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain)
    {
        return S_OK;
    }
};


//
// A stubbed version of the ISoundEngine interface
//
class DummySoundEngineImpl : public ISoundEngine, public ISoundBufferSource
{
    TList<TRef<DummySoundInstance> > m_listPlayingSounds;
    TRef<IntegerEventSourceImpl> m_peventsourceUpdate;
    DWORD m_dwLastUpdateTime;
    TRef<ISoundBufferSource> m_pBufferSourceDelegate;

public:

    // constructor
    DummySoundEngineImpl()
    {
        m_peventsourceUpdate = new IntegerEventSourceImpl();
        m_dwLastUpdateTime = timeGetTime(); 
        m_pBufferSourceDelegate = new SoundBufferSourceDelegate(this);
    }

    // destructor
    ~DummySoundEngineImpl()
    {
    }

    // Rebuild the sound stage to reflect any recent changes in sound
    virtual HRESULT Update()
    {
        DWORD dwUpdateTime = timeGetTime();
        DWORD dwUpdatePeriod = dwUpdateTime - m_dwLastUpdateTime; 

        m_peventsourceUpdate->Trigger(dwUpdatePeriod);

        // loop through the sounds, deleting those that are no longer playing
        TList<TRef<DummySoundInstance> >::Iterator iterSound(m_listPlayingSounds);

        while (!iterSound.End())
        {
            if (iterSound.Value()->Update())
            {
                iterSound.Next();
            }
            else
            {
                iterSound.Remove();
            }
        }

        m_dwLastUpdateTime = dwUpdateTime;

        return S_OK;
    };

    // Gets a buffer source for this object (not guarenteed to keep the sound 
    // engine alive due to circular reference problems)
    virtual ISoundBufferSource* GetBufferSource()
    {
        return m_pBufferSourceDelegate;
    };

    // Gets the number of virtual sound buffers that are playing at a given 
    // moment.  (no guarantees on how this number changes - it's just a perf.
    // number to use.)
    virtual HRESULT GetNumPlayingVirtualBuffers(int& nBuffers)
    {
        nBuffers = m_listPlayingSounds.GetCount();

        return S_OK;
    };

    //
    // General environment functions
    //

    // Sets the listener to use for the current sounds
    HRESULT SetListener(ISoundListener* plistener)
    {
        TRef<ISoundListener> plistenerRef = plistener; // cycle the ref count
        return S_OK;
    };

    // Sets a general quality of playback (CPU time vs. fidelity)
    HRESULT SetQuality(Quality quality)
    {
        return S_OK;
    };

    // Allows/disallows hardware acceleration.
    HRESULT EnableHardware(bool bEnable)
    {
        return S_OK;
    }

    // Sets the conversion from game units to meters
    HRESULT SetDistanceFactor(float fMetersPerUnit)
    {
        return S_OK;
    }

    // Sets the rolloff factor, where 1.0 is the real world attenuation with 
    // distance, 2.0 is twice the attenuation of the real world, etc..
    HRESULT SetRolloffFactor(float fRolloffFactor)
    {
        return S_OK;
    }

    // Sets the doppler factor, where 1.0 is real-world doppler
    HRESULT SetDopplerFactor(float fDopplerFactor)
    {
        return S_OK;
    }


    //
    // ISoundBufferSource
    //

    // Creates a static sound buffer of the given wave file.  If bLooping is 
    // true, the sound will loop until stopped.
    virtual HRESULT CreateStaticBuffer(TRef<ISoundInstance>& psoundNew, 
        ISoundPCMData* pcmdata, bool bLooping, ISoundPositionSource* psource = nullptr)
    {
        DummySoundInstance* dummySound = new DummySoundInstance(bLooping, psource);
        psoundNew = (ISoundInstance*)dummySound;
        m_listPlayingSounds.PushEnd(dummySound);
        return S_OK;
    };

    // Creates a sound buffer with a loop in the middle.  The sound will play
    // the start sound, play the loop sound until it gets a soft stop, then
    // play the rest of the sound.  
    virtual HRESULT CreateASRBuffer(TRef<ISoundInstance>& psoundNew, 
        ISoundPCMData* pcmdata, unsigned uLoopStart, unsigned uLoopLength, 
        ISoundPositionSource* psource = nullptr)
    {
        DummySoundInstance* dummySound = new DummySoundInstance(true, psource);
        psoundNew = (ISoundInstance*)dummySound;
        m_listPlayingSounds.PushEnd(dummySound);
        return S_OK;
    };

    // Gets an event which fires each time update is called.  This can be used
    // for some of the trickier sounds that change with time.
    virtual IIntegerEventSource* GetUpdateEventSource()
    {
        return m_peventsourceUpdate;
    };
};


// create a dummy sound engine
HRESULT CreateDummySoundEngine(TRef<ISoundEngine>& psoundengine)
{
    psoundengine = new DummySoundEngineImpl();

    return S_OK;
}

}
