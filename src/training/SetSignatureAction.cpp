/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setsignatureaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setsignatureaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetSignatureAction.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetSignatureAction::SetSignatureAction (ImodelIGC* pObject, float fSignature) : 
    m_pTarget (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_fSignature (fSignature)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetSignatureAction::SetSignatureAction (ObjectType objectType, ObjectID objectID, float fSignature) : 
    m_pTarget (new TypeIDTarget (objectType, objectID)),
    m_fSignature (fSignature)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetSignatureAction::SetSignatureAction (AbstractTarget* pTarget, float fSignature) :
    m_pTarget (pTarget),
    m_fSignature (fSignature)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetSignatureAction::~SetSignatureAction (void)
    {
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    void        SetSignatureAction::Execute (void)
    {
        if (*m_pTarget)
            (*m_pTarget)->SetSignature (m_fSignature);
    }

    //------------------------------------------------------------------------------
}
