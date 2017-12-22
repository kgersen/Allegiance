/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getstatebitscondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getstatebitscondition" interface.
**
**  History:
*/
#ifndef _GET_STATE_BITS_CONDITION_H_
#define _GET_STATE_BITS_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetStateBitsCondition : public Condition
    {
        public:
                    /* void */          GetStateBitsCondition (const TRef<IshipIGC>& ship, int iBits);
            virtual /* void */          ~GetStateBitsCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    int                 m_iBits;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_STATE_BITS_CONDITION_H_
