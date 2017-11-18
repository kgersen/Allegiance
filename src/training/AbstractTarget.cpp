/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	AbstractTarget.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "AbstractTarget" interface.
**
**  History:
*/
#include    "pch.h"
#include    "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  AbstractTarget::~AbstractTarget (void)
    {
    }

    //------------------------------------------------------------------------------
    ImodelIGC*  AbstractTarget::operator -> (void)
    {
        return operator ImodelIGC* ();
    }

    //------------------------------------------------------------------------------
}
