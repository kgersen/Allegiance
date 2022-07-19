/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	conditionalaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "conditionalaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ConditionalAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ConditionalAction::ConditionalAction (Condition* pCondition, Action* pAction, bool bImmediateExecute, bool bRunOnce) : 
    m_pCondition (pCondition),
    m_pAction (pAction),
    m_bImmediateExecute (bImmediateExecute),
	m_bRunOnce (bRunOnce)
    {
		m_bRanOnce = false;
    }
    
    //------------------------------------------------------------------------------
    /* void */  ConditionalAction::~ConditionalAction (void)
    {
        delete m_pCondition;
        delete m_pAction;
    }

    //------------------------------------------------------------------------------
    bool        ConditionalAction::Start (void)
    {
        m_bExecute = m_pCondition->Start ();
        if ((m_bImmediateExecute == IMMEDIATE_EXECUTION)and (m_bExecute == true))
            m_pAction->Execute ();
        return m_bExecute;
    }

    //------------------------------------------------------------------------------
    void        ConditionalAction::Stop (void)
    {
        m_pCondition->Stop ();
        m_pAction->Stop ();
    }

    //------------------------------------------------------------------------------
    bool        ConditionalAction::Evaluate (void)
    {
		if ( (m_bRanOnce == false && m_bRunOnce == true ) || m_bRunOnce == false)
		{
			// some scenarios demand that the condition return value be processed
			// before the action is executed, so we provide that functionality
			// in the form of two flags.
			if (m_bImmediateExecute == IMMEDIATE_EXECUTION)
			{
				// immediate execution checks the enclosed condition and performs the action
				// if it is true
				m_bExecute = m_pCondition->Evaluate ();
				if (m_bExecute == true)
				{
					m_bRanOnce=true;
					m_pAction->Execute ();
				}
			}
			else
			{
				// delayed execution checks to see if the enclosed condition was true last
				// time this condition was tested, and performs the action if it was. It
				// then checks the enclosed condition and saves the result for next time.
				if (m_bExecute == true)
				{
					m_bRanOnce=true;
					m_pAction->Execute ();
				}
				m_bExecute = m_pCondition->Evaluate ();
			}
			return m_bExecute;
		}
		return false;
    }

    //------------------------------------------------------------------------------
}
