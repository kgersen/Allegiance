/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateObjectAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CreateObjectAction" interface.
**
**  History:
*/
#ifndef _CREATE_OBJECT_ACTION_H_
#define _CREATE_OBJECT_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CreateObjectAction : public Action
    {
        public:
                    /* void */          CreateObjectAction (ObjectType objectType, const void* pData, int iDataSize);
            virtual /* void */          ~CreateObjectAction (void);
            virtual void                Execute (void);

        protected:
                    ObjectType          m_objectType;
                    const void*         m_pData;
                    int                 m_iDataSize;
                    TRef<IbaseIGC>      m_result;
    };

    //------------------------------------------------------------------------------
}

#endif  //_CREATE_OBJECT_ACTION_H_
