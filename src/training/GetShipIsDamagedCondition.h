/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshipisdamagedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshipisdamagedcondition" interface.
**
**  History:
*/
#ifndef _GET_SHIP_IS_DAMAGED_CONDITION_H_
#define _GET_SHIP_IS_DAMAGED_CONDITION_H_

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
    class GetShipIsDamagedCondition : public Condition
    {
        public:
                    /* void */          GetShipIsDamagedCondition (IdamageIGC* pShip, float fFraction = 1.0f);
                    /* void */          GetShipIsDamagedCondition (ObjectType objectType, ObjectID objectID, float fFraction = 1.0f);
                    /* void */          GetShipIsDamagedCondition (AbstractTarget* pTarget, float fFraction = 1.0f);
            virtual /* void */          ~GetShipIsDamagedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pTarget;
                    float               m_fFraction;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_SHIP_IS_DAMAGED_CONDITION_H_
