/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	PlayerShipTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "PlayerShipTarget" interface.
**
**  History:
*/
#ifndef _PLAYER_SHIP_TARGET_H_
#define _PLAYER_SHIP_TARGET_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class PlayerShipTarget : public AbstractTarget
    {
        public:
            virtual /* void */          ~PlayerShipTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_PLAYER_SHIP_TARGET_H_
