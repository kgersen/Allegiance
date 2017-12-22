/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ReleaseConsumerAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ReleaseConsumerAction" interface.
**
**  History:
*/
#ifndef _RELEASE_CONSUMER_ACTION_H_
#define _RELEASE_CONSUMER_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ReleaseConsumerAction : public Action
    {
        public:
                    /* void */          ReleaseConsumerAction (const TRef<IbuoyIGC>& buoy);
            virtual /* void */          ~ReleaseConsumerAction (void);
            virtual void                Execute (void);

        protected:
                    TRef<IbuoyIGC>      m_buoy;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_SIGNATURE_ACTION_H_
