/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	suspendedplaysoundaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "suspendedplaysoundaction" interface.
**
**  History:
*/
#ifndef _SUSPENDED_PLAY_SOUND_ACTION_H_
#define _SUSPENDED_PLAY_SOUND_ACTION_H_

#ifndef _PLAY_SOUND_ACTION_H_
#include    "PlaySoundAction.h"
#endif// _PLAY_SOUND_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SuspendedPlaySoundAction : public PlaySoundAction
    {
        public:
                    /* void */          SuspendedPlaySoundAction (SoundID soundID, bool bDeleteWhenDone = true);
            virtual /* void */          ~SuspendedPlaySoundAction (void);
            virtual void                Execute (void);

        protected:
                    bool                m_bDeleteWhenDone;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SUSPENDED_PLAY_SOUND_ACTION_H_
