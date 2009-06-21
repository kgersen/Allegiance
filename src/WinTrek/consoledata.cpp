#include "pch.h"
#include "consoledata.h"

/////////////////////////////////////////////////////////////////////////////
//
// ModelData
//
/////////////////////////////////////////////////////////////////////////////

ImodelIGC* ModelData::GetModel()
{
    return (m_pmodel && m_pmodel->GetMission())
           ? ((m_pmodel->GetObjectType() == OT_ship)
              ? ((IshipIGC*)(ImodelIGC*)m_pmodel)->GetSourceShip()
              : m_pmodel)
           : NULL;
}

ObjectType ModelData::GetModelTypeInternal()
{
    ImodelIGC* pmodel = GetModel();
    return pmodel ? pmodel->GetObjectType() : OT_invalid;
}

float ModelData::GetModelType()
{

    return (float)GetModelTypeInternal();
}

static ZString  s_empty("");
static ZString  s_unknown("unknown");

ZString ModelData::GetModelTypeDesc()
{
    ImodelIGC*      pmodel = GetModel();
    if (pmodel)
    {
        IclusterIGC*    pcluster = pmodel->GetCluster();
        if (pcluster && trekClient.GetShip()->CanSee(pmodel))
            return ::GetModelType(pmodel);
        else if (pmodel->GetObjectType() == OT_ship)
        {
            PlayerInfo* ppi = (PlayerInfo*)(((IshipIGC*)pmodel)->GetPrivateData());
            HullID      hid = ppi->LastSeenShipType();
            if (hid != NA)
                return trekClient.m_pCoreIGC->GetHullType(hid)->GetName();
        }
    }

    return s_unknown;
}

TRef<Image> ModelData::GetModelTypeIcon()
{
    return Image::GetEmpty();
}

ZString ModelData::GetName()
{
    return m_pmodel ? ZString(::GetModelName(m_pmodel)) : s_empty;
}

ZString ModelData::GetSectorName()
{
    ImodelIGC*      pmodel = GetModel();
    if (pmodel)
    {
        IclusterIGC*    pcluster = pmodel->GetCluster();
        if (pcluster && trekClient.GetShip()->CanSee(pmodel))
            return pcluster->GetName();
        else if (pmodel->GetObjectType() == OT_ship)
        {
            PlayerInfo* ppi = (PlayerInfo*)(((IshipIGC*)pmodel)->GetPrivateData());
            SectorID    sid = ppi->LastSeenSector();
            if (sid != NA)
                return trekClient.m_pCoreIGC->GetCluster(sid)->GetName();
        }
    }

    return s_unknown;
}
ZString ModelData::GetSideName()
{
    if (m_pmodel)
    {
        IsideIGC*   pside = m_pmodel->GetSide();
        return pside ? ZString(pside->GetName()) : s_empty;
    }
    return s_empty;
}

Color ModelData::GetSideColor()
{
    if (m_pmodel)
    {
        IsideIGC*   pside = m_pmodel->GetSide();
        if (pside)
            return pside->GetColor();
    }
    return Color(1,1,1);
}

TRef<Image> ModelData::GetSideIcon()
{
    return Image::GetEmpty();
}
//Andon: Flag for friendly targets.
//Returns a 1 if it is on your team
//Returns a 2 if it is an ally
//Returns a 3 if it is an enemy
float ModelData::IsFriendly()
{
	float f = 0.0f;
	if (m_pmodel)
	{
		IsideIGC*   pside = m_pmodel->GetSide();
		ImodelIGC*	pmodel = GetModel();
		if (m_pmodel && (pside ? ZString(pside->GetName()) : s_empty) == ZString(trekClient.GetSide()->GetName()))
		{
			return 1;
		}
		else if ((pside ? ZString(pside->GetAllies()) : s_empty) == ZString(trekClient.GetSide()->GetAllies()))
		{
			return 2;
		}
	}
	return 3;
}
float ModelData::GetSpeed()
{
    float   speed = 0.0f;

    ImodelIGC*      pmodel = GetModel();
    if (pmodel && ((pmodel->GetAttributes() & c_mtStatic) == 0))
    {
        IclusterIGC*    pcluster = pmodel->GetCluster();
        if (pcluster && trekClient.GetShip()->CanSee(pmodel))
            speed = pmodel->GetVelocity().Length();
        else
            speed = -1.0f;
    }

    return speed;
}

//Andon: Gets the ship's X position
//Only works on self.
float ModelData::GetX()
{
    float   f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip())
    {
		f = pmodel->GetPosition().X();
    }

    return f;
}

//Andon: Gets the ship's Y position
//Only works on self.
float ModelData::GetY()
{
    float   f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip())
    {
		f = pmodel->GetPosition().Y();
    }

    return f;
}

//Andon: Gets the ship's Z position
//Only works on self.
float ModelData::GetZ()
{
    float   f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip())
    {
		f = pmodel->GetPosition().Z();
    }

    return f;
}
//Andon: Grabs the maximum speed the target can sustain.
//Note that this does not include speed from boosters.
//Only functions on self
float ModelData::GetMaxSpeed()
{
    float   f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
    {
        f = (float) GetShip()->GetHullType()->GetMaxSpeed();
    }

    return f;
}

