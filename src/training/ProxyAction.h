/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	proxyaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "proxyaction" interface.
**
**  History:
*/
#ifndef _PROXY_ACTION_H_
#define _PROXY_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ProxyAction : public Action
    {
        public:
                    /* void */          ProxyAction (Action* pAction);
            virtual /* void */          ~ProxyAction (void);
            virtual void                Execute (void);

        protected:
                    Action*             m_pAction;
    };

    //------------------------------------------------------------------------------
}

#endif  //_PROXY_ACTION_H_
