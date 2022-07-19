/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SetControlsAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "SetControlsAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetControlsAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern  ControlData         g_setInputControls;         // yaw, pitch, roll, throttle - the ones we are forcing on the player
    extern  int                 g_iSetInputAction;          // fire, afterburners, ... - the ones we force on the player

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetControlsAction::SetControlsAction (void)
    {
        // assign empty values to the controls
        m_inputControls.jsValues[c_axisYaw] = NA;
        m_inputControls.jsValues[c_axisPitch] = NA;
        m_inputControls.jsValues[c_axisRoll] = NA;
        m_inputControls.jsValues[c_axisThrottle] = NA;
        m_iInputAction = 0;
    }

    //------------------------------------------------------------------------------
    /* void */  SetControlsAction::~SetControlsAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetControlsAction::Execute (void)
    {
        // Set the control values
        g_setInputControls = m_inputControls;
        g_iSetInputAction = m_iInputAction;
    }

    //------------------------------------------------------------------------------
    void        SetControlsAction::SetInputAction (int iInputAction, int iMask)
    {
        m_iInputAction = (m_iInputAction bit_and iMask) bit_or iInputAction;
    }

    //------------------------------------------------------------------------------
    void        SetControlsAction::SetInputControl (Axis axis, float fScalar)
    {
        m_inputControls.jsValues[axis] = fScalar;
    }

    //------------------------------------------------------------------------------
}
