/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SetControlsAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "SetControlsAction" interface.
**
**  History:
*/
#ifndef _SET_CONTROLS_ACTION_H_
#define _SET_CONTROLS_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetControlsAction : public Action
    {
        public:
                    /* void */          SetControlsAction (void);
            virtual /* void */          ~SetControlsAction (void);
            virtual void                Execute (void);
                    void                SetInputAction (int inputAction, int iMask = 0);
                    void                SetInputControl (Axis axis, float fScalar);

        protected:
                    ControlData         m_inputControls;
                    int                 m_iInputAction;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_CONTROLS_ACTION_H_
