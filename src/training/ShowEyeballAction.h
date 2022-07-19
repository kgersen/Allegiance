/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShowEyeballAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ShowEyeballAction" interface.
**
**  History:
*/
#ifndef _SHOW_EYEBALL_ACTION_H_
#define _SHOW_EYEBALL_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ShowEyeballAction : public Action
    {
        public:
                    /* void */          ShowEyeballAction (bool showEyeball = false);
            virtual /* void */          ~ShowEyeballAction (void);
            virtual void                Execute (void);

        protected:
                    bool                m_showEyeball;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SHOW_EYEBALL_ACTION_H_
