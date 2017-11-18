/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getautopilotcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getautopilotcondition" interface.
**
**  History:
*/
#ifndef _GET_AUTOPILOT_CONDITION_H_
#define _GET_AUTOPILOT_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetAutopilotCondition : public Condition
    {
        public:
                    /* void */          GetAutopilotCondition (const TRef<IshipIGC>& ship);
            virtual /* void */          ~GetAutopilotCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_AUTOPILOT_CONDITION_H_
