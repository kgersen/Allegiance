/////////////////////////////////////////////////////////////////////////////
// AGCHullType.cpp : Implementation of CAGCHullType
//

#include "pch.h"
#include "AGCHullType.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCHullType

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCHullType)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCHullType::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCHullType
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCHullType Interface Methods

STDMETHODIMP CAGCHullType::get_Length(float* pfLength)
{
  assert(GetIGC());
  CLEAROUT(pfLength, GetIGC()->GetLength());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxSpeed(float* pfMaxSpeed)
{
  assert(GetIGC());
  CLEAROUT(pfMaxSpeed, GetIGC()->GetMaxSpeed());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxTurnRate(AGCAxis eAxis, float* pfMaxTurnRate)
{
  assert(GetIGC());
  CLEAROUT(pfMaxTurnRate, GetIGC()->GetMaxTurnRate(eAxis));
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_TurnTorque(AGCAxis eAxis, float* pfTurnTorque)
{
  assert(GetIGC());
  CLEAROUT(pfTurnTorque, GetIGC()->GetTurnTorque(eAxis));
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_Thrust(float* pfThrust)
{
  assert(GetIGC());
  CLEAROUT(pfThrust, GetIGC()->GetThrust());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_SideMultiplier(float* pfSideMultiplier)
{
  assert(GetIGC());
  CLEAROUT(pfSideMultiplier, GetIGC()->GetSideMultiplier());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_BackMultiplier(float* pfBackMultiplier)
{
  assert(GetIGC());
  CLEAROUT(pfBackMultiplier, GetIGC()->GetBackMultiplier());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_ScannerRange(float* pfScannerRange)
{
  assert(GetIGC());
  CLEAROUT(pfScannerRange, GetIGC()->GetScannerRange());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxEnergy(float* pfMaxEnergy)
{
  assert(GetIGC());
  CLEAROUT(pfMaxEnergy, GetIGC()->GetMaxEnergy());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_RechargeRate(float* pfRechargeRate)
{
  assert(GetIGC());
  CLEAROUT(pfRechargeRate, GetIGC()->GetRechargeRate());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_HitPoints(AGCHitPoints* pHitPoints)
{
  assert(GetIGC());
  CLEAROUT(pHitPoints, (AGCHitPoints)GetIGC()->GetHitPoints());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_PartMask(AGCEquipmentType et, AGCMount mountID,
  AGCPartMask* pPartMask)
{
  assert(GetIGC());
  CLEAROUT(pPartMask, (AGCPartMask)GetIGC()->GetPartMask(et, mountID));
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxWeapons(AGCMount* pMaxWeapons)
{
  assert(GetIGC());
  CLEAROUT(pMaxWeapons, (AGCMount)GetIGC()->GetMaxWeapons());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxFixedWeapons(AGCMount* pMaxFixedWeapons)
{
  assert(GetIGC());
  CLEAROUT(pMaxFixedWeapons, (AGCMount)GetIGC()->GetMaxFixedWeapons());
  return S_OK;
}

//  STDMETHODIMP CAGCHullType::get_CanMount(IAGCPartType* pPartType, AGCMount mountID,
//    VARIANT_BOOL* pbCanMount)
// {
//    assert(GetIGC());
// }

STDMETHODIMP CAGCHullType::get_Mass(float* pfMass)
{
  assert(GetIGC());
  CLEAROUT(pfMass, GetIGC()->GetMass());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_Signature(float* pfSignature)
{
  assert(GetIGC());
  CLEAROUT(pfSignature, GetIGC()->GetSignature());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_Capabilities(AGCHullAbilityBitMask* phabmCapabilities)
{
  assert(GetIGC());
  CLEAROUT(phabmCapabilities, GetIGC()->GetCapabilities());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_HasCapability(AGCHullAbilityBitMask habm,
  VARIANT_BOOL* pbHasCapability)
{
  assert(GetIGC());
  CLEAROUT(pbHasCapability, VARBOOL(GetIGC()->HasCapability(habm)));
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxAmmo(short* pnMaxAmmo)
{
  assert(GetIGC());
  CLEAROUT(pnMaxAmmo, GetIGC()->GetMaxAmmo());
  return S_OK;
}

STDMETHODIMP CAGCHullType::get_MaxFuel(float* pfMaxFuel)
{
  assert(GetIGC());
  CLEAROUT(pfMaxFuel, GetIGC()->GetMaxFuel());
  return S_OK;
}


