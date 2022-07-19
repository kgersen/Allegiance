/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	predicate.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "predicate" interface.
**
**  History:
*/
#ifndef _PREDICATE_H_
#define _PREDICATE_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Predicate : public Condition
    {
        public:
                    /* void */          Predicate (Condition* pCondition, bool bExpectedValue);
            virtual /* void */          ~Predicate (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
                    bool                m_bExpectedValue;
    };

    //------------------------------------------------------------------------------
}

#endif  //_PREDICATE_H_
