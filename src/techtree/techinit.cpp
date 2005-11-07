
#include "pch.h"

#define TECH_ITEM_BITMASK_LENGTH    128


void htoc(BYTE *c, BYTE b[])
{
  *c = b[0] >= '0' && b[0] <= '9' ? b[0] - '0' : 10 + b[0] - 'A';
  *c <<= 4;
  *c |= b[1] >= '0' && b[1] <= '9' ? b[1] - '0' : 10 + b[1] - 'A';
}


TechTreeBitMask GetBitMask(CHAR * szBitMask)
{
    BYTE bTemp[TECH_ITEM_BITMASK_LENGTH * 2 + 4];
    TechTreeBitMask ttbm;
    DWORD dwIndex;

    dwIndex = 0;
    while('\0' != *szBitMask)
    {
        htoc(&bTemp[dwIndex], (BYTE *) szBitMask);
        dwIndex++;
        szBitMask += 2;
    }

    ttbm.Set(bTemp);
    return(ttbm);
}


BOOL InitializeCivs(CSQLWrap * pSQLWrap, CTechItemList * pCivList)
{
    CHAR szBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szName[TECH_ITEM_NAME_LENGTH + 4];
    SHORT shID;
    SHORT shStationID;
    SHORT shConstructionDroneID;
    SHORT shEjectPodID;
    LONG lLen[10];
    SQL_PARAMETER_LIST Parameter[] =
    {
        SQL_POUT_INT2(shID, lLen[0]),
        SQL_POUT_CHAR(szName, TECH_ITEM_NAME_LENGTH + 1, lLen[1]),
        SQL_POUT_CHAR(szBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[2]),
        SQL_POUT_INT2(shStationID, lLen[3]),
        SQL_POUT_INT2(shConstructionDroneID, lLen[4]),
        SQL_POUT_INT2(shEjectPodID, lLen[5]),
        SQL_PNULL
    } ;
    SQLRETURN SQLResult;
    SQLHSTMT hStatement;
    CCivTechItem * pCiv;
    BOOL fResult;

    fResult = TRUE;

    SQLResult = pSQLWrap->Execute("select CivID, RTRIM(Name), "
                                    "RTRIM(TechBitsCiv), "
                                    "InitialStationTypeID, "
                                    "ConstructionDroneTypeID, "
                                    "EscapePodShipTypeID "
                                    "from civs",
                                    Parameter,
                                    &hStatement);
    if (SQL_SUCCEEDED(SQLResult))
    {
        SQLResult = pSQLWrap->FetchRow(hStatement);
        while(SQL_SUCCEEDED(SQLResult))
        {
            pSQLWrap->TerminateString(szName, lLen[1]);
            pSQLWrap->TerminateString(szBitMask, lLen[2]);
            
            pCiv = new CCivTechItem;
            if (NULL != pCiv)
            {
                pCiv->SetObjectID(shID);
                pCiv->SetObjectType(OT_civilization);
                pCiv->SetName(szName);
                pCiv->SetEffectBitMask(GetBitMask(szBitMask));
                pCiv->SetStartingStationID(shStationID);
                pCiv->SetConstructionDroneID(shConstructionDroneID);
                pCiv->SetEjectPodShipID(shEjectPodID);
                pCivList->Enqueue(pCiv);
            } else
                fResult = FALSE;

            SQLResult = pSQLWrap->FetchRow(hStatement);
        }

        pSQLWrap->EndStatement(hStatement);
    }

    return(fResult);
}


