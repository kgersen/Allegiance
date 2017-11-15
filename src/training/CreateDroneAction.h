/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateDroneAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CreateDroneAction" interface.
**
**  History:
*/
#ifndef _CREATE_DRONE_ACTION_H_
#define _CREATE_DRONE_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif// _ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CreateDroneAction : public Action
    {
        public:
                    /* void */          CreateDroneAction (const ZString& name, ShipID shipID, HullID hullID, SideID sideID, PilotType pilotType, int techLeve = 1);
            virtual /* void */          ~CreateDroneAction (void);
            virtual void                Execute (void);
                    void                SetCreatedLocation (SectorID sectorID, const Vector& position);
                    void                SetCreatedOrientation (const Vector& forward, const Vector& up);
                    void                SetStationType (StationTypeID stationTypeID);
                    void                SetExpendableType (ExpendableTypeID expendableTypeID);

        protected:
                    ZString             m_name;
                    ShipID              m_shipID;
                    HullID              m_hullID;
                    SideID              m_sideID;
                    PilotType           m_pilotType;
                    SectorID            m_sectorID;
                    Vector              m_position;
                    Orientation         m_orientation;
                    StationTypeID       m_stationTypeID;
                    ExpendableTypeID    m_expendableTypeID;
                    int                 m_techLevel;
    };

    //------------------------------------------------------------------------------
}

#endif  //_CREATE_DRONE_ACTION_H_
