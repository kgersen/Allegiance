//
// soundutil.h
//
// Useful utility classes/functions
//

#pragma once

#include <vector.h>

#include "soundbase.h"

namespace SoundEngine {

// returns a PCM data object for a given wave file.
HRESULT LoadWaveFile(TRef<ISoundPCMData>& pdata, const ZString& strFilename, const bool convertMono); //AEM - Added convertMono parameter (7.4.07)

// returns a dummy PCM data object.
HRESULT CreateDummyPCMData(TRef<ISoundPCMData>& pdata);

//
// a generic implementation of a sound source.  Its position can be set and 
// retrieved, so it can be used if you want to manually set the position and
// velocity of a sound instead of having it polled.  
//
class GenericSoundSource : public ISoundPositionSource
{
private:
    Vector m_vectPosition;
    Vector m_vectVelocity;
    Vector m_vectOrientation;
    bool m_bListenerRelative;
    bool m_bIsPlaying;

public:
    // constructors
    GenericSoundSource();
    GenericSoundSource(
        const Vector& vectPosition, 
        const Vector& vectVelocity = Vector(0, 0, 0), 
        const Vector& vectOrientation = Vector(0, 0, 1),
        bool          bListenerRelative = false
        );

    //
    // Position setting functions
    //

    // Sets the position of the sound in space
    void SetPosition(const Vector& vectPosition);
    
    // Sets the velocity of the sound in space
    void SetVelocity(const Vector& vectVelocity);
    
    // Sets the orientation of the sound in space, used for sound cones.
    void SetOrientation(const Vector& vectOrientation);

    // Sets whether or not the sound position source is listener relative
    void SetIsListenerRelative(bool bListenerRelative);

    // Stops any sounds on this source
    void Stop();

    
    //
    // ISoundPositionSource
    //

    // Gets the position of the sound in space
    HRESULT GetPosition(Vector& vectPosition);
    
    // Gets the velocity of the sound in space
    HRESULT GetVelocity(Vector& vectVelocity);
    
    // Gets the orientation of the sound in space, used for sound cones.
    HRESULT GetOrientation(Vector& vectOrientation);

    // Returns S_OK if the position, velocity and orientation reported are 
    // relative to the listener, S_FALSE otherwise.  
    HRESULT IsListenerRelative();

    // Returns S_OK if this source is still playing the sound, S_FALSE 
    // otherwise.  This might be false if a sound emitter is destroyed, for 
    // example, in which case the sound might fade out.  Once it returns
    // S_FALSE once, it should never return S_OK again.
    HRESULT IsPlaying();
};



// a generic wrapper for an ISoundPositionSource which just delegates everything to
// the object it wraps.
// (not very useful by itself, but useful as a base class)
class SoundPositionSourceWrapper : public ISoundPositionSource
{
protected:
    // the object which is wrapped
    TRef<ISoundPositionSource> m_pBase;

public:
    // constructor
    SoundPositionSourceWrapper(ISoundPositionSource* pBase) :
            m_pBase(pBase) {};

    //
    // ISoundPositionSource
    //

    // Gets the position of the sound in space
    HRESULT GetPosition(Vector& vectPosition);
    
    // Gets the velocity of the sound in space
    HRESULT GetVelocity(Vector& vectVelocity);
    
    // Gets the orientation of the sound in space, used for sound cones.
    HRESULT GetOrientation(Vector& vectOrientation);

    // Returns S_OK if this source is still playing the sound, S_FALSE 
    // otherwise.  This might be false if a sound emitter is destroyed, for 
    // example, in which case the sound might fade out.  Once it returns
    // S_FALSE once, it should never return S_OK again.
    HRESULT IsPlaying();
};



//
// a generic implementation of a listener.  Its position can be set and 
// retrieved, so it can be used if you want to manually set the position and
// velocity of the listener instead of having it polled.  
//
class GenericListener : public GenericSoundSource, public ISoundListener
{
private:
    Vector m_vectUp;

public:

    // constructor
    GenericListener();

    // sets the up direction for this listener
    void SetUpDirection(const Vector& vectUp);


    //
    // ISoundListener
    //

    // get the "up" vector for the listener
    HRESULT GetUpDirection(Vector& vectUp);


    //
    // ISoundPositionSource
    //

    // Gets the position of the sound in space
    HRESULT GetPosition(Vector& vectPosition);
    
    // Gets the velocity of the sound in space
    HRESULT GetVelocity(Vector& vectVelocity);
    
    // Gets the orientation of the sound in space, used for sound cones.
    HRESULT GetOrientation(Vector& vectOrientation);

    // Returns S_OK if this source is still playing the sound, S_FALSE 
    // otherwise.  This might be false if a sound emitter is destroyed, for 
    // example, in which case the sound might fade out.  Once it returns
    // S_FALSE once, it should never return S_OK again.
    HRESULT IsPlaying();
};


// a generic wrapper for an ISoundInstance which just delegates everything to
// the object it wraps.
// (not very useful by itself, but useful as a base class)
class SoundInstanceWrapper : public ISoundInstance
{
protected:
    TRef<ISoundInstance> m_pBase;

public:
    SoundInstanceWrapper(ISoundInstance* pBase) :
      m_pBase(pBase) { ZAssert(pBase != NULL); };

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
    virtual TRef<ISoundTweakable3D> GetISoundTweakable3D();
};



// a generic wrapper for an ISoundTweakable which just delegates everything to
// the object it wraps.
// (not very useful by itself, but useful as a base class)
class SoundTweakableWrapper : public ISoundTweakable
{
protected:
    TRef<ISoundTweakable> m_pBase;

public:
    SoundTweakableWrapper(ISoundTweakable* pBase) :
      m_pBase(pBase) { ZAssert(pBase != NULL); };

