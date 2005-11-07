/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	goal.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "goal" interface.
**
**  History:
*/
#include    "pch.h"
#include    "Training.h"
#include    "GoalList.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  Goal::Goal (Condition* pSuccessCondition) :
    m_pSuccessCondition (pSuccessCondition)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  Goal::~Goal (void)
    {
        delete m_pSuccessCondition;
    }

    //------------------------------------------------------------------------------
    bool        Goal::Start (void)
    {
        // if the success condition is already true, then prepare to evaluate 
        // the goal.
        if (m_pSuccessCondition->Start () == c_GoalIncomplete)
        {
            // execute the start actions
            m_startActionList.Execute ();

            // start the constraint conditions
            m_constraintConditionList.Start ();

            // return false to indicate that the goal condition is false.
            return c_GoalIncomplete;
        }

        // if it gets here, the goal state is already true and we will be 
        // skipping this goal.
        return c_GoalComplete;
    }

    //------------------------------------------------------------------------------
    void        Goal::Stop (void)
    {
        m_pSuccessCondition->Stop ();

        // stop the start actions
        m_startActionList.Stop ();

        // stop the constraint conditions
        m_constraintConditionList.Stop ();
    }

    //------------------------------------------------------------------------------
    bool        Goal::Evaluate (void)
    {
        // For starters, if the success condition is true, then the goal is complete.
        if (m_pSuccessCondition->Evaluate ())
            return c_GoalComplete;

        // check the constraint condition, don't care about the return value
        m_constraintConditionList.Evaluate ();

        // The goal is not complete, so return false.
        return c_GoalIncomplete;
    }

    //------------------------------------------------------------------------------
    void        Goal::AddStartAction (Action* pStartAction)
    {
        m_startActionList.AddAction (pStartAction);
    }

    //------------------------------------------------------------------------------
    void        Goal::AddConstraintCondition (Condition* pConstraintCondition)
    {
        m_constraintConditionList.AddCondition (pConstraintCondition);
    }

    //------------------------------------------------------------------------------
}
