/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SetHUDOverlayAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "SetHUDOverlayAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetHUDOverlayAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetHUDOverlayAction::SetHUDOverlayAction (TrainingOverlay overlay) : 
    m_overlay (overlay)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetHUDOverlayAction::~SetHUDOverlayAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetHUDOverlayAction::Execute (void)
    {
        SetTrainingOverlay (m_overlay);
    }

    //------------------------------------------------------------------------------
}
