/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	TypeIDTarget.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "TypeIDTarget" interface.
**
**  History:
*/
#include    "pch.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  TypeIDTarget::TypeIDTarget (ObjectType targetType, ObjectID targetID) :
    m_targetType (targetType),
    m_targetID (targetID)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  TypeIDTarget::~TypeIDTarget (void)
    {
    }

    //------------------------------------------------------------------------------
    /* ImodelIGC* */    TypeIDTarget::operator ImodelIGC* (void)
    {
        return trekClient.GetCore ()->GetModel (m_targetType, m_targetID);
    }

    //------------------------------------------------------------------------------
}
