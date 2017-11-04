/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	sideIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CsideIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// sideIGC.cpp : Implementation of CsideIGC
#include "sideigc.h"

/////////////////////////////////////////////////////////////////////////////
// CsideIGC
HRESULT         CsideIGC::Initialize(ImissionIGC*   pMission,
                                     Time           now,
                                     const void*    data,
                                     int            dataSize)
{
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize >= sizeof(DataSideIGC)));
    m_data = *((DataSideIGC*)data);

    m_pCivilization = pMission->GetCivilization(m_data.civilizationID);
    assert (m_pCivilization);
    m_pCivilization->AddRef();

    m_ttbmBuildingTechs.ClearAll(); //Will automatically adjust for starting buildings

    m_lastUpdate = now;

    pMission->AddSide(this);

    m_pMission->GetIgcSite()->CreateSideEvent(static_cast<IsideIGC*>(this));

	//#ALLY
	m_data.allies = NA;	

	bRandomCivilization = false; //Xynth #170 8/10 Initialize new bool
    return S_OK;
}

void            CsideIGC::Terminate(void)
{
    m_pMission->GetIgcSite()->DestroySideEvent(static_cast<IsideIGC*>(this));

    AddRef();

    {
        //Nuke all of the side's buckets
        BucketLinkIGC*  l;
        while (l = m_buckets.first())       //intentional assignment
            l->data()->Terminate();
    }

    {
        //Set all of the side's ships to the neutral side
        ShipLinkIGC*  l;
        while (l = m_ships.first())       //intentional assignment
            l->data()->SetSide(NULL);
    }

    {
        //Nuke all of the stations
        StationLinkIGC*  l;
        while (l = m_stations.first())       //intentional assignment
            l->data()->Terminate();
    }

    m_pMission->DeleteSide(this);

    m_pCivilization->Release();
    m_pCivilization = NULL;

    Release();
}

int             CsideIGC::Export(void*  data) const
{
    int nBytes = sizeof(DataSideIGC);

    if (data)
        *((DataSideIGC*)data) = m_data;

    return sizeof(DataSideIGC);
}

void            CsideIGC::DestroyBuckets(void)
{
    m_data.ttbmDevelopmentTechs = GetCivilization()->GetBaseTechs();
    m_ttbmBuildingTechs.ClearAll();

    {
        //Nuke all of the side's buckets
        BucketLinkIGC*  l;
        while (l = m_buckets.first())       //intentional assignment
            l->data()->Terminate();
    }
}

