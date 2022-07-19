/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SetHUDOverlayAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "SetHUDOverlayAction" interface.
**
**  History:
*/
#ifndef _SET_HUD_OVERLAY_ACTION_H_
#define _SET_HUD_OVERLAY_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetHUDOverlayAction : public Action
    {
        public:
                    /* void */          SetHUDOverlayAction (TrainingOverlay overlay);
            virtual /* void */          ~SetHUDOverlayAction (void);
            virtual void                Execute (void);

        protected:
                    TrainingOverlay     m_overlay;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_HUD_OVERLAY_ACTION_H_
