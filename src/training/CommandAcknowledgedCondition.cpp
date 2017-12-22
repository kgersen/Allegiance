/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	commandacknowledgedcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "commandacknowledgedcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "CommandAcknowledgedCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class static data
    //------------------------------------------------------------------------------
    char        CommandAcknowledgedCondition::m_cCommandAcknowledged = c_cidDoNothing;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  CommandAcknowledgedCondition::CommandAcknowledgedCondition (char cTargetCommand) :
    m_cTargetCommand (cTargetCommand)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  CommandAcknowledgedCondition::~CommandAcknowledgedCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        CommandAcknowledgedCondition::Evaluate (void)
    {
        return (m_cCommandAcknowledged == m_cTargetCommand) ? true : false;
    }

    //------------------------------------------------------------------------------
    void        CommandAcknowledgedCondition::SetCommandAcknowledged (char cAcknowledged)
    {
        m_cCommandAcknowledged = cAcknowledged;
    }

    //------------------------------------------------------------------------------
}
