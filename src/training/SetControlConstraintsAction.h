/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setcontrolconstraintsaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setcontrolconstraintsaction" interface.
**
**  History:
*/
#ifndef _SET_CONTROL_CONSTRAINTS_ACTION_H_
#define _SET_CONTROL_CONSTRAINTS_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetControlConstraintsAction : public Action
    {
        public:
                    /* void */          SetControlConstraintsAction (void);
            virtual /* void */          ~SetControlConstraintsAction (void);
            virtual void                Execute (void);
                    void                EnableInputAction (int iInputAction);
                    void                DisableInputAction (int iInputAction);
                    void                ScaleInputControl (Axis axis, float fScalar);

        protected:
                    ControlData             m_inputControls;
                    int                     m_iEnableActionsMask;
                    int                     m_iDisableActionsMask;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_CONTROL_CONSTRAINTS_ACTION_H_
