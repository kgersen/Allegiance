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
    /* void */  MessageAction::MessageAction (const ZString message, const SoundID soundID)
    {
        AddMessage (message, soundID); //TheBored 06-JUL-2007: Bahdohday's alteration to this function added the ability to send a sound with the message (replicating VCs)
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
        if (m_soundIterator == m_soundList.end())
            m_soundIterator = m_soundList.begin();
        if (m_iterator != m_messageList.end ())
            trekClient.ReceiveChat (g_pMission->GetCommanderShip (), CHAT_INDIVIDUAL, trekClient.GetShipID (), *m_soundIterator++, *m_iterator++, c_cidNone, NA, NA, 0, false); //TheBored 06-JUL-2007: Bahdohday's edit to allow sound w/text. 
    }

    //------------------------------------------------------------------------------
    void        MessageAction::AddMessage (const ZString message, const SoundID soundID)
    {
        //TheBored 06-JUL-2007: Bahdohday's edit to allow sound w/text.
		
		// Store the messages in the order they are added. This gives the
        // author some modicum of control over playback order.
        m_messageList.push_back (message);
		m_soundList.push_back(soundID);
        
        // I'm being lazy and getting the list iterator at each addition.
        // This saves having to have a separate method for saying we are
        // done installing new messages and are ready to start traversal.
        m_iterator = m_messageList.begin ();
		m_soundIterator = m_soundList.begin();
    }

    //------------------------------------------------------------------------------
}
