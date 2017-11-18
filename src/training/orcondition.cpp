/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	orcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "orcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "OrCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  OrCondition::OrCondition (Condition* pConditionA, Condition* pConditionB) :
    m_pConditionA (pConditionA),
    m_pConditionB (pConditionB)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  OrCondition::~OrCondition (void)
    {
        delete m_pConditionA;
        delete m_pConditionB;
    }

    //------------------------------------------------------------------------------
    bool        OrCondition::Start (void)
    {
        // make sure that both conditions get started before combining the results
        bool    resultA = m_pConditionA->Start ();
        bool    resultB = m_pConditionB->Start ();
        return resultA or resultB;
    }

    //------------------------------------------------------------------------------
    void        OrCondition::Stop (void)
    {
        // make sure that both conditions get stopped
        m_pConditionA->Stop ();
        m_pConditionB->Stop ();
    }

    //------------------------------------------------------------------------------
    bool        OrCondition::Evaluate (void)
    {
        // make sure that both conditions get evaluated before combining the results
        bool    resultA = m_pConditionA->Evaluate ();
        bool    resultB = m_pConditionB->Evaluate ();
        return resultA or resultB;
    }

    //------------------------------------------------------------------------------
}
