//
// ds3dengine.h
//
// Classes representing a DirectSound3D implementation of a sound engine
//

#include <dsound.h>
#include <list>
#include <zadapt.h>

#include "ds3dvirtualbuffer.h"
#include "soundengine.h"

namespace SoundEngine {

//
// A sound engine implementation using direct sound 3D that handles basic
// resource management.  
//
class DS3DSoundEngine : public ISoundEngine, public ISoundBufferSource  SoundDebugImpl
{

    // the currently chosen DirectSound implementation
	// mdvalley: Now uses DirectSound8 interface.
    TRef<IDirectSound8> m_pDirectSound8;
	TRef<IDirectSound> m_pDirectSound;

    // the primary buffer of this DirectSound instance
	// mdvalley: The DX SDK says to keep the old interface for the primary
	// buffer. I'm not arguing.
    TRef<IDirectSoundBuffer> m_pPrimaryBuffer;

    // the capabilities of the chosen DirectSound implementation
    DSCAPS m_dscaps;


    // the listener used to render sound
    TRef<ISoundListener> m_plistener;

    // the coresponding DirectSound3D listener for this instance.
    TRef<IDirectSound3DListener> m_pDSListener;

    // the rolloff factor, where 1.0f is the real-world rolloff
    float m_fRolloffFactor;

    // the doppler factor, where 1.0f is the real-world doppler
    float m_fDopplerFactor;

    // the distance factor, where 1.0f is the real-world distance factor
    float m_fDistanceFactor;


    // the update event source, triggered on each call to update.
    TRef<IntegerEventSourceImpl> m_peventsourceUpdate;

    // a delegate buffer source for this (needed to avoid circular refs)
    TRef<ISoundBufferSource> m_pBufferSourceDelegate;

    // a list of currently playing virtual sounds
    typedef std::list<ZAdapt<TRef<DSVirtualSoundBuffer> > > VirtualSoundList;
    VirtualSoundList m_listActiveSounds;

    // the time of the last call to update
    DWORD m_dwLastUpdateTime;

    // the sound quality to use
    Quality m_quality;

    // whether to allow hardware acceleration
    bool m_bAllowHardware;

    // the desired number of sounds to play at once.  This number may reserve 
    // a few voices for fading sounds out.
    int m_nNumBuffersDesired;

    // the maximum number of sounds we can play at once, including sounds that 
    // are fading in or out.
    int m_nNumBuffersMax;

    // dumps the capablilites of this DirectSound implementation to the 
    // debug output.
    void DumpCaps();

    // Sets the format of the primary buffer to the given sample rate, number 
    // of bits, and number of channels
    HRESULT SetPrimaryBufferFormat(int nSampleRate, int nNumberOfBits, int nChannels);

    // updates the listener position, orientation, etc. for direct sound 3D.
    HRESULT UpdateListener();

    // Is this an error worth killing a virtual sound for, or just something 
    // transitory?  For example, we may lose a sound buffer when being swapped 
    // to the background, but we want the sound to continue playing when we 
    // are swapped back in.  
    static bool IsSeriousError(HRESULT hr);

    // provides a complete ordering of virtual sound buffers by priority.
    struct SoundPriorityCompare
    {
        typedef DSVirtualSoundBuffer* first_argument_type;
        typedef DSVirtualSoundBuffer* second_argument_type;
        typedef bool result_type;

        bool operator () (DSVirtualSoundBuffer* psound1, DSVirtualSoundBuffer* psound2) const
        {
            return psound1->GetDynamicPriority() > psound2->GetDynamicPriority()
                || (psound1->GetDynamicPriority() == psound2->GetDynamicPriority()
                    && psound1 > psound2);
        }
    };

    template <class T>
    struct RefToPtr
    {
        T* operator () (TRef<T>& rt)
        {
            return rt;
        }
    };

public:

    // Constructor - the real initialization is done in init.
    DS3DSoundEngine();

    ~DS3DSoundEngine();


    // Basic initialization.  This was pulled out of the constructor so that we
    // can return error values.
    HRESULT Init(HWND hwnd, bool bUseDSound8);


    //
    // ISoundEngine Interface
    //

    // Rebuild the sound stage to reflect any recent changes in sound
    virtual HRESULT Update();

    // Gets a buffer source for this object (not guarenteed to keep the sound 
    // engine alive due to circular reference problems)
    virtual ISoundBufferSource* GetBufferSource();

    // Gets the number of virtual sound buffers that are playing at a given 
    // moment.  (no guarantees on how this number changes - it's just a perf.
    // number to use.)
    virtual HRESULT GetNumPlayingVirtualBuffers(int& nBuffers);


    // Sets a general quality of playback (CPU time vs. fidelity)
    HRESULT SetQuality(Quality quality);

    // Allows/disallows hardware acceleration.
    HRESULT EnableHardware(bool bEnable);

    // Sets the listener to use for the current sounds
    virtual HRESULT SetListener(ISoundListener* plistener);

    // Sets the conversion from game units to meters
    virtual HRESULT SetDistanceFactor(float fMetersPerUnit);

    // Sets the rolloff factor, where 1.0 is the real world attenuation with 
    // distance, 2.0 is twice the attenuation of the real world, etc..
    virtual HRESULT SetRolloffFactor(float fRolloffFactor);

    // Sets the doppler factor, where 1.0 is real-world doppler
    virtual HRESULT SetDopplerFactor(float fDopplerFactor);



    //
    // ISoundBufferSource
    //

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


    //
    // ISoundDebugDump
    //

#ifdef _DEBUG
    // return a human-readable description of the object, prepending
    // strIndent to the beginning of each line.  
    ZString DebugDump(const ZString& strIndent = "");
#endif
};

};
