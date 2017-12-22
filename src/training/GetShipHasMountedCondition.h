/*
**
**  File:	getshiphasmountedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshiphasmountedcondition" interface.
**
**  History:
*/
#ifndef _GET_SHIP_HAS_MOUNTED_CONDITION_H_
#define _GET_SHIP_HAS_MOUNTED_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetShipHasMountedCondition : public Condition
    {
        public:
                    /* void */          GetShipHasMountedCondition (const TRef<IshipIGC>& ship, PartID pPart);
            virtual /* void */          ~GetShipHasMountedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    PartID	            m_part;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_SHIP_HAS_MOUNTED_CONDITION_H_
