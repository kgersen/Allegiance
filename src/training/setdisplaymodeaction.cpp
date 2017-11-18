/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setdisplaymodeaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setdisplaymodeaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetDisplayModeAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetDisplayModeAction::SetDisplayModeAction (TrekWindow::CameraMode mode) : 
    m_mode (mode)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetDisplayModeAction::~SetDisplayModeAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetDisplayModeAction::Execute (void)
    {
        if (TrekWindow::CommandCamera (m_mode))
            GetWindow()->SetViewMode (TrekWindow::vmCommand);
        else
            GetWindow()->SetViewMode (TrekWindow::vmCombat);
        GetWindow ()->EnableDisplacementCommandView (true);
        GetWindow ()->SetCameraMode (m_mode);
    }

    //------------------------------------------------------------------------------
}
