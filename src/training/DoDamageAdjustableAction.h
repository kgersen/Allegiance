/*
**  File:	DoDamageAdjustableAction.h
**
**  Author:  Adam Davison
**
**  Description:
**      Header file for the training library "DoDamageAdjustableAction" interface.
**
**  History:
*/

#ifndef _DO_DAMAGE_ADJUSTABLE_ACTION_H_
#define _DO_DAMAGE_ADJUSTABLE_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    class DoDamageAdjustableAction : public Action
    {
        public:
                    /* void */          DoDamageAdjustableAction (ShipID ship, float amount);
            virtual /* void */          ~DoDamageAdjustableAction (void);
            virtual void                Execute (void);

        protected:
					ShipID				m_shipID;
					float				m_amount;
    };
}

#endif  //_DO_DAMAGE_ADJUSTABLE_ACTION_H_
