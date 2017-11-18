/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	commandacknowledgedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "commandacknowledgedcondition" interface.
**
**  History:
*/
#ifndef _GET_COMMAND_ACKNOWLEDGED_CONDITION_H_
#define _GET_COMMAND_ACKNOWLEDGED_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CommandAcknowledgedCondition : public Condition
    {
        public:
                    /* void */          CommandAcknowledgedCondition (char cTargetCommand);
            virtual /* void */          ~CommandAcknowledgedCondition (void);
            virtual bool                Evaluate (void);
            static  void                SetCommandAcknowledged (char cAcknowledged);

        protected:
                    char                m_cTargetCommand;
            static  char                m_cCommandAcknowledged;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_COMMAND_ACKNOWLEDGED_CONDITION_H_
