/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	actionlist.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "actionlist" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ActionList.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ActionList::ActionList (void)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ActionList::~ActionList (void)
    {
        // assume that no one else is managing the memory for these actions, and delete
        // them myself.
        std::list<Action*>::iterator    iterator = m_actionList.begin ();
        for (; iterator != m_actionList.end (); iterator++)
            delete *iterator;
    }

    //------------------------------------------------------------------------------
    void        ActionList::Execute (void)
    {
        std::list<Action*>::iterator    iterator = m_actionList.begin ();
        for (; iterator != m_actionList.end (); iterator++)
            (*iterator)->Execute ();
    }

    //------------------------------------------------------------------------------
    void        ActionList::Stop (void)
    {
        std::list<Action*>::iterator    iterator = m_actionList.begin ();
        for (; iterator != m_actionList.end (); iterator++)
            (*iterator)->Stop ();
    }

    //------------------------------------------------------------------------------
    void        ActionList::AddAction (Action* pAction)
    {
        // Store the actions in the order they are added. This gives the
        // author some modicum of control over playback order.
        m_actionList.push_back (pAction);
    }

    //------------------------------------------------------------------------------
}
