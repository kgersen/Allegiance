/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ReleaseConsumerAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "ReleaseConsumerAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ReleaseConsumerAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ReleaseConsumerAction::ReleaseConsumerAction (const TRef<IbuoyIGC>& buoy) : 
    m_buoy (buoy)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ReleaseConsumerAction::~ReleaseConsumerAction (void)
    {
        m_buoy = 0;
    }

    //------------------------------------------------------------------------------
    void        ReleaseConsumerAction::Execute (void)
    {
        m_buoy->ReleaseConsumer ();
    }

    //------------------------------------------------------------------------------
}