    // Sets the gain, from 0 to -100 dB
    virtual HRESULT SetGain(float fGain);

    // Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
    // and 2.0 is twice normal speed.  
    virtual HRESULT SetPitch(float fPitch);

    // sets the priority - used as a addition to volume when choosing which 
    // sounds are most important to play.
    virtual HRESULT SetPriority(float fPriority);
};



// a generic wrapper for an ISoundTweakable3D which just delegates everything to
// the object it wraps.
// (not very useful by itself, but useful as a base class)
class SoundTweakable3DWrapper : public ISoundTweakable
{
protected:
    TRef<ISoundTweakable3D> m_pBase;

public:
    SoundTweakable3DWrapper(ISoundTweakable3D* pBase) :
      m_pBase(pBase) { ZAssert(pBase != NULL); };

    // toggles 3D Positioning on and off for the given sound.
    virtual HRESULT Set3D(bool b3D);

    // Sets the distance at which the sound will be at max volume.  This
    // effects how quickly the sound drops off with distance.  
    virtual HRESULT SetMinimumDistance(float fMinimumDistance);

    // Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
    // normal levels, outside of which it fades down by fOutsideGain 
    // (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
    virtual HRESULT SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain);
};


// A sound wrapper which merely records the settings it's given (a useful base
// class for sounds that may wait before playing).
class StubbedTweakableSoundInstance : public ISoundInstance, public ISoundTweakable3D,
    public IIntegerEventSink
{
    // The current priority of the sound
    float m_fPriority;
    bool m_bPrioritySet;

    // The current gain of the sound
    float m_fGain;
    bool m_bGainSet;

    // the current pitch of the sound
    float m_fPitch;
    bool m_bPitchSet;

    // is 3D on at the moment?
    bool m_b3D;
    bool m_b3DSet;

    // the current maximum distance of the sound
    float m_fMinimumDistance;
    bool m_bMinimumDistanceSet;

    // the current sound cone
    float m_fInnerAngle, m_fOuterAngle, m_fOutsideGain;
    bool m_bConeSet;

protected:

    // The source of this sound, position-wise
    TRef<ISoundPositionSource> m_pposSource;

    // this sound has been stopped
    bool m_bStopped;

    // this sound has been stopped with bForceNow set
    bool m_bForcedStop;

    // the "stopped" event source
    TRef<EventSourceImpl> m_peventsourceStopped;

public:

    StubbedTweakableSoundInstance(ISoundPositionSource* pposSource);

    // fires the m_peventsourceStopped event if bHasStopped is true
    virtual void Update(DWORD dwElapsedTime, bool bHasStopped);

    // calls Stop(true) if m_pposSource has stopped. calls Update, 
    // returns IsPlaying()
    virtual bool OnEvent(IIntegerEventSource* pevent, int value);

    // copies any tweaks that have been made to the given sound instance
    void UpdateSound(ISoundInstance* psound);


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
    virtual TRef<ISoundTweakable3D> GetISoundTweakable3D();


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
};


// a delegate for an ISoundTweakable3D which just delegates everything to
// the object it wraps.
class SoundBufferSourceDelegate : public ISoundBufferSource
{
protected:
    ISoundBufferSource* m_pBase;

public:
    SoundBufferSourceDelegate(ISoundBufferSource* pBase) :
      m_pBase(pBase) { ZAssert(pBase != NULL); };

    // Creates a static sound buffer of the given wave file.  If bLooping is 
    // true, the sound will loop until stopped.
    virtual HRESULT CreateStaticBuffer(TRef<ISoundInstance>& psoundNew, 
        ISoundPCMData* pcmdata, bool bLooping, ISoundPositionSource* psource = NULL);

    // Creates a sound buffer with a loop in the middle.  The sound will play
    // the start sound, play the loop sound until it gets a soft stop, then
    // play the rest of the sound.  
    virtual HRESULT CreateASRBuffer(TRef<ISoundInstance>& psoundNew, 
        ISoundPCMData* pcmdata, unsigned uLoopStart, unsigned uLoopLength, 
        ISoundPositionSource* psource = NULL);

    // Gets an event which fires each time update is called.  This can be used
    // for some of the trickier sounds that change with time.
    virtual IIntegerEventSource* GetUpdateEventSource();
};


// a generic wrapper for an ISoundTweakable3D which just delegates everything to
// the object it wraps.
// (not very useful by itself, but useful as a base class)
class SoundBufferSourceWrapper : public SoundBufferSourceDelegate
{
protected:
    TRef<ISoundBufferSource> m_pBaseRef;

public:
    SoundBufferSourceWrapper(ISoundBufferSource* pBase) :
      m_pBaseRef(pBase), SoundBufferSourceDelegate(pBase) {};
};

}
