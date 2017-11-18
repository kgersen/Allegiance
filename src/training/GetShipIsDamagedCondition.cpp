/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshipisdamagedcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getshipisdamagedcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetShipIsDamagedCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetShipIsDamagedCondition::GetShipIsDamagedCondition (IdamageIGC* pShip, float fFraction) :
    m_pTarget (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_fFraction (fFraction)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipIsDamagedCondition::GetShipIsDamagedCondition (ObjectType objectType, ObjectID objectID, float fFraction) :
    m_pTarget (new TypeIDTarget (objectType, objectID)),
    m_fFraction (fFraction)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipIsDamagedCondition::GetShipIsDamagedCondition (AbstractTarget* pTarget, float fFraction) :
    m_pTarget (pTarget),
    m_fFraction (fFraction)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipIsDamagedCondition::~GetShipIsDamagedCondition (void)
    {
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    bool        GetShipIsDamagedCondition::Evaluate (void)
    {
        if (*m_pTarget)
        {
            // we are testing for specific values here. Useful values of m_fFraction
            // are 0.0f and 1.0f. 1.0f can be used to tell if the ship is damaged,
            // and 0.0f can be used to tell if the ship is dead
            return (static_cast<IdamageIGC*>(static_cast<ImodelIGC*> (*m_pTarget))->GetFraction () == m_fFraction) ? false : true;
        }
        // assume result to be 0 here, since there is no ship
        return (m_fFraction == 0.0f) ? false : true;
    }

    //------------------------------------------------------------------------------
}
