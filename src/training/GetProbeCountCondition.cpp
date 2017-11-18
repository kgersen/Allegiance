/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetProbeCountCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetProbeCountCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetProbeCountCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetProbeCountCondition::GetProbeCountCondition (int iProbeCount) :
    m_iProbeCount (iProbeCount)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetProbeCountCondition::~GetProbeCountCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        GetProbeCountCondition::Evaluate (void)
    {
        return (trekClient.GetCore ()->GetProbes ()->n () == m_iProbeCount) ? true : false;
    }

    //------------------------------------------------------------------------------
}
