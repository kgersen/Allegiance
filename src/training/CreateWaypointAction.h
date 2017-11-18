/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateWaypointAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CreateWaypointAction" interface.
**
**  History:
*/
#ifndef _CREATE_WAYPOINT_ACTION_H_
#define _CREATE_WAYPOINT_ACTION_H_

#ifndef _CREATE_OBJECT_ACTION_H_
#include "CreateObjectAction.h"
#endif// _CREATE_OBJECT_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CreateWaypointAction : public CreateObjectAction
    {
        public:
                    /* void */          CreateWaypointAction (BuoyID buoyID, const Vector& position, SectorID sectorID);
            virtual /* void */          ~CreateWaypointAction (void);
            virtual void                Execute (void);

        protected:
                    BuoyID              m_buoyID;
                    Vector              m_position;
                    SectorID            m_sectorID;
                    DataBuoyIGC         m_buoyData;
    };

    //------------------------------------------------------------------------------
}

#endif  //_CREATE_WAYPOINT_ACTION_H_
