/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	resetaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "resetaction" interface.
**
**  History:
*/
#ifndef _RESET_ACTION_H_
#define _RESET_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

#ifndef _GOAL_H_
#include "Goal.h"
#endif  //_GOAL_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ResetAction : public Action
    {
        public:
                    enum    ResetStage  {
                                            RESET_START,
                                            RESET_NOTIFY,
                                            RESET_SPECIFIC_NOTIFY,
                                            RESET_COMPLETE
                                        };
        public:
                    /* void */          ResetAction (Goal* pGoal, SoundID soundID = NA);
            virtual /* void */          ~ResetAction (void);
            virtual void                Execute (void);
            static  void                Initialize (void);

        protected:
                    void                PlaySound (SoundID soundID, ResetStage nextStage);

        protected:
                    Goal*               m_pGoal;
                    SoundID             m_soundID;
            static  int                 m_iResetCount;
                    ResetStage          m_resetStage;
    };

    //------------------------------------------------------------------------------
}

#endif  //_RESET_ACTION_H_
