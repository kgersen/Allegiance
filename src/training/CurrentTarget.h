/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CurrentTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CurrentTarget" interface.
**
**  History:
*/
#ifndef _CURRENT_TARGET_H_
#define _CURRENT_TARGET_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CurrentTarget : public AbstractTarget
    {
        public:
                    /* void */          CurrentTarget (AbstractTarget* pTarget, bool bLockOnFirstFetch = false);
            virtual /* void */          ~CurrentTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void);

        protected:
                    AbstractTarget*     m_pTarget;
                    bool                m_bLockOnFirstFetch;
                    AbstractTarget*     m_pFetched;
    };

    //------------------------------------------------------------------------------
}

#endif  //_CURRENT_TARGET_H_
