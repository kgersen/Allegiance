/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SwapWeaponAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "SwapWeaponAction" interface.
**
**  History:
*/
#ifndef _SWAP_WEAPON_ACTION_H_
#define _SWAP_WEAPON_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SwapWeaponAction : public Action
    {
        public:
                    /* void */          SwapWeaponAction (EquipmentType equipmentType, Mount mount);
            virtual /* void */          ~SwapWeaponAction (void);
            virtual void                Execute (void);

        protected:
                    EquipmentType       m_equipmentType;
                    Mount               m_mount;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SWAP_WEAPON_ACTION_H_
