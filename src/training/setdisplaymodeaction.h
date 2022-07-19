/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setdisplaymodeaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setdisplaymodeaction" interface.
**
**  History:
*/
#ifndef _SET_DISPLAY_MODE_ACTION_H_
#define _SET_DISPLAY_MODE_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetDisplayModeAction : public Action
    {
        public:
                    /* void */          SetDisplayModeAction (TrekWindow::CameraMode mode);
            virtual /* void */          ~SetDisplayModeAction (void);
            virtual void                Execute (void);

        protected:
                    TrekWindow::CameraMode  m_mode;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_DISPLAY_MODE_ACTION_H_
