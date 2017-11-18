/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ToggleWeaponAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "ToggleWeaponAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ToggleWeaponAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ToggleWeaponAction::ToggleWeaponAction (Mount mount) : 
    m_mount (mount)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ToggleWeaponAction::~ToggleWeaponAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        ToggleWeaponAction::Execute (void)
    {
        // This is the functionality
        /*
        Mount   mountSelected = (trekClient.GetShip ()->GetStateM () & selectedWeaponMaskIGC) >> selectedWeaponShiftIGC;
        if (mountSelected == m_mount)
        {
            trekClient.fGroupFire = true;
        }
        else
        {
            trekClient.SetSelectedWeapon (m_mount);
            trekClient.fGroupFire = false;
        }
        */

        // This uses the normal key press interface
        GetWindow ()->ToggleWeapon (m_mount);
    }

    //------------------------------------------------------------------------------
}