//Andon: Grabs the maximum thrust the Target can have
//Only functions on self
float ModelData::GetMaxThrust()
{
    float   f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
	IshipIGC*		pship = GetShip();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
    {
        f = (float) pship->GetHullType()->GetThrust();
    }

    return f;
}

//Andon: Gets the target's sidethruster modifier.
//Only works on self.
float ModelData::GetSideThrust()
{
	float f = 0.0f;
	ImodelIGC*	pmodel = GetModel();
	IshipIGC*	pship = GetShip();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = (float) pship->GetHullType()->GetSideMultiplier();
	}
	return f;
}
//Andon: Gets the target's rearthruster modifier
//Only works on self
float ModelData::GetRearThrust()
{
	float f = 0.0f;
	ImodelIGC*	pmodel = GetModel();
	IshipIGC*	pship = GetShip();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = (float) pship->GetHullType()->GetBackMultiplier();
	}
	return f;
}
//Andon: Gets the ECM modifier of the target
float ModelData::GetECM()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
            IshipIGC*		pship = GetShip();
			IclusterIGC*	pcluster = pship->GetCluster();
			if (pcluster && trekClient.GetShip()->CanSee(pship))
			{
				return (float)pship->GetHullType()->GetECM();
			}
			return 0;
        }
        break;
    default:
        return 1;
        break;
    }
}

//Andon: Gets the Scan Range of the target
//Only works on self
float ModelData::GetScanRange()
{
	float f=0.0f;
	
	ImodelIGC*	pmodel	=	GetModel();
	IshipIGC*	pship	=	GetShip();
	if(pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetHullType()->GetScannerRange();
	}

	return f;
}


//Andon: Gets whether the target has the 'Station at Risk' flag
//Returns 0 or 1. 1 means the target has the flag, 0 means it does not
//As with all ship flag checks, this only works on ships
float ModelData::IsStationRisk()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
            IshipIGC*     pship     = GetShip();
            const IhullTypeIGC* phullType = pship->GetHullType();
            
			return (float)phullType->HasCapability(c_habmThreatToStation);
        }
        break;
    default:
        return 0;
        break;
    }
}

//Andon: Gets whether the target has the 'Capture Station' flag
//Returns 0 or 1. 1 means the target has the flag, 0 means it does not
//As with all ship flag checks, this only works on ships
float ModelData::IsStationCapture()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
            IshipIGC*     pship     = GetShip();
            const IhullTypeIGC* phullType = pship->GetHullType();
            
			return (float)phullType->HasCapability(c_habmBoard);
        }
        break;
    default:
        return 0;
        break;
    }
}

//Andon: Gets whether the target has the 'Rescue Pods' flag
//Returns 0 or 1. 1 means the target has the flag, 0 means it does not
//Works on friendly stations, ships, and probes only.
float ModelData::IsRescue()
{
	ObjectType ot = GetModelTypeInternal();
	float f = 0.0f;

	if (m_pmodel)
	{
		IsideIGC*   pside = m_pmodel->GetSide();
		ImodelIGC*	pmodel = GetModel();
		if (m_pmodel && (pside ? ZString(pside->GetName()) : s_empty) == ZString(trekClient.GetSide()->GetName()))
		{
			if (ot == OT_ship)
			{
				IshipIGC*		pship = GetShip();
				IclusterIGC*	pcluster = pship->GetCluster();
				if (pcluster && trekClient.GetShip()->CanSee(pship))
				{
					f = (float)pship->GetHullType()->HasCapability(c_habmRescue);
				}
			}
			else if (ot == OT_station)
			{
				IstationIGC*	pstation = GetStation();
				f = (float)pstation->GetStationType()->HasCapability(c_sabmRescue);
			}
		}
		else if (ot == OT_probe)
		{
			IprobeIGC*		pprobe = GetProbe();
			IclusterIGC*	pcluster = pprobe->GetCluster();
			if (pcluster && trekClient.GetShip()->CanSee(pprobe))
			{
				f = (float)pprobe->GetProbeType()->HasCapability(c_eabmRescue);
			}
		}
	}
	return f;
}

//Andon: Gets the target type
//Returns the following:
//0 for Ship
//1 for Station
//2 for Missile
//3 for Mine
//4 for Probe
//5 for Asteroid
//7 for Aleph
//8 for Treasure
//Other numbers are not used for anything that can be targeted

float ModelData::GetType()
{
	float ot = GetModelTypeInternal();
	ImodelIGC*	pmodel = GetModel();
	float f = 0.0f;

	if (m_pmodel)
	{
		f = ot;
	}
	return f;
}


