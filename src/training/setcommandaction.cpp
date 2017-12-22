/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setcommandaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setcommandaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetCommandAction.h"
#include    "TypeIDTarget.h"
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
    /* void */  SetCommandAction::SetCommandAction (IshipIGC* pShip, Command command, ImodelIGC* pTarget, CommandID commandID) : 
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (new TypeIDTarget (pTarget ? pTarget->GetObjectType () : NA, pTarget ? pTarget->GetObjectID () : NA)),
    m_command (command),
    m_commandID (commandID)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetCommandAction::SetCommandAction (IshipIGC* pShip, Command command, ObjectType targetType, ObjectID targetID, CommandID commandID) :
    m_pShip (new TypeIDTarget (pShip->GetObjectType (), pShip->GetObjectID ())),
    m_pTarget (new TypeIDTarget (targetType, targetID)),
    m_command (command),
    m_commandID (commandID)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetCommandAction::SetCommandAction (ShipID shipID, Command command, ObjectType targetType, ObjectID targetID, CommandID commandID) :
    m_pShip (new TypeIDTarget (OT_ship, shipID)),
    m_pTarget (new TypeIDTarget (targetType, targetID)),
    m_command (command),
    m_commandID (commandID)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetCommandAction::SetCommandAction (ShipID shipID, Command command, ImodelIGC* pTarget, CommandID commandID) :
    m_pShip (new TypeIDTarget (OT_ship, shipID)),
    m_pTarget (new TypeIDTarget (pTarget ? pTarget->GetObjectType () : NA, pTarget ? pTarget->GetObjectID () : NA)),
    m_command (command),
    m_commandID (commandID)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetCommandAction::~SetCommandAction (void)
    {
        delete m_pShip;
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    void        SetCommandAction::Execute (void)
    {
        // get a pointer to the actual ship and the target
        IshipIGC*   pShip = static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pShip));
        ImodelIGC*  pTarget = static_cast<ImodelIGC*> (*m_pTarget);

        // the ship might not be there for any of a hundred reasons,
        // including that it was destroyed. If it's not there, we want
        // to skip setting its command.
        if (pShip)
        {
            // we want to use the pretty interface if this is a command for the player
            if ((pShip == trekClient.GetShip ()) and (m_command == c_cmdQueued))
            {
                TrekWindow* pWindow = GetWindow ();
                IshipIGC*   pCommander = g_pMission->GetCommanderShip ();
                ZString     strOrder = ZString (c_cdAllCommands[m_commandID].szVerb);
                if (pTarget)
                    strOrder = strOrder + " " + GetModelName (pTarget);
                pWindow->SetQueuedCommand (pCommander, m_commandID, pTarget);
                trekClient.PostText(true, "New orders from %s to %s: %s. Press [insert] to accept.", (const char*)GetModelName (pCommander), GetModelName (pShip), (const char*) strOrder);
            }
            else
                pShip->SetCommand (m_command, pTarget, m_commandID);
        }
    }

    //------------------------------------------------------------------------------
}
