/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	periodiccondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "periodiccondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "PeriodicCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  PeriodicCondition::PeriodicCondition (Condition* pCondition, float fPeriod) :
    m_pCondition (pCondition),
    m_dwPeriod (static_cast<DWORD> (fPeriod * Time::fResolution ()))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  PeriodicCondition::~PeriodicCondition (void)
    {
        // assume no one else will be cleaning this up
        delete m_pCondition;
    }

    //------------------------------------------------------------------------------
    bool        PeriodicCondition::Start (void)
    {
        // record the start time
        m_dwStartTime = Time::Now ().clock ();

        // always evaluate the condition the first time through
        return m_pCondition->Start ();
    }

    //------------------------------------------------------------------------------
    void        PeriodicCondition::Stop (void)
    {
        m_pCondition->Stop ();
    }

    //------------------------------------------------------------------------------
    bool        PeriodicCondition::Evaluate (void)
    {
        // get the current clock time, and compute how much time has elapsed.
        DWORD   dwElapsedTime = (Time::Now ().clock ()  - m_dwStartTime);

        // if the elapsed time is longer than the desired period
        if (dwElapsedTime > m_dwPeriod)
        {
            // advance the clock base to the beginning of the next period. This is clock accurate, as
            // opposed to just using the current time for the beginning of the next cycle. This method
            // will be better for avoiding sampling errors, even though nobody would ever notice.
            m_dwStartTime += m_dwPeriod;

            // return the result of evaluating the enclosed condition
            return m_pCondition->Evaluate ();
        }

        // return false to indicate the condition hasn't been evaluated because the period hasn't
        // elapsed yet.
        return false;
    }

    //------------------------------------------------------------------------------
}
