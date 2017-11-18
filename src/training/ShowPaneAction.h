/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShowPaneAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ShowPaneAction" interface.
**
**  History:
*/
#ifndef _SHOW_PANE_ACTION_H_
#define _SHOW_PANE_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ShowPaneAction : public Action
    {
        public:
                    /* void */          ShowPaneAction (OverlayMask overlayMask = 0);
            virtual /* void */          ~ShowPaneAction (void);
            virtual void                Execute (void);

        protected:
                    OverlayMask         m_overlayMask;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SHOW_PANE_ACTION_H_
