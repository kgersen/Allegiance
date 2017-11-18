/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	resetshipaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "resetshipaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ResetShipAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ResetShipAction::ResetShipAction (const TRef<IshipIGC>& ship) :
    m_ship (ship),
    m_pState (0)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ResetShipAction::~ResetShipAction (void)
    {
        m_ship = 0;
        delete m_pState;
    }

    //------------------------------------------------------------------------------
    void        ResetShipAction::Execute (void)
    {
        IshieldIGC*   shield = static_cast<IshieldIGC*> (m_ship->GetMountedPart (ET_Shield, 0));
        if (m_pState)
        {
            m_ship->SetPosition (m_pState->position);
            m_ship->SetVelocity (m_pState->velocity);
            m_ship->SetOrientation (m_pState->orientation);
            m_ship->SetRotation (m_pState->rotation);
            m_ship->SetFuel (m_pState->fFuel);
            if (shield)
                shield->SetFraction (m_pState->fShield);
            m_ship->SetFraction (m_pState->fHull);
            m_ship->SetAmmo (m_pState->sAmmo);
            m_ship->SetEnergy (m_pState->fEnergy);
        }
        else if (m_ship->GetMission ())
        {
            // allocate a state pointer, and fill it in with needed info
            m_pState = new State;
            m_pState->position = m_ship->GetPosition ();
            m_pState->velocity = m_ship->GetVelocity ();
            m_pState->orientation = m_ship->GetOrientation ();
            m_pState->rotation = m_ship->GetRotation ();
            m_pState->fFuel = m_ship->GetFuel ();
            m_pState->fShield = shield ? shield->GetFraction () : 0.0f;
            m_pState->fHull = m_ship->GetFraction ();
            m_pState->sAmmo = m_ship->GetAmmo ();
            m_pState->fEnergy = m_ship->GetEnergy ();
        }
    }

    //------------------------------------------------------------------------------
}
