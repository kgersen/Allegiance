/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    bucketIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CbucketIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// bucketIGC.h : Declaration of the CbucketIGC

#ifndef __BUCKETIGC_H_
#define __BUCKETIGC_H_

/////////////////////////////////////////////////////////////////////////////
// CbucketIGC
class       CbucketIGC : public IbucketIGC
{
    public:
        CbucketIGC(void)
        :
            m_investedMilliseconds(0),
            m_investedMoney(0),
            m_lastPercentCompleteNotified(0),
            m_pbucketPredecessor(NULL),
			m_isBuildingForFree(false)
        {
        }

    public:
    // IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);

        virtual void    Update(Time now)
        {
            assert (now >= m_lastUpdate);

			if (m_investedMoney > 0 || m_isBuildingForFree) //#264 was if(m_investedMoney > 0)
            {
                DWORD   ttbms = m_timeToBuild * 1000;
                if (m_investedMilliseconds < ttbms)
                {
                    m_investedMilliseconds += now.clock() - m_lastUpdate.clock();

                    if ((m_investedMoney >= m_price) && (m_investedMilliseconds >= ttbms))
                    {
                        ForceComplete(now);
                    }

                    if (m_price > 0)
                    {
                        DWORD   mim = 1000 * ((m_investedMoney * m_timeToBuild) / m_price);
                        if (m_investedMilliseconds > mim)
                            m_investedMilliseconds = mim;
                    }

                    {
                        int p = GetPercentComplete();
                        if (m_lastPercentCompleteNotified != p)
                        {
                            m_pMission->GetIgcSite()->BucketChangeEvent(c_bcPercentCompleteChange, this);
                            m_lastPercentCompleteNotified = p;
                        }
                    }
                }
            }
			//#264 Turkey 07/12
			//If a free bucket can start, start it. If it shouldn't be, stop.
			if (m_price == 0 && m_investedMilliseconds < (m_timeToBuild * 1000)) 
			{
				if (IsAvailableForFree())  
				{
					m_isBuildingForFree = true;
				} else {
					m_isBuildingForFree = false;
					m_investedMilliseconds = 0;
				}
			}
            m_lastUpdate = now;
        }

