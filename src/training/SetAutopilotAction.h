/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setautopilotaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setautopilotaction" interface.
**
**  History:
*/
#ifndef _SET_AUTOPILOT_ACTION_H_
#define _SET_AUTOPILOT_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetAutopilotAction : public Action
    {
        public:
                    /* void */          SetAutopilotAction (IshipIGC* pShip, bool bNewValue = true);
                    /* void */          SetAutopilotAction (ShipID shipID, bool bNewValue = true);
            virtual /* void */          ~SetAutopilotAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pShip;
                    bool                m_bNewValue;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_AUTOPILOT_ACTION_H_
