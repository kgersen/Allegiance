/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ProxyCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ProxyCondition" interface.
**
**  History:
*/
#ifndef _PROXY_CONDITION_H_
#define _PROXY_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ProxyCondition : public Condition
    {
        public:
                    /* void */          ProxyCondition (Condition* pCondition);
            virtual /* void */          ~ProxyCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
    };

    //------------------------------------------------------------------------------
}

#endif  //_PROXY_CONDITION_H_