BOOL InitializeStations(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    CHAR szRequiredBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    //CHAR szUpgradeBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szEffectBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szLocalBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szName[TECH_ITEM_NAME_LENGTH + 4];
    SHORT shID;
    SHORT shUpgradeID;
    LONG lLen[10];
    SQL_PARAMETER_LIST Parameter[] =
    {
        SQL_POUT_INT2(shID, lLen[0]),
        SQL_POUT_CHAR(szName, TECH_ITEM_NAME_LENGTH + 1, lLen[1]),
        SQL_POUT_CHAR(szRequiredBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[2]),
        SQL_POUT_CHAR(szEffectBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[3]),
        SQL_POUT_CHAR(szLocalBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[4]),
        SQL_POUT_INT2(shUpgradeID, lLen[5]),
        //SQL_POUT_CHAR(szUpgradeBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[6]),
        SQL_PNULL
    } ;
    CStationTechItem * pItem;
    SQLRETURN SQLResult;
    SQLHSTMT hStatement;
    BOOL fResult;

    fResult = TRUE;

    SQLResult = pSQLWrap->Execute("select stationtypeid, "
                                        "RTRIM(Name), "
                                        "RTRIM(TechBitsRequired), "
                                        "RTRIM(TechBitsEffect), "
                                        "RTRIM(TechBitsLocal), "
                                        "UpgradeStationTypeID, "
                                        //"UpgradeTechBitsReq "
                                        "from stationtypes "
                                        "order by stationtypeid",
                                    Parameter,
                                    &hStatement);
    if (SQL_SUCCEEDED(SQLResult))
    {
        SQLResult = pSQLWrap->FetchRow(hStatement);
        while(SQL_SUCCEEDED(SQLResult))
        {
            pSQLWrap->TerminateString(szName, lLen[1]);
            pSQLWrap->TerminateString(szRequiredBitMask, lLen[2]);
            pSQLWrap->TerminateString(szEffectBitMask, lLen[3]);
            pSQLWrap->TerminateString(szLocalBitMask, lLen[4]);
            //pSQLWrap->TerminateString(szUpgradeBitMask, lLen[6]);

            pItem = new CStationTechItem;
            if (NULL != pItem)
            {
                pItem->SetName(szName); 
                pItem->SetObjectType(OT_station);
                pItem->SetObjectID(shID);
                pItem->SetRequiredBitMask(GetBitMask(szRequiredBitMask));
                pItem->SetEffectBitMask(GetBitMask(szEffectBitMask));
                pItem->SetLocalBitMask(GetBitMask(szLocalBitMask));
                //pItem->SetUpgradeStationID(shUpgradeID);
                //pItem->SetUpgradeFromBitMask(GetBitMask(szUpgradeBitMask));

                pItemList->Enqueue(pItem);
            } else
                fResult = FALSE;

            SQLResult = pSQLWrap->FetchRow(hStatement);
        }
        pSQLWrap->EndStatement(hStatement);
    }

    return(fResult);
}


BOOL InitializeGeneric(CSQLWrap * pSQLWrap, CTechItemList * pItemList,
                        CHAR * szQuery, ObjectType ot)
{
    CHAR szRequiredBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szEffectBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szName[TECH_ITEM_NAME_LENGTH + 4];
    SHORT shID;
    LONG lLen[10];
    SQL_PARAMETER_LIST Parameter[] =
    {
        SQL_POUT_INT2(shID, lLen[0]),
        SQL_POUT_CHAR(szName, TECH_ITEM_NAME_LENGTH + 1, lLen[1]),
        SQL_POUT_CHAR(szRequiredBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[2]),
        SQL_POUT_CHAR(szEffectBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[3]),
        SQL_PNULL
    } ;
    SQLRETURN SQLResult;
    SQLHSTMT hStatement;
    CTechItem * pItem;
    BOOL fResult;

    fResult = TRUE;

    SQLResult = pSQLWrap->Execute(szQuery, Parameter, &hStatement);
    if (SQL_SUCCEEDED(SQLResult))
    {
        SQLResult = pSQLWrap->FetchRow(hStatement);
        while(SQL_SUCCEEDED(SQLResult))
        {
            pSQLWrap->TerminateString(szName, lLen[1]);
            pSQLWrap->TerminateString(szRequiredBitMask, lLen[2]);
            pSQLWrap->TerminateString(szEffectBitMask, lLen[3]);
            pItem = new CTechItem;

            if (NULL != pItem)
            {
                pItem->SetName(szName);
                pItem->SetObjectType(ot);
                pItem->SetObjectID(shID);

                pItem->SetRequiredBitMask(GetBitMask(szRequiredBitMask));
                pItem->SetEffectBitMask(GetBitMask(szEffectBitMask));

                pItemList->Enqueue(pItem);
            } else
                fResult = FALSE;

            SQLResult = pSQLWrap->FetchRow(hStatement);
        }
        pSQLWrap->EndStatement(hStatement);
    }

    return(fResult);
}


