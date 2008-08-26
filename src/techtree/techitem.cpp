

#include "pch.h"

typedef struct _TYPE_TO_STRING_DATA
{
    ObjectType ot;
    ObjectType reserved;
    CHAR * szTypeName;
} TYPE_TO_STRING_DATA;

TYPE_TO_STRING_DATA mTypeToStringData[] =
{
    { OT_civilization,      OT_invalid, "Civ" },
    { OT_station,           OT_invalid, "Station" },
    { OT_hullType,          OT_invalid, "Hull" },
    { OT_partType,          OT_invalid, "Part" },
    { OT_mineType,          OT_invalid, "Mine" },
    { OT_missileType,       OT_invalid, "Missile" },
    { OT_probeType,         OT_invalid, "Probe" },
    { OT_development,       OT_invalid, "Development" },
    { OT_droneType,         OT_invalid, "Drone" },
//    { OT_stationUpgrade,    OT_invalid, "Upgrade" },
    { OT_invalid,           OT_invalid, "Unknown" }
} ;


BOOL CTechItemList::CopyTo(CTechItemList * pDest)
{
    CTechItem * pItem, * pNewItem;
    BOOL fSuccess;

    fSuccess = TRUE;
    pDest->Empty();

    for(pItem = mpHead; NULL != pItem; pItem = pItem->mpNext)
    {
        pNewItem = pItem->Clone();
        if (NULL != pNewItem)
            pDest->Enqueue(pNewItem);
        else
            fSuccess = FALSE;
    }

    return(fSuccess);
}


CTechItem * CTechItemList::Find(ObjectType ot, ObjectID id)
{
    CTechItem * pItem;

    for(pItem = mpHead; NULL != pItem; pItem = pItem->mpNext)
    {
        if ((ot == pItem->GetObjectType()) && (id == pItem->GetObjectID()))
            break;
    }

    return(pItem);
}


CTechItem * CTechItemList::Remove(ObjectType ot, ObjectID id)
{
    CTechItem * pItem, * pPrev;

    for(pItem = mpHead, pPrev = NULL; NULL != pItem; pItem = pItem->mpNext)
    {
        if ((ot == pItem->GetObjectType()) && (id == pItem->GetObjectID()))
        {
            if (NULL == pPrev)
                mpHead = pItem->mpNext;
            else
                pPrev->mpNext = pItem->mpNext;

            if (pItem == mpTail)
                mpTail = pPrev;

            mcElements--;
            pItem->mpNext = NULL;
            break;
        }

        pPrev = pItem;
    }

    return(pItem);
}


CTechItem * CTechItemList::RemoveNth(DWORD n)
{
    CTechItem * pItem, * pPrev;

    pItem = NULL;
    if (n < mcElements)
    {
        if (0 == n)
        {
            pItem = mpHead;
            mpHead = pItem->mpNext;
            if (NULL == mpHead)
                mpTail = NULL;
        } else
        {
            pPrev = NULL;
            for(pItem = mpHead; n > 0; pItem = pItem->mpNext)
            {
                pPrev = pItem;
                n--;
            }

            pPrev->mpNext = pItem->mpNext;
            if (pItem == mpTail)
                mpTail = pPrev;
        }

        mcElements--;
        pItem->mpNext = NULL;
    }

    return(pItem);
}


CTechItem::CTechItem()
{
    ZeroMemory(mszName, sizeof(mszName));
    mObjectType = OT_invalid;
    mObjectID = NA;

    mttbmRequired.ClearAll();
    mttbmEffect.ClearAll();

    mpOriginal = NULL;
    mfEverBought = FALSE;

    mpNext = NULL;
}


CTechItem::CTechItem(CTechItem * pOriginal)
{
    CopyMemory(mszName, pOriginal->mszName, sizeof(mszName));
    mObjectType   = pOriginal->mObjectType;
    mObjectID     = pOriginal->mObjectID;

    mttbmRequired = pOriginal->mttbmRequired;
    mttbmEffect   = pOriginal->mttbmEffect;

    mpOriginal    = pOriginal;
    mfEverBought  = FALSE;

    mpNext        = NULL;
}


CHAR * CTechItem::GetObjectTypeName()
{
    DWORD dwIndex;

    for(dwIndex = 0; OT_invalid != mTypeToStringData[dwIndex].ot; dwIndex++)
    {
        if (this->GetObjectType() == mTypeToStringData[dwIndex].ot)
            break;
    }

    return(mTypeToStringData[dwIndex].szTypeName);
}


VOID CTechItem::SetWasBought(BOOL fBought)
{
    mfEverBought = fBought;
    if ((TRUE == fBought) && (NULL != mpOriginal))
        mpOriginal->SetWasBought(TRUE);
}


CTechItem * CTechItem::Clone()
{
    CTechItem * pClone;

    pClone = new CTechItem(this);
    return(pClone);
}


BOOL CTechItem::CanPurchase(PURCHASE_DATA * pPurchaseData)
{
    BOOL fCanPurchase;

    fCanPurchase = FALSE;
    if (mttbmRequired <= pPurchaseData->ttbmCurrent)
        fCanPurchase = TRUE;

    return(fCanPurchase);
}


