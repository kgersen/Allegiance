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
        
        float maxAfterburnerThrust = pa->GetMaxThrustWithGA(); //TheRock 15-8-2009
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
