/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	soundfinishedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "soundfinishedcondition" interface.
**
**  History:
*/
#ifndef _SOUND_FINISHED_CONDITION_H_
#define _SOUND_FINISHED_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

#ifndef _PLAY_SOUND_ACTION_H_
#include "PlaySoundAction.h"
#endif  //_PLAY_SOUND_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SoundFinishedCondition : public Condition
    {
        public:
            /* void */                  SoundFinishedCondition (PlaySoundAction* pPlaySoundAction);
            virtual /* void */          ~SoundFinishedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    PlaySoundAction*    m_pPlaySoundAction;
                    bool                m_bHasStarted;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SOUND_FINISHED_CONDITION_H_