BOOL InitializeHulls(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    return(InitializeGeneric(pSQLWrap,
                            pItemList,
                            "select ShipTypeID, RTRIM(Name), "
                                "RTRIM(TechBitsRequired), "
                                "RTRIM(TechBitsEffect) from shiptypes",
                            OT_hullType));
}


BOOL InitializeParts(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    //
    // Magazines are listed in this table as PartTypeID 2.  The data
    // however, is bogus (it's stored in the expendables table).
    // So we ignore them.
    //
    return(InitializeGeneric(pSQLWrap,
                            pItemList,
                            "select PartID, RTRIM(Name), "
                                "RTRIM(TechBitsRequired), "
                                "RTRIM(TechBitsEffect) from Parts where "
                                "(PartTypeID <> 2)",
                            OT_partType));
}


BOOL InitializeMines(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    return(InitializeGeneric(pSQLWrap,
                            pItemList,
                            "select E.ExpendableID, RTRIM(Name), "
                                "RTRIM(TechBitsRequired), "
                                "RTRIM(TechBitsEffect) from Expendables E, "
                                "Mines M where "
                                "(E.ExpendableID = M.ExpendableID)",
                            OT_mineType));
}


BOOL InitializeMissiles(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    return(InitializeGeneric(pSQLWrap,
                            pItemList,
                            "select E.ExpendableID, RTRIM(Name), "
                                "RTRIM(TechBitsRequired), "
                                "RTRIM(TechBitsEffect) from Expendables E, "
                                "Missiles M where "
                                "(E.ExpendableID = M.ExpendableID)",
                            OT_missileType));
}


BOOL InitializeProbes(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    return(InitializeGeneric(pSQLWrap,
                            pItemList,
                            "select E.ExpendableID, RTRIM(Name), "
                                "RTRIM(TechBitsRequired), "
                                "RTRIM(TechBitsEffect) from Expendables E, "
                                "Probes P where "
                                "(E.ExpendableID = P.ExpendableID)",
                            OT_probeType));
}


BOOL InitializeDevelopments(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    return(InitializeGeneric(pSQLWrap,
                            pItemList,
                            "select DevelopmentID, RTRIM(Name), "
                                "RTRIM(TechBitsRequired), "
                                "RTRIM(TechBitsEffect) from Developments",
                            OT_development));
}


BOOL InitializeDrones(CSQLWrap * pSQLWrap, CTechItemList * pItemList)
{
    CHAR szRequiredBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szEffectBitMask[TECH_ITEM_BITMASK_LENGTH + 4];
    CHAR szName[TECH_ITEM_NAME_LENGTH + 4];
    SHORT shID;
    SHORT shHullID;
    LONG lLen[10];
    SQL_PARAMETER_LIST Parameter[] =
    {
        SQL_POUT_INT2(shID, lLen[0]),
        SQL_POUT_CHAR(szName, TECH_ITEM_NAME_LENGTH + 1, lLen[1]),
        SQL_POUT_CHAR(szRequiredBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[2]),
        SQL_POUT_CHAR(szEffectBitMask, TECH_ITEM_BITMASK_LENGTH + 1, lLen[3]),
        SQL_POUT_INT2(shHullID, lLen[4]),
        SQL_PNULL
    } ;
    CDroneTechItem * pItem;
    SQLRETURN SQLResult;
    SQLHSTMT hStatement;
    BOOL fResult;

    fResult = TRUE;

    SQLResult = pSQLWrap->Execute("select DroneID, RTRIM(Name), "
                                        "RTRIM(TechBitsRequired), "
                                        "RTRIM(TechBitsEffect), "
                                        "ShipTypeID from Drones",
                                    Parameter,
                                    &hStatement);
    if (SQL_SUCCEEDED(SQLResult))
    {
        SQLResult = pSQLWrap->FetchRow(hStatement);
        while(SQL_SUCCEEDED(SQLResult))
        {
            pSQLWrap->TerminateString(szName, lLen[1]);
            pSQLWrap->TerminateString(szRequiredBitMask, lLen[2]);
            pSQLWrap->TerminateString(szEffectBitMask, lLen[3]);
            pItem = new CDroneTechItem;

            if (NULL != pItem)
            {
                pItem->SetName(szName);
                pItem->SetObjectType(OT_droneType);
                pItem->SetObjectID(shID);
                pItem->SetRequiredBitMask(GetBitMask(szRequiredBitMask));
                pItem->SetEffectBitMask(GetBitMask(szEffectBitMask));

                pItem->SetHullID(shHullID);

                pItemList->Enqueue(pItem);
            } else
                fResult = FALSE;

            SQLResult = pSQLWrap->FetchRow(hStatement);
        }
        pSQLWrap->EndStatement(hStatement);
    }

    return(fResult);
}


