/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	condition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "condition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "Condition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  Condition::~Condition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        Condition::Start (void)
    {
        return Evaluate ();
    }

    //------------------------------------------------------------------------------
    void        Condition::Stop (void)
    {
    }

    //------------------------------------------------------------------------------
}
