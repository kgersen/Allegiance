/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateDroneAction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "CreateDroneAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "CreateDroneAction.h"
#include    "TrainingMission.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // global variabes
    //------------------------------------------------------------------------------
    extern TrainingMission*    g_pMission;

    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  CreateDroneAction::CreateDroneAction(const ZString& name, ShipID shipID, HullID hullID, SideID sideID, PilotType pilotType, int techLevel) :
        m_name(name),
        m_shipID(shipID),
        m_hullID(hullID),
        m_sideID(sideID),
        m_pilotType(pilotType),
        m_techLevel(techLevel),
        m_sectorID(NA),
        m_position(0.0f, 0.0f, 0.0f),
        m_orientation(Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)),
        m_stationTypeID(NA),
        m_expendableTypeID(NA)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  CreateDroneAction::~CreateDroneAction(void)
    {
    }

    //------------------------------------------------------------------------------
    void        CreateDroneAction::Execute(void)
    {
        if (m_sectorID == NA)
        {
            // figure some default location
        }
        IshipIGC*   pShip = g_pMission->CreateDrone(m_name, m_shipID, m_hullID, m_sideID, m_pilotType);
        pShip->SetPosition(m_position);
        pShip->SetOrientation(m_orientation);
        pShip->SetCluster(trekClient.GetCore()->GetCluster(m_sectorID));

        if (m_stationTypeID != NA)
            pShip->SetBaseData(trekClient.GetCore()->GetStationType(m_stationTypeID));
        else if (m_expendableTypeID != NA)
            pShip->SetBaseData(trekClient.GetCore()->GetExpendableType(m_expendableTypeID));

        debugf("CreateDroneActionExecute for %s\n", (LPCSTR)m_name);
        if (m_techLevel != 1) {
            for (int i = 0; i < pShip->GetHullType()->GetMaxWeapons(); i++) {
                IpartIGC* pPart = pShip->GetMountedPart(ET_Weapon, i);
                if (pPart) {
                    int partTypeID = pPart->GetPartType()->GetObjectID();
                    debugf(" Weapon slot %d: %d", i, partTypeID);
                    if (partTypeID == 33) { //33 PW Gat Gun
                        if (m_techLevel > 1)
                            pPart->Terminate();
                        if (m_techLevel == 2)
                            pShip->CreateAndAddPart(trekClient.GetCore()->GetPartType(71), i, 1); //PW Gat 2
                        else if (m_techLevel >= 3)
                            pShip->CreateAndAddPart(trekClient.GetCore()->GetPartType(71), i, 1); //PW Gat 2
                    }
                    else if (partTypeID == 200) //Nerf for rix bomber. One gun vs Bios outpost is enough
                        if (i > 0 && m_techLevel < 1)
                            pPart->Terminate();
                    pPart = pShip->GetMountedPart(ET_Weapon, i);
                    if (pPart)
                        debugf(" changed to %d\n", pPart->GetPartType()->GetObjectID());
                    else
                        debugf(" removed\n");
                }
            }
            trekClient.SaveLoadout(pShip);
        }

    }

    //------------------------------------------------------------------------------
    void        CreateDroneAction::SetCreatedLocation (SectorID sectorID, const Vector& position)
    {
        m_sectorID = sectorID;
        m_position = position;
    }

    //------------------------------------------------------------------------------
    void        CreateDroneAction::SetCreatedOrientation (const Vector& forward, const Vector& up)
    {
        m_orientation.Set (forward, up);
    }

    //------------------------------------------------------------------------------
    void        CreateDroneAction::SetStationType (StationTypeID stationTypeID)
    {
        m_stationTypeID = stationTypeID;
    }

    //------------------------------------------------------------------------------
    void        CreateDroneAction::SetExpendableType (ExpendableTypeID expendableTypeID)
    {
        m_expendableTypeID = expendableTypeID;
    }

    //------------------------------------------------------------------------------
}
