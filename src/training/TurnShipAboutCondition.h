/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	turnshipaboutcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "turnshipaboutcondition" interface.
**
**  History:
*/
#ifndef _TURN_SHIP_ABOUT_CONDITION_H_
#define _TURN_SHIP_ABOUT_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class TurnShipAboutCondition : public Condition
    {
        public:
            /* void */                  TurnShipAboutCondition (const TRef<ImodelIGC>& ship);
            virtual /* void */          ~TurnShipAboutCondition (void);
            virtual bool                Start (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<ImodelIGC>     m_ship;
                    Vector              m_startingForward;
    };

    //------------------------------------------------------------------------------
}

#endif  //_TURN_SHIP_ABOUT_CONDITION_H_
