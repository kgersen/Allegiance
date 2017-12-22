/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	andcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "andcondition" interface.
**
**  History:
*/
#ifndef _AND_CONDITION_H_
#define _AND_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class AndCondition : public Condition
    {
        public:
                    /* void */          AndCondition (Condition* pConditionA, Condition* pConditionB);
            virtual /* void */          ~AndCondition (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pConditionA;
                    Condition*          m_pConditionB;
    };

    //------------------------------------------------------------------------------
}

#endif  //_TRUE_CONDITION_H_
