/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	TurnToAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "TurnToAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "TurnToAction.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern  ControlData         g_setInputControls;         // yaw, pitch, roll, throttle - the ones we are forcing on the player

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  TurnToAction::TurnToAction (ImodelIGC* pObject, ImodelIGC* pTarget) : 
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (new TypeIDTarget (pTarget->GetObjectType (), pTarget->GetObjectID ()))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  TurnToAction::TurnToAction (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID) : 
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (new TypeIDTarget (targetType, targetID))
    {
    }

    //------------------------------------------------------------------------------
    /* void */  TurnToAction::TurnToAction (ImodelIGC* pObject, AbstractTarget* pTarget) : 
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (pTarget)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  TurnToAction::~TurnToAction (void)
    {
        delete m_pObject;
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    void        TurnToAction::Execute (void)
    {
        IshipIGC*   pShip = static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pObject));
        ImodelIGC*  pTarget = static_cast<ImodelIGC*> (*m_pTarget);
        if (pShip && pTarget)
        {
            Vector      delta = pTarget->GetPosition () - pShip->GetPosition ();
            turnToFace (delta, GetWindow ()->GetDeltaTime (), pShip, &g_setInputControls);
            g_setInputControls.jsValues[c_axisYaw] *= 0.5f;
            g_setInputControls.jsValues[c_axisPitch] *= 0.5f;
            g_setInputControls.jsValues[c_axisRoll] *= 0.5f;
        }
        else
        {
            g_setInputControls.jsValues[c_axisYaw] = NA;
            g_setInputControls.jsValues[c_axisPitch] = NA;
            g_setInputControls.jsValues[c_axisRoll] = NA;
        }
    }

    //------------------------------------------------------------------------------
}
