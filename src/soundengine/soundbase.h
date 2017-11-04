// 
// soundbase.h
//
// General interfaces for sounds
//

#pragma once

#include <event.h>
#include <vector.h>
#include <tref.h>

namespace SoundEngine {

// choose a base class we can change if we decide to, say, go to COM+
class ISoundObject : public IObject
{
};


//
// Tweakable general sound properties
//
class ISoundTweakable : public ISoundObject
{
public:
    // Sets the gain, from 0 to -100 dB
    virtual HRESULT SetGain(float fGain) = 0;

    // Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
    // and 2.0 is twice normal speed.  
    virtual HRESULT SetPitch(float fPitch) = 0;

    // sets the priority - used as a addition to volume when choosing which 
    // sounds are most important to play.
    virtual HRESULT SetPriority(float fPriority) = 0;
};


//
// Tweakable 3D sound properties
// 
class ISoundTweakable3D : public ISoundTweakable
{
public:
    // toggles 3D Positioning on and off for the given sound.
    virtual HRESULT Set3D(bool b3D) = 0;

    // Sets the distance at which the sound will be at max volume.  This
    // effects how quickly the sound drops off with distance.  
    virtual HRESULT SetMinimumDistance(float fMinimumDistance) = 0;

    // Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
    // normal levels, outside of which it fades down by fOutsideGain 
    // (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
    virtual HRESULT SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain) = 0;
};


//
// Playback controls
//
class ISoundInstance : public ISoundObject
{
public:
    // Stops the sound.  If bForceNow is true the sound will stop ASAP, 
    // possibly popping.  If it is false some sounds may play a trail-off 
    // sound or fade away.  
    virtual HRESULT Stop(bool bForceNow = false) = 0;

    // returns S_OK if the sound is currently playing, S_FALSE otherwise.
    virtual HRESULT IsPlaying() = 0;

    // Gets an event which fires when the sound finishes playing (for any 
    // reason)
    virtual IEventSource* GetFinishEventSource() = 0;

    // Gets an interface for tweaking the sound, if supported, NULL otherwise.
    virtual TRef<ISoundTweakable> GetISoundTweakable() = 0;
    virtual TRef<ISoundTweakable3D> GetISoundTweakable3D() = 0;
};


//
// Provides positioning information for a sound emiter or listener
//
class ISoundPositionSource : public ISoundObject
{
public:
    // Gets the position of the sound in space
    virtual HRESULT GetPosition(Vector& vectPosition) = 0;
    
    // Gets the velocity of the sound in space
    virtual HRESULT GetVelocity(Vector& vectVelocity) = 0;
    
    // Gets the orientation of the sound in space, used for sound cones.
    virtual HRESULT GetOrientation(Vector& vectOrientation) = 0;

    // Returns S_OK if the position, velocity and orientation reported are 
    // relative to the listener, S_FALSE otherwise.  
    virtual HRESULT IsListenerRelative() = 0;

    // Returns S_OK if this source is still playing the sound, S_FALSE 
    // otherwise.  This might be false if a sound emitter is destroyed, for 
    // example, in which case the sound might fade out.  Once it returns
    // S_FALSE once, it should never return S_OK again.
    virtual HRESULT IsPlaying() = 0;
};


//
// Provides positioning info for a listener of a sound
//
class ISoundListener : public ISoundPositionSource
{
public:
    // get the "up" vector for the listener
    virtual HRESULT GetUpDirection(Vector& vectUp) = 0;
};


//
// A source of PCM wave data
//
class ISoundPCMData : public ISoundObject
{
public:
    // Gets the number of channels in the data
    virtual unsigned GetNumberOfChannels() = 0;

    // Gets the number of bits per sample
    virtual unsigned GetBitsPerSample() = 0;

    // Gets the default frequency (in samples per second) of the data
    virtual unsigned GetSampleRate() = 0;

    // Gets the size of the data
    virtual unsigned GetSize() = 0;

    // Copies the specified portion of the data
    virtual void GetData(void* dest, unsigned nOffset, unsigned nLength) = 0;

    // calculates the playback rate in bytes per second.
    DWORD GetBytesPerSample()
    {
        return GetBitsPerSample()/8 * GetNumberOfChannels();
    }

