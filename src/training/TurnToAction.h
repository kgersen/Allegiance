/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	TurnToAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "TurnToAction" interface.
**
**  History:
*/
#ifndef _TURN_TO_ACTION_H_
#define _TURN_TO_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class TurnToAction : public Action
    {
        public:
            /* void */                  TurnToAction (ImodelIGC* pObject, ImodelIGC* pTarget);
            /* void */                  TurnToAction (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID);
            /* void */                  TurnToAction (ImodelIGC* pObject, AbstractTarget* pTarget);
            virtual /* void */          ~TurnToAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pObject;
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}

#endif  //_TURN_TO_ACTION_H_
