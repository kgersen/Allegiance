/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	playsoundaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "playsoundaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "PlaySoundAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  PlaySoundAction::PlaySoundAction (SoundID soundID) : 
    m_soundID (soundID),
    m_soundInstance (0),
    m_bHasStarted (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  PlaySoundAction::~PlaySoundAction (void)
    {
        m_soundInstance = 0;
    }

    //------------------------------------------------------------------------------
    void        PlaySoundAction::Execute (void)
    {
        if (m_soundID != NA)
        {
            /*ThingSitePrivate*           pThingSite = static_cast<ThingSitePrivate*> (trekClient.GetShip ()->GetThingSite());
            TRef<ISoundPositionSource>  pSource = pThingSite ? pThingSite->GetSoundSource () : NULL;*/
            m_soundInstance = trekClient.StartSound (m_soundID); //use NULL instead of pSource, so the sounds don't stop on cluster change
        }
        m_bHasStarted = true;
    }

    //------------------------------------------------------------------------------
    void        PlaySoundAction::Stop (void)
    {
        if ((m_soundID != NA) and m_bHasStarted and (m_soundInstance->IsPlaying () == S_OK))
            m_soundInstance->Stop (true);
    }

    //------------------------------------------------------------------------------
    bool        PlaySoundAction::HasStarted (void) const
    {
        return (m_bHasStarted);
    }

    //------------------------------------------------------------------------------
    bool        PlaySoundAction::HasStopped (void) const
    {
        if (m_soundID == NA)
            return true;
        return (m_soundInstance->IsPlaying () == S_OK) ? false : true;
    }

    //------------------------------------------------------------------------------
}
