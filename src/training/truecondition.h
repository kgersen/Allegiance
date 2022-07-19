/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	truecondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "truecondition" interface.
**
**  History:
*/
#ifndef _TRUE_CONDITION_H_
#define _TRUE_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class TrueCondition : public Condition
    {
        public:
            virtual /* void */          ~TrueCondition (void);
            virtual bool                Start (void);
            virtual bool                Evaluate (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_TRUE_CONDITION_H_
