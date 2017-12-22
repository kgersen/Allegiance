/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateProbeAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "CreateProbeAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "CreateProbeAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  CreateProbeAction::CreateProbeAction (ProbeID probeID, ExpendableTypeID typeID, const Vector& position, SideID sideID, SectorID sectorID) : 
    CreateObjectAction (OT_probe, &m_probeData, sizeof (m_probeData)),
    m_probeID (probeID),
    m_typeID (typeID),
    m_position (position),
    m_sideID (sideID),
    m_sectorID (sectorID)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  CreateProbeAction::~CreateProbeAction (void)
    {
        // We delete nothing here
    }

    //------------------------------------------------------------------------------
    void        CreateProbeAction::Execute (void)
    {
        ImissionIGC*    pMission = trekClient.GetCore();
        m_probeData.p0 = m_position;
        m_probeData.time0 = pMission->GetLastUpdate();
        m_probeData.probeID = m_probeID;
        m_probeData.exportF = false;
        m_probeData.pprobetype = static_cast<IprobeTypeIGC*> (pMission->GetExpendableType (m_typeID));
        m_probeData.pside = pMission->GetSide (m_sideID);
        m_probeData.pcluster = pMission->GetCluster (m_sectorID);
        CreateObjectAction::Execute ();
    }

    //------------------------------------------------------------------------------
}
