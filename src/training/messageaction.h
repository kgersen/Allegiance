/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	messageaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "messageaction" interface.
**
**  History:
*/
#ifndef _MESSAGE_ACTION_H_
#define _MESSAGE_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class MessageAction : public Action
    {
        public:
                    /* void */          MessageAction (void);
                    /* void */          MessageAction (const ZString message);
            virtual /* void */          ~MessageAction (void);
            virtual void                Execute (void);
                    void                AddMessage (const ZString message);

        protected:
                    std::list<ZString>              m_messageList;
                    std::list<ZString>::iterator    m_iterator;
    };

    //------------------------------------------------------------------------------
}

#endif  //_MESSAGE_ACTION_H_
