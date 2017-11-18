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
                    /* void */          MessageAction (const ZString message, const SoundID soundID = NA); //TheBored 06-JUL-07: Bahdohday's sound w/text edit.
            virtual /* void */          ~MessageAction (void);
            virtual void                Execute (void);
                    void                AddMessage (const ZString message, const SoundID soundID = NA); //TheBored 06-JUL-07: Bahdohday's sound w/text edit.

        protected:
                    std::list<ZString>              m_messageList;
                    std::list<ZString>::iterator    m_iterator;
					//TheBored 06-JUL-07: Bahdohday's sound w/text edit.
					std::list<SoundID>				m_soundList;
					std::list<SoundID>::iterator	m_soundIterator;
					//End TB
    };

    //------------------------------------------------------------------------------
}

#endif  //_MESSAGE_ACTION_H_
