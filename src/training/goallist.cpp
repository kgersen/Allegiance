/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	goallist.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "goallist" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GoalList.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GoalList::GoalList (void)
    {
        // This is done to be sure the goal list iterator is always valid.
        m_iterator = m_goalList.end ();
    }

    //------------------------------------------------------------------------------
    /* void */  GoalList::~GoalList (void)
    {
        // assume that no one else is managing the memory for these goals, and delete
        // them myself.
        for (m_iterator = m_goalList.begin (); m_iterator != m_goalList.end (); m_iterator++)
            delete *m_iterator;
   }

    //------------------------------------------------------------------------------
    bool        GoalList::Start (void)
    {
        // Check to see if we are at the end of the list
        while (m_iterator != m_goalList.end ())
        {
            // start the goal, and check whether or not it is complete
            if ((*m_iterator)->Start () == Goal::c_GoalComplete)
            {
                // the goal is complete, so move to the next goal
                m_iterator++;
            }
            else
            {
                // the goal is not complete, so return false
                return Goal::c_GoalIncomplete;
            }
        }

        // If we got here, then the goal list is complete.
        return Goal::c_GoalComplete;
    }
    
    //------------------------------------------------------------------------------
    void        GoalList::Stop (void)
    {
        if (m_iterator != m_goalList.end ())
            // stop the current goal in the list
            (*m_iterator)->Stop ();
    }
    
    //------------------------------------------------------------------------------
    bool        GoalList::Evaluate (void)
    {
        // Check to see if we are at the end of the list
        if (m_iterator != m_goalList.end ())
        {
            // evaluate the goal condition
            if ((*m_iterator)->Evaluate () == true)
            {
                // the goal condition is true, meaning the current goal is complete.
                // move to the next goal and return the result of starting it.
                Stop ();
                m_iterator++;
                return Start ();
            }
            else
            {
                // the goal condition is false, meaning the current goal is not complete.
                // return false to indicate this.
                return Goal::c_GoalIncomplete;
            }
        }

        // If we got here, then the goal list is complete.
        return Goal::c_GoalComplete;
    }

    //------------------------------------------------------------------------------
    void        GoalList::AddGoal (Goal* pGoal)
    {
        // Store the goals in the order they are added. This gives the
        // author some modicum of control over playback order.
        m_goalList.push_back (pGoal);

        // I'm being lazy and getting the list iterator at each addition.
        // This saves having to have a separate method for saying we are
        // done installing new goals and are ready to start traversal.
        m_iterator = m_goalList.begin ();
    }

    //------------------------------------------------------------------------------
}
