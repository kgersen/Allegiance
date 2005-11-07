/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	messageaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "messageaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "TrainingMission.h"
#include    "MessageAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern TrainingMission*    g_pMission;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  MessageAction::MessageAction (void)
    {
        // This is to be sure the message list iterator is always valid.
        m_iterator = m_messageList.end ();
    }

    //------------------------------------------------------------------------------
    /* void */  MessageAction::MessageAction (const ZString message)
    {
        AddMessage (message);
    }

    //------------------------------------------------------------------------------
    /* void */  MessageAction::~MessageAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        MessageAction::Execute (void)
    {
        if (m_iterator == m_messageList.end ())
            m_iterator = m_messageList.begin ();
        if (m_iterator != m_messageList.end ())
            trekClient.ReceiveChat (g_pMission->GetCommanderShip (), CHAT_INDIVIDUAL, trekClient.GetShipID (), NA, *m_iterator++, c_cidNone, NA, NA, 0, false);
    }

    //------------------------------------------------------------------------------
    void        MessageAction::AddMessage (const ZString message)
    {
        // Store the messages in the order they are added. This gives the
        // author some modicum of control over playback order.
        m_messageList.push_back (message);
        
        // I'm being lazy and getting the list iterator at each addition.
        // This saves having to have a separate method for saying we are
        // done installing new messages and are ready to start traversal.
        m_iterator = m_messageList.begin ();
    }

    //------------------------------------------------------------------------------
}
