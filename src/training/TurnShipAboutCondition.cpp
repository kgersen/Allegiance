/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	turnshipaboutcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "turnshipaboutcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "TurnShipAboutCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  TurnShipAboutCondition::TurnShipAboutCondition (const TRef<ImodelIGC>& ship) :
    m_ship (ship)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  TurnShipAboutCondition::~TurnShipAboutCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        TurnShipAboutCondition::Start (void)
    {
        m_startingForward = m_ship->GetOrientation ().GetForward ();
        return false;
    }

    //------------------------------------------------------------------------------
    bool        TurnShipAboutCondition::Evaluate (void)
    {
        Vector  forward = m_ship->GetOrientation ().GetForward ();
        float   cosTheta = forward * m_startingForward;
        float   minAngleDegrees = 10.0f;
        float   minCosAngle = cosf (RadiansFromDegrees(minAngleDegrees));
        // the ship has "turned about" if the angle of movement is greate than the min angle
        return (cosTheta < minCosAngle) ? true : false;
    }

    //------------------------------------------------------------------------------
}