//Andon: Gets whether the target relays a lead indicator
//Returns 0 or 1. 1 means the target has the flag, 0 means it does not
//Only works on friendly ships and stations
float ModelData::IsLeadRelay()
{
	ObjectType ot = GetModelTypeInternal();
	float f = 0.0f;

	if (m_pmodel)
	{
		IsideIGC*   pside = m_pmodel->GetSide();
		ImodelIGC*	pmodel = GetModel();
		if (m_pmodel && (pside ? ZString(pside->GetName()) : s_empty) == ZString(trekClient.GetSide()->GetName()))
		{
			if (ot == OT_ship)
			{
				IshipIGC*		pship = GetShip();
				IclusterIGC*	pcluster = pship->GetCluster();
				if (pcluster && trekClient.GetShip()->CanSee(pship))
				{
					f = (float)pship->GetHullType()->HasCapability(c_habmRemoteLeadIndicator);
				}
			}
			else if (ot == OT_station)
			{
				IstationIGC*	pstation = GetStation();
				f = (float)pstation->GetStationType()->HasCapability(c_sabmRemoteLeadIndicator);
			}
		}
	}
	return f;
}
//Andon: Gets whether the target has the 'Land on Carrier' flag
//Returns 0 or 1. 1 means the target has the flag, 0 means it does not
//Only works on self.
float ModelData::CanLandCarrier()
{
    float f = 0.0f;

	IshipIGC*	pship = GetShip();
	ImodelIGC*	pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = (float)pship->GetHullType()->HasCapability(c_habmLandOnCarrier);
	}
	return f;
}

//Andon: Returns the mass
//Modified to only work on self
float ModelData::GetMass()
{
	float f = 0.0f;

    ImodelIGC* pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pmodel->GetMass();
	}
	return f;
}

float ModelData::GetRange()
{
    float   range = -1.0f;
    ImodelIGC*      pmodel = GetModel();
    if (pmodel)
    {
        IclusterIGC*    pcluster = pmodel->GetCluster();
        if (pcluster && trekClient.GetShip()->CanSee(pmodel) && (pcluster == trekClient.GetCluster()))
        {
            float   fSeparation = (pmodel->GetPosition() - trekClient.GetShip()->GetSourceShip()->GetPosition()).Length();
            // this is the same code used in radar image. There is something about the rounding mode that
            // keeps me from being able to just use floorf instead of the cast to int...
            //range = float (int (fSeparation + 0.5f)); //Andon: This makes it an int and give only whole numbers.
			range = fSeparation; //Andon: This allows it to give decimals
        }
    }

    return range;
}

float ModelData::GetPercentHitPoints()
{
    ImodelIGC*      pmodel = GetModel();

    float   f = 0.0f;
    if (pmodel && ((pmodel->GetAttributes() & c_mtDamagable) != 0))
    {
        IclusterIGC*    pcluster = pmodel->GetCluster();
        if (pcluster && trekClient.GetShip()->CanSee(pmodel) &&
            ((pmodel->GetCluster() == trekClient.GetCluster()) ||
             (pmodel == trekClient.GetShip()->GetStation())))
            f = ((IdamageIGC*)pmodel)->GetFraction();
        /*      //NYI special case for unknown
        else
            f = -1.0f;
            */
    }

    return f;
}

//Andon: Grabs the current HitPoints the target has
//Only works on self.
float ModelData::GetNumberHitPoints()
{
    ImodelIGC*      pmodel = GetModel();
	//IshipIGC*		pship = GetSourceShip();
    float   f = 0.0f;
	if ((pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip()) && ((pmodel->GetAttributes() & c_mtDamagable) != 0))
    {
            f = ((IdamageIGC*)pmodel)->GetHitPoints();
    }

    return f;
}

//Andon:; Grabs the Maximum Hitpoints the target can have
//Only works on Self
float ModelData::GetMaxHitPoints()
{
    ImodelIGC*      pmodel = GetModel();

    float   f = 0.0f;
    if ((pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip()) && ((pmodel->GetAttributes() & c_mtDamagable) != 0))
    {
            f = (1/((IdamageIGC*)pmodel)->GetFraction())*((IdamageIGC*)pmodel)->GetHitPoints();
    }

    return f;
}

//Andon: Grabs the Armor Class of the target and spits it out as a number.
//It returns a number between 1 and 20. Numbers go as follows:
//1: Asteroid 2: Light 3: Medium 4: Heavy 5: Extra Heavy
//6: Utility 7: Minor Base Hull 8: Major Base Hull
//9: Light/Medium Shield 10: Minor Base Shield
//11: Major Base Shield 12: Parts 13: Light Base Hull
//14: Light Base Shield 15: Large Shield
//16: AC15 17: AC16 18: AC17 19: AC18 20: AC20
float ModelData::GetArmorClass()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
			IshipIGC*     pship     = GetShip();
			IclusterIGC*    pcluster = pship->GetCluster();
            if (pcluster && trekClient.GetShip()->CanSee(pship))
			{
				return (float) 1 + (pship->GetHullType()->GetDefenseType());
			}
			return 0;
        }
        break;
	case OT_station:
		{
            IstationIGC*	pstation	= GetStation();
			IclusterIGC*    pcluster = pstation->GetCluster();
            if (pcluster == trekClient.GetCluster())
			{
				return (float) 1 + (pstation->GetBaseStationType()->GetArmorDefenseType());
			}
			return 0;
		}
		break;
	case OT_asteroid:
		{
			return (float) 1; //Andon: No need to look up the asteroid's AC. Asteroids are always the same AC.
		}					  //If asteroids are able to be given different ACs for things like debris, then this should be revisited
    default:
        return 0;
        break;
    }
}

