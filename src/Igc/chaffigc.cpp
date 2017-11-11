/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	chaffIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CchaffIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**      This particular implementation of chaffs supports non-tracking missiles with
**  a fixed run (time at which they expire).
**
**  History:
*/
// chaffIGC.cpp : Implementation of CchaffIGC
#include "chaffigc.h"


const float c_fLifespan = 5.0f;

/////////////////////////////////////////////////////////////////////////////
// CchaffIGC
CchaffIGC::CchaffIGC(void)
{
}

CchaffIGC::~CchaffIGC(void)
{
}

HRESULT CchaffIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    TmodelIGC<IchaffIGC>::Initialize(pMission, now, data, dataSize);

    ZRetailAssert (data && (dataSize == sizeof(DataChaffIGC)));
    DataChaffIGC*  dataChaff = (DataChaffIGC*)data;

    m_pChaffTypeData = (DataChaffTypeIGC*)(dataChaff->pchafftype->GetData());

    LoadDecal(m_pChaffTypeData->textureName, NULL,
              Color(1.0f, 1.0f, 1.0f, 1.0f),
              false,
              1.0f,
              c_mtNotPickable | c_mtPredictable);

    Time    time0 = pMission->GetIgcSite()->ClientTimeFromServerTime(dataChaff->time0);

    SetRadius(1.0f);
    SetPosition(dataChaff->p0 + (now - time0) * dataChaff->v0);
    SetVelocity(dataChaff->v0);

    {
        Rotation    r(Vector(0.0f, 0.0f, 1.0f), 2.75f);
        SetRotation(r);
    }

    SetMass(0.0f);

    m_timeExpire = time0 + m_pChaffTypeData->lifespan;

    SetCluster(dataChaff->pcluster);

    return S_OK;
}

void    CchaffIGC::Terminate(void)
{
    AddRef();
	TmodelIGC<IchaffIGC>::Terminate();
    Release();
}

void    CchaffIGC::Update(Time now)
{
    float   dtLeft = m_timeExpire - now;
    if (dtLeft <= 0.0f)
        Terminate();
    else
    {
        float   r = m_pChaffTypeData->radius * (1.0f - dtLeft / m_pChaffTypeData->lifespan);
        SetRadius(r > 1.0f ? r : 1.0f);
        TmodelIGC<IchaffIGC>::Update(now);
    }
}
