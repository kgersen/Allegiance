/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getradarlodcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "getradarlodcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "GetRadarLODCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  GetRadarLODCondition::GetRadarLODCondition (RadarImage::RadarLOD expectedRadarLOD) :
    m_expectedRadarLOD (expectedRadarLOD)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  GetRadarLODCondition::~GetRadarLODCondition (void)
    {
    }

    //------------------------------------------------------------------------------
    bool        GetRadarLODCondition::Evaluate (void)
    {
        return (GetWindow ()->GetRadarMode () == static_cast<int> (m_expectedRadarLOD)) ? true : false;
    }

    //------------------------------------------------------------------------------
}
