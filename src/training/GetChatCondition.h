/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetChatCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetChatCondition" interface.
**
**  History:
*/
#ifndef _GET_CHAT_CONDITION_H_
#define _GET_CHAT_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetChatCondition : public Condition
    {
        public:
                    /* void */          GetChatCondition (ChatTarget expectedRecipient);
            virtual /* void */          ~GetChatCondition (void);
            virtual bool                Start (void);
            virtual bool                Evaluate (void);
                    void                RecordChat (ChatTarget recipient);

        protected:
                    ChatTarget          m_expectedRecipient;
                    bool                m_bGotExpectedChat;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_CHAT_CONDITION_H_
