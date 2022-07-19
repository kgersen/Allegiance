/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	soundfinishedcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "soundfinishedcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SoundFinishedCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SoundFinishedCondition::SoundFinishedCondition (PlaySoundAction* pPlaySoundAction) :
    m_pPlaySoundAction (pPlaySoundAction),
    m_bHasStarted (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SoundFinishedCondition::~SoundFinishedCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        SoundFinishedCondition::Evaluate (void)
    {
        if (not m_bHasStarted)
        {
            m_bHasStarted = m_pPlaySoundAction->HasStarted ();
            return false;
        }
        else
            return m_pPlaySoundAction->HasStopped ();
    }

    //------------------------------------------------------------------------------
}
