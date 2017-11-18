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
       pStation = static_cast<IstationIGC*>(theStation);
       ZAssert(pStation);
    }

    //------------------------------------------------------------------------------
    /* void */  SetStationDestroyedAction::SetStationDestroyedAction(IstationIGC* theStation)
    {
        pStation = theStation;
        ZAssert(pStation);
    }

    //------------------------------------------------------------------------------
    /* void */  SetStationDestroyedAction::~SetStationDestroyedAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetStationDestroyedAction::Execute (void)
    {
        if (pStation) {
            pStation->GetCluster()->GetClusterSite()->AddExplosion(pStation,
                pStation->GetStationType()->HasCapability(c_sabmFlag)
                ? c_etLargeStation
                : c_etSmallStation);

            if (pStation->GetSide() == trekClient.GetSide())
                trekClient.PlaySoundEffect(pStation->GetStationType()->GetDestroyedSound());
            else
                trekClient.PlaySoundEffect(pStation->GetStationType()->GetEnemyDestroyedSound());

            trekClient.PostText(true, "The " START_COLOR_STRING "%s" END_COLOR_STRING " station has been destroyed.",
                (PCC)ConvertColorToString(pStation ? pStation->GetSide()->GetColor() : Color::White()),
                (pStation ? pStation->GetName() : "Unknown station")
            );

            pStation->Terminate();
        }
    }
}
