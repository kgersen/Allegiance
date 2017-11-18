/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShowPaneAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "ShowPaneAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ShowPaneAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ShowPaneAction::ShowPaneAction (OverlayMask overlayMask) :
    m_overlayMask (overlayMask)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ShowPaneAction::~ShowPaneAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        ShowPaneAction::Execute (void)
    {
        TrekWindow* window = GetWindow ();
        window->TurnOffOverlayFlags (ofTeleportPane bit_or 
                                     ofInvestment bit_or 
                                     ofGameState bit_or 
                                     ofTeam bit_or 
                                     ofExpandedTeam bit_or 
                                     ofInventory bit_or 
                                     ofCommandPane bit_or 
                                     ofSectorPane);
        window->TurnOnOverlayFlags (m_overlayMask);
    }

    //------------------------------------------------------------------------------
}