    // calculates the number of bytes per sample
    DWORD GetBytesPerSec()
    {
        return GetBytesPerSample() * GetSampleRate();
    }
};


//
// A creation point for raw sound buffers
//
class ISoundBufferSource : public ISoundObject
{
public:
    // Creates a static sound buffer of the given wave file.  If bLooping is 
    // true, the sound will loop until stopped.
    virtual HRESULT CreateStaticBuffer(TRef<ISoundInstance>& psoundNew, 
        ISoundPCMData* pcmdata, bool bLooping, ISoundPositionSource* psource = NULL) = 0;

    // Creates a sound buffer with a loop in the middle.  The sound will play
    // the start sound, play the loop sound until it gets a soft stop, then
    // play the rest of the sound.  
    virtual HRESULT CreateASRBuffer(TRef<ISoundInstance>& psoundNew, 
        ISoundPCMData* pcmdata, unsigned uLoopStart, unsigned uLoopLength, 
        ISoundPositionSource* psource = NULL) = 0;

    // Gets an event which fires each time update is called with the number of
    // milliseconds elapsed since the last update.  This can be used for some 
    // of the trickier sounds that change with time.
    virtual IIntegerEventSource* GetUpdateEventSource() = 0;
};


//
// The general provider/manager of sound buffers and the sound stage.
//
class ISoundEngine : public ISoundObject
{
public:
    // Rebuild the sound stage to reflect any recent changes in sound
    virtual HRESULT Update() = 0;

    // Gets a buffer source for this object (not guarenteed to keep the sound 
    // engine alive due to circular reference problems)
    virtual ISoundBufferSource* GetBufferSource() = 0;

    // Gets the number of virtual sound buffers that are playing at a given 
    // moment.  (no guarantees on how this number changes - it's just a perf.
    // number to use.)
    virtual HRESULT GetNumPlayingVirtualBuffers(int& nBuffers) = 0;


    //
    // General environment functions
    //

    // Sets a general quality of playback (CPU time vs. fidelity)
    enum Quality { minQuality, midQuality, maxQuality };
    virtual HRESULT SetQuality(Quality quality) = 0;

    // Allows/disallows hardware acceleration.
    virtual HRESULT EnableHardware(bool bEnable) = 0;

    // Sets the listener to use for the current sounds
    virtual HRESULT SetListener(ISoundListener* plistener) = 0;

    // Sets the conversion from game units to meters
    virtual HRESULT SetDistanceFactor(float fMetersPerUnit) = 0;

    // Sets the rolloff factor, where 1.0 is the real world attenuation with 
    // distance, 2.0 is twice the attenuation of the real world, etc..
    virtual HRESULT SetRolloffFactor(float fRolloffFactor) = 0;

    // Sets the doppler factor, where 1.0 is real-world doppler
    virtual HRESULT SetDopplerFactor(float fDopplerFactor) = 0;
};


//
// provides a template for creating new sound instances
// 
class ISoundTemplate : public ISoundObject
{
public:
    // Creates a new instance of the given sound
    virtual HRESULT CreateSound(TRef<ISoundInstance>& psoundNew, 
        ISoundBufferSource* pbufferSource, ISoundPositionSource* psource = NULL) = 0;
};


//
// Helper functions
//

// create a sound engine for the default sound device
HRESULT CreateSoundEngine(TRef<ISoundEngine>& psoundengine, HWND hwnd, bool bUseDSound8);

// create a dummy sound engine
HRESULT CreateDummySoundEngine(TRef<ISoundEngine>& psoundengine);


//
// Debugging facilities
//

#ifdef _DEBUG
// an object which can provide debug info about itself
class ISoundDebugDump
{
public:
    // return a human-readable description of the object, prepending
    // strIndent to the beginning of each line.  
    virtual ZString DebugDump(const ZString& strIndent = "") = 0;
};

// a helper function for trying to dump an arbitrary object
ZString SoundDebugDump(ISoundObject* pobject, const ZString& strIndent = "");

#define SoundDebugImpl , public ISoundDebugDump

#else

#define SoundDebugImpl

#endif
}
