/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	objectpointingatcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "objectpointingatcondition" interface.
**
**  History:
*/
#ifndef _OBJECT_POINTING_AT_CONDITION_H_
#define _OBJECT_POINTING_AT_CONDITION_H_

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
    class ObjectPointingAtCondition : public Condition
    {
        public:
            /* void */                  ObjectPointingAtCondition (ImodelIGC* pObject, ImodelIGC* pTarget);
            /* void */                  ObjectPointingAtCondition (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID);
            /* void */                  ObjectPointingAtCondition (ImodelIGC* pObject, AbstractTarget* pTarget);
            virtual /* void */          ~ObjectPointingAtCondition (void);
            virtual bool                Evaluate (void);
                    void                SetVectorMask (const Vector& vectorMask);
                    void                ClearVectorMask (void);
                    void                SetMinimumAngle (float fMinimumAngle);

        protected:
                    Vector              MaskedVector (const Vector& vector);

        protected:
                    AbstractTarget*     m_pObject;
                    AbstractTarget*     m_pTarget;
                    Vector              m_vectorMask;
                    float               m_fMinimumAngle;
    };

    //------------------------------------------------------------------------------
}

#endif  //_OBJECT_POINTING_AT_CONDITION_H_
