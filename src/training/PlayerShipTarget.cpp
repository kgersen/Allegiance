/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	PlayerShipTarget.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "PlayerShipTarget" interface.
**
**  History:
*/
#include    "pch.h"
#include    "PlayerShipTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  PlayerShipTarget::~PlayerShipTarget (void)
    {
    }

    //------------------------------------------------------------------------------
    /* ImodelIGC* */    PlayerShipTarget::operator ImodelIGC* (void)
    {
        return trekClient.GetShip ();
    }

    //------------------------------------------------------------------------------
}
