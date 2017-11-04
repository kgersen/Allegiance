#pragma once
//
// ds3dvirtualbuffer.h
//
// Classes representing virtual DirectSound3D buffers that may or may not have
// a real DirectSoundBuffer associated with them at any given moment.
//

#include "ds3dbuffer.h"
#include "soundbase.h"

namespace SoundEngine {

//
// Tracks a buffer position over the lifetime of a virtual sound.  This is 
// designed to lighter-weight than an actual sound buffer, because the sound
// it represents may never be played.  It's designed to be accurate, however,
// so that a buffer created using this information can start at the appropriate
// point.  
//
class BufferPositionTracker
{
    // the current position in the sound
    DWORD m_dwPosition;

    // the length of the buffer
    DWORD m_dwLength;

    // the sample rate of the sound, in bytes per second.
    DWORD m_dwBytesPerSec;

    // a mask of valid position bits (since a position must start on a sample 
    // boundary)
    DWORD m_dwPositionMask;

    // true iff end has been called on this
    bool m_bEnding;


    //
    // Non-ASR data
    //

    // true if this is looping
    bool m_bLooping;


    //
    // ASR data
    //

    // true iff this is an ASR sound.
    bool m_bASR;

    // the starting position of the sustain loop.
    DWORD m_dwLoopStart;

    // the length of the sustain loop
    DWORD m_dwLoopLength;


public:
    
    // Create a position tracker for a sound which plays until it reaches the 
    // end (for non-looping sounds) or has stop called upon it.  
    BufferPositionTracker(ISoundPCMData* pdata, bool bLooping = false);

    // Creates a position tracker for an ASR buffer.
    BufferPositionTracker(ISoundPCMData* pdata, 
            DWORD dwLoopStart, DWORD dwLoopLength);


    //
    // Events that the buffer tracker needs to know about
    //

    // the sound has been given a stop.  
    void Stop(bool bForceNow = false);

    // the given number of miliseconds have elapsed
    void AddTime(DWORD dwElapsedTime);

    // Resets the position to the given one, presumably retrieved from a 
    // playing buffer.
    void SetPosition(DWORD dwPosition);


    //
    // Info a SoundInstance may need.  
    //

    // true iff this has stopped playing.
    bool IsStopped()
    {
        return m_dwPosition >= m_dwLength;
    };

    // is the sound in the process of stopping?
    bool IsStopping()
    {
        return m_bASR && m_bEnding;
    }

    // what is the current offset in the source buffer?
    DWORD GetPosition()
    {
        return m_dwPosition & m_dwPositionMask;
    }


    //
    // ASR info
    //

    // is this tracking an ASR sound?
    bool IsASR()
    {
        return m_bASR;
    }

    // get the loop offset in the ASR sound
    DWORD GetLoopOffset()
    {
        ZAssert(m_bASR);
        return m_dwLoopStart;
    }

    // get the loop length in an ASR sound
    DWORD GetLoopLength()
    {
        ZAssert(m_bASR);
        return m_dwLoopLength;
    }


    //
    // non-ASR info
    //

    // true if this is a looping sound
    bool IsLooping()
    {
        ZAssert(!m_bASR);
        return m_bLooping;
    }
};



//
// A sound instance implementation representing a single virtual DirectSound3D
// sound buffer.  This may or may not have an actual direct sound buffer 
// associated with it at any time - the sound engine controls that aspect of 
// things.
//
class DSVirtualSoundBuffer : public ISoundInstance, public ISoundTweakable  SoundDebugImpl
{
protected:

    // An event source which is triggered when a sound is finished (often NULL)
    TRef<EventSourceImpl> m_peventsourceFinished;

    // the current buffer position of the sound
    BufferPositionTracker m_bufferPositionTracker;

    // The data source used to create this sound
    TRef<ISoundPCMData> m_pdata;

    // The current sound buffer (possibly NULL)
    TRef<DS3DSoundBuffer> m_pds3dbuffer;

    // the aproximate age of this sound in seconds, used for priority purposes
    float m_fAge;

    // has this sound started?
    bool m_bStarted;

    // has this sound stopped?
    bool m_bStopped;

    // are we trying to stop this sound?
    bool m_bRetryStop;

    // is the buffer currently playing?
    bool m_bBufferPlaying;

    // is the buffer currently audible?
    bool m_bAudible;

    // what is the quality of the sound currently?
    ISoundEngine::Quality m_quality;

    // can the current buffer play in hardware, if available?
    bool m_bAllowHardware;

    // the dynamic priority of the sound.  The only meaning this number has is
    // in relation to other priorities, where the higher number is the higher
    // priority.  
    float m_fDynamicPriority;


    //
    // cached settings
    //

    // The current priority of the sound
    float m_fPriority;

    // The current gain of the sound
    float m_fGain;

    // the current pitch of the sound
    float m_fPitch;


    // prepare a sound buffer with the given quality and 3D support using the
    // direct sound object pointed to by pDirectSound.
	// mdvalley: DirectSound8
    virtual HRESULT PrepareBuffer(IDirectSound* pDirectSound, 
        ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D);
	virtual HRESULT PrepareBuffer8(IDirectSound8* pDirectSound,
		ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D);

public:

    // Creates a virtual sound buffer for a normal sound, possibly looping
    DSVirtualSoundBuffer(ISoundPCMData* pdata, bool bLooping);

    // Creates a virtual sound buffer for an ASR sound
    DSVirtualSoundBuffer(ISoundPCMData* pdata, DWORD dwLoopOffset, DWORD dwLoopLength);


    //
    // Accessors
    //
        
    // returns true iff the buffer is no longer playing (and can be discarded)
    bool IsStopped()
    {
        return m_bStopped;
    };