HRESULT Initialize(CHAR * szDSN, CTechItemList * pCivList,
                    CTechItemList * pItemList)
{
    DWORD cStations, cHulls, cParts, cMines, cMissiles, cProbes;
    DWORD cDevelopments, cDrones; // , cUpgrades;
    SQLRETURN SQLResult;
    CSQLWrap SQLWrap;
    HRESULT hr;

    hr = E_FAIL;

    SQLResult = SQLWrap.Initialize(szDSN);
    if (SQL_SUCCEEDED(SQLResult))
    {
        if (FALSE == InitializeCivs(&SQLWrap, pCivList))
            goto ErrorLabel;

        if (FALSE == InitializeStations(&SQLWrap, pItemList))
            goto ErrorLabel;

        cStations = pItemList->Length();
        if (FALSE == InitializeHulls(&SQLWrap, pItemList))
            goto ErrorLabel;

        cHulls = pItemList->Length();
        if (FALSE == InitializeParts(&SQLWrap, pItemList))
            goto ErrorLabel;

        cParts = pItemList->Length();
        if (FALSE == InitializeMines(&SQLWrap, pItemList))
            goto ErrorLabel;

        cMines = pItemList->Length();
        if (FALSE == InitializeMissiles(&SQLWrap, pItemList))
            goto ErrorLabel;

        cMissiles = pItemList->Length();
        if (FALSE == InitializeProbes(&SQLWrap, pItemList))
            goto ErrorLabel;

        cProbes = pItemList->Length();
        if (FALSE == InitializeDevelopments(&SQLWrap, pItemList))
            goto ErrorLabel;

        cDevelopments = pItemList->Length();
        if (FALSE == InitializeDrones(&SQLWrap, pItemList))
            goto ErrorLabel;

        cDrones = pItemList->Length();
/*
        if (FALSE == InitializeStationUpgrades(&SQLWrap, pItemList))
            goto ErrorLabel;

        cUpgrades = gUpgradeList.Length();
*/

        hr = S_OK;

ErrorLabel:
        SQLWrap.Terminate();
    }

    cDrones -= cDevelopments;
    cDevelopments -= cProbes;
    cProbes -= cMissiles;
    cMissiles -= cMines;
    cMines -= cParts;
    cParts -= cHulls;
    cHulls -= cStations;
    printf("Loaded %d civs.\n", pCivList->Length());
    printf("Loaded %d stations.\n", cStations);
    printf("Loaded %d hulls.\n", cHulls);
    printf("Loaded %d parts.\n", cParts);
    printf("Loaded %d mines.\n", cMines);
    printf("Loaded %d missiles.\n", cMissiles);
    printf("Loaded %d probes.\n", cProbes);
    printf("Loaded %d developments.\n", cDevelopments);
    printf("Loaded %d drones.\n", cDrones);
/*
    printf("Loaded %d station upgrades.\n", cUpgrades);
*/
    printf("\n");

    return(hr);
}


VOID Terminate(CTechItemList * pCivList, CTechItemList * pItemList)
{
    pCivList->Empty();
    pItemList->Empty();
}


