/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	suspendedsoundfinishedcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "suspendedsoundfinishedcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SuspendedSoundFinishedCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SuspendedSoundFinishedCondition::SuspendedSoundFinishedCondition (PlaySoundAction* pPlaySoundAction, bool bDeleteWhenDone) :
    SoundFinishedCondition (pPlaySoundAction),
    m_bDeleteWhenDone (bDeleteWhenDone)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SuspendedSoundFinishedCondition::~SuspendedSoundFinishedCondition (void)
    {
        if (m_bDeleteWhenDone)
            delete m_pPlaySoundAction;
    }

    //------------------------------------------------------------------------------
}
