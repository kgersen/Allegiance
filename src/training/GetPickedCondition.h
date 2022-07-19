/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetPickedCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetPickedCondition" interface.
**
**  History:
*/
#ifndef _GET_PICKED_CONDITION_H_
#define _GET_PICKED_CONDITION_H_

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
    class GetPickedCondition : public Condition
    {
        public:
                    /* void */          GetPickedCondition (ObjectType targetType, ObjectID targetID);
            virtual /* void */          ~GetPickedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_PICKED_CONDITION_H_
