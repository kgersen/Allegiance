/*
**
**  File:	setdronennaaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setdronenanaction" interface.
**
**  History:
*/
#ifndef _SET_DRONE_NAN_ACTION_H_
#define _SET_DRONE_NAN_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetDroneNanAction : public Action
    {
        public:
                    /* void */          SetDroneNanAction (ShipID theShip);
            virtual /* void */          ~SetDroneNanAction (void);
            virtual void                Execute (void);

        protected:
                    ShipID nanShipID;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_DRONE_NAN_ACTION_H_
