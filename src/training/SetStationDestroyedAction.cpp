/*
**
**  File:	setstationdestroyedaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setstationdestroyedaction" interface.
**
**  History:
*/
#include    "pch.h"
#ifndef _TRAINING_MISSION_H_
#include "TrainingMission.h"
#endif
#include    "SetStationDestroyedAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetStationDestroyedAction::SetStationDestroyedAction (ImodelIGC* theStation)
    {
       pStation = theStation;
    }

    //------------------------------------------------------------------------------
    /* void */  SetStationDestroyedAction::~SetStationDestroyedAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetStationDestroyedAction::Execute (void)
    {
		if (pStation)
			pStation->Terminate();
    }
}
