/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShipSeesObjectCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "ShipSeesObjectCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ShipSeesObjectCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ShipSeesObjectCondition::ShipSeesObjectCondition (IshipIGC* pShip, ImodelIGC* pTarget) :
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (new TypeIDTarget (pTarget->GetObjectType (), pTarget->GetObjectID ()))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ShipSeesObjectCondition::ShipSeesObjectCondition (IshipIGC* pShip, ObjectType targetType, ObjectID targetID) :
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (new TypeIDTarget (targetType, targetID))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ShipSeesObjectCondition::ShipSeesObjectCondition (ObjectID shipID, ObjectType targetType, ObjectID targetID) :
    m_pShip (new TypeIDTarget (OT_ship, shipID)),
    m_pTarget (new TypeIDTarget (targetType, targetID))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ShipSeesObjectCondition::~ShipSeesObjectCondition (void)
    {
        delete m_pShip;
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    bool        ShipSeesObjectCondition::Evaluate (void)
    {
        IshipIGC*   pShip = static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pShip));
        if (pShip)
            return pShip->InScannerRange (*m_pTarget);
        return false;
    }

    //------------------------------------------------------------------------------
}
