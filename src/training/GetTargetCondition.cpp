/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	gettargetcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "gettargetcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetTargetCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetTargetCondition::GetTargetCondition (IshipIGC* pShip, ImodelIGC* pExpectedTarget) :
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (new TypeIDTarget (pExpectedTarget ? pExpectedTarget->GetObjectType () : NA, pExpectedTarget ? pExpectedTarget->GetObjectID () : NA))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetTargetCondition::GetTargetCondition (IshipIGC* pShip, ObjectType expectedTargetType, ObjectID expectedTargetID) :
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (new TypeIDTarget (expectedTargetType, expectedTargetID))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetTargetCondition::GetTargetCondition (IshipIGC* pShip, AbstractTarget* pTarget) :
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (pTarget)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetTargetCondition::~GetTargetCondition (void)
    {
        delete m_pShip;
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    bool        GetTargetCondition::Evaluate (void)
    {
        return (static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pShip))->GetCommandTarget (c_cmdCurrent) == (*m_pTarget)) ? true : false;
    }

    //------------------------------------------------------------------------------
}
