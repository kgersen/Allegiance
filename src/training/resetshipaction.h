/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	resetshipaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "resetshipaction" interface.
**
**  History:
*/
#ifndef _RESET_SHIP_ACTION_H_
#define _RESET_SHIP_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ResetShipAction : public Action
    {
        public:
                    /* void */          ResetShipAction (const TRef<IshipIGC>& ship);
            virtual /* void */          ~ResetShipAction (void);
            virtual void                Execute (void);

        protected:
            struct  State
            {
                    Vector              position;
                    Vector              velocity;
                    Orientation         orientation;
                    Rotation            rotation;
                    float               fFuel;
                    float               fShield;
                    float               fHull;
                    short               sAmmo;
                    float               fEnergy;
            };
                    State*              m_pState;
                    TRef<IshipIGC>      m_ship;
    };

    //------------------------------------------------------------------------------
}

#endif  //_RESET_SHIP_ACTION_H_
