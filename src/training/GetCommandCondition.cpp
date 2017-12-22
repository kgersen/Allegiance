/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetCommandCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetCommandCondition" interface.
**
**  History:
*/

#include    "pch.h"
#include    "GetCommandCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetCommandCondition::GetCommandCondition (IshipIGC* pShip, CommandID command) :
    m_pShip (new TypeIDTarget (OT_ship, pShip->GetObjectID ())),
    m_command (command)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetCommandCondition::GetCommandCondition (ObjectID shipID, CommandID command) :
    m_pShip (new TypeIDTarget (OT_ship, shipID)),
    m_command (command)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetCommandCondition::~GetCommandCondition (void)
    {
        delete m_pShip;
    }

    //------------------------------------------------------------------------------
    bool        GetCommandCondition::Evaluate (void)
    {
        IshipIGC*   pShip = static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pShip));
        // check that the ship is present
        if (pShip)
        {
            // the ship is there, so we return true if it has a command like the one we 
            // are checking for.
            return ((pShip->GetCommandID (c_cmdAccepted) == m_command) or (pShip->GetCommandID (c_cmdCurrent) == m_command)) ? true : false;
        }
        // the ship wasn't there for some reason, so we assume that this condition is true
        return true;
    }

    //------------------------------------------------------------------------------
}
