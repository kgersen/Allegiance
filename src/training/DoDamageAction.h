/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	DoDamageAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "DoDamageAction" interface.
**
**  History:
*/
#ifndef _DO_DAMAGE_ACTION_H_
#define _DO_DAMAGE_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class DoDamageAction : public Action
    {
        public:
                    /* void */          DoDamageAction (bool bHull);
            virtual /* void */          ~DoDamageAction (void);
            virtual void                Execute (void);

        protected:
                    bool                m_bHull;
    };

    //------------------------------------------------------------------------------
}

#endif  //_DO_DAMAGE_ACTION_H_