float ModelData::GetPercentShields()
{
    ImodelIGC*      pmodel = GetModel();

    float   f = 0.0f;
    if (pmodel)
    {
        ObjectType  type = pmodel->GetObjectType();
        if (type == OT_ship)
        {
            IshipIGC*       pship = GetShip();
            IclusterIGC*    pcluster = pship->GetCluster();
            if (pcluster && trekClient.GetShip()->CanSee(pship))
            {
                IshieldIGC* pshield = (IshieldIGC*)(pship->GetMountedPart(ET_Shield, 0));
                if (pshield)
                    f = pshield->GetFraction();
            }
            /*      //NYI special case for unknown
            else
                f = -1.0f;
                */
        }
        else if (type == OT_station)
        {
            assert (pmodel->GetCluster());
            if (trekClient.GetShip()->CanSee(pmodel) &&
                ((pmodel->GetCluster() == trekClient.GetCluster()) ||
                 (pmodel == trekClient.GetShip()->GetStation())))
                f = GetStation()->GetShieldFraction();
            /*
            else
                f = -1.0f;
                */
        }
    }

    return f;
}

//Andon: Grabs the current Hitpoints of the target's shields
//Only works on self
float ModelData::GetNumberShields()
{
    ImodelIGC*      pmodel = GetModel();

    float   f = 0.0f;
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
    {
		IshipIGC*       pship = GetShip();
		IshieldIGC* pshield = (IshieldIGC*)(pship->GetMountedPart(ET_Shield, 0));
		if (pshield)
			f = (pshield->GetFraction()*pshield->GetMaxStrength());
    }

    return f;
}

//Andon: Grabs the maximum hit points the target's shields can have
//Only works on self
float ModelData::GetMaxShields()
{
    ImodelIGC*      pmodel = GetModel();

    float   f = 0.0f;
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
    {
		IshipIGC*       pship = GetShip();
		IshieldIGC* pshield = (IshieldIGC*)(pship->GetMountedPart(ET_Shield, 0));
		if (pshield)
			f = pshield->GetMaxStrength();
    }

    return f;
}

//Andon: Grabs the Armor Class of the target's shields and spits it out as a number.
//It returns a number between 1 and 20. Numbers go as follows:
//1: Asteroid 2: Light 3: Medium 4: Heavy 5: Extra Heavy
//6: Utility 7: Minor Base Hull 8: Major Base Hull
//9: Light/Medium Shield 10: Minor Base Shield
//11: Major Base Shield 12: Parts 13: Light Base Hull
//14: Light Base Shield 15: Large Shield
//16: AC15 17: AC16 18: AC17 19: AC18 20: AC20
float ModelData::GetShieldClass()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
            IshipIGC*       pship = GetShip();
            IclusterIGC*    pcluster = pship->GetCluster();
            if (pcluster && trekClient.GetShip()->CanSee(pship))
            {
                IshieldIGC* pshield = (IshieldIGC*)(pship->GetMountedPart(ET_Shield, 0));
                if (pshield)
                return 1 + (pshield->GetDefenseType());
            }
			return 0;
        }
        break;
	case OT_station:
		{
            IstationIGC*	pstation	= GetStation();
			IclusterIGC*    pcluster = pstation->GetCluster();
            if (pcluster == trekClient.GetCluster())
			{
				return (float) 1 + (pstation->GetBaseStationType()->GetShieldDefenseType());
			}
			return 0;
		}
		break;
	case OT_asteroid:
		{
			return (float) 0; //Andon: No need to look up the asteroid's shield AC. Asteroids don't have shields
		}					  //If asteroids are able to be given shields, then this should be revisited
    default:
        return 0;
        break;
    }
}

//Andon: Grabs the percent of energy.
//Modified to only work for self.
float ModelData::GetPercentEnergy()
{
	float f = 0.0f;
    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
        f = GetShip()->GetEnergy()/GetShip()->GetHullType()->GetMaxEnergy();
    }
	return f;
}

//Andon: Grabs the current energy the target has
//Only works on self.
float ModelData::GetNumberEnergy()
{
	float f = 0.0f;
    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
        f = GetShip()->GetEnergy();
    }
	return f;
}

//Andon: Grabs the maximum energy the target can have
//Only works on self.
float ModelData::GetMaxEnergy()
{
	float f = 0.0f;
    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
        f = GetShip()->GetHullType()->GetMaxEnergy();
    }
	return f;
}

