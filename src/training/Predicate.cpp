/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	condition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "condition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "Predicate.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  Predicate::Predicate (Condition* pCondition, bool bExpectedValue) : 
    m_pCondition (pCondition),
    m_bExpectedValue (bExpectedValue)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  Predicate::~Predicate (void)
    {
        delete m_pCondition;
    }

    //------------------------------------------------------------------------------
    bool        Predicate::Start (void)
    {
        return (m_pCondition->Start () == m_bExpectedValue) ? true : false;
    }

    //------------------------------------------------------------------------------
    void        Predicate::Stop (void)
    {
        m_pCondition->Stop ();
    }

    //------------------------------------------------------------------------------
    bool        Predicate::Evaluate (void)
    {
        return (m_pCondition->Evaluate () == m_bExpectedValue) ? true : false;
    }

    //------------------------------------------------------------------------------
}