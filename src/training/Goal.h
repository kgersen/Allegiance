/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	goal.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "goal" interface.
**
**  History:
*/
#ifndef _GOAL_H_
#define _GOAL_H_

#ifndef _CONDITION_LIST_H_
#include "ConditionList.h"
#endif  //_CONDITION_LIST_H_

#ifndef _ACTION_LIST_H_
#include "ActionList.h"
#endif  //_ACTION_LIST_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Goal : public Condition
    {
        public:
            enum    {c_GoalIncomplete = false, c_GoalComplete = true};

        public:
                    /* void */          Goal (Condition* pSuccessCondition);
            virtual /* void */          ~Goal (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);
                    void                AddStartAction (Action* pStartAction);
                    void                AddConstraintCondition (Condition* pConstraintCondition);
                    void                SetSkipGoalCondition(Condition* pSkipCondition);

        protected:
                    Condition*          m_pSuccessCondition;
                    ActionList          m_startActionList;
                    ConditionList       m_constraintConditionList;
                    Condition*          m_pSkipGoalCondition;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GOAL_H_
