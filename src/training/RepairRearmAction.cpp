/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	RepairRearmAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "RepairRearmAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "RepairRearmAction.h"
#include    "TrainingMission.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern TrainingMission*    g_pMission;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  RepairRearmAction::~RepairRearmAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        RepairRearmAction::Execute (void)
    {
        IshipIGC*   pShip = trekClient.GetShip ();

        // heal the ship entirely
        IshieldIGC* pShield = static_cast<IshieldIGC*> (pShip->GetMountedPart (ET_Shield, 0));
        if (pShield)
            pShield->SetFraction (1.0f);
        pShip->SetFraction (1.0f);

        // reload the ammunition, energy, and fuel
        pShip->SetEnergy (pShip->GetHullType ()->GetMaxEnergy ());
        pShip->SetAmmo (pShip->GetHullType ()->GetMaxAmmo ());
        pShip->SetFuel (pShip->GetHullType ()->GetMaxFuel ());

		// mdvalley: i needs defining outside the fors

		int i;

        // fill the first empty cargo slot with missiles
        for (i = -1; i > -c_maxCargo; i--)
            if (pShip->GetMountedPart (NA, i) == 0)
            {
                g_pMission->AddPartToShip (150, i, 0x7fff);
                break;
            }

        // fill the second empty cargo slot with ammo
        for (; i > -c_maxCargo; i--)
            if (pShip->GetMountedPart (NA, i) == 0)
                g_pMission->AddPartToShip (24, i, 0x7fff);

        // fill all subsequent slots with missiles
        for (; i > -c_maxCargo; i--)
            if (pShip->GetMountedPart (NA, i) == 0)
                g_pMission->AddPartToShip (150, i, 0x7fff);
    }

    //------------------------------------------------------------------------------
}
