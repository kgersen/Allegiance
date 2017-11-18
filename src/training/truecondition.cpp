/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	truecondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "truecondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "TrueCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  TrueCondition::~TrueCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        TrueCondition::Start (void)
    {
        return true;
    }

    //------------------------------------------------------------------------------
    bool        TrueCondition::Evaluate (void)
    {
        return true;
    }

    //------------------------------------------------------------------------------
}
