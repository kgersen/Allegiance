/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getstatebitscondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getstatebitscondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetStateBitsCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetStateBitsCondition::GetStateBitsCondition (const TRef<IshipIGC>& ship, int iBits) :
    m_ship (ship),
    m_iBits (iBits)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetStateBitsCondition::~GetStateBitsCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetStateBitsCondition::Evaluate (void)
    {
        int     iBits = m_ship->GetStateM ();
        return (iBits bit_and m_iBits) ? true : false;
    }

    //------------------------------------------------------------------------------
}