//Andon: Gets the energy recharge of the target
//Only works on self
float ModelData::GetEnergyRegen()
{
	float f = 0.0f;
    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = GetShip()->GetHullType()->GetRechargeRate();
    }
	return f;
}
//Andon: Gets the percent of ammo.
//Modified to only work for self.
float ModelData::GetAmmo()
{
	float f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = (float)GetShip()->GetAmmo()/(float)GetShip()->GetHullType()->GetMaxAmmo();
	}
	return f;
}
//Andon: Grabs the amount of ammo the target currently has
//Only works on self.
float ModelData::GetNumberAmmo()
{
	float f = 0.0f;
    ImodelIGC*      pmodel = GetModel();
    if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
        f = (float)GetShip()->GetAmmo();
    }
	return f;
}

//Andon: Grabs the maximum amount of ammo the target has
//Only works on self
float ModelData::GetMaxAmmo()
{
	float f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = GetShip()->GetHullType()->GetMaxAmmo();
	}
	return f;
}
//Andon: Gets percent of fuel of the target
//Modified to only work on self.
float ModelData::GetFuel()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetFuel()/pship->GetHullType()->GetMaxFuel();
	}
	return f;
}

//Andon: Grabs the current amount of fuel the target has
//Only works on self.
float ModelData::GetNumberFuel()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetFuel();
	}
	return f;
}

//Andon: Grabs the maximum fuel the target can carry
//Only works on self.
float ModelData::GetMaxFuel()
{
	float f = 0.0f;
		IshipIGC*     pship     = GetShip();
		ImodelIGC*      pmodel = GetModel();
		if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
		{
			f = pship->GetHullType()->GetMaxFuel();
		}
	return f;
}

//Andon: Grabs the time that the target takes to Ripcord
//Only works on self.
float ModelData::GetRipcordTime()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetHullType()->GetRipcordSpeed();
	}
	return f;
}

//Andon: Gets whether the target can ripcord or not.
//Returns 0 or 1. 1 means the target cannot ripcord. 0 means it can.
//As with all ship flag checks, this only works on ships
float ModelData::CanRipcord()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
            IshipIGC*		pship = GetShip();
			IclusterIGC*	pcluster = pship->GetCluster();
			if (pcluster && trekClient.GetShip()->CanSee(pship))
			{
				return (float)pship->GetHullType()->HasCapability(c_habmNoRipcord);
			}
			return 1;
        }
        break;
    default:
        return 1;
        break;
    }
}

//Andon: Gets whether the target can Small ripcord or not.
//Returns 0 or 1. 0 means the target cannot Small ripcord. 1 means it can.
//As with all ship flag checks, this only works on ships
float ModelData::CanSmallRipcord()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
		{
			IshipIGC*		pship = GetShip();
			IclusterIGC*	pcluster = pship->GetCluster();
			if (pcluster && trekClient.GetShip()->CanSee(pship))
			{
				return (float)pship->GetHullType()->HasCapability(c_habmCanLtRipcord);
			}
			return 0;
		}
        break;
    default:
        return 0;
        break;
    }
}

//Andon: Gets whether the target is a ripcord or not.
//Returns 0 or 1. 0 means the target is not a ripcord. 1 means the target is.
float ModelData::IsRipcord()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
		{
			IshipIGC*		pship = GetShip();
			IclusterIGC*	pcluster = pship->GetCluster();
			if (pcluster && trekClient.GetShip()->CanSee(pship))
			{
				return (float)pship->GetHullType()->HasCapability(c_habmIsRipcordTarget);
			}
			return 0;
		}
        break;
	case OT_station:
		{
			IstationIGC*	pstation = GetStation();
			IclusterIGC*	pcluster = pstation->GetCluster();
			if (pcluster == trekClient.GetCluster())
			{
				return (float)pstation->GetBaseStationType()->HasCapability(c_sabmRipcord);
			}
			return 0;
		}
		break;
    default:
        return 0;
        break;
    }
}

//Andon: Gets whether the target is a Small ripcord or not.
//Returns 0 or 1. 0 means the target is not a Small ripcord. 1 means the target is.
//As with all ship flag checks, this only works on ships
float ModelData::IsSmallRipcord()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:
        {
			IshipIGC*		pship = GetShip();
			IclusterIGC*	pcluster = pship->GetCluster();
			if (pcluster && trekClient.GetShip()->CanSee(pship))
			{
				return (float)pship->GetHullType()->HasCapability(c_habmIsLtRipcordTarget);
			}
			return 0;
		}
        break;
    default:
        return 0;
        break;
    }
}

//Andon: Grabs the Energy the target takes to ripcord
//Only works on self
float ModelData::GetRipcordEnergy()
{
	float f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = GetShip()->GetHullType()->GetRipcordCost();
	}
	return f;
}

float ModelData::GetOre()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_asteroid:
        return GetAsteroid()->GetOre();
    
    default:
        return 0.f;
    }
}

//Andon: Flag for determining if Vector Lock is on or not.
//If it is 1, then the ship's VL is active. If it is 0, then it is not.
//Modified to only work on self.
float ModelData::GetVectorLock()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetVectorLock();
	}
	return f;
}

//Andon: Returns the % cloaking. Is not, however, 0.0 to 0.1 - each % is a whole number
//Modified to only work on self.
float ModelData::GetCloaking()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetCloaking();
	}
	return f;
}

