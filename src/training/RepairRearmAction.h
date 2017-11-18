/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	RepairRearmAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "RepairRearmAction" interface.
**
**  History:
*/
#ifndef _REPAIR_REARM_ACTION_H_
#define _REPAIR_REARM_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class RepairRearmAction : public Action
    {
        public:
            virtual /* void */          ~RepairRearmAction (void);
            virtual void                Execute (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_REPAIR_REARM_ACTION_H_
