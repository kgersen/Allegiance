/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshiphascargocondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getshiphascargocondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetShipHasCargoCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetShipHasCargoCondition::GetShipHasCargoCondition (const TRef<IshipIGC>& ship, EquipmentType part) :
    m_ship (ship),
    m_part (part)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipHasCargoCondition::~GetShipHasCargoCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetShipHasCargoCondition::Evaluate (void)
    {
        const   PartListIGC*    pPartsList = m_ship->GetParts ();
        PartLinkIGC*            pLink = pPartsList->first ();
        while (pLink != NULL)
        {
            if (pLink->data ()->GetEquipmentType () == m_part)
                return true;
            pLink = pLink->next();
        }
        return false;
    }

    //------------------------------------------------------------------------------
}
