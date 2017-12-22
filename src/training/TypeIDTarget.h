/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	TypeIDTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "TypeIDTarget" interface.
**
**  History:
*/
#ifndef _TYPE_ID_TARGET_H_
#define _TYPE_ID_TARGET_H_

#ifndef _ABSTRACT_TARGET_H_
#include "AbstractTarget.h"
#endif  //_ABSTRACT_TARGET_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class TypeIDTarget : public AbstractTarget
    {
        public:
                    /* void */          TypeIDTarget (ObjectType targetType, ObjectID targetID);
            virtual /* void */          ~TypeIDTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void);

        protected:
                    ObjectType          m_targetType;
                    ObjectID            m_targetID;
    };

    //------------------------------------------------------------------------------
}

#endif  //_TYPE_ID_TARGET_H_
