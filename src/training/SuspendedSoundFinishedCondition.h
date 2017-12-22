/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	suspendedsoundfinishedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "suspendedsoundfinishedcondition" interface.
**
**  History:
*/
#ifndef _SUSPENDED_SOUND_FINISHED_CONDITION_H_
#define _SUSPENDED_SOUND_FINISHED_CONDITION_H_

#ifndef _SOUND_FINISHED_CONDITION_H_
#include "SoundFinishedCondition.h"
#endif  //_SOUND_FINISHED_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SuspendedSoundFinishedCondition : public SoundFinishedCondition
    {
        public:
                    /* void */          SuspendedSoundFinishedCondition (PlaySoundAction* pPlaySoundAction, bool bDeleteWhenDone = true);
            virtual /* void */          ~SuspendedSoundFinishedCondition (void);

        protected:
                    bool                m_bDeleteWhenDone;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SUSPENDED_SOUND_FINISHED_CONDITION_H_
