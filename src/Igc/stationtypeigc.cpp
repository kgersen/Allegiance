/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	stationTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CstationTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// stationTypeIGC.cpp : Implementation of CstationTypeIGC
#include "stationtypeigc.h"

/////////////////////////////////////////////////////////////////////////////
// CstationTypeIGC
HRESULT         CstationTypeIGC::Initialize(ImissionIGC* pMission,
                                            Time         now,
                                            const void*  data,
                                            int          dataSize)
{
    assert (pMission);
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize >= sizeof(DataStationTypeIGC)));
    m_data = *((DataStationTypeIGC*)data);

    pMission->GetIgcSite()->Preload(m_data.modelName, iswalpha(m_data.textureName[0])
                                                      ? m_data.textureName
                                                      : NULL);

    m_pdtConstruction = pMission->GetDroneType(m_data.constructionDroneTypeID);
    assert (m_pdtConstruction);
    m_pdtConstruction->AddRef();
    assert (m_pdtConstruction->GetPilotType() == c_ptBuilder);


    MultiHullBase*  pmhb = HitTest::Load(m_data.modelName);
    m_nLandSlots = 0;
    m_nCapLandSlots = 0;
    m_nLaunchSlots = 0;

    assert (pmhb);
    {
        float   scale = (m_data.radius / pmhb->GetOriginalRadius());

        {
            //Get the stations launch slots
            static char    bfrLaunch[] = "launchX";

            while (true)
            {
                //Do we have a launch slot?
                bfrLaunch[6] = '1' + m_nLaunchSlots;
                const FrameDataUTL* pfd = pmhb->GetFrame(bfrLaunch);
                if (pfd)
                {
                    assert (m_nLaunchSlots < c_maxLaunchSlots);
                    m_positionLaunches[m_nLaunchSlots] = pfd->position * scale;
                    m_directionLaunches[m_nLaunchSlots++] = pfd->forward;
                }
                else
                    break;
            }
        }
        {
            //debugf("Station %s/%s\n", GetName(), m_data.modelName);
            //Refigure the stations launch slots
            static char    bfrLand[] = "capgarageXA";

            while (true)
            {
                //Do we have a launch slot?
                bfrLand[9] = '1' + m_nLandSlots;
                bfrLand[10] = 'A';
                const FrameDataUTL* pfd = pmhb->GetFrame(bfrLand);
                if (pfd)
                {
                    assert (m_nLandSlots < c_maxLandSlots);

                    int i = 0;
                    do
                    {
                        assert (i < c_maxLandPlanes);
                        m_positionLandPlanes[m_nLandSlots][i] = pfd->position * scale;
                        m_directionLandPlanes[m_nLandSlots][i] = pfd->forward;

                        //debugf("\t%s %f %f %f\n", bfrLand, m_positionLandPlanes[m_nLandSlots][i].x, m_positionLandPlanes[m_nLandSlots][i].y, m_positionLandPlanes[m_nLandSlots][i].z);

                        bfrLand[10] = 'A' + ++i;
                        pfd = pmhb->GetFrame(bfrLand);
                    }
                    while (pfd);

                    m_nLandPlanes[m_nLandSlots++] = i;
                }
                else
                    break;
            }

            m_nCapLandSlots = m_nLandSlots;
        }
        {
            //debugf("Station %s/%s\n", GetName(), m_data.modelName);
            //Refigure the stations launch slots
            static char    bfrLand[] = "garageXA";

            while (true)
            {
                //Do we have a launch slot?
                bfrLand[6] = '1' + m_nLandSlots;
                bfrLand[7] = 'A';
                const FrameDataUTL* pfd = pmhb->GetFrame(bfrLand);
                if (pfd)
                {
                    assert (m_nLandSlots < c_maxLandSlots);

                    int i = 0;
                    do
                    {
                        assert (i < c_maxLandPlanes);
                        m_positionLandPlanes[m_nLandSlots][i] = pfd->position * scale;
                        m_directionLandPlanes[m_nLandSlots][i] = pfd->forward;

                        //debugf("\t%s %f %f %f\n", bfrLand, m_positionLandPlanes[m_nLandSlots][i].x, m_positionLandPlanes[m_nLandSlots][i].y, m_positionLandPlanes[m_nLandSlots][i].z);

                        bfrLand[7] = 'A' + ++i;
                        pfd = pmhb->GetFrame(bfrLand);
                    }
                    while (pfd);

                    m_nLandPlanes[m_nLandSlots++] = i;
                }
                else
                    break;
            }
        }
    }
    pMission->AddStationType(this);

    {
        //Create a station upgrade (if this station has an upgrade)
        if (m_data.successorStationTypeID != NA)
        {
            m_pstSuccessor = pMission->GetStationType(m_data.successorStationTypeID);
            assert (m_pstSuccessor);
            m_pstSuccessor->AddRef();
        }
        else
            assert (m_pstSuccessor == NULL);
    }

    return S_OK;
}

void            CstationTypeIGC::Terminate(void)
{
    if (m_pstSuccessor)
    {
        m_pstSuccessor->Release();
        m_pstSuccessor = NULL;
    }

    assert (m_pdtConstruction);
    m_pdtConstruction->Release();
    m_pdtConstruction = NULL;

    m_pMission->DeleteStationType(this);
}

int             CstationTypeIGC::Export(void*  data) const
{
    if (data)
        *((DataStationTypeIGC*)data) = m_data;

    return sizeof(DataStationTypeIGC);
}