VOID CTechItem::Purchase(PURCHASE_DATA * pPurchaseData)
{
    CTechItem * pItem;
    DWORD dwIndex;

    pPurchaseData->ttbmCurrent |= mttbmEffect;
    this->SetWasBought(TRUE);

    pPurchaseData->BoughtList.Enqueue(this);

    //
    // Go through the item list and see if there's anything new we
    // can now buy as a result of this purchase.
    //
    for(dwIndex = pPurchaseData->TechItemList.Length(); dwIndex > 0; dwIndex--)
    {
        pItem = pPurchaseData->TechItemList.Dequeue();
        if (TRUE == pItem->CanPurchase(pPurchaseData))
            pPurchaseData->CanBuyList.Enqueue(pItem);
        else
            pPurchaseData->TechItemList.Enqueue(pItem);
    }
}


//
// Civs.
//
CCivTechItem::CCivTechItem() : CTechItem()
{
    mStartingStationID = NA;
    mConstructionDroneID = NA;
    mEjectPodShipID = NA;
}


VOID CCivTechItem::Purchase(PURCHASE_DATA * pPurchaseData)
{
    CDroneTechItem * pDrone;
    CTechItem * pFound;

    pPurchaseData->ttbmCurrent |= mttbmEffect;

    //
    // Find the starting station in the global list and purchase it.
    //
    pFound = pPurchaseData->TechItemList.Remove(OT_station, mStartingStationID);
    if (NULL != pFound)
        pFound->Purchase(pPurchaseData);

    //
    // Make sure we mark our builder as "bought" since it's a drone
    // only hull and we can't buy builders.
    //
    pDrone = (CDroneTechItem *)
        pPurchaseData->TechItemList.Remove(OT_droneType, mConstructionDroneID);
    if (NULL != pDrone)
        pDrone->Purchase(pPurchaseData);

    //
    // Buy our escape pod.
    //
    pFound = pPurchaseData->TechItemList.Remove(OT_hullType, mEjectPodShipID);
    if (NULL != pFound)
        pFound->Purchase(pPurchaseData);
}



CDroneTechItem::CDroneTechItem() : CTechItem()
{
    mHullID = NA;
}


CDroneTechItem::CDroneTechItem(CDroneTechItem * pOriginal) :
    CTechItem(pOriginal)
{
    mHullID = pOriginal->mHullID;
}


CTechItem * CDroneTechItem::Clone()
{
    CDroneTechItem * pClone;

    pClone = new CDroneTechItem(this);
    return(pClone);
}


VOID CDroneTechItem::Purchase(PURCHASE_DATA * pPurchaseData)
{
    CTechItem * pFound;

    CTechItem::Purchase(pPurchaseData);

    //
    // Try to mark the hull.
    //
    pFound = pPurchaseData->TechItemList.Find(OT_hullType, this->GetHullID());
    if (NULL != pFound)
        pFound->SetWasBought(TRUE);
}


//
// Station Types.
//
CStationTechItem::CStationTechItem() : CTechItem()
{
    mttbmLocal.ClearAll();
    mUpgradeStationID = NA;
//    mttbmUpgradeFrom.ClearAll();
//    mcUpgrades = 0;
}


CStationTechItem::CStationTechItem(CStationTechItem * pOriginal) :
    CTechItem(pOriginal)
{
    mttbmLocal = pOriginal->mttbmLocal;
    mUpgradeStationID = pOriginal->mUpgradeStationID;
//    mttbmUpgradeFrom = pOriginal->mttbmUpgradeFrom;
//    mcUpgrades = 0;
}


//VOID CStationTechItem::AddUpgradeBitmask(TechTreeBitMask ttbm)
//{
//    if (mcUpgrades < MAX_UPGRADES_TO_STATION)
//        mttbmUpgrade[mcUpgrades++] = ttbm;
//}


CTechItem * CStationTechItem::Clone()
{
    CStationTechItem * pClone;

    pClone = new CStationTechItem(this);
    return(pClone);
}


BOOL CStationTechItem::CanPurchase(PURCHASE_DATA * pPurchaseData)
{
    BOOL fCanPurchase;
//    DWORD dwIndex;

    fCanPurchase = CTechItem::CanPurchase(pPurchaseData);
    if (FALSE == fCanPurchase)
    {
        //
        // Check to see if we can be upgraded to.
        //
        //for(dwIndex = 0; dwIndex < mcUpgrades; dwIndex++)
        //{
        //    if (mttbmUpgrade[dwIndex] <= pPurchaseData->ttbmCurrent)
        //    {
        //        this->SetObjectType(OT_stationUpgrade);
        //        fCanPurchase = TRUE;
        //    }
        //}

        // need to just check the bits
    }

    return(fCanPurchase);
}


VOID CStationTechItem::Purchase(PURCHASE_DATA * pPurchaseData)
{
//    CStationTechItem * pUpgrade;

    pPurchaseData->ttbmCurrent |= mttbmLocal;

    //
    // Now that we've been bought, find our upgrade and tell it
    // what it needs to be upgraded from us.
    //
//    pUpgrade = (CStationTechItem *)
//            pPurchaseData->TechItemList.Find(OT_station, mUpgradeStationID);
//    if (NULL != pUpgrade)
//        pUpgrade->AddUpgradeBitmask(mttbmUpgradeFrom);

    CTechItem::Purchase(pPurchaseData);
}