        virtual int         Export(void* data) const;

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_bucket;
        }

        virtual short                   GetObjectID(void) const
        {
            const short c_bucketMultiplier = 0x7fff / (1 + OT_bucketEnd - OT_bucketStart);
            assert (c_bucketMultiplier >= 500);

            return m_data.buyable->GetObjectID() +
                   (m_data.buyable->GetObjectType() - OT_bucketStart) * c_bucketMultiplier;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

    // ItypeIGC
        virtual const void*   GetData(void) const
        {
            return m_data.buyable->GetData();
        }

    // IbuyableIGC
        virtual const char*          GetModelName(void) const
        {
            return m_data.buyable->GetModelName();
        }

        virtual const char*          GetIconName(void) const
        {
            return m_data.buyable->GetIconName();
        }

        virtual const char*          GetName(void) const
        {
            return m_data.buyable->GetName();
        }
        virtual const char*          GetDescription(void) const
        {
            return m_data.buyable->GetDescription();
        }
        virtual Money                GetPrice(void) const
        {
            return m_price;
        }

        virtual const TechTreeBitMask&  GetRequiredTechs(void) const
        {
            return m_data.buyable->GetRequiredTechs();
        }

        virtual const TechTreeBitMask&  GetEffectTechs(void) const
        {
            return m_data.buyable->GetEffectTechs();
        }
    
        virtual DWORD                   GetTimeToBuild(void) const
        {
            return m_timeToBuild;
        }

        virtual BuyableGroupID          GetGroupID(void) const
        {
            return m_data.buyable->GetGroupID();
        }

        // IbucketIGC
        virtual IbuyableIGC*            GetBuyable(void) const
        {
            return m_data.buyable;
        }
        virtual IsideIGC*               GetSide(void) const
        {
            return m_data.side;
        }

        virtual void                    SetPrice(Money m)
        {
            assert (m_investedMoney == 0);
            assert (m > 0);
            m_price = m;
        }

        virtual int                     GetPercentBought(void) const
        {
            if (m_price > 0) { //#264 was assert(m_price > 0)
				return ((100 * m_investedMoney) / m_price);
			}
			return m_isBuildingForFree * 100;
        }
        virtual int                     GetPercentComplete(void) const
        {
            //m_investedMilliseconds
            return m_investedMilliseconds / (10 * m_timeToBuild);
        }

        virtual bool                    GetCompleteF(void) const
        {
            return m_investedMilliseconds >= (1000 * m_timeToBuild);
        }

		//#264 Turkey 07/12
		//make a few checks to find out whether this bucket should be getting researched.
		virtual bool					IsAvailableForFree() const
		{
			if (m_pMission->GetMissionStage() == STAGE_STARTED && m_data.side->CanBuy(m_data.buyable))
			{
				
				switch(m_data.buyable->GetObjectType()) 
				{
				case OT_stationType:
					{
						//check there isn't already a constructor for this
						IstationTypeIGC* pStationtype = ((IstationTypeIGC*)(m_data.buyable));
						for (ShipLinkIGC* shipLink = m_data.side->GetShips()->first(); 
							shipLink != NULL; shipLink = shipLink->next())
						{
							if (shipLink->data()->GetBaseData() == pStationtype)
								return false;
						}

						break;
					}
				case OT_droneType:
					{	
						//check we're not over the max drone limit
						//can't find a way to to it by DroneTypeID, do it by hull type. 
						//This will give extra drones if you upgrade the hull type of a free drone.

						HullID hullID = ((IdroneTypeIGC*)(m_data.buyable))->GetHullTypeID();
						int droneCount = 0;
						for (ShipLinkIGC* shipLink = m_data.side->GetShips()->first(); 
							shipLink != NULL; shipLink = shipLink->next())
						{
							if (hullID == shipLink->data()->GetHullType()->GetObjectID())
								droneCount++;
						}
						if (droneCount >= m_pMission->GetMissionParams()->nMaxDronesPerTeam) return false;

						break;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

        virtual void                    ForceComplete(Time now)
        {
            assert (m_data.side->CanBuy(m_data.buyable));

			m_isBuildingForFree = false; //#264

            switch(m_data.buyable->GetObjectType())
            {
                case OT_development:
                {
                    m_pMission->GetIgcSite()->DevelopmentCompleted(this, (IdevelopmentIGC*)(m_data.buyable), now);

                    m_investedMilliseconds = 1000 * m_timeToBuild;
                    m_investedMoney = m_price;
                }
                break;
                case OT_droneType:
                {
                    //Find a station where it can be built
                    IstationIGC*    pstation;
                    {
                        StationLinkIGC*    psl = m_data.side->GetStations()->first();
                        while (true)
                        {
                            assert (psl);
                            if (psl->data()->CanBuy(m_data.buyable))
                            {
                                pstation = psl->data();
                                break;
                            }

                            psl = psl->next();
                        }
                    }
                    m_pMission->GetIgcSite()->DroneTypeCompleted(this, pstation, (IdroneTypeIGC*)(m_data.buyable), now);

                    m_investedMoney = 0;
                    m_investedMilliseconds = 0;
                }
                break;
                case OT_stationType:
                {
                    //Find a station where it can be built
                    IstationIGC*    pstation;
                    {
                        StationLinkIGC*    psl = m_data.side->GetStations()->first();
                        while (true)
                        {
                            assert (psl);
                            if (psl->data()->CanBuy(m_data.buyable))
                            {
                                pstation = psl->data();
                                break;
                            }

                            psl = psl->next();
                        }
                    }
                    m_pMission->GetIgcSite()->StationTypeCompleted(this, pstation, (IstationTypeIGC*)(m_data.buyable), now);

                    m_investedMoney = 0;
                    m_investedMilliseconds = 0;
                }
                break;

                case OT_hullType:
                {
                    m_pMission->GetIgcSite()->HullTypeCompleted(m_data.side, (IhullTypeIGC*)(m_data.buyable));

                    m_investedMoney = 0;
                    m_investedMilliseconds = 0;
                }
                break;
                case OT_partType:
                {
                    m_pMission->GetIgcSite()->PartTypeCompleted(m_data.side, (IpartTypeIGC*)(m_data.buyable));

                    m_investedMoney = 0;
                    m_investedMilliseconds = 0;
                }
                break;
                default:
                    assert (false);
            }
        }

        virtual DWORD                   GetTime(void) const
        {
            return m_investedMilliseconds;
        }

        virtual Money                   GetMoney(void) const
        {
            return m_investedMoney;
        }

        virtual void                    SetTimeAndMoney(DWORD dwTime, Money money)
        {
            m_investedMilliseconds = dwTime;
            m_investedMoney = money;

            m_pMission->GetIgcSite()->BucketChangeEvent(c_bcMoneyChange, this);
        }
        virtual Money                    AddMoney(Money m)
        {
            {
                Money leftover = m_price - m_investedMoney - m;
                if (leftover < 0)
                    m += leftover;
            }

            m_investedMoney += m;
            m_pMission->GetIgcSite()->BucketChangeEvent(c_bcMoneyChange, this);

            return m;
        }
        virtual void                    SetEmpty(void)
        {
            m_investedMoney = 0;
            m_investedMilliseconds = 0;
			m_isBuildingForFree = false;//#264

            m_pMission->GetIgcSite()->BucketChangeEvent(c_bcEmptied, this);
        }

        virtual ObjectType              GetBucketType(void) const
        {
            return m_data.buyable->GetObjectType();
        }
        virtual IbucketIGC*             GetPredecessor(void) const
        {
            return m_pbucketPredecessor;
        }
    private:
        ImissionIGC*        m_pMission;
        DataBucketIGC       m_data;

        Time                m_lastUpdate;

        int                 m_lastPercentCompleteNotified;
        DWORD               m_investedMilliseconds;
        DWORD               m_timeToBuild;
        Money               m_investedMoney;
        Money               m_price;
		bool				m_isBuildingForFree; //#264
        IbucketIGC*         m_pbucketPredecessor;
};

#endif //__BUCKETIGC_H_
