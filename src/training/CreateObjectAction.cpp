/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateObjectAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "CreateObjectAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "CreateObjectAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  CreateObjectAction::CreateObjectAction (ObjectType objectType, const void* pData, int iDataSize) : 
    m_objectType (objectType), 
    m_pData (pData),
    m_iDataSize (iDataSize)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  CreateObjectAction::~CreateObjectAction (void)
    {
        m_result = 0;
    }

    //------------------------------------------------------------------------------
    void        CreateObjectAction::Execute (void)
    {
        ImissionIGC*    pMission = trekClient.GetCore();
        m_result = pMission->CreateObject (pMission->GetLastUpdate(), m_objectType, m_pData, m_iDataSize);
    }

    //------------------------------------------------------------------------------
}
