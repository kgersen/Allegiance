/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	action.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "action" interface.
**
**  History:
*/
#ifndef _ACTION_H_
#define _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Action
    {
        public:
            virtual /* void */          ~Action (void);
            virtual void                Execute (void) = 0;
            virtual void                Stop (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_ACTION_H_
