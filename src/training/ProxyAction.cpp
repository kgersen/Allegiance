/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	proxyaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "proxyaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ProxyAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ProxyAction::ProxyAction (Action* pAction) : 
    m_pAction (pAction)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ProxyAction::~ProxyAction (void)
    {
        // Note that this class exists explicitly to allow an action to be held at more
        // than one point. Since the training code cleans up its pointers when it's
        // done (as opposed to using reference counting), this class's usefulness lies
        // in the fact that it *DOESN'T* delete the contained action when it is deleted.
    }

    //------------------------------------------------------------------------------
    void        ProxyAction::Execute (void)
    {
        m_pAction->Execute ();
    }

    //------------------------------------------------------------------------------
}