float ModelData::GetNumObservers()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
    case OT_ship:        
        {
            int nObservers = 0;

            for (ShipLinkIGC*  psl = GetShip()->GetChildShips()->first(); (psl != NULL); psl = psl->next())
            {
                IshipIGC*   pship = psl->data();
                if (pship->GetTurretID() == NA)
                {
                    nObservers++;
                }
            }

            return (float)nObservers;
        }
    default:
        return 0.0f;
    }
}

//Andon: Returns the amount of ripcord time left.
//Modified so it only works for self and can pull a decimal if wanted.
//Also carried over the +1 for the countdown
float ModelData::GetRipcordTimeLeft()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if ((pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip()) && pship->fRipcordActive())
	{
		f = 1+trekClient.GetShip()->GetSourceShip()->GetRipcordTimeLeft();
	}
	return f;
}

//Andon: Returns the time left in a lifepod.
//Modified to only work with self.
float ModelData::GetEndurance()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip())
	{
		f = pship->GetEndurance();
	}
	return f;
}

//Andon: Returns the signature of a ship
//Modified to only be usable on self.
float ModelData::GetSignature()
{
	float f = 0.0f;

    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = (pmodel->GetSignature()*100.0f);
	}
	return f;
}

//Andon: Flag for if the ship is cloaked. Returns 1 if it is, 0 if it isn't.
//Modified to only be usable on self.
float ModelData::IsCloaked()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->GetCloaking() <1.0f ? 1.0f : 0.0f;
	}
	return f;
}

float ModelData::IsEjectPod()
{
    ObjectType ot = GetModelTypeInternal();
    switch (ot)
    {
        case OT_ship:
            if (GetShip()->GetBaseHullType()->HasCapability(c_habmLifepod) &&
                GetShip()->GetCluster())
                return 1.0f;
            else
                return 0.0f;

        default:
            return 0.0f;
    }
}

//Andon: Flag that shows if the target is ripcording or not. 1 is yes, 0 is no.
//Modified to only work for self.
float ModelData::IsRipcording()
{
	float f = 0.0f;

	IshipIGC*     pship     = GetShip();
    ImodelIGC*      pmodel = GetModel();
	if (pmodel == trekClient.GetShip() || pmodel == trekClient.GetShip()->GetParentShip())
	{
		f = pship->fRipcordActive() ? 1.0f : 0.0f;
	}
	return f;
}

bool ModelData::IsNotNull()
{
    return m_pmodel != NULL;
}
bool ModelData::IsVisible()
{
    ImodelIGC* pmodel = GetModel();
    return pmodel && trekClient.GetShip()->CanSee(pmodel);
}

/////////////////////////////////////////////////////////////////////////////
//
// PartWrapper
//
/////////////////////////////////////////////////////////////////////////////

ZString PartWrapper::GetPartName()
{
    return m_ppart ? m_ppart->GetPartType()->GetName() : "";
}

