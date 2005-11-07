/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshipisstoppedcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getshipisstoppedcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetShipIsStoppedCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetShipIsStoppedCondition::GetShipIsStoppedCondition (const TRef<IshipIGC>& ship) :
    m_ship (ship)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipIsStoppedCondition::~GetShipIsStoppedCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetShipIsStoppedCondition::Evaluate (void)
    {
        Vector  velocity = m_ship->GetVelocity ();
        float   fSpeedSquared = velocity.LengthSquared ();
        return (fSpeedSquared < 1.0f) ? true : false;
    }

    //------------------------------------------------------------------------------
}
