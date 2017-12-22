/*
**
**  File:	setcargonanaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setcargonanaction" interface.
**
**  History:
*/
#ifndef _SET_CARGO_NAN_ACTION_H_
#define _SET_CARGO_NAN_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetCargoNanAction : public Action
    {
        public:
                    /* void */          SetCargoNanAction (ShipID theShip);
            virtual /* void */          ~SetCargoNanAction (void);
            virtual void                Execute (void);

        protected:
                    ShipID nanShipID;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_CARGO_NAN_ACTION_H_
