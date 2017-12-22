/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	andcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "andcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "AndCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  AndCondition::AndCondition (Condition* pConditionA, Condition* pConditionB) :
    m_pConditionA (pConditionA),
    m_pConditionB (pConditionB)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  AndCondition::~AndCondition (void)
    {
        delete m_pConditionA;
        delete m_pConditionB;
    }

    //------------------------------------------------------------------------------
    bool        AndCondition::Start (void)
    {
        // make sure that both conditions get started before combining the results
        bool    resultA = m_pConditionA->Start ();
        bool    resultB = m_pConditionB->Start ();
        return resultA and resultB;
    }

    //------------------------------------------------------------------------------
    void        AndCondition::Stop (void)
    {
        // make sure that both conditions get stopped
        m_pConditionA->Stop ();
        m_pConditionB->Stop ();
    }

    //------------------------------------------------------------------------------
    bool        AndCondition::Evaluate (void)
    {
        // make sure that both conditions get evaluated before combining the results
        bool    resultA = m_pConditionA->Evaluate ();
        bool    resultB = m_pConditionB->Evaluate ();
        return resultA and resultB;
    }

    //------------------------------------------------------------------------------
}
