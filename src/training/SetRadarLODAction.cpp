/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setradarlodaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setradarlodaction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "SetRadarLODAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetRadarLODAction::SetRadarLODAction (RadarImage::RadarLOD radarLOD) : 
    m_radarLOD (radarLOD)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  SetRadarLODAction::~SetRadarLODAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetRadarLODAction::Execute (void)
    {
        TrekWindow* window = GetWindow ();
        if (window->GetRadarMode () != static_cast<int> (m_radarLOD))
            GetWindow ()->SetRadarMode (static_cast<int> (m_radarLOD));
    }

    //------------------------------------------------------------------------------
}
