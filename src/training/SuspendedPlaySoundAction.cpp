/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	suspendedplaysoundaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "suspendedplaysoundaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SuspendedPlaySoundAction.h"
#include    "SuspendedSoundFinishedCondition.h"
#include    "TrainingMission.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern TrainingMission*    g_pMission;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SuspendedPlaySoundAction::SuspendedPlaySoundAction (SoundID soundID, bool bDeleteWhenDone) : 
    PlaySoundAction (soundID),
    m_bDeleteWhenDone (bDeleteWhenDone)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SuspendedPlaySoundAction::~SuspendedPlaySoundAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SuspendedPlaySoundAction::Execute (void)
    {
        if (m_soundID != NA)
        {
            g_pMission->AddWaitCondition (new SuspendedSoundFinishedCondition (this, m_bDeleteWhenDone));
            PlaySoundAction::Execute ();
        }
    }

    //------------------------------------------------------------------------------
}
