/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setsignatureaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setsignatureaction" interface.
**
**  History:
*/
#ifndef _SET_SIGNATURE_ACTION_H_
#define _SET_SIGNATURE_ACTION_H_

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
    class SetSignatureAction : public Action
    {
        public:
                    /* void */          SetSignatureAction (ImodelIGC* object, float fSignature);
                    /* void */          SetSignatureAction (ObjectType objectType, ObjectID objectID, float fSignature);
                    /* void */          SetSignatureAction (AbstractTarget* pTarget, float fSignature);
            virtual /* void */          ~SetSignatureAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pTarget;
                    float               m_fSignature;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_SIGNATURE_ACTION_H_
