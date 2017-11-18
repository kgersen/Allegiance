/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetPickedCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetPickedCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetPickedCondition.h"
#include    "TypeIDTarget.h"
#include    "console.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetPickedCondition::GetPickedCondition (ObjectType targetType, ObjectID targetID) :
    m_pTarget (new TypeIDTarget (targetType, targetID))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetPickedCondition::~GetPickedCondition (void)
    {
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    bool        GetPickedCondition::Evaluate (void)
    {
        return (GetWindow ()->GetConsoleImage ()->GetConsoleData ()->GetPickedObject () == static_cast<IbaseIGC*> (static_cast<ImodelIGC*> (*m_pTarget))) ? true : false;
    }

    //------------------------------------------------------------------------------
}
