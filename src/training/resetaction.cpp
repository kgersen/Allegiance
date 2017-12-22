/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	resetaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "resetaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ResetAction.h"
#include    "PlaySoundAction.h"
#include    "SuspendedSoundFinishedCondition.h"
#include    "MessageAction.h"
#include    "TrainingMission.h"
#include    "ConditionalAction.h"
#include    "ProxyAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern TrainingMission*    g_pMission;

    //------------------------------------------------------------------------------
    // class static variables
    //------------------------------------------------------------------------------
    int         ResetAction::m_iResetCount;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ResetAction::ResetAction (Goal* pGoal, SoundID soundID) :
    m_pGoal (pGoal),
    m_soundID (soundID),
    m_resetStage (RESET_START)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ResetAction::~ResetAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        ResetAction::Execute (void)
    {
        // XXX this functionality now allows the game engine to continue to run while we reset, thus 
        // keeping the reset from looking like a lockup. However, it would still be nice to disable controls
        // and such during the reset, and possibly switch to a different screen. Maybe set ship velocity to 0.
        /*
        SoundID     resetSoundID[] = {tm_reset_0Sound, tm_reset_1Sound, tm_reset_2Sound, tm_reset_3Sound, tm_reset_4Sound};
        switch (m_resetStage)
        {
            case    RESET_START:
                // play a "unique" sound
                m_iResetCount++;
                PlaySound (resetSoundID[0], RESET_NOTIFY);
                break;

            case    RESET_NOTIFY:
                // play the mission reset voice over
                PlaySound (resetSoundID[m_iResetCount], (m_iResetCount < 4) ? RESET_SPECIFIC_NOTIFY : RESET_COMPLETE);
                break;

            case    RESET_SPECIFIC_NOTIFY:
                // play a goal specific reset voice over
                PlaySound (m_soundID, RESET_COMPLETE);
                break;

            case    RESET_COMPLETE:
                // finish up the reset
                if (m_iResetCount < 4)
                {
                    MessageAction   messageAction ("Mission stage reset.");
                    messageAction.Execute ();
                    m_resetStage = RESET_START;
                    m_pGoal->Start ();
                }
                else
                    g_pMission->Terminate ();
                break;
        }
        */
    }

    //------------------------------------------------------------------------------
    void        ResetAction::Initialize (void)
    {
        m_iResetCount = 0;
    }

    //------------------------------------------------------------------------------
    void        ResetAction::PlaySound (SoundID soundID, ResetStage nextStage)
    {
        if (m_soundID != NA)
        {
            PlaySoundAction*        pPlaySoundAction = new PlaySoundAction (soundID);
            // make sure the play sound action is deleted when it is done
            SoundFinishedCondition* pSoundFinishedCondition = new SuspendedSoundFinishedCondition (pPlaySoundAction);
            ConditionalAction*      pConditionalAction = new ConditionalAction (pSoundFinishedCondition, new ProxyAction (this));
            g_pMission->AddWaitCondition (pConditionalAction);
            pPlaySoundAction->Execute ();
        }
        m_resetStage = nextStage;
    }

    //------------------------------------------------------------------------------
}
