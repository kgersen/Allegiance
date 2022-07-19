/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshiphascargocondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshiphascargocondition" interface.
**
**  History:
*/
#ifndef _GET_SHIP_HAS_CARGO_CONDITION_H_
#define _GET_SHIP_HAS_CARGO_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetShipHasCargoCondition : public Condition
    {
        public:
                    /* void */          GetShipHasCargoCondition (const TRef<IshipIGC>& ship, EquipmentType part);
            virtual /* void */          ~GetShipHasCargoCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    EquipmentType       m_part;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_SHIP_HAS_CARGO_CONDITION_H_
