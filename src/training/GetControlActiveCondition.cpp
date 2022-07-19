/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getcontrolactivecondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getcontrolactivecondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetControlActiveCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern ControlData  g_inputControls;      // yaw, pitch, roll, throttle - the ones the player is actually inputting

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetControlActiveCondition::GetControlActiveCondition (const TRef<IshipIGC>& ship, Axis axis, float fExpectedValue, bool bUseInputControls) :
    m_ship (ship),
    m_axis (axis),
    m_fExpectedValue (fExpectedValue),
    m_bUseInputControls (bUseInputControls)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetControlActiveCondition::~GetControlActiveCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetControlActiveCondition::Evaluate (void)
    {
        float   fValue = m_bUseInputControls ? g_inputControls.jsValues[m_axis] : m_ship->GetControls ().jsValues[m_axis];
        if (m_axis == c_axisThrottle)
            // if the axis is the throttle, we need to translate the value into the 0..1 range
            fValue = (fValue + 1.0f) * 0.5f;
        else
            // otherwise we just take the absolute value
            fValue = fabsf (fValue);
        // we will test to see that the control is within 0.1 (10%) of the expected value.
        return (fabsf (fValue - m_fExpectedValue) < 0.1f) ? true : false;
    }

    //------------------------------------------------------------------------------
}
