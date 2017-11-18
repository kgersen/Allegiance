/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getautopilotcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getautopilotcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetAutopilotCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetAutopilotCondition::GetAutopilotCondition (const TRef<IshipIGC>& ship) :
    m_ship (ship)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetAutopilotCondition::~GetAutopilotCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetAutopilotCondition::Evaluate (void)
    {
        return m_ship->GetAutopilot ();
    }

    //------------------------------------------------------------------------------
}
