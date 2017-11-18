/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	endmissionaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "endmissionaction" interface.
**
**  History:
*/
#ifndef _END_MISSION_ACTION_H_
#define _END_MISSION_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class EndMissionAction : public Action
    {
        public:
            virtual /* void */          ~EndMissionAction (void);
            virtual void                Execute (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_END_MISSION_ACTION_H_