float PartWrapper::GetRange()
{
    if (!m_ppart)
        return 0;

    EquipmentType et = m_ppart->GetPartType()->GetEquipmentType();

    if (et == ET_Weapon)
    {
        IprojectileTypeIGC* ppt = ((IweaponIGC*)(IpartIGC*)m_ppart)->GetProjectileType();
        float               range = ppt->GetSpeed()*ppt->GetLifespan();

        const GlobalAttributeSet&   ga = trekClient.GetSide()->GetGlobalAttributeSet();
        range *= ga.GetAttribute((((IweaponIGC*)(IpartIGC*)m_ppart)->GetAmmoPerShot())
                                 ? c_gaSpeedAmmo
                                 : c_gaLifespanEnergy);

        return range;
    }
    else if (et == ET_Magazine)
    {
        ImissileTypeIGC* pmt = (ImissileTypeIGC*)((IlauncherTypeIGC*)m_ppart->GetPartType())->GetExpendableType();
        float range = pmt->GetLifespan()*(pmt->GetInitialSpeed()+0.5f*pmt->GetLifespan()*pmt->GetAcceleration());
        return range;
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetDamage()
{
    if (!m_ppart)
        return 0;

    EquipmentType et = m_ppart->GetPartType()->GetEquipmentType();

    const GlobalAttributeSet&   ga = trekClient.GetSide()->GetGlobalAttributeSet();

    if (et == ET_Weapon)
    {
        IprojectileTypeIGC* ppt = ((IweaponIGC*)(IpartIGC*)m_ppart)->GetProjectileType();
        return (ppt->GetPower() + ppt->GetBlastPower()) * ga.GetAttribute(c_gaDamageGuns);
    }
    else if (et == ET_Magazine)
    {
        ImissileTypeIGC* pmt = (ImissileTypeIGC*)((IlauncherTypeIGC*)m_ppart->GetPartType())->GetExpendableType();
        return (pmt->GetPower() + pmt->GetBlastPower()) * ga.GetAttribute(c_gaDamageMissiles);
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetRate()
{
	if (!m_ppart)
        return 0;

	EquipmentType et = m_ppart->GetPartType()->GetEquipmentType();

	if (et == ET_Weapon)
    {
        DataWeaponTypeIGC*  pdwt = (DataWeaponTypeIGC*)((IpartTypeIGC*)m_ppart->GetPartType())->GetData();
        return (1.0f /pdwt->dtimeBurst);
    }
    
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetCount()
{
    if (!m_ppart)
        return 0;
    else
    {
        ObjectType ot = m_ppart->GetObjectType();
        if (m_ppart->GetShip() && m_ppart->GetMountID() < 0)
        {
            float fCount = 0.0f;
            
            IpartTypeIGC *ppartType = m_ppart->GetPartType();
            
            // add up all of the instances of this in the cargo
            for (Mount mount = -1; mount >= -c_maxCargo; --mount)
            {
                IpartIGC* ppart = m_ppart->GetShip()->GetMountedPart(NA, mount);

                if (ppart && ppart->GetPartType() == ppartType)
                {
                    fCount += ppart->GetAmount();
                }
            }

            if (m_ppart->GetObjectType() == OT_pack)
            {
                IpackIGC*   p = (IpackIGC*)(IpartIGC*)m_ppart;
                const IhullTypeIGC *pht = m_ppart->GetShip()->GetHullType();

                if (pht && p->GetPackType() == c_packAmmo)
                {
                    fCount /= pht->GetMaxAmmo();
                }
                else
                {
                    fCount /= pht->GetMaxFuel();
                }
            }

            return fCount;
        }
        else
            return (float)(m_ppart->GetAmount());
    }
}

float PartWrapper::GetAfterburnerFuelConsumption()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Afterburner)
    {
        IafterburnerIGC* pa = (IafterburnerIGC*)(IpartIGC*)m_ppart;
        return pa->GetFuelConsumption() * pa->GetMaxThrust();
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetAfterburnerFuelLeft()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Afterburner)
    {
        IafterburnerIGC* pa = (IafterburnerIGC*)(IpartIGC*)m_ppart;
        IshipIGC* pship = trekClient.GetShip()->GetSourceShip();
        
        return pship->GetFuel() / (pa->GetFuelConsumption() * pa->GetMaxThrust());
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetAfterburnerTopSpeed()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Afterburner)
    {
        IafterburnerIGC* pa = (IafterburnerIGC*)(IpartIGC*)m_ppart;
        const IhullTypeIGC* pht = trekClient.GetShip()->GetSourceShip()->GetHullType();
        
        float maxAfterburnerThrust = pa->GetMaxThrust();
        float thrust = pht->GetThrust();

        return pht->GetMaxSpeed() * (1.0f + (maxAfterburnerThrust / thrust));
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetAfterburnerTimeLeft()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Afterburner)
	{
		IafterburnerIGC* pa = (IafterburnerIGC*)(IpartIGC*)m_ppart;
		const IhullTypeIGC* pht = trekClient.GetShip()->GetSourceShip()->GetHullType();	
				
		float fuel = trekClient.GetShip()->GetFuel();
		float fuelConsumption = pa->GetFuelConsumption();

		if (fuelConsumption <= 0){
			return -1.0f;
		}
		else
			return fuel / (fuelConsumption * pa->GetMaxThrust());
	}
	else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetMaxShieldStrength()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Shield)
    {
        IshieldIGC* pshield = (IshieldIGC*)(IpartIGC*)m_ppart; 
        return pshield->GetMaxStrength();
    }
    else
    {
        ZAssert(false);
        return 0;
    }

}

float PartWrapper::GetShieldStrength()
{
	float   f = 0.0f;
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Shield)
    {
        IshieldIGC* pshield = (IshieldIGC*)(IpartIGC*)m_ppart;           
		f = pshield->GetFraction();
		return f * pshield->GetMaxStrength();
	}
	else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetRegenRate()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Shield)
    {
        IshieldIGC* pshield = (IshieldIGC*)(IpartIGC*)m_ppart; 
        return pshield->GetRegeneration() / pshield->GetMaxStrength();
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::GetRechargeTime()
{
	float   f = 0.0f;
	float MaxHP = 0.0f;
	float HP = 0.0f;
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Shield)
    {
        IshieldIGC* pshield = (IshieldIGC*)(IpartIGC*)m_ppart; 
		MaxHP = pshield->GetMaxStrength();
		f = pshield->GetFraction();
		HP = f * pshield->GetMaxStrength();
		return (MaxHP - HP) / pshield->GetRegeneration();
	}
	else
    {
        ZAssert(false);
        return 0;
    }
}
		
float PartWrapper::GetCloakTimeLeft()
{	
	if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Cloak)
	{
		const IhullTypeIGC* pht = trekClient.GetShip()->GetSourceShip()->GetHullType();	
		IcloakIGC* pcloak = (IcloakIGC*)(IpartIGC*)m_ppart;
		
		float shipEnergy = trekClient.GetShip()->GetEnergy();
		float shipEnergyGen = pht->GetRechargeRate();
		float cloakConsumption = pcloak->GetEnergyConsumption();

		if (cloakConsumption <= shipEnergyGen){
			return -1.0f;
		}
		else
			return shipEnergy/(cloakConsumption - shipEnergyGen);
	}
	else
    {
        ZAssert(false);
        return 0;
    }
}
 

	

