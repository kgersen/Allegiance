/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	partTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CpartTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// partTypeIGC.cpp : Implementation of CpartTypeIGC
#include "pch.h"
#include "launcherTypeIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CpartTypeIGC
HRESULT         ClauncherTypeIGC::Initialize(ImissionIGC*   pMission,
                                             Time           now,
                                             const void*    data,
                                             int            dataSize)
{
    assert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize >= sizeof(DataLauncherTypeIGC)));
    {
        m_data = *((const DataLauncherTypeIGC*)data);

        m_pexpendabletype = m_pMission->GetExpendableType(m_data.expendabletypeID);

        if (m_pexpendabletype)
        {
            m_equipmenttype = m_pexpendabletype->GetEquipmentType();

            m_pexpendabletype->AddRef();
            m_launcherDef = m_pexpendabletype->GetLauncherDef();

            if (m_data.successorPartID != NA)
            {
                m_pptSuccessor = pMission->GetPartType(m_data.successorPartID);
                assert (m_pptSuccessor);
            }

            pMission->AddPartType(this);
        }
    }

    return S_OK;
}


void        ClauncherTypeIGC::Terminate(void)
{
    assert (m_pMission);

    if (m_pexpendabletype != NULL)
    {
        m_pexpendabletype->Release();
        m_pexpendabletype = NULL;
    }

    m_pMission->DeletePartType(this);
}

int         ClauncherTypeIGC::Export(void*  data) const
{
    if (data)
        *((DataLauncherTypeIGC*)data) = m_data;

    return sizeof(m_data);
}