void            CsideIGC::CreateBuckets(void)
{
    assert(!m_buckets.first());

    if (m_pMission->GetMissionParams()->bAllowDevelopments)
    {
        TechTreeBitMask     ttbmLocalUltimate;
        ttbmLocalUltimate.ClearAll();
        {
            //Resolve what the ultimate techs are for this civ ...
            m_ttbmUltimateTechs = m_data.ttbmDevelopmentTechs;

            {
                //Start with their initial techs and add anything they could capture.
                for (PartTypeLinkIGC*   l = m_pMission->GetPartTypes()->first();
                     (l != NULL);
                     l = l->next())
                {
                    m_ttbmUltimateTechs |= l->data()->GetEffectTechs();
                }
            }

            {
                //Do the building techs ... assume they capture every type of building
                for (StationTypeLinkIGC*    l = m_pMission->GetStationTypes()->first();
                     (l != NULL);
                     l = l->next())
                {
                    m_ttbmUltimateTechs |= l->data()->GetEffectTechs();
                    ttbmLocalUltimate |= l->data()->GetLocalTechs();
                }
            }

            {
                //Now add any techs they could buy (assuming they could build or
                //buy them) until we have a pass where we didn't add any.
                bool    continueF;
                do
                {
                    continueF = false;
                    {
                        //Do the same for developments
                        for (DevelopmentLinkIGC*    l = m_pMission->GetDevelopments()->first();
                             (l != NULL);
                             l = l->next())
                        {
                            //Can we buy it?
                            if (l->data()->GetRequiredTechs() <= m_ttbmUltimateTechs)
                            {
                                //yes ... does it make a difference?
                                if (!(l->data()->GetEffectTechs() <= m_ttbmUltimateTechs))
                                {
                                    //yes ... 'buy it'
                                    m_ttbmUltimateTechs |= l->data()->GetEffectTechs();
                                    continueF = true;
                                }
                            }
                        }
                    }
                }
                while (continueF);
            }
        }

        {
            //Add all developments that the side could, potentially, produce
            for (DevelopmentLinkIGC*    l = m_pMission->GetDevelopments()->first();
                 (l != NULL);
                 l = l->next())
            {
                IdevelopmentIGC*    d = l->data();

                //Only add the bucket if I'll be able to buy it eventually and it is not
                //obsolete.
                if ((d->GetObjectID() != c_didTeamMoney) && (d->GetRequiredTechs() <= m_ttbmUltimateTechs) && 
                    !d->IsObsolete(m_data.ttbmInitialTechs))
                {
                    //It is something we might be able to build and it will actually
                    //do something useful
                    DataBucketIGC   db = {d, this};
                    IbucketIGC*     b = (IbucketIGC*)(m_pMission->CreateObject(m_lastUpdate,
                                                                               OT_bucket,
                                                                               &db,
                                                                               sizeof(db)));
                    assert (b);

                    b->Release();   //Creating the bucket adds it to the side's list of buckets
                }
            }
        }

        {
            //Add All drone and station types that they might, theoretically, be able to produce
            ttbmLocalUltimate |= m_ttbmUltimateTechs;

            {
                //Add all developments that the side could, potentially, produce
				for (DroneTypeLinkIGC*    l = m_pMission->GetDroneTypes()->first();
					(l != NULL);
					l = l->next())
				{
					IdroneTypeIGC*    d = l->data();

					if (d != NULL) //Xynth d->GetRequiredTechs() bombed out 
					{
						if (d->GetRequiredTechs() <= ttbmLocalUltimate)
						{
							//It is something we might be able to build and it will actually
							//do something useful
							DataBucketIGC   db = { d, this };
							IbucketIGC*     b = (IbucketIGC*)(m_pMission->CreateObject(m_lastUpdate,
								OT_bucket,
								&db,
								sizeof(db)));
							assert(b);
							b->Release();   //Creating the bucket adds it to the side's list of buckets
						}
					}
                }
            }

            {
                //Add all developments that the side could, potentially, produce
                for (StationTypeLinkIGC*    l = m_pMission->GetStationTypes()->first();
                     (l != NULL);
                     l = l->next())
                {
                    IstationTypeIGC*    s = l->data();

                    if (s->GetRequiredTechs() <= ttbmLocalUltimate)
                    {
                        //It is something we might be able to build and it will actually
                        //do something useful
                        DataBucketIGC   db = {s, this};
                        IbucketIGC*     b = (IbucketIGC*)(m_pMission->CreateObject(m_lastUpdate,
                                                                                   OT_bucket,
                                                                                   &db,
                                                                                   sizeof(db)));
                        assert (b);
                        b->Release();   //Creating the bucket adds it to the side's list of buckets
                    }
                }
            }

            //Add all hull and part types that cost money
            {
                /*
                for (HullTypeLinkIGC*    l = m_pMission->GetHullTypes()->first();
                     (l != NULL);
                     l = l->next())
                {
                    IhullTypeIGC*    h = l->data();

                    if ((h->GetPrice() != 0) && (h->GetTimeToBuild() != 0) && 
                        (h->GetRequiredTechs() <= ttbmLocalUltimate))
                    {
                        //It is something we might be able to build and it will actually
                        //do something useful
                        DataBucketIGC   db = {h, this};
                        IbucketIGC*     b = (IbucketIGC*)(m_pMission->CreateObject(m_lastUpdate,
                                                                                   OT_bucket,
                                                                                   &db,
                                                                                   sizeof(db)));
                        assert (b);
                        b->Release();   //Creating the bucket adds it to the side's list of buckets
                    }
                }
                */
            }

            {
                /*
                for (PartTypeLinkIGC*    l = m_pMission->GetPartTypes()->first();
                     (l != NULL);
                     l = l->next())
                {
                    IpartTypeIGC*    p = l->data();

                    if ((p->GetPrice() != 0) && (p->GetTimeToBuild() != 0) &&
                        (p->GetRequiredTechs() <= ttbmLocalUltimate))
                    {
                        //It is something we might be able to build and it will actually
                        //do something useful
                        DataBucketIGC   db = {p, this};
                        IbucketIGC*     b = (IbucketIGC*)(m_pMission->CreateObject(m_lastUpdate,
                                                                                   OT_bucket,
                                                                                   &db,
                                                                                   sizeof(db)));
                        assert (b);

                        if (IlauncherTypeIGC::IsLauncherType(p->GetEquipmentType()))
                        {
                            b->SetPrice(b->GetPrice() * p->GetAmount(NULL));
                        }

                        b->Release();   //Creating the bucket adds it to the side's list of buckets
                    }
                }
                */
            }
        }
    }
}

long CsideIGC::GetProsperityPercentBought(void) const
{
  // Iterate through each bucket, looking for the prosperity development
  const BucketListIGC* pBuckets = GetBuckets();
  for (BucketLinkIGC* it = pBuckets->first(); it; it = it->next())
  {
    IbucketIGC* pBucket = it->data();
    assert(pBucket);

    if (OT_development == pBucket->GetBucketType())
      if (c_didTeamMoney == pBucket->GetBuyable()->GetObjectID())
        return pBucket->GetPercentBought();
  }

  // Return zero if no prosperity development was found
  return 0;
}

long CsideIGC::GetProsperityPercentComplete(void) const
{
  // Iterate through each bucket, looking for the prosperity development
  const BucketListIGC* pBuckets = GetBuckets();
  for (BucketLinkIGC* it = pBuckets->first(); it; it = it->next())
  {
    IbucketIGC* pBucket = it->data();
    assert(pBucket);

    if (OT_development == pBucket->GetBucketType())
      if (c_didTeamMoney == pBucket->GetBuyable()->GetObjectID())
        return pBucket->GetPercentComplete();
  }

  // Return zero if no prosperity development was found
  return 0;
}

