
#ifndef _TECHITEM_H_
#define _TECHITEM_H_

class CTechItem;
class CCivTechItem;

class CTechItemList : public FList<CTechItem>
{
    public:
        BOOL        CopyTo(CTechItemList * pDest);
        CTechItem * Find(ObjectType ot, ObjectID id);
        CTechItem * Remove(ObjectType ot, ObjectID id);
        CTechItem * RemoveNth(DWORD n);
} ;

typedef struct _PURCHASE_DATA
{
    CTechItemList TechItemList;
    CTechItemList CanBuyList;
    CTechItemList BoughtList;
    TechTreeBitMask ttbmCurrent;
    DWORD dwLastCanBuyLength;
} PURCHASE_DATA, *PPURCHASE_DATA;


#define TECH_ITEM_NAME_LENGTH   64


class CTechItem
{
    protected:
        CHAR mszName[TECH_ITEM_NAME_LENGTH + 4];
        ObjectType mObjectType;
        ObjectID mObjectID;

        TechTreeBitMask mttbmRequired;
        TechTreeBitMask mttbmEffect;

        CTechItem * mpOriginal;
        BOOL mfEverBought;

    public:
        //
        // Hack to get the lists to work.
        //
        CTechItem * mpNext;

    public:
        CTechItem();
        CTechItem(CTechItem * pOriginal);

        //
        // Readers and writers.
        //
        CHAR * GetName() { return(mszName); }
        ObjectType GetObjectType() { return(mObjectType); }
        ObjectID GetObjectID() { return(mObjectID); }

        VOID SetName(CHAR * szName) { strcpy(mszName, szName); }
        VOID SetObjectType(ObjectType ot) { mObjectType = ot; }
        VOID SetObjectID(ObjectID id) { mObjectID = id; }

        VOID SetRequiredBitMask(TechTreeBitMask ttbm)
            { mttbmRequired = ttbm; }
        VOID SetEffectBitMask(TechTreeBitMask ttbm)
            { mttbmEffect = ttbm; }

        CTechItem * GetOriginal() { return(mpOriginal); }
        BOOL WasEverBought() { return(mfEverBought); }

        //
        // Our own unique methods.
        //
        CHAR *  GetObjectTypeName();
        VOID    SetWasBought(BOOL fBought);
        virtual CTechItem * Clone();
        virtual BOOL CanPurchase(PURCHASE_DATA * pPurchaseData);
        virtual VOID Purchase(PURCHASE_DATA * pPurchaseData);
} ;


class CCivTechItem : public CTechItem
{
    protected:
        StationID mStartingStationID;
        DroneTypeID mConstructionDroneID;
        ShipID mEjectPodShipID;

    public:
        CCivTechItem();

        //
        // Readers and writers.
        //
        VOID SetStartingStationID(StationID id) { mStartingStationID = id; }
        VOID SetConstructionDroneID(DroneTypeID id)
            { mConstructionDroneID = id; }
        VOID SetEjectPodShipID(ShipID id)
            { mEjectPodShipID = id; }

        //
        // Our own unique methods.
        //
        virtual VOID Purchase(PURCHASE_DATA * pPurchaseData);
} ;


class CDroneTechItem : public CTechItem
{
    protected:
        ObjectID mHullID;

    public:
        CDroneTechItem();
        CDroneTechItem(CDroneTechItem * pOriginal);

        //
        // Readers and writers.
        //
        ObjectID GetHullID() { return(mHullID); }
        VOID SetHullID(ObjectID id) { mHullID = id; }

        //
        // Our own unique methods.
        //
        virtual CTechItem * Clone();
        virtual VOID Purchase(PURCHASE_DATA * pPurchaseData);
} ;


#define MAX_UPGRADES_TO_STATION     10

class CStationTechItem : public CTechItem
{
    protected:
        TechTreeBitMask mttbmLocal;
        StationID mUpgradeStationID;
        //TechTreeBitMask mttbmUpgradeFrom;
        //TechTreeBitMask mttbmUpgrade[MAX_UPGRADES_TO_STATION];
        //DWORD mcUpgrades;

    public:
        CStationTechItem();
        CStationTechItem(CStationTechItem * pOriginal);

        //
        // Readers and writers.
        //
        VOID SetLocalBitMask(TechTreeBitMask ttbm)
            { mttbmLocal = ttbm; }
        //VOID SetUpgradeStationID(StationID stationID)
        //    { mUpgradeStationID = stationID; }
        //VOID SetUpgradeFromBitMask(TechTreeBitMask ttbm)
        //    { mttbmUpgradeFrom = ttbm; }
        //VOID AddUpgradeBitmask(TechTreeBitMask ttbm);

        //
        // Our own unique methods.
        //
        virtual CTechItem * Clone();
        virtual BOOL CanPurchase(PURCHASE_DATA * pPurchaseData);
        virtual VOID Purchase(PURCHASE_DATA * pPurchaseData);
} ;

#endif


