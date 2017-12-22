/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetSectorCondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "GetSectorCondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetSectorCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetSectorCondition::GetSectorCondition (ImodelIGC* pModel, SectorID sector) :
    m_pModel (new TypeIDTarget (pModel->GetObjectType (), pModel->GetObjectID ())),
    m_sector (sector)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetSectorCondition::GetSectorCondition (ObjectType objectType, ObjectID objectID, SectorID sector) :
    m_pModel (new TypeIDTarget (objectType, objectID)),
    m_sector (sector)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetSectorCondition::GetSectorCondition (AbstractTarget* pTarget, SectorID sector) :
    m_pModel (pTarget),
    m_sector (sector)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetSectorCondition::~GetSectorCondition (void)
    {
        delete m_pModel;
    }

    //------------------------------------------------------------------------------
    bool        GetSectorCondition::Evaluate (void)
    {
        ImodelIGC*  pModel = *m_pModel;
        if (pModel)
        {
            // here we know that the model to be tested is valid
            return (static_cast<SectorID> (pModel->GetCluster ()->GetObjectID ()) == m_sector) ? true : false;
        }
        else
        {
            // here the model is not valid, so we assume the condition is false
            return false;
        }
    }

    //------------------------------------------------------------------------------
}
