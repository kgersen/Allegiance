/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setautopilotaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setautopilotaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetAutopilotAction.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetAutopilotAction::SetAutopilotAction (IshipIGC* pShip, bool bNewValue) : 
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_bNewValue (bNewValue)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetAutopilotAction::SetAutopilotAction (ShipID shipID, bool bNewValue) : 
    m_pShip (new TypeIDTarget (OT_ship, shipID)),
    m_bNewValue (bNewValue)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetAutopilotAction::~SetAutopilotAction (void)
    {
        delete m_pShip;
    }

    //------------------------------------------------------------------------------
    void        SetAutopilotAction::Execute (void)
    {
        static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pShip))->SetAutopilot (m_bNewValue);
    }

    //------------------------------------------------------------------------------
}
