/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetKeyCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetKeyCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetKeyCondition.h"
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
    /* void */  GetKeyCondition::GetKeyCondition (TrekKey key) :
    m_key (key),
    m_pressed (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetKeyCondition::~GetKeyCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        GetKeyCondition::Start (void)
    {
        g_pMission->AddKeyCondition (this);
        m_pressed = false;
        return false;
    }

    //------------------------------------------------------------------------------
    bool        GetKeyCondition::Evaluate (void)
    {
        bool    result = m_pressed;
        m_pressed = false;
        return result;
    }

    //------------------------------------------------------------------------------
    void        GetKeyCondition::KeyPressed (TrekKey key)
    {
        if (key == m_key)
            m_pressed = true;
    }

    //------------------------------------------------------------------------------
}
