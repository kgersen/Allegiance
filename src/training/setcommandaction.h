/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setcommandaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setcommandaction" interface.
**
**  History:
*/
#ifndef _SET_COMMAND_ACTION_H_
#define _SET_COMMAND_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetCommandAction : public Action
    {
        public:
                    /* void */          SetCommandAction (IshipIGC* pShip, Command command, ImodelIGC* pTarget, CommandID commandID);
                    /* void */          SetCommandAction (IshipIGC* pShip, Command command, ObjectType targetType, ObjectID targetID, CommandID commandID);
                    /* void */          SetCommandAction (ShipID shipID, Command command, ObjectType targetType, ObjectID targetID, CommandID commandID);
                    /* void */          SetCommandAction (ShipID shipID, Command command, ImodelIGC* pTarget, CommandID commandID);
            virtual /* void */          ~SetCommandAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pShip;
                    Command             m_command;
                    AbstractTarget*     m_pTarget;
                    CommandID           m_commandID;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_COMMAND_ACTION_H_
