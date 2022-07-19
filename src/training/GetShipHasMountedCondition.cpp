/*
**
**  File:	getshiphasmountedcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getshiphasmountedcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetShipHasMountedCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetShipHasMountedCondition::GetShipHasMountedCondition (const TRef<IshipIGC>& ship, PartID pPart) :
    m_ship (ship)
    {
		m_part = pPart;
    }

    //------------------------------------------------------------------------------
    /* void */  GetShipHasMountedCondition::~GetShipHasMountedCondition (void)
    {
        m_ship = 0;
    }

    //------------------------------------------------------------------------------
    bool        GetShipHasMountedCondition::Evaluate (void)
	{
        const   PartListIGC*    pPartsList = m_ship->GetParts();
        PartLinkIGC*            pLink = pPartsList->first ();
        while (pLink != NULL)
        {
            if (pLink->data ()->GetPartType()->GetObjectID() == m_part && pLink->data()->GetMountID()>=0)
                return true;
            pLink = pLink->next();
        }
        return false;
    }

    //------------------------------------------------------------------------------
}
