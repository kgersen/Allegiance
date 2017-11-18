/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	onetimeaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "onetimeaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "OneTimeAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  OneTimeAction::OneTimeAction (Action* pAction) : 
    m_pAction (pAction),
    m_bHasExecuted (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  OneTimeAction::~OneTimeAction (void)
    {
        delete m_pAction;
    }

    //------------------------------------------------------------------------------
    void        OneTimeAction::Execute (void)
    {
        if (not m_bHasExecuted)
        {
            m_bHasExecuted = true;
            m_pAction->Execute ();
        }
    }

    //------------------------------------------------------------------------------
    void        OneTimeAction::Stop (void)
    {
        m_pAction->Stop ();
    }

    //------------------------------------------------------------------------------
}
