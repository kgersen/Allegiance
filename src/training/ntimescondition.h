/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ntimescondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ntimescondition" interface.
**
**  History:
*/
#ifndef _N_TIMES_CONDITION_H_
#define _N_TIMES_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class NTimesCondition : public Condition
    {
        public:
                    /* void */          NTimesCondition (Condition* pCondition, unsigned int iTimes, bool bSticky = false);
            virtual /* void */          ~NTimesCondition (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
                    unsigned int        m_iTimes;
                    unsigned int        m_iCount;
                    bool                m_bSticky;
    };

    //------------------------------------------------------------------------------
}

#endif  //_N_TIMES_CONDITION_H_
