#pragma once
//
// redbooksound.h
//
// SoundEngine support for redbook audio
//

#include "soundbase.h"

namespace SoundEngine {

//
// A generic disk player wrapper
//

class IDiskPlayer : public ISoundObject
{
public:
    // plays one track
    virtual HRESULT Play(int nTrack) = 0;

    // stops the CD player
    virtual HRESULT Stop() = 0;

    // returns S_OK if the CD player is playing, S_FALSE otherwise
    virtual HRESULT IsPlaying() = 0;

    // returns the current track of the CD player
    virtual HRESULT GetCurrentTrack(int& nTrack) = 0;

    // sets the gain on the CD player, from 0 to -100 dB
    virtual HRESULT SetGain(float fGain) = 0;
};


//
// Helper functions
//

// Creates a new disk player object (CD, minidisk, etc.).  strDevice can be 
// empty (choose any device), a path ("E:\"), or a volume label.  
HRESULT CreateDiskPlayer(TRef<IDiskPlayer>& pdiskplayer, const ZString& strDevice = "");

// Creates a new disk player object that only has stubs for each of the calls
HRESULT CreateDummyDiskPlayer(TRef<IDiskPlayer>& pdiskplayer);

// creates a sound template representing a redbook audio track
HRESULT CreateRedbookSoundTemplate(TRef<ISoundTemplate>& pstDest, TRef<IDiskPlayer> diskPlayer, int nTrack);

};
