/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetChatCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetChatCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetChatCondition.h"
#include    "TrainingMission.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern TrainingMission*    g_pMission;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetChatCondition::GetChatCondition (ChatTarget expectedRecipient) :
    m_expectedRecipient (expectedRecipient),
    m_bGotExpectedChat (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetChatCondition::~GetChatCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        GetChatCondition::Start (void)
    {
        g_pMission->SetChatCondition (this);
        m_bGotExpectedChat = false;
        return false;
    }

    //------------------------------------------------------------------------------
    bool        GetChatCondition::Evaluate (void)
    {
        bool    result = m_bGotExpectedChat;
        m_bGotExpectedChat = false;
        return result;
    }

    //------------------------------------------------------------------------------
    void        GetChatCondition::RecordChat (ChatTarget recipient)
    {
        if (recipient == m_expectedRecipient)
            m_bGotExpectedChat = true;
    }

    //------------------------------------------------------------------------------
}
