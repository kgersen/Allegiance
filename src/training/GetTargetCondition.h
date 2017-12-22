/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	gettargetcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "gettargetcondition" interface.
**
**  History:
*/
#ifndef _GET_TARGET_CONDITION_H_
#define _GET_TARGET_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetTargetCondition : public Condition
    {
        public:
                    /* void */          GetTargetCondition (IshipIGC* pShip, ImodelIGC* pExpectedTarget);
                    /* void */          GetTargetCondition (IshipIGC* pShip, ObjectType targetType, ObjectID targetID);
                    /* void */          GetTargetCondition (IshipIGC* pShip, AbstractTarget* pTarget);
            virtual /* void */          ~GetTargetCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pShip;
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_TARGET_CONDITION_H_
