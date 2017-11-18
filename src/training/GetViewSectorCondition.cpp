/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetViewSectorCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetViewSectorCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetViewSectorCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetViewSectorCondition::GetViewSectorCondition (SectorID sector) :
    m_sector (sector)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetViewSectorCondition::~GetViewSectorCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        GetViewSectorCondition::Evaluate (void)
    {
        return (static_cast<SectorID> (trekClient.GetViewCluster ()->GetObjectID ()) == m_sector) ? true : false;
    }

    //------------------------------------------------------------------------------
}
