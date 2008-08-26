/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	warpIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CwarpIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// warpIGC.cpp : Implementation of CwarpIGC
#include "pch.h"
#include "warpIGC.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// CwarpIGC
HRESULT     CwarpIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    TmodelIGC<IwarpIGC>::Initialize(pMission, now, data, dataSize);

    ZRetailAssert (data && (dataSize == sizeof(DataWarpIGC)));
    {
        DataWarpIGC*  dataWarp = (DataWarpIGC*)data;
        m_warpDef = dataWarp->warpDef;

        IclusterIGC*    cluster = pMission->GetCluster(dataWarp->clusterID);
        ZRetailAssert (cluster);
        {
            //  , use the real texture name
            HRESULT rc = LoadWarp(dataWarp->warpDef.textureName,
                                  dataWarp->warpDef.iconName,
                                  c_mtStatic | c_mtHitable | c_mtSeenBySide | c_mtPredictable);

            assert (SUCCEEDED(rc));
            {
                SetRadius((float)m_warpDef.radius);

                SetPosition(dataWarp->position);
                SetVelocity(Vector::GetZero());

                {
                    Orientation o(dataWarp->forward);
                    SetOrientation(o);
                }
                SetRotation(dataWarp->rotation);
                SetCluster(cluster);

                SetMass(0.0f);
				// KG- hack for unmovable alephs (to avoid a IGC file format change)
				// aleph name with a leading '*' denotes a fixed position aleph
				// so remove the '*' from the name and set m_bFixedPosition to true
				if (dataWarp->name[0] == '*')
				{
					m_bFixedPosition = true;
					SetName(&(dataWarp->name[1])); // skip the leading '*'
				}
				else
                	SetName(dataWarp->name);

                SetSignature(dataWarp->signature);

                pMission->AddWarp(this);
            }
        }
    }

    return S_OK;
}

int         CwarpIGC::Export(void* data) const
{
    if (data)
    {
        DataWarpIGC*  dataWarp = (DataWarpIGC*)data;
        dataWarp->warpDef = m_warpDef;

        dataWarp->position = GetPosition();
        dataWarp->forward = GetOrientation().GetForward();
        dataWarp->rotation = GetRotation();

        assert (GetCluster());
        dataWarp->clusterID = GetCluster()->GetObjectID();
        UTL::putName(dataWarp->name, GetName());
        dataWarp->signature = GetSignature();
    }

    return sizeof(DataWarpIGC);
}
