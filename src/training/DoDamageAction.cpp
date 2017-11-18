/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	DoDamageAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "DoDamageAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "DoDamageAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  DoDamageAction::DoDamageAction (bool bHull) : 
    m_bHull (bHull)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  DoDamageAction::~DoDamageAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        DoDamageAction::Execute (void)
    {
        IshipIGC*   pShip = trekClient.GetShip ();
        if (not m_bHull)
        {
            IshieldIGC* pShield = static_cast<IshieldIGC*> (pShip->GetMountedPart (ET_Shield, 0));
            if (pShield and (pShield->GetFraction () > 0.2f))
                pShip->ReceiveDamage (c_dmgidCollision, pShield->GetMaxStrength () * 0.03f, Time::Now (), Vector (-1.0f, 0.0f, 0.0f), Vector (1.0f, 0.0f, 0.0f), 0);

        }
        else
        {
            IshieldIGC* pShield = static_cast<IshieldIGC*> (pShip->GetMountedPart (ET_Shield, 0));
            if (pShield and (pShield->GetFraction () > 0.25f))
                pShield->SetFraction (0.2f);
            else if (pShip->GetFraction () > 0.5f)
                pShip->ReceiveDamage (c_dmgidCollision, 7.5f, Time::Now (), Vector (-1.0f, 0.0f, 0.0f), Vector (1.0f, 0.0f, 0.0f), 0);
        }
    }

    //------------------------------------------------------------------------------
}