    // Returns true if this has a playing physical buffer
    bool HasPlayingBuffer()
    {
        return m_bBufferPlaying;
    };

    // Returns true if this is audible at the moment
    bool IsAudible()
    {
        return m_bAudible;
    };

    // Gets the dynamic priority of the sound.  The only meaning this number 
    // has is in relation to other priorities, where the higher number is the
    // higher priority.  
    float GetDynamicPriority()
    {
        return m_fDynamicPriority;
    };



    // Recalculates the sounds position, loudness, whether it's playing, etc..
    virtual HRESULT Update(DWORD dwTimeElapsed, 
            const Vector& vectListenerPosition, float fRolloffFactor);

    // Creates and starts a real dsound buffer for this sound
	// mdvalley: DirectSound8
    virtual HRESULT StartBuffer8(IDirectSound8* pDirectSound, 
        ISoundEngine::Quality quality, bool bAllowHardware);
	virtual HRESULT StartBuffer(IDirectSound* pDirectSound,
		ISoundEngine::Quality quality, bool bAllowHardware);

    // forcibly stops the given buffer.
    HRESULT StopBuffer();


    //
    // ISoundTweakable interface
    //

    // Sets the gain, from 0 to -100 dB
    virtual HRESULT SetGain(float fGain);

    // Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
    // and 2.0 is twice normal speed.  
    virtual HRESULT SetPitch(float fPitch);

    // sets the priority - used as a addition to volume when choosing which 
    // sounds are most important to play.
    virtual HRESULT SetPriority(float fPriority);


    //
    // ISoundInstance interface
    //

    // Stops the sound.  If bForceNow is true the sound will stop ASAP, 
    // possibly popping.  If it is false some sounds may play a trail-off 
    // sound or fade away.  
    virtual HRESULT Stop(bool bForceNow = false);

    // returns S_OK if the sound is currently playing, S_FALSE otherwise.
    virtual HRESULT IsPlaying();

    // Gets an event which fires when the sound finishes playing (for any 
    // reason)
    virtual IEventSource* GetFinishEventSource();

    // Gets an interface for tweaking the sound, if supported, NULL otherwise.
    virtual TRef<ISoundTweakable> GetISoundTweakable();

    // Gets an interface for tweaking the sound, if supported, NULL otherwise.
    virtual TRef<ISoundTweakable3D> GetISoundTweakable3D();


    //
    // ISoundDebugDump
    //

#ifdef _DEBUG
    // return a human-readable description of the object, prepending
    // strIndent to the beginning of each line.  
    ZString DebugDump(const ZString& strIndent = "");
#endif
};


class DS3DVirtualSoundBuffer : public DSVirtualSoundBuffer, public ISoundTweakable3D
{
private:

    //
    // cached settings
    //

    // The source of this sound, position-wise
    TRef<ISoundPositionSource> m_pposSource;

    // is 3D on at the moment?
    bool m_b3D;

    // have we decided to play in 3D at the moment?
    bool m_bPlayingIn3D;

    // the current maximum distance of the sound
    float m_fMinimumDistance;

    // the current sound cone
    float m_fInnerAngle, m_fOuterAngle, m_fOutsideGain;

    // the last known position of the sound
    Vector m_vectPosition;

    // the last known velocity of the sound
    Vector m_vectVelocity;

    // the last known orientation of the sound
    Vector m_vectOrientation;

    // whether or not the last known info is relative to the listener
    bool m_bListenerRelative;

public:

    // constructs a simple 3D capable sound
    DS3DVirtualSoundBuffer(ISoundPCMData* pdata, bool bLooping, 
            TRef<ISoundPositionSource> psource);

    // constructs an ASR 3D capable sound
    DS3DVirtualSoundBuffer(ISoundPCMData* pdata, DWORD dwLoopOffset, 
            DWORD dwLoopLength, TRef<ISoundPositionSource> psource);

    //
    // DSVirtualSoundBuffer overides
    //


protected:

    // prepare a sound buffer with the given quality and 3D support using the
    // direct sound object pointed to by pDirectSound.
	// mdvalley: DirectSound8
    virtual HRESULT PrepareBuffer(IDirectSound* pDirectSound, 
        ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D);
	virtual HRESULT PrepareBuffer8(IDirectSound8* pDirectSound,
		ISoundEngine::Quality quality, bool bAllowHardware, bool bSupport3D);

public:

    // Recalculates the sounds position, loudness, whether it's playing, etc..
    virtual HRESULT Update(DWORD dwTimeElapsed, 
            const Vector& vectListenerPosition, float fRolloffFactor);


    //
    // ISoundTweakable3D interface
    //

    // toggles 3D Positioning on and off for the given sound.
    virtual HRESULT Set3D(bool b3D);

    // Sets the distance at which the sound will be at max volume.  This
    // effects how quickly the sound drops off with distance.  
    virtual HRESULT SetMinimumDistance(float fMinimumDistance);

    // Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
    // normal levels, outside of which it fades down by fOutsideGain 
    // (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
    virtual HRESULT SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain);


    //
    // ISoundTweakable interface
    //

    // Sets the gain, from 0 to -100 dB
    virtual HRESULT SetGain(float fGain);

    // Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
    // and 2.0 is twice normal speed.  
    virtual HRESULT SetPitch(float fPitch);

    // sets the priority - used as a addition to volume when choosing which 
    // sounds are most important to play.
    virtual HRESULT SetPriority(float fPriority);

    // Gets an interface for tweaking the sound, if supported, NULL otherwise.
    virtual TRef<ISoundTweakable> GetISoundTweakable();

    // Gets an interface for tweaking the sound, if supported, NULL otherwise.
    virtual TRef<ISoundTweakable3D> GetISoundTweakable3D();
};

};
