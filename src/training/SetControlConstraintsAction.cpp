/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setcontrolconstraintsaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setcontrolconstraintsaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetControlConstraintsAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern  ControlData g_allowableControlScalars;  // yaw, pitch, roll, throttle
    extern  int         g_iAllowableActionMask;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetControlConstraintsAction::SetControlConstraintsAction (void)
    {
        // assign default values to the control masks
        m_inputControls.jsValues[c_axisYaw] = NA;
        m_inputControls.jsValues[c_axisPitch] = NA;
        m_inputControls.jsValues[c_axisRoll] = NA;
        m_inputControls.jsValues[c_axisThrottle] = NA;

        m_iEnableActionsMask = 0;
        m_iDisableActionsMask = 0;
    }

    //------------------------------------------------------------------------------
    /* void */  SetControlConstraintsAction::~SetControlConstraintsAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetControlConstraintsAction::Execute (void)
    {
        // Set the control values
        if (m_inputControls.jsValues[c_axisYaw] != NA)
            g_allowableControlScalars.jsValues[c_axisYaw] = m_inputControls.jsValues[c_axisYaw];
        if (m_inputControls.jsValues[c_axisPitch] != NA)
            g_allowableControlScalars.jsValues[c_axisPitch] = m_inputControls.jsValues[c_axisPitch];
        if (m_inputControls.jsValues[c_axisRoll] != NA)
            g_allowableControlScalars.jsValues[c_axisRoll] = m_inputControls.jsValues[c_axisRoll];
        if (m_inputControls.jsValues[c_axisThrottle] != NA)
            g_allowableControlScalars.jsValues[c_axisThrottle] = m_inputControls.jsValues[c_axisThrottle];

        // Set the disabled control masks
        for (int i = 0; i < 32; i++)
        {
            int action = m_iDisableActionsMask & (1 << i);
            if (g_iAllowableActionMask & action)
                g_iAllowableActionMask ^= action;
        }
        
        // Set the enabled control masks
        g_iAllowableActionMask |= m_iEnableActionsMask;
    }

    //------------------------------------------------------------------------------
    void        SetControlConstraintsAction::EnableInputAction (int iInputAction)
    {
        m_iEnableActionsMask |= iInputAction;
        for (int i = 0; i < 32; i++)
        {
            int action = m_iDisableActionsMask & (1 << i);
            if (iInputAction & action)
                m_iDisableActionsMask ^= action;
        }
    }

    //------------------------------------------------------------------------------
    void        SetControlConstraintsAction::DisableInputAction (int iInputAction)
    {
        m_iDisableActionsMask |= iInputAction;
        for (int i = 0; i < 32; i++)
        {
            int action = m_iEnableActionsMask & (1 << i);
            if (iInputAction & action)
                m_iEnableActionsMask ^= action;
        }
    }

    //------------------------------------------------------------------------------
    void        SetControlConstraintsAction::ScaleInputControl (Axis axis, float fScalar)
    {
        m_inputControls.jsValues[axis] = fScalar;
    }

    //------------------------------------------------------------------------------
}
