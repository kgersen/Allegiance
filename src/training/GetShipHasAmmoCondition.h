/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshiphasammocondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshiphasammocondition" interface.
**
**  History:
*/
#ifndef _GET_SHIP_HAS_AMMO_CONDITION_H_
#define _GET_SHIP_HAS_AMMO_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetShipHasAmmoCondition : public Condition
    {
        public:
                    /* void */          GetShipHasAmmoCondition (const TRef<IshipIGC>& ship, short sCount = -1);
            virtual /* void */          ~GetShipHasAmmoCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    short               m_sCount;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_SHIP_HAS_AMMO_CONDITION_H_