float PartWrapper::IsEnergyDamage()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Weapon)
    {
        //IprojectileTypeIGC* ppt = ((IweaponIGC*)(IpartIGC*)m_ppart)->GetProjectileType();
        return 0.0f;
    }
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Magazine)
    {
        //ImissileTypeIGC* pmt = (ImissileTypeIGC*)((IlauncherTypeIGC*)m_ppart->GetPartType())->GetExpendableType();
        return 0.0f;
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::IsShipKiller()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Weapon)
    {
        //IprojectileTypeIGC* ppt = ((IweaponIGC*)(IpartIGC*)m_ppart)->GetProjectileType();
        return 0.0f;
    }
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Magazine)
    {
        //ImissileTypeIGC* pmt = (ImissileTypeIGC*)((IlauncherTypeIGC*)m_ppart->GetPartType())->GetExpendableType();
        return 0.0f;
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::IsStationKiller()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Weapon)
    {
        //IprojectileTypeIGC* ppt = ((IweaponIGC*)(IpartIGC*)m_ppart)->GetProjectileType();
        return 0.0f;
    }
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Magazine)
    {
        //ImissileTypeIGC* pmt = (ImissileTypeIGC*)((IlauncherTypeIGC*)m_ppart->GetPartType())->GetExpendableType();
        return 0.0f;
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::IsAsteroidKiller()
{
    if (!m_ppart)
        return 0;
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Weapon)
    {
        //IprojectileTypeIGC* ppt = ((IweaponIGC*)(IpartIGC*)m_ppart)->GetProjectileType();
        return 0.0f;
    }
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Magazine)
    {
        //ImissileTypeIGC* pmt = (ImissileTypeIGC*)((IlauncherTypeIGC*)m_ppart->GetPartType())->GetExpendableType();
        return 0.0f;
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::IsSelected()
{
    if (!m_ppart)
    {
        return 0;
    }
    else if (m_ppart->GetPartType()->GetEquipmentType() == ET_Weapon)
    {
        IweaponIGC* pweapon = (IweaponIGC*)(IpartIGC*)m_ppart;
        bool fSelected;

        if (pweapon->GetMountID() < 0)
        {
            fSelected = false;
        }
        else if (trekClient.GetShip() != m_ppart->GetShip())
        {
            fSelected = pweapon->GetGunner() == trekClient.GetShip();
        }
        else
        {
            int nMaxFixedWeapons = m_ppart->GetShip()->GetHullType()->GetMaxFixedWeapons();

            if (m_ppart->GetMountID() < nMaxFixedWeapons)
            {
                int stateM = trekClient.GetShip()->GetStateM();
                Mount mountSelected = (stateM & selectedWeaponMaskIGC) >> selectedWeaponShiftIGC;

                fSelected = trekClient.fGroupFire
                    || (m_ppart->GetMountID() == mountSelected);
            }
        }

        return fSelected ? 1.0f : 0.0f;
    }
    else
    {
        ZAssert(false);
        return 0;
    }
}

float PartWrapper::IsActive()
{
    if (!m_ppart)
        return 0;
    else 
    {        
        return m_ppart->fActive() ? 1.0f : 0.0f;
    }
}

float PartWrapper::IsOutOfAmmo()
{
    if (!m_ppart)
        return 0.0f;
    
    ObjectType type = m_ppart->GetObjectType();

    if (type == OT_weapon)
    {
        return (m_ppart->GetShip()->GetAmmo() == 0) ? 1.0f : 0.0f;
    }
    else if (type == OT_afterburner)
    {
        return (m_ppart->GetShip()->GetFuel() == 0) ? 1.0f : 0.0f;
    }
    else if (IlauncherIGC::IsLauncher(type))
    {        
        return (m_ppart->GetAmount() == 0) ? 1.0f : 0.0f;
    }
    else 
    {        
        return 0.0f;
    }
}

float PartWrapper::GetReadyState()
{
    if (!m_ppart || m_ppart->GetMountID() < 0)
        return 0;
    else if (m_ppart->GetMountedFraction() < 1.0f)
    {        
        return 1;
    }
    else if (IlauncherIGC::IsLauncher(m_ppart->GetObjectType())
        && ((IlauncherIGC*)(IpartIGC*)m_ppart)->GetArmedFraction() < 1.0f)
    {
        return 2;
    }
    else 
    {
        // ready to rock and roll
        return 3;
    }
}

float PartWrapper::GetMountedFraction()
{
    if (!m_ppart || m_ppart->GetMountID() < 0)
    {
        return 0;
    }
    else
    {
        return m_ppart->GetMountedFraction();
    }
}

float PartWrapper::GetArmedFraction()
{
    if (!m_ppart || m_ppart->GetMountID() < 0 
        || !IlauncherIGC::IsLauncher(m_ppart->GetObjectType()))
    {
        return 0;
    }
    else
    {
        return ((IlauncherIGC*)(IpartIGC*)m_ppart)->GetArmedFraction();
    }
}
