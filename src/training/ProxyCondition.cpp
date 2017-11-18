/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ProxyCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "ProxyCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ProxyCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ProxyCondition::ProxyCondition (Condition* pCondition) :
    m_pCondition (pCondition)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ProxyCondition::~ProxyCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        ProxyCondition::Evaluate (void)
    {
        return m_pCondition->Evaluate ();
    }

    //------------------------------------------------------------------------------
}
