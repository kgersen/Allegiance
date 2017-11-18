/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetProbeCountCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetProbeCountCondition" interface.
**
**  History:
*/
#ifndef _GET_PROBE_COUNT_CONDITION_H_
#define _GET_PROBE_COUNT_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetProbeCountCondition : public Condition
    {
        public:
                    /* void */          GetProbeCountCondition (int iProbeCount);
            virtual /* void */          ~GetProbeCountCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    int                 m_iProbeCount;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_PROBE_COUNT_CONDITION_H_
