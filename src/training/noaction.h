/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	noaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "noaction" interface.
**
**  History:
*/
#ifndef _NO_ACTION_H_
#define _NO_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class NoAction : public Action
    {
        public:
            virtual /* void */          ~NoAction (void);
            virtual void                Execute (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_NO_ACTION_H_
