/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SwapWeaponAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "SwapWeaponAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SwapWeaponAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SwapWeaponAction::SwapWeaponAction (EquipmentType equipmentType, Mount mount) : 
    m_equipmentType (equipmentType),
    m_mount (mount)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SwapWeaponAction::~SwapWeaponAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SwapWeaponAction::Execute (void)
    {
        GetWindow ()->SwapPart(m_equipmentType, m_mount);
    }

    //------------------------------------------------------------------------------
}
