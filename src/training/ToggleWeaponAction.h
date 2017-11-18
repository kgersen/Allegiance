/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ToggleWeaponAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ToggleWeaponAction" interface.
**
**  History:
*/
#ifndef _TOGGLE_WEAPON_ACTION_H_
#define _TOGGLE_WEAPON_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ToggleWeaponAction : public Action
    {
        public:
                    /* void */          ToggleWeaponAction (Mount mount);
            virtual /* void */          ~ToggleWeaponAction (void);
            virtual void                Execute (void);

        protected:
                    Mount               m_mount;
    };

    //------------------------------------------------------------------------------
}

#endif  //_TOGGLE_WEAPON_ACTION_H_
