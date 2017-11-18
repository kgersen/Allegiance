/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshiphasammocondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getshiphasammocondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetShipHasAmmoCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetShipHasAmmoCondition::GetShipHasAmmoCondition (const TRef<IshipIGC>& ship, short sCount) :
    m_ship (ship),
    m_sCount (sCount)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipHasAmmoCondition::~GetShipHasAmmoCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetShipHasAmmoCondition::Evaluate (void)
    {
        return (m_ship->GetAmmo () >= m_sCount) ? false : true;
    }

    //------------------------------------------------------------------------------
}
