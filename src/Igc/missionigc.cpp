/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	missionIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmissionIGC class. This file was initially created by
**  the ATL wizard for the mission object.
**
**      mission is currently setup as an object that can be instantiated multiple times
**  (generating a unique instance each time). This should probably change to allowing
**  only a single instance ... but it doesn't matter much. All the state (except for the
**  ref count) used by mission is stored as a static in the igc class.
**
**  History:
*/
// missionIGC.cpp : Implementation of CmissionIGC
#include    "pch.h"
#include    "missionIGC.h"
#include    "clusterIGC.h"
#include    "warpIGC.h"
#include    "dispenserIGC.h"
#include    "buildingEffectIGC.h"
#include    "hullTypeIGC.h"
#include    "buoyIGC.h"
#include    "shipIGC.h"
#include    "chaffIGC.h"
#include    "shieldIGC.h"
#include    "cloakIGC.h"
#include    "projectileIGC.h"
#include    "projectileTypeIGC.h"
#include    "partTypeIGC.h"
#include    "launcherTypeIGC.h"
#include    "weaponIGC.h"
#include    "asteroidIGC.h"
#include    "stationIGC.h"
#include    "treasureIGC.h"
#include    "treasureSetIGC.h"
#include    "afterburnerIGC.h"
#include    "packIGC.h"
#include    "magazineIGC.h"
#include    "missileTypeIGC.h"
#include    "probeIGC.h"
#include    "probeTypeIGC.h"
#include    "mineTypeIGC.h"
#include    "chaffTypeIGC.h"
#include    "missileIGC.h"
#include    "mineIGC.h"
#include    "civilizationIGC.h"
#include    "sideIGC.h"
#include    "stationTypeIGC.h"
#include    "developmentIGC.h"
#include    "droneTypeIGC.h"
#include    "bucketigc.h"
#include    "mapmakerigc.h"
static void DoDecrypt(int size, char* pdata)
{
    DWORD encrypt = 0;
    //Do a rolling XOR to demunge the data
    for (int i = 0; (i < size); i += 4)
    {
        DWORD*  p = (DWORD*)(pdata + i);

        encrypt = *p = *p ^ encrypt;
    }
}

//------------------------------------------------------------------------------
void    DumpIGCFile (FILE* file, ImissionIGC* pMission, __int64 iMaskExportTypes,  void (*munge)(int size, char* data))
{
    int         iSize = pMission->Export(iMaskExportTypes, 0);
    char*       pData = new char[iSize+4];      //leave a little extra space for the encryption (which takes dword chunks)
    pMission->Export (iMaskExportTypes, pData);
    fwrite (&iSize, sizeof(int), 1, file);

    if (munge)
    {
        (*munge)(iSize, pData);
    }

    fwrite (pData, sizeof(char), iSize, file);
    delete [] pData;
}

//------------------------------------------------------------------------------
void    LoadIGCFile (FILE* file, ImissionIGC* pMission, void (*munge)(int size, char* data))
{
    Time            now = pMission->GetLastUpdate();
    int             iDatasize;
    int             iReadCount = fread (&iDatasize, sizeof(iDatasize), 1, file);
    assert (iReadCount == 1);
    char*           pData = new char[iDatasize+4];      //leave a little extra space for the encryption (which takes dword chunks)
    iReadCount = fread (pData, sizeof(char), iDatasize, file);
    assert (iReadCount == iDatasize);

    if (munge)
    {
        (*munge)(iDatasize, pData);
    }

    pMission->Import (now, -1, pData, iDatasize);
    delete [] pData;
}

//------------------------------------------------------------------------------
bool    DumpIGCFile (const char* name, ImissionIGC* pMission, __int64 iMaskExportTypes, void (*munge)(int size, char* data))
{
    char        szFilename[MAX_PATH + 1];
    HRESULT     hr = UTL::getFile(name, ".igc", szFilename, true, true);
    FILE*       file = fopen (szFilename, "wb");
    if (file)
    {
        DumpIGCFile (file, pMission, iMaskExportTypes, munge);
        fclose (file);
        return true;
    }
    else
        return false;
}

//------------------------------------------------------------------------------
bool    LoadIGCFile (const char* name, ImissionIGC* pMission, void (*munge)(int size, char* data))
{
    char        szFilename[MAX_PATH + 1];
    HRESULT     hr = UTL::getFile(name, ".igc", szFilename, true, true);
    FILE*       file = fopen(szFilename, "rb");
    if (file)
    {
        LoadIGCFile (file, pMission, munge);
        fclose (file);

        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
#define IGC_FILE_VERSION_TYPE   int

//------------------------------------------------------------------------------
bool    DumpIGCStaticCore (const char* name, ImissionIGC* pMission, __int64 iMaskExportTypes, void (*munge)(int size, char* data))
{
    char        szFilename[MAX_PATH + 1];
    HRESULT     hr = UTL::getFile(name, ".igc", szFilename, true, true);
    FILE*       file = fopen (szFilename, "wb");
    if (file)
    {
        IGC_FILE_VERSION_TYPE   iStaticCoreVersion = static_cast<IGC_FILE_VERSION_TYPE> (time (0));
        fwrite (&iStaticCoreVersion, sizeof(iStaticCoreVersion), 1, file);
        DumpIGCFile (file, pMission, iMaskExportTypes, munge);
        fclose (file);
        
        return true;
    }
    else
    {
        assert ("Dump IGC Static Core failed." && false);
        return false;
    }
}

struct  CachedStaticCore
{
    CstaticIGC*             pstatic;
    IGC_FILE_VERSION_TYPE   version;
    int                     consumers;
    char                    name[32]; // was c_cbFileName - needed it longer
};

typedef Slist_utl<CachedStaticCore> CachedList;
typedef Slink_utl<CachedStaticCore> CachedLink;

static CachedList   s_cores;

int     LoadIGCStaticCore (const char* name, ImissionIGC* pMission, bool fGetVersionOnly, void (*munge)(int size, char* data))
{
    //See if we've already loaded the static core
    {
        for (CachedLink*    pcl = s_cores.first(); (pcl != NULL); pcl = pcl->next())
        {
            if (_stricmp(name, pcl->data().name) == 0)
            {
                if (!fGetVersionOnly)
                {
                    pMission->SetStaticCore(pcl->data().pstatic);
                    pcl->data().consumers++;
                }

                return pcl->data().version;
            }
        }
    }

    char        szFilename[MAX_PATH + 1];
    HRESULT     hr = UTL::getFile(name, ".igc", szFilename, true, true);
    FILE*       file = fopen(szFilename, "rb");
    if (file)
    {
        IGC_FILE_VERSION_TYPE   iStaticCoreVersion;
        int                     iReadCount = fread (&iStaticCoreVersion, sizeof(iStaticCoreVersion), 1, file);
        assert (iReadCount == 1);

        if (!fGetVersionOnly)
        {
            pMission->SetStaticCore(NULL);

            LoadIGCFile (file, pMission, munge);

            CachedLink* pcl = new CachedLink;

            pcl->data().pstatic = pMission->GetStaticCore();
            strcpy(pcl->data().name, name);
            pcl->data().version = iStaticCoreVersion;
            pcl->data().consumers = 1;

            s_cores.last(pcl);


#if 0
            {
                debugf("Weapons\n");
                for (PartTypeLinkIGC*   ppl = pMission->GetPartTypes()->first(); (ppl != NULL); ppl = ppl->next())
                {
                    IpartTypeIGC*   ppt = ppl->data();
                    if (ppt->GetEquipmentType() == ET_Weapon)
                    {
                        DataWeaponTypeIGC*  data = (DataWeaponTypeIGC*)ppt->GetData();
                        IprojectileTypeIGC* pt = pMission->GetProjectileType(data->projectileTypeID);
                        float   dps = (pt->GetBlastPower() + pt->GetPower()) / data->dtimeBurst;

                        debugf("Name: %s\n", ppt->GetName());
                        debugf("\tShots/second\t%f\n", (1.0f / data->dtimeBurst));
                        debugf("\tammo/second\t\t%f\n", float(data->cAmmoPerShot) / data->dtimeBurst);
                        debugf("\tenergy/second\t%f\n", data->energyPerShot / data->dtimeBurst);
                        debugf("\tTime to kill: (seconds)\t(target)\n");
                        {
                            for (PartTypeLinkIGC*   l2 = pMission->GetPartTypes()->first(); (l2 != NULL); l2 = l2->next())
                            {
                                IpartTypeIGC*   p2 = l2->data();
                                if (p2->GetEquipmentType() == ET_Shield)
                                {
                                    DataShieldTypeIGC*   d2 = (DataShieldTypeIGC*)(p2->GetData());
                                    float   dtm = pMission->GetDamageConstant(pt->GetDamageType(), d2->defenseType);
                                    float   d = dps * dtm; // - d2->rateRegen;
                                    if (d > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", d2->maxStrength / d);

                                        debugf("\t%s\n", p2->GetName());
                                    }
                                }
                            }
                        }
                        {
                            for (HullTypeLinkIGC*   l2 = pMission->GetHullTypes()->first(); (l2 != NULL); l2 = l2->next())
                            {
                                IhullTypeIGC*   h2 = l2->data();
                                for (HullTypeLinkIGC*   l3 = l2->txen(); (l3 != NULL); l3 = l3->txen())
                                    if (strcmp(h2->GetName(), l3->data()->GetName()) == 0)
                                        break;

                                if (l3 == NULL)
                                {
                                    float   dtm = pMission->GetDamageConstant(pt->GetDamageType(), h2->GetDefenseType());
                                    float   d = dps * dtm;
                                    if (d > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", h2->GetHitPoints() / d);

                                        debugf("\t%s\n", h2->GetName());
                                    }
                                }
                            }
                        }
                        {
                            for (StationTypeLinkIGC*   l2 = pMission->GetStationTypes()->first(); (l2 != NULL); l2 = l2->next())
                            {
                                IstationTypeIGC*   s2 = l2->data();
                                for (StationTypeLinkIGC*   l3 = l2->txen(); (l3 != NULL); l3 = l3->txen())
                                    if (strcmp(s2->GetName(), l3->data()->GetName()) == 0)
                                        break;

                                if (l3 == NULL)
                                {
                                    float   dtmA = pMission->GetDamageConstant(pt->GetDamageType(), s2->GetArmorDefenseType());
                                    float   dtmS = pMission->GetDamageConstant(pt->GetDamageType(), s2->GetShieldDefenseType());

                                    float   dS = dps * dtmS; // - s2->GetShieldRegeneration();
                                    if (dS > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", s2->GetMaxShieldHitPoints() / dS);

                                        debugf("\t%s (shield)\n", s2->GetName());
                                    }

                                    float   dA = dps * dtmA; // - s2->GetArmorRegeneration();
                                    if (dA > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", s2->GetMaxArmorHitPoints() / dA);

                                        debugf("\t%s (armor)\n", s2->GetName());
                                    }
                                }
                            }
                        }
                    }
                }
            }

            {
                debugf("Missiles\n");
                for (PartTypeLinkIGC*   ppl = pMission->GetPartTypes()->first(); (ppl != NULL); ppl = ppl->next())
                {
                    IpartTypeIGC*   ppt = ppl->data();
                    if (ppt->GetEquipmentType() == ET_Magazine)
                    {
                        ImissileTypeIGC* pmt = (ImissileTypeIGC*)(((IlauncherTypeIGC*)ppt)->GetExpendableType());
                        float   dps = (pmt->GetPower() + pmt->GetBlastPower());

                        debugf("Name: %s\n", pmt->GetName());
                        debugf("\tShots to kill: (shots)\t(target)\n");
                        {
                            for (PartTypeLinkIGC*   l2 = pMission->GetPartTypes()->first(); (l2 != NULL); l2 = l2->next())
                            {
                                IpartTypeIGC*   p2 = l2->data();
                                if (p2->GetEquipmentType() == ET_Shield)
                                {
                                    DataShieldTypeIGC*   d2 = (DataShieldTypeIGC*)(p2->GetData());
                                    float   dtm = pMission->GetDamageConstant(pmt->GetDamageType(), d2->defenseType);
                                    float   d = dps * dtm; // - d2->rateRegen;
                                    if (d > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", d2->maxStrength / d);

                                        debugf("\t%s\n", p2->GetName());
                                    }
                                }
                            }
                        }
                        {
                            for (HullTypeLinkIGC*   l2 = pMission->GetHullTypes()->first(); (l2 != NULL); l2 = l2->next())
                            {
                                IhullTypeIGC*   h2 = l2->data();
                                for (HullTypeLinkIGC*   l3 = l2->txen(); (l3 != NULL); l3 = l3->txen())
                                    if (strcmp(h2->GetName(), l3->data()->GetName()) == 0)
                                        break;

                                if (l3 == NULL)
                                {
                                    float   dtm = pMission->GetDamageConstant(pmt->GetDamageType(), h2->GetDefenseType());
                                    float   d = dps * dtm;
                                    if (d > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", h2->GetHitPoints() / d);

                                        debugf("\t%s\n", h2->GetName());
                                    }
                                }
                            }
                        }
                        {
                            for (StationTypeLinkIGC*   l2 = pMission->GetStationTypes()->first(); (l2 != NULL); l2 = l2->next())
                            {
                                IstationTypeIGC*   s2 = l2->data();
                                for (StationTypeLinkIGC*   l3 = l2->txen(); (l3 != NULL); l3 = l3->txen())
                                    if (strcmp(s2->GetName(), l3->data()->GetName()) == 0)
                                        break;

                                if (l3 == NULL)
                                {
                                    float   dtmA = pMission->GetDamageConstant(pmt->GetDamageType(), s2->GetArmorDefenseType());
                                    float   dtmS = pMission->GetDamageConstant(pmt->GetDamageType(), s2->GetShieldDefenseType());

                                    float   dS = dps * dtmS; // - s2->GetShieldRegeneration();
                                    if (dS > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", s2->GetMaxShieldHitPoints() / dS);

                                        debugf("\t%s (shield)\n", s2->GetName());
                                    }

                                    float   dA = dps * dtmA; // - s2->GetArmorRegeneration();
                                    if (dA > 0.0f)
                                    {
                                        debugf("\t\t\t%8.2f", s2->GetMaxArmorHitPoints() / dA);

                                        debugf("\t%s (armor)\n", s2->GetName());
                                    }
                                }
                            }
                        }
                    }
                }
            }

#endif
        }

        fclose (file);
        return iStaticCoreVersion;
    }
    else
    {
        assert ("Load IGC Static Core failed." && false);
        return NA;
    }
}

ImissionIGC*    CreateMission(void)
{
    return new CmissionIGC;
}

const float c_zLensMultiplier = 0.2f; // DON'T CHANGE THIS, people get really pissy about
const float c_radiusUniverse = 5000.0f;
const float c_outOfBounds = 25.0f;
const float c_fExitWarpSpeed = 15.0f;
const float c_fExitStationSpeed = 50.0f;
const float c_downedShield = 0.1f;
const float c_dtBetweenPaydays = 60.0f;             //one minute
const float c_fCapacityHe3 = 90.0f;
const float c_fValueHe3 = 80.0f;
const float c_fAmountHe3 = 120000.0f / c_fValueHe3; //total amount of He3 in universe/team
const float c_mountRate = 1.0f / 5.0f;

const float c_fStartingMoney = 16000.0f;
const float c_fWinTheGame    = c_fAmountHe3 * c_fValueHe3 * 0.6f;

const float c_dtRipcord = 10.0f;

const float c_fHe3Regeneration = 0.01f/60.0f;     //1% of the asteroid's value/minute

const float c_pointsWarp        = 2.0f;
const float c_pointsAsteroid    = 1.0f;
const float c_pointsTech        = 5.0f;
const float c_pointsMiner       = 10.0f;
const float c_pointsBuilder     = 20.0f;
const float c_pointsLayer       = 10.0f;
const float c_pointsCarrier     = 10.0f;
const float c_pointsPlayer      = 10.0f;
const float c_pointsBaseKill    = 25.0f;
const float c_pointsBaseCapture = 25.0f;

const float c_pointsFlags       = 10.0f;
const float c_pointsArtifacts   = 10.0f;
const float c_pointsRescues     = 5.0f;

const float c_crAdd             = 1.25f;
const float c_crDivide          = 100.0f;

const float c_fIncome           = 600.0f;

const float c_fLifepodEndurance = 300.0f;

const float c_fWarpBombDelay    = 15.0f;

const float c_fLifepodCost      = 0.5f;
const float c_fTurretCost       = 0.5f;
const float c_fDroneCost        = 0.5f;
const float c_fPlayerCost       = 1.0f;
const float c_fBaseClusterCost  = -36.0f;
const float c_fClusterDivisor   = 400.0f;

CstaticIGC::CstaticIGC(void)
:
    m_pptAmmoPack(NULL),
    m_pptFuelPack(NULL)
{
    m_constants.floatConstants[c_fcidLensMultiplier]        = c_zLensMultiplier;
    m_constants.floatConstants[c_fcidRadiusUniverse]        = c_radiusUniverse;
    m_constants.floatConstants[c_fcidOutOfBounds]           = c_outOfBounds;
    m_constants.floatConstants[c_fcidExitWarpSpeed]         = c_fExitWarpSpeed;
    m_constants.floatConstants[c_fcidExitStationSpeed]      = c_fExitStationSpeed;
    m_constants.floatConstants[c_fcidDownedShield]          = c_downedShield;
    m_constants.floatConstants[c_fcidSecondsBetweenPaydays] = c_dtBetweenPaydays;
    m_constants.floatConstants[c_fcidCapacityHe3]           = c_fCapacityHe3;
    m_constants.floatConstants[c_fcidValueHe3]              = c_fValueHe3;
    m_constants.floatConstants[c_fcidAmountHe3]             = c_fAmountHe3;
    m_constants.floatConstants[c_fcidMountRate]             = c_mountRate;

    m_constants.floatConstants[c_fcidStartingMoney]         = c_fStartingMoney;
    m_constants.floatConstants[c_fcidWinTheGameMoney]       = c_fWinTheGame;
    m_constants.floatConstants[c_fcidRipcordTime]           = c_dtRipcord;
    m_constants.floatConstants[c_fcidHe3Regeneration]       = c_fHe3Regeneration;

    m_constants.floatConstants[c_fcidPointsWarp]            = c_pointsWarp;
    m_constants.floatConstants[c_fcidPointsAsteroid]        = c_pointsAsteroid;
    m_constants.floatConstants[c_fcidPointsTech]            = c_pointsTech;
    m_constants.floatConstants[c_fcidPointsMiner]           = c_pointsMiner;
    m_constants.floatConstants[c_fcidPointsBuilder]         = c_pointsBuilder;
    m_constants.floatConstants[c_fcidPointsLayer]           = c_pointsLayer;
    m_constants.floatConstants[c_fcidPointsCarrier]         = c_pointsCarrier;
    m_constants.floatConstants[c_fcidPointsPlayer]          = c_pointsPlayer;
    m_constants.floatConstants[c_fcidPointsBaseKill]        = c_pointsBaseKill;
    m_constants.floatConstants[c_fcidPointsBaseCapture]     = c_pointsBaseCapture;
    m_constants.floatConstants[c_fcidPointsFlags]           = c_pointsFlags;
    m_constants.floatConstants[c_fcidPointsArtifacts]       = c_pointsArtifacts;
    m_constants.floatConstants[c_fcidPointsRescues]         = c_pointsRescues;
    m_constants.floatConstants[c_fcidRatingAdd]             = c_crAdd;
    m_constants.floatConstants[c_fcidRatingDivide]          = c_crDivide;

    m_constants.floatConstants[c_fcidIncome]                = c_fIncome;

    m_constants.floatConstants[c_fcidLifepodEndurance]      = c_fLifepodEndurance;

    m_constants.floatConstants[c_fcidWarpBombDelay]         = c_fWarpBombDelay;

    m_constants.floatConstants[c_fcidLifepodCost]           = c_fLifepodCost;
    m_constants.floatConstants[c_fcidTurretCost]            = c_fTurretCost;
    m_constants.floatConstants[c_fcidDroneCost]             = c_fDroneCost;
    m_constants.floatConstants[c_fcidPlayerCost]            = c_fPlayerCost;
    m_constants.floatConstants[c_fcidBaseClusterCost]       = c_fBaseClusterCost;
    m_constants.floatConstants[c_fcidClusterDivisor]        = c_fClusterDivisor;

    for (int i = 0; (i < c_dmgidMax); i++)
    {
        m_constants.damageConstants[i][0] = 0.0f;       //Everything vs. asteroid defaults to 0
        for (int j = 1; (j < c_defidMax); j++)
            m_constants.damageConstants[i][j] = 1.0f;
    }

    //NYI hard code the starting exceptions.
    m_constants.damageConstants[1][1] = 1.00f;        //Plasma rifle vs. lt ship
    m_constants.damageConstants[1][2] = 0.5f;         //Plasma rifle vs. md ship
    m_constants.damageConstants[1][3] = 0.5f;         //Plasma rifle vs. hv ship
    m_constants.damageConstants[1][4] = 0.25f;        //Plasma rifle vs. shv ship
    m_constants.damageConstants[1][5] = 2.00f;        //Plasma rifle vs. utility ships
    m_constants.damageConstants[1][6] = 0.0f;         //Plasma rifle vs. lt base
    m_constants.damageConstants[1][7] = 0.0f;         //Plasma rifle vs. hv base
    m_constants.damageConstants[1][8] = 0.75f;        //Plasma rifle vs. ship shields
    m_constants.damageConstants[1][9] = 0.0f;         //Plasma rifle vs. lt base shields
    m_constants.damageConstants[1][10]= 0.0f;         //Plasma rifle vs. hv base shields
    m_constants.damageConstants[1][11]= 1.0f;         //Plasma rifle vs. probe
    m_constants.damageConstants[1][12]= 0.25f;        //Plasma rifle vs. ultralt base armor
    m_constants.damageConstants[1][13]= 1.0f;         //Plasma rifle vs. ultralt base shield
    m_constants.damageConstants[1][14] = 0.75f;       //Plasma rifle vs. lg ship shields

    m_constants.damageConstants[2][1] = 1.00f;        //MG vs. lt ship
    m_constants.damageConstants[2][2] = 0.75f;        //MG vs. md ship
    m_constants.damageConstants[2][3] = 0.75f;        //MG vs. hv ship
    m_constants.damageConstants[2][4] = 0.5f;         //MG vs. shv ship
    m_constants.damageConstants[2][5] = 0.5f;         //MG vs. utility ships
    m_constants.damageConstants[2][6] = 0.0f;         //MG vs. lt base
    m_constants.damageConstants[2][7] = 0.0f;         //MG vs. hv base
    m_constants.damageConstants[2][8] = 0.5f;         //MG vs. ship shields
    m_constants.damageConstants[2][9] = 0.00f;        //MG vs. lt base shields
    m_constants.damageConstants[2][10]= 0.00f;        //MG vs. hv base shields
    m_constants.damageConstants[2][11]= 1.00f;        //MG vs. probe
    m_constants.damageConstants[2][12]= 0.5f;         //MG vs. ultralt base armor
    m_constants.damageConstants[2][13]= 0.5f;         //MG vs. ultralt base shield
    m_constants.damageConstants[2][14] = 0.5f;        //MG vs. lg ship shields

    m_constants.damageConstants[3][1] = 1.00f;        //Disruptors vs. lt ship
    m_constants.damageConstants[3][2] = 1.50f;        //Disruptors vs. md ship
    m_constants.damageConstants[3][3] = 4.00f;        //Disruptors vs. hv ship
    m_constants.damageConstants[3][4] = 4.00f;        //Disruptors vs. shv ship
    m_constants.damageConstants[3][5] = 0.50f;        //Disruptors vs. utility ships
    m_constants.damageConstants[3][6] = 0.00f;        //Disruptors vs. lt base
    m_constants.damageConstants[3][7] = 0.00f;        //Disruptors vs. hv base
    m_constants.damageConstants[3][8] = 3.0f;         //Disruptors vs. ship shields
    m_constants.damageConstants[3][9] = 0.0f;         //Disruptors vs. lt base shields
    m_constants.damageConstants[3][10]= 0.0f;         //Disruptors vs. hv base shields
    m_constants.damageConstants[3][11]= 1.0f;         //Disruptors vs. probe
    m_constants.damageConstants[3][12]= 4.0f;         //Disruptors vs. ultralt base armor
    m_constants.damageConstants[3][13]= 3.0f;         //Disruptors vs. ultralt base shield
    m_constants.damageConstants[3][14] = 3.0f;        //Disruptors vs. lg ship shields

    m_constants.damageConstants[4][1] = 1.00f;        //Sniper rifle vs. lt ship
    m_constants.damageConstants[4][2] = 1.00f;        //Sniper rifle vs. md ship
    m_constants.damageConstants[4][3] = 1.33f;        //Sniper rifle vs. hv ship
    m_constants.damageConstants[4][4] = 1.00f;        //Sniper rifle vs. shv ship
    m_constants.damageConstants[4][5] = 0.5f;         //Sniper rifle vs. utility ships
    m_constants.damageConstants[4][6] = 0.00f;        //Sniper rifle vs. lt base
    m_constants.damageConstants[4][7] = 0.00f;        //Sniper rifle vs. hv base
    m_constants.damageConstants[4][8] = 1.00f;        //Sniper rifle vs. ship shields
    m_constants.damageConstants[4][9] = 0.00f;        //Sniper rifle vs. lt base shields
    m_constants.damageConstants[4][10]= 0.00f;        //Sniper rifle vs. hv base shields
    m_constants.damageConstants[4][11]= 1.00f;        //Sniper rifle vs. probe
    m_constants.damageConstants[4][12]= 1.00f;        //Sniper rifle vs. ultralt base armor
    m_constants.damageConstants[4][13]= 1.0f;         //Sniper rifle vs. ultralt base shield
    m_constants.damageConstants[4][14] = 1.00f;       //Sniper rifle vs. lg ship shields

    m_constants.damageConstants[5][1] = 1.00f;        //Mini-gun vs. lt ship
    m_constants.damageConstants[5][2] = 0.75f;        //Mini-gun vs. md ship
    m_constants.damageConstants[5][3] = 0.50f;        //Mini-gun vs. hv ship
    m_constants.damageConstants[5][4] = 0.25f;        //Mini-gun vs. shv ship
    m_constants.damageConstants[5][5] = 0.25f;        //Mini-gun vs. utility ships
    m_constants.damageConstants[5][6] = 0.0f;         //Mini-gun vs. lt base
    m_constants.damageConstants[5][7] = 0.0f;         //Mini-gun vs. hv base
    m_constants.damageConstants[5][8] = 0.5f;         //Mini-gun vs. ship shields
    m_constants.damageConstants[5][9] = 0.0f;         //Mini-gun vs. lt base shields
    m_constants.damageConstants[5][10]= 0.0f;         //Mini-gun vs. hv base shields
    m_constants.damageConstants[5][11]= 1.0f;         //Mini-gun vs. probe
    m_constants.damageConstants[5][12]= 0.25f;        //Mini-gun vs. ultralt base armor
    m_constants.damageConstants[5][13]= 0.5f;         //Mini-gun vs. ultralt base shield
    m_constants.damageConstants[5][14] = 0.5f;        //Mini-gun vs. lg ship shields

    m_constants.damageConstants[6][1] = 1.00f;        //Cannon vs. lt ship
    m_constants.damageConstants[6][2] = 0.5f;         //Cannon vs. md ship
    m_constants.damageConstants[6][3] = 0.25f;        //Cannon vs. hv ship
    m_constants.damageConstants[6][4] = 0.25f;        //Cannon vs. shv ship
    m_constants.damageConstants[6][5] = 0.5f;         //Cannon vs. utility ships
    m_constants.damageConstants[6][6] = 0.0f;         //Cannon vs. lt base
    m_constants.damageConstants[6][7] = 0.0f;         //Cannon vs. hv base
    m_constants.damageConstants[6][8] = 1.0f;         //Cannon vs. ship shields
    m_constants.damageConstants[6][9] = 0.0f;         //Cannon vs. lt base shields
    m_constants.damageConstants[6][10]= 0.0f;         //Cannon vs. hv base shields
    m_constants.damageConstants[6][11]= 1.0f;         //Cannon vs. probe
    m_constants.damageConstants[6][12]= 0.25f;        //Cannon vs. ultralt base armor
    m_constants.damageConstants[6][13]= 1.0f;         //Cannon vs. ultralt base shield
    m_constants.damageConstants[6][14] = 1.0f;        //Cannon vs. lg ship shields

    m_constants.damageConstants[7][0] = 1.00f;        //Galvonic vs. asteroids
    m_constants.damageConstants[7][1] = 1.00f;        //Galvonic vs. lt ship
    m_constants.damageConstants[7][2] = 1.50f;        //Galvonic vs. md ship
    m_constants.damageConstants[7][3] = 4.00f;        //Galvonic vs. hv ship
    m_constants.damageConstants[7][4] = 4.00f;        //Galvonic vs. shv ship
    m_constants.damageConstants[7][5] = 0.50f;        //Galvonic vs. utility ships
    m_constants.damageConstants[7][6] = 10.00f;       //Galvonic vs. lt base
    m_constants.damageConstants[7][7] = 0.00f;        //Galvonic vs. hv base
    m_constants.damageConstants[7][8] = 3.0f;         //Galvonic vs. ship shields
    m_constants.damageConstants[7][9] = 4.0f;         //Galvonic vs. lt base shields
    m_constants.damageConstants[7][10]= 0.0f;         //Galvonic vs. hv base shields
    m_constants.damageConstants[7][11]= 1.0f;         //Galvonic vs. probe
    m_constants.damageConstants[7][12]= 10.00f;       //Galvonic vs. ultralt base armor
    m_constants.damageConstants[7][13]= 4.0f;         //Galvonic vs. ultralt base shield
    m_constants.damageConstants[7][14] = 3.0f;        //Galvonic vs. lg ship shields

    m_constants.damageConstants[8][0] = 1.0f;         //Anti-base weapons vs. asteroid
    m_constants.damageConstants[8][6] = 15.0f;        //Anti-base weapons vs. lt base
    m_constants.damageConstants[8][7] = 10.0f;        //Anti-base weapons vs. hv base
    m_constants.damageConstants[8][9] = 10.0f;        //Anti-base weapons vs. lt base shield
    m_constants.damageConstants[8][10]= 10.0f;        //Anti-base weapons vs. hv base shield
    m_constants.damageConstants[8][12]= 15.00f;       //Anti-base weapons vs. ultralt base armor
    m_constants.damageConstants[8][13]= 15.0f;        //Anti-base weapons vs. ultralt base shield

    m_constants.damageConstants[9][0] = 15.0f;        //Anti-rock weapons vs. asteroids

                                                      //Repair weapons

    m_constants.damageConstants[11][1] = 1.00f;       //Turret vs. lt ship
    m_constants.damageConstants[11][2] = 0.85f;       //Turret vs. md ship
    m_constants.damageConstants[11][3] = 0.50f;       //Turret vs. hv ship
    m_constants.damageConstants[11][4] = 0.25f;       //Turret vs. shv ship
    m_constants.damageConstants[11][5] = 0.50f;       //Turret vs. utility ships
    m_constants.damageConstants[11][6] = 0.0f;        //Turret vs. lt base
    m_constants.damageConstants[11][7] = 0.0f;        //Turret vs. hv base
    m_constants.damageConstants[11][8] = 0.75f;       //Turret vs. ship shields
    m_constants.damageConstants[11][9] = 0.0f;        //Turret vs. lt base shields
    m_constants.damageConstants[11][10]= 0.0f;        //Turret vs. hv base shields
    m_constants.damageConstants[11][11]= 1.0f;        //Turret vs. probe
    m_constants.damageConstants[11][12]= 0.25f;       //Turret vs. ultralt base armor
    m_constants.damageConstants[11][13]= 0.75f;       //Turret vs. ultralt base shield
    m_constants.damageConstants[11][14] = 0.75f;      //Turret vs. lg ship shields

    m_constants.damageConstants[12][0] = 0.00f;       //shield buster vs. asteroids
    m_constants.damageConstants[12][1] = 0.00f;       //shield buster vs. lt ship
    m_constants.damageConstants[12][2] = 0.00f;       //shield buster vs. md ship
    m_constants.damageConstants[12][3] = 0.00f;       //shield buster vs. hv ship
    m_constants.damageConstants[12][4] = 0.00f;       //shield buster vs. shv ship
    m_constants.damageConstants[12][5] = 0.00f;       //shield buster vs. utility ships
    m_constants.damageConstants[12][6] = 0.00f;       //shield buster vs. lt base
    m_constants.damageConstants[12][7] = 0.00f;       //shield buster vs. hv base
    m_constants.damageConstants[12][8] = 1.0f;        //shield buster vs. ship shields
    m_constants.damageConstants[12][9] = 1.0f;        //shield buster vs. base shields
    m_constants.damageConstants[12][10]= 1.0f;        //shield buster vs. base shields
    m_constants.damageConstants[12][11]= 0.0f;        //shield buster vs. probe
    m_constants.damageConstants[12][12]= 0.0f;        //shield buster vs. ultralt base armor
    m_constants.damageConstants[12][13]= 1.0f;        //shield buster vs. ultralt base shield
    m_constants.damageConstants[12][14] = 1.0f;       //shield buster vs. lg ship shields

    m_constants.damageConstants[13][0] = 0.00f;       //shield buster vs. asteroids
    m_constants.damageConstants[13][1] = 0.00f;       //shield buster vs. lt ship
    m_constants.damageConstants[13][2] = 0.00f;       //shield buster vs. md ship
    m_constants.damageConstants[13][3] = 0.00f;       //shield buster vs. hv ship
    m_constants.damageConstants[13][4] = 0.00f;       //shield buster vs. shv ship
    m_constants.damageConstants[13][5] = 0.00f;       //shield buster vs. utility ships
    m_constants.damageConstants[13][6] = 0.00f;       //shield buster vs. lt base
    m_constants.damageConstants[13][7] = 0.00f;       //shield buster vs. hv base
    m_constants.damageConstants[13][8] = 1.0f;        //shield buster vs. ship shields
    m_constants.damageConstants[13][9] = 0.5f;        //shield buster vs. base shields
    m_constants.damageConstants[13][10]= 0.0f;        //shield buster vs. base shields
    m_constants.damageConstants[13][11]= 0.0f;        //shield buster vs. probe
    m_constants.damageConstants[13][12]= 0.0f;        //shield buster vs. ultralt base armor
    m_constants.damageConstants[13][13]= 1.0f;        //shield buster vs. ultralt base shield
    m_constants.damageConstants[13][14] = 1.0f;       //shield buster vs. lg ship shields

                                                        //catpure weapon

    m_constants.damageConstants[15][0] = 1.00f;        //Skyripper vs. asteroid
    m_constants.damageConstants[15][1] = 1.00f;        //Skyripper vs. lt ship
    m_constants.damageConstants[15][2] = 1.00f;        //Skyripper vs. md ship
    m_constants.damageConstants[15][3] = 2.50f;        //Skyripper vs. hv ship
    m_constants.damageConstants[15][4] = 2.00f;        //Skyripper vs. shv ship
    m_constants.damageConstants[15][5] = 1.00f;        //Skyripper vs. utility ships
    m_constants.damageConstants[15][6] = 1.5f;         //Skyripper vs. lt base
    m_constants.damageConstants[15][7] = 1.00f;        //Skyripper vs. hv base
    m_constants.damageConstants[15][8] = 1.5f;         //Skyripper vs. ship shields
    m_constants.damageConstants[15][9] = 2.00f;        //Skyripper vs. lt base shields
    m_constants.damageConstants[15][10]= 2.00f;        //Skyripper vs. hv base shields
    m_constants.damageConstants[15][11]= 1.00f;        //Skyripper vs. probe
    m_constants.damageConstants[15][12]= 2.0f;         //Skyripper vs. ultralt base armor
    m_constants.damageConstants[15][13]= 2.0f;         //Skyripper vs. ultralt base shield
    m_constants.damageConstants[15][14] = 1.5f;        //Skyripper vs. lg ship shields

    m_constants.damageConstants[16][1] = 1.00f;        //Mine vs. lt ship
    m_constants.damageConstants[16][2] = 0.75f;        //Mine vs. md ship
    m_constants.damageConstants[16][3] = 0.75f;        //Mine vs. hv ship
    m_constants.damageConstants[16][4] = 0.50f;        //Mine vs. shv ship
    m_constants.damageConstants[16][5] = 0.25f;        //Mine vs. utility ships
    m_constants.damageConstants[16][6] = 0.0f;         //Mine vs. lt base
    m_constants.damageConstants[16][7] = 0.0f;         //Mine vs. hv base
    m_constants.damageConstants[16][8] = 0.5f;         //Mine vs. ship shields
    m_constants.damageConstants[16][9] = 0.0f;         //Mine vs. lt base shields
    m_constants.damageConstants[16][10]= 0.0f;         //Mine vs. hv base shields
    m_constants.damageConstants[16][11]= 1.0f;         //Mine vs. probe
    m_constants.damageConstants[16][12]= 0.25f;        //Mine vs. ultralt base armor
    m_constants.damageConstants[16][13]= 0.5f;         //Mine vs. ultralt base shield
    m_constants.damageConstants[16][14] = 0.5f;        //Mine vs. lg ship shields

    m_constants.damageConstants[17][0] = 1.0f;         //Stinger vs. asteroid
    m_constants.damageConstants[17][6] = 15.0f;        //Stinger vs. lt base
    m_constants.damageConstants[17][7] = 10.0f;        //Stinger vs. hv base
    m_constants.damageConstants[17][9] = 15.0f;        //Stinger vs. lt base shield
    m_constants.damageConstants[17][10]= 15.0f;        //Stinger vs. hv base shield
    m_constants.damageConstants[17][12]= 15.0f;        //Stinger vs. ultralt base armor
    m_constants.damageConstants[17][13]= 15.0f;        //Stinger vs. ultralt base shield

    m_constants.damageConstants[18][1] = 1.00f;        //Lancer vs. lt ship
    m_constants.damageConstants[18][2] = 0.75f;        //Lancer vs. md ship
    m_constants.damageConstants[18][3] = 0.50f;        //Lancer vs. hv ship
    m_constants.damageConstants[18][4] = 0.40f;        //Lancer vs. shv ship
    m_constants.damageConstants[18][5] = 0.25f;        //Lancer vs. utility ships
    m_constants.damageConstants[18][6] = 0.0f;         //Lancer vs. lt base
    m_constants.damageConstants[18][7] = 0.0f;         //Lancer vs. hv base
    m_constants.damageConstants[18][8] = 0.75f;        //Lancer vs. ship shields
    m_constants.damageConstants[18][9] = 0.0f;         //Lancer vs. lt base shields
    m_constants.damageConstants[18][10]= 0.0f;         //Lancer vs. hv base shields
    m_constants.damageConstants[18][11]= 0.05f;        //Lancer vs. probe
    m_constants.damageConstants[18][12]= 0.25f;        //Lancer vs. ultralt base armor
    m_constants.damageConstants[18][13]= 0.5f;         //Lancer vs. ultralt base shield
    m_constants.damageConstants[18][14] = 0.50f;       //Lancer vs. lg ship shields
}


CmissionIGC::~CmissionIGC(void)
{
    if (m_pStatic != NULL)
    {
        for (CachedLink*    pcl = s_cores.first(); (pcl != NULL); pcl = pcl->next())
        {
            if (m_pStatic == pcl->data().pstatic)
            {
                if ((pcl->data().consumers--) == 1)
                {
                    m_pStatic->Terminate();
                    delete m_pStatic;
                    delete pcl;
                    break;
                }
            }
        }
    }
}

void CmissionIGC::Initialize(Time now, IIgcSite* pIgcSite)
{
    debugf("Mission Initialize id=%d this=%x now=%d\n", GetMissionID(), this, now.clock());
    m_pIgcSite = pIgcSite;
    m_lastUpdate = now;

    m_damageTracks.Initialize(now);

 #ifndef DREAMCAST
    //preload the convex hulls used for the various asteroids
    ZVerify(HitTest::Load("bgrnd50"));
    ZVerify(HitTest::Load("bgrnd51"));
    ZVerify(HitTest::Load("bgrnd52"));
    ZVerify(HitTest::Load("bgrnd53"));
    ZVerify(HitTest::Load("bgrnd03"));
#endif

    m_sideTeamLobby = NULL;
}

void    CmissionIGC::Terminate(void)
{
    debugf("Terminating mission id=%d, this=%x\n", GetMissionID(), this);
  
    m_pIgcSite->TerminateMissionEvent(this);

    {
        //Ships are not terminated ... they are mearly moved to the NULL mission
        //So do it before nuking the clusters since that would nuke the ships in the
        //cluster.
        ShipLinkIGC*  l;
        while ((l = m_ships.first()) != NULL)
        {
            l->data()->SetMission(NULL);
        }
    }
    {
        ClusterLinkIGC*  l;
        while ((l = m_clusters.first()) != NULL)
        {
            l->data()->Terminate();
        }
    }
    {
        SideLinkIGC*  l;
        while ((l = m_sides.first()) != NULL)
        {
            l->data()->Terminate();
        }
    }
    if (m_sideTeamLobby)
    {
        m_sideTeamLobby->Terminate();
        m_sideTeamLobby->Release();
        m_sideTeamLobby = NULL;
    };

    assert (m_clusters.n() == 0);
    assert (m_warps.n() == 0);
    assert (m_stations.n() == 0);
    assert (m_asteroids.n() == 0);
    assert (m_treasures.n() == 0);
    assert (m_ships.n() == 0);
    assert (m_sides.n() == 0);
}

void     CmissionIGC::Update(Time now)
{
    assert (now >= m_lastUpdate);

    if (now != m_lastUpdate)
    {
        if ((m_stageMission == STAGE_STARTED) ||
            (m_stageMission == STAGE_STARTING))
        {
            {
                //Update the various sides
                for (SideLinkIGC*   l = m_sides.first();
                     (l != NULL);
                     l = l->next())
                {
                    l->data()->Update(now);
                }
            }

            m_damageTracks.Update(now);

            assert (now - m_lastUpdate < 600.0f);   //never 10 minutes at a time.
            float   deltaT = now - m_lastUpdate;

            //Update the components in chunks of no more than 250ms each
            //if more than one chuck of time is required, evenly divide the
            //total time into nice, even chunks.
            assert (deltaT > 0.0f);

            int n = 1 + (int)(deltaT / c_fTimeIncrement);
            //ShouldBe(n == 1);

            Time    startTime = m_lastUpdate;

            for (int i = 1; (i <= n); i++)
            {
                Time    timeUpdate = startTime + (deltaT * (((float)i) / ((float)n)));

                {
                    //Update all clusters (which update all the models in clusters, etc.)
                    //Clusters can never be deleted on an update, so this is safe.
                    for (ClusterLinkIGC*  l = m_clusters.first();
                         (l != NULL);
                         l = l->next())
                    {
                        assert (l->data());
                        l->data()->Update(timeUpdate);
                    }
                }

                m_lastUpdate = timeUpdate;
            }
        }
        else
            m_lastUpdate = now;
    }

    assert (m_lastUpdate == now);
}

static int  ExportList(__int64              maskTypes,
                       const BaseListIGC*   plist,
                       char**               ppdata)
{
    int offset = 0;
    int size = 0;

    for (BaseLinkIGC*   pbl = plist->first();
         (pbl != NULL);
         pbl = pbl->next())
    {
        ObjectType  type = pbl->data()->GetObjectType();
        if (maskTypes & (__int64(1) << __int64(type)))
        {
            if (*ppdata)
            {
                int sizeItem = pbl->data()->Export(*ppdata + sizeof(int) + sizeof(ObjectType));
            
                *((ObjectType*)*ppdata) = type;
                *((int*)(*ppdata + sizeof(ObjectType))) = sizeItem;

                *ppdata += sizeItem + sizeof(int) + sizeof(ObjectType);
                size += sizeItem + sizeof(int) + sizeof(ObjectType);
            }
            else
                size += pbl->data()->Export(NULL) + sizeof(int) + sizeof(ObjectType);
        }
    }

    return size;
}

int                 CmissionIGC::Export(__int64  maskTypes,
                                        char*    pdata) const
{
    int datasize = 0;

    if (maskTypes & (__int64(1) << __int64(OT_constants)))
    {
        int  size = GetSizeOfConstants();
        if (pdata)
        {
            *((ObjectType*)pdata) = OT_constants;
            *((int*)(pdata + sizeof(ObjectType))) = size;
            memcpy(pdata + sizeof(int) + sizeof(ObjectType), GetConstants(), size);

            pdata += sizeof(int) + sizeof(ObjectType) + size;
        }
        datasize += size + sizeof(int) + sizeof(ObjectType);
    }

    datasize += ExportList(maskTypes, (BaseListIGC*)GetProjectileTypes(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetExpendableTypes(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetPartTypes(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetHullTypes(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetDevelopments(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetDroneTypes(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetStationTypes(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetTreasureSets(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)GetCivilizations(), &pdata);

    datasize += ExportList(maskTypes, (BaseListIGC*)&m_sides, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_clusters, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_warps, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_asteroids, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_stations, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_probes, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_mines, &pdata);
    datasize += ExportList(maskTypes, (BaseListIGC*)&m_treasures, &pdata);


    return datasize;
}

void                CmissionIGC::Import(Time    now,
                                        __int64 maskTypes,
                                        char*   pdata,
                                        int     datasize)
{
    while (datasize > 0)
    {
        ObjectType  type = *((ObjectType*)pdata);
        int size = *((int*)(pdata + sizeof(ObjectType)));

        if (maskTypes & (__int64(1) << __int64(type)))
        {
            if (type == OT_constants)
            {
                SetConstants((void*)(pdata + sizeof(int) + sizeof(ObjectType)));
            }
            else
            {
                IbaseIGC*   b = CreateObject(now, type, (void*)(pdata + sizeof(int) + sizeof(ObjectType)), size);

                assert (b);
                b->Release();
            }
            pdata += size + sizeof(int) + sizeof(ObjectType);
        }

        datasize -= (size + sizeof(int) + sizeof(ObjectType));
    }
}

IbaseIGC*           CmissionIGC::CreateObject(Time now, ObjectType objecttype,
                                              const void* data, int dataSize)
{
    #define OBJECT(CLS)   case OT_##CLS## :\
                                { pBase = new C##CLS##IGC; } break;

    IbaseIGC*   pBase;

    //Create an instance of the class
    switch (objecttype)
    {
        OBJECT(projectile)
        OBJECT(missile)
        OBJECT(mine)
        OBJECT(probe)
        OBJECT(ship)
        OBJECT(weapon)
        OBJECT(shield)
        OBJECT(cloak)
        OBJECT(chaff)
        OBJECT(afterburner)
        OBJECT(pack)
        OBJECT(magazine)
        OBJECT(dispenser)
        OBJECT(treasure)
        OBJECT(treasureSet)
        OBJECT(cluster)
        OBJECT(warp)
        OBJECT(buoy)
        OBJECT(buildingEffect)
        OBJECT(side)
        OBJECT(civilization)
        OBJECT(station)
        OBJECT(asteroid)
        OBJECT(projectileType)
        OBJECT(hullType)
        OBJECT(stationType)
        OBJECT(missileType)
        OBJECT(mineType)
        OBJECT(chaffType)
        OBJECT(probeType)
        OBJECT(droneType)
        OBJECT(development)
        OBJECT(bucket)

        case OT_partType:
        {
            //Part types and magazine part types get special treatment
            assert ((dataSize >= sizeof(DataLauncherTypeIGC)) ||
                    (dataSize >= sizeof(DataPartTypeIGC)));
            assert (sizeof(DataLauncherTypeIGC) < sizeof(DataPartTypeIGC));

            pBase = (dataSize == sizeof(DataLauncherTypeIGC))
                    ? (IbaseIGC*)(new ClauncherTypeIGC)
                    : (IbaseIGC*)(new CpartTypeIGC);
        }
        break;

        default:
        {
            ShouldBe(false);
            pBase = NULL;
        }
    }

    if (pBase)
    {
        //Increment the ref count because IGC objects should be created with a ref count of 1.
        pBase->AddRef();

		HRESULT hr = pBase->Initialize(this, now, data, dataSize);
        if (FAILED(hr) || (hr == S_FALSE))  // mmf added or check for S_FALSE to support destroyed TP
        {
			debugf("mmf Initialize return failed or S_FALSE in missionigc, S_FALSE added to support TP destroy\n");
            if (hr != E_ABORT)
                pBase->Terminate();

            pBase->Release();
            pBase = NULL;
        }
    }

    #undef  OBJECT

    return pBase;
}

ImodelIGC*  CmissionIGC::GetModel(ObjectType type, ObjectID id) const
{
    switch (type)
    {
        case OT_station:
            return GetStation(id);

        case OT_warp:
            return GetWarp(id);

        case OT_ship:
            return GetShip(id);

        case OT_asteroid:
            return GetAsteroid(id);

        case OT_treasure:
            return GetTreasure(id);

        case OT_probe:
            return GetProbe(id);

        case OT_mine:
            return GetMine(id);

        case OT_buoy:
            return GetBuoy(id);
    }

    return NULL;
}

//
// Note: Some objects aren't supported by this.  This returns NULL in such
//       cases.
//
IbaseIGC*  CmissionIGC::GetBase(ObjectType type, ObjectID id) const
{
    switch (type)
    {
        case OT_station:
            return GetStation(id);

        case OT_warp:
            return GetWarp(id);

        case OT_ship:
            return GetShip(id);

        case OT_asteroid:
            return GetAsteroid(id);

        case OT_treasure:
            return GetTreasure(id);

        case OT_probe:
            return GetProbe(id);

        case OT_mine:
            return GetMine(id);

        case OT_projectile:  
            assert(0);
            return NULL;//GetProjectile(id);

        case OT_side:        
            return GetSide(id);

        case OT_cluster:     
            return GetCluster(id);

        case OT_weapon:      
            return NULL;//GetWeapon(id);

        case OT_treasureSet:
            return GetTreasureSet(id);
/*
        case OT_shield:
        case OT_cloak:            
        case OT_pack:             
        case OT_magazine:         
        case OT_dispenser:        
        case OT_afterburner:     
        case OT_bucket:           
          return NULL;
*/
        case OT_projectileType:  
          return GetProjectileType(id);

        case OT_hullType:         
          return GetHullType(id);

        case OT_partType:         
          return GetPartType(id);

        case OT_missileType:   
            return NULL;//GetMissile??(id);

        case OT_mineType:         
            return GetMine(id);

        case OT_probeType:
            return GetProbe(id);

        case OT_civilization:     
            return GetCivilization(id);

        case OT_stationType:      
            return GetStationType(id);

        case OT_development:      
            return GetDevelopment(id);

        case OT_droneType:        
            return GetDroneType(id);
    }

    return NULL;
}

void                        CmissionIGC::AddWarp(IwarpIGC*  w)
{
    AddIbaseIGC((BaseListIGC*)&m_warps, w);
}
void                        CmissionIGC::DeleteWarp(IwarpIGC* w)
{
    DeleteIbaseIGC((BaseListIGC*)&m_warps, w);
}
IwarpIGC*                   CmissionIGC::GetWarp(WarpID     id) const
{
    return (IwarpIGC*)GetIbaseIGC((BaseListIGC*)&m_warps, id);
}
const WarpListIGC*    CmissionIGC::GetWarps(void) const
{
    return &m_warps;
}

void                        CmissionIGC::AddStation(IstationIGC*  s)
{
    AddIbaseIGC((BaseListIGC*)&m_stations, s);
}
void                        CmissionIGC::DeleteStation(IstationIGC* s)
{
    DeleteIbaseIGC((BaseListIGC*)&m_stations, s);
}
IstationIGC*                CmissionIGC::GetStation(StationID   id) const
{
    return (IstationIGC*)GetIbaseIGC((BaseListIGC*)&m_stations, id);
}
const StationListIGC* CmissionIGC::GetStations(void) const
{
    return &m_stations;
}

void                            CmissionIGC::AddTreasure(ItreasureIGC*  t)
{
    AddIbaseIGC((BaseListIGC*)&m_treasures, t);
}
void                            CmissionIGC::DeleteTreasure(ItreasureIGC* t)
{
    DeleteIbaseIGC((BaseListIGC*)&m_treasures, t);
}
ItreasureIGC*                   CmissionIGC::GetTreasure(TreasureID  id) const
{
    return (ItreasureIGC*)GetIbaseIGC((BaseListIGC*)&m_treasures, id);
}
const TreasureListIGC*    CmissionIGC::GetTreasures(void) const
{
    return &m_treasures;
}

void                        CmissionIGC::AddAsteroid(IasteroidIGC*  a)
{
    AddIbaseIGC((BaseListIGC*)&m_asteroids, a);
}
void                        CmissionIGC::DeleteAsteroid(IasteroidIGC* a)
{
    DeleteIbaseIGC((BaseListIGC*)&m_asteroids, a);
}
IasteroidIGC*                 CmissionIGC::GetAsteroid(AsteroidID       id) const
{
    return (IasteroidIGC*)GetIbaseIGC((BaseListIGC*)&m_asteroids, id);
}
const AsteroidListIGC*        CmissionIGC::GetAsteroids(void) const
{
    return &m_asteroids;
}

void                        CmissionIGC::AddProbe(IprobeIGC*  a)
{
    AddIbaseIGC((BaseListIGC*)&m_probes, a);
}
void                        CmissionIGC::DeleteProbe(IprobeIGC* a)
{
    DeleteIbaseIGC((BaseListIGC*)&m_probes, a);
}
IprobeIGC*                 CmissionIGC::GetProbe(ProbeID       id) const
{
    return (IprobeIGC*)GetIbaseIGC((BaseListIGC*)&m_probes, id);
}
const ProbeListIGC*        CmissionIGC::GetProbes(void) const
{
    return &m_probes;
}

void                        CmissionIGC::AddMine(ImineIGC*  a)
{
    AddIbaseIGC((BaseListIGC*)&m_mines, a);
}
void                        CmissionIGC::DeleteMine(ImineIGC* a)
{
    DeleteIbaseIGC((BaseListIGC*)&m_mines, a);
}
ImineIGC*                 CmissionIGC::GetMine(MineID       id) const
{
    return (ImineIGC*)GetIbaseIGC((BaseListIGC*)&m_mines, id);
}
const MineListIGC*        CmissionIGC::GetMines(void) const
{
    return &m_mines;
}

void                        CmissionIGC::AddSide(IsideIGC*  s)
{
    // if there is not a team lobby but the civs do exist, go ahead and 
    // create the team lobby.
    if (!m_sideTeamLobby && GetCivilizations()->first() && s->GetObjectID() != SIDE_TEAMLOBBY)
    {
        //create the lobby side
        DataSideIGC sidedata;
        strcpy(sidedata.name, "Team Lobby");
        sidedata.civilizationID = GetCivilizations()->first()->data()->GetObjectID();
        sidedata.sideID = SIDE_TEAMLOBBY;
        sidedata.gasAttributes.Initialize();
        sidedata.ttbmDevelopmentTechs.ClearAll();
        sidedata.ttbmInitialTechs.ClearAll();
        sidedata.color = Color(1.0f, 1.0f, 1.0f);
        sidedata.conquest = 0;
        sidedata.territory = 0;
        sidedata.nKills = sidedata.nEjections = sidedata.nDeaths = sidedata.nBaseKills 
            = sidedata.nBaseCaptures = sidedata.nFlags = sidedata.nArtifacts = 0;
        sidedata.squadID = NA;

        m_sideTeamLobby = (IsideIGC*)CreateObject(Time::Now(), OT_side, &sidedata, sizeof(sidedata));
        m_sideTeamLobby->SetActiveF(true);
        DeleteSide(m_sideTeamLobby); // make sure it does not appear in the normal side list
        ZAssert(m_sideTeamLobby != NULL);
    }

    AddIbaseIGC((BaseListIGC*)&m_sides, s);
}
void                        CmissionIGC::DeleteSide(IsideIGC* s)
{
    DeleteIbaseIGC((BaseListIGC*)&m_sides, s);
}
IsideIGC*                   CmissionIGC::GetSide(SideID id) const
{
    if (id == SIDE_TEAMLOBBY)
        return m_sideTeamLobby;
    else
        return (IsideIGC*)GetIbaseIGC((BaseListIGC*)&m_sides, id);
}
const SideListIGC*          CmissionIGC::GetSides(void) const
{
    return &m_sides;
}

void                        CmissionIGC::AddCluster(IclusterIGC*    c)
{
    AddIbaseIGC((BaseListIGC*)&m_clusters, c);
}
void                        CmissionIGC::DeleteCluster(IclusterIGC* c)
{
    DeleteIbaseIGC((BaseListIGC*)&m_clusters, c);
}
IclusterIGC*                CmissionIGC::GetCluster(SectorID id) const
{
    return (IclusterIGC*)GetIbaseIGC((BaseListIGC*)&m_clusters, id);
}
const ClusterListIGC*       CmissionIGC::GetClusters(void) const
{
    return &m_clusters;
}

void                        CmissionIGC::AddShip(IshipIGC*    s)
{
    AddIbaseIGC((BaseListIGC*)&m_ships, s);
}
void                        CmissionIGC::DeleteShip(IshipIGC* s)
{
    DeleteIbaseIGC((BaseListIGC*)&m_ships, s);
}
IshipIGC*                   CmissionIGC::GetShip(ShipID   id) const
{
    return (IshipIGC*)GetIbaseIGC((BaseListIGC*)&m_ships, id);
}
const ShipListIGC*          CmissionIGC::GetShips(void) const
{
    return &m_ships;
}

void                        CmissionIGC::AddBuoy(IbuoyIGC*    s)
{
    AddIbaseIGC((BaseListIGC*)&m_buoys, s);
}
void                        CmissionIGC::DeleteBuoy(IbuoyIGC* s)
{
    DeleteIbaseIGC((BaseListIGC*)&m_buoys, s);
}
IbuoyIGC*                   CmissionIGC::GetBuoy(BuoyID   id) const
{
    return (IbuoyIGC*)GetIbaseIGC((BaseListIGC*)&m_buoys, id);
}
const BuoyListIGC*          CmissionIGC::GetBuoys(void) const
{
    return &m_buoys;
}

void                        CmissionIGC::UpdateSides(Time now,
                                                     const MissionParams * pmp,
                                                     const char sideNames[c_cSidesMax][c_cbSideName])
{
    static const float sideColors[c_cSidesMax][3] =
                                { {188.0f/255.0f, 160.0f/255.0f,   0.0f/255.0f}, //Gold
                                  {  0.0f/255.0f, 138.0f/255.0f, 217.0f/255.0f}, //Blue
                                  {156.0f/255.0f,  16.0f/255.0f, 102.0f/255.0f}, //Purple
                                  { 50.0f/255.0f, 140.0f/255.0f,  20.0f/255.0f}, //icky yellow
                                  {255.0f/255.0f, 145.0f/255.0f, 145.0f/255.0f}, //icky orange
                                  { 50.0f/255.0f, 200.0f/255.0f, 125.0f/255.0f}};//icky magenta

    for (SideID sid = GetSides()->n(); sid < pmp->nTeams; sid++)
    {
        IcivilizationIGC*   pcivilization = GetCivilization(pmp->rgCivID[sid]);
        assert (pcivilization);

        DataSideIGC ds;
        ds.sideID = sid;
        ds.gasAttributes.Initialize();
        ds.civilizationID = pmp->rgCivID[sid];
        ds.color.SetRGBA(sideColors[sid][0],
                         sideColors[sid][1],
                         sideColors[sid][2]);
        memset(ds.name, 0x00, sizeof(ds.name));
        strcpy(ds.name, sideNames[sid]);
        ds.ttbmDevelopmentTechs.ClearAll();
        ds.ttbmInitialTechs.ClearAll();
        ds.conquest = 0;
        ds.territory = 0;
        ds.nKills = ds.nEjections = ds.nDeaths = ds.nBaseKills = ds.nBaseCaptures = ds.nFlags 
            = ds.nArtifacts = 0;
        ds.squadID = NA;

        IObject*    o = CreateObject(now,
                                     OT_side,
                                     &ds,
                                     sizeof(ds));
        assert (o);
        ((IsideIGC*)o)->SetActiveF(true);

        o->Release();
    }

    // if it looks like we need to delete a few sides to match the mission parameters, do so.
    if (sid > pmp->nTeams)
    {
        for (SideID sid = GetSides()->n() - 1; sid >= pmp->nTeams; sid--)
        {
            IsideIGC* pside = GetSide(sid);
            assert(pside->GetShips()->n() == 0); // side should be empty
            pside->Terminate();
        }
    }

    assert (GetSides()->n() == pmp->nTeams);
}

void                    CmissionIGC::ResetMission()
{
    {
        // move all of the ships to the null cluster
        for (ShipLinkIGC*  l = m_ships.first(); l != NULL; l = l->next())
        {
            l->data()->SetCluster(NULL);
            l->data()->SetFlag(NA);
        }
    }
    {
        // nuke all clusters
        const ClusterListIGC*   plistClusters = GetClusters();
        ClusterLinkIGC*         plinkCluster;
        while (plinkCluster = plistClusters->first())  //Not ==
            plinkCluster->data()->Terminate();
    }    

    // reset all of the side techs
    for (SideLinkIGC * plinkSides = GetSides()->first(); 
        (plinkSides != NULL); plinkSides = plinkSides->next())
    {
        plinkSides->data()->DestroyBuckets();
    }
}

void                    CmissionIGC::GenerateMission(Time                   now,
                                                     const MissionParams *  pmp,
                                                     TechTreeBitMask *      pttbmShouldOverride,
                                                     TechTreeBitMask *      pttbmOverrideValue)
{
    m_nextBuoyID = 0;
    m_nextShipID = 0;
    m_nextAsteroidID = 0;
    m_nextTreasureID = 0;
    m_nextMineID = 0;
    m_nextProbeID = 0;
    m_nextMissileID = 0;
    m_nextStationID = 0;

    m_missionParams = *pmp;

    // Increment replay count, if we have been generated once before
    if (m_bHasGenerated)
    {
      ++m_nReplayCount;

      // Clear the context name, so that it re-generates
      m_strContextName.SetEmpty();
    }
    else
    {
      m_bHasGenerated = true;
    }


    //Create buckets for all of the sides in the game
    {
        unsigned char   pctConquest = (unsigned char)(100 / m_sides.n());
        for (SideLinkIGC*   psl = m_sides.first(); (psl != NULL); psl = psl->next())
        {
            IsideIGC*           pside = psl->data();
            IcivilizationIGC*   pciv = pside->GetCivilization();

            TechTreeBitMask ttbmDefaultTechs;
            if (pmp->bAllowDevelopments)
            {
                ttbmDefaultTechs = pciv->GetBaseTechs();

                if (pmp->bAllowShipyardPath)
                    ttbmDefaultTechs.SetBit(c_ttbShipyardAllowed);
                if (pmp->bAllowExpansionPath)
                    ttbmDefaultTechs.SetBit(c_ttbExpansionAllowed);
                if (pmp->bAllowTacticalPath)
                    ttbmDefaultTechs.SetBit(c_ttbTacticalAllowed);
                if (pmp->bAllowSupremacyPath)
                    ttbmDefaultTechs.SetBit(c_ttbSupremacyAllowed);
            }
            else
                ttbmDefaultTechs = pciv->GetNoDevTechs();
            
            if (pttbmShouldOverride && pttbmOverrideValue)
            {
              int nID = pside->GetObjectID();
              pside->SetDevelopmentTechs((pttbmShouldOverride[nID] & pttbmOverrideValue[nID]) | 
                                          (ttbmDefaultTechs - pttbmShouldOverride[nID]));
            }
            else
              pside->SetDevelopmentTechs(ttbmDefaultTechs);

            pside->UpdateInitialTechs();

            pside->SetConquestPercent(pctConquest);

            psl->data()->CreateBuckets();
        }
    }

    // check for custom map, and make sure it exists.
    if (pmp->szCustomMapFile[0] == '\0' || !LoadIGCFile(pmp->szCustomMapFile, this))
    {
        ImapMakerIGC::Create(now, pmp, this);
    }
    else
    {
        //
        // Make a mission with a custom map
        //

		// KGJV - randomize alephs positions
		// ---------------------------------
		// code from CmapMakerIGC::GenerateMission
		// adapted for predefined alephs.
		{
			//Adjust the position of all alephs in all clusters
			float   majorRadius = GetFloatConstant(c_fcidRadiusUniverse);
			for (ClusterLinkIGC*    pcl = m_clusters.first(); (pcl != NULL); pcl = pcl->next())
			{
				IclusterIGC*    pcluster = pcl->data();
				const WarpListIGC*  pwarps = pcluster->GetWarps();
				if (pwarps->n() != 0)
				{
					float   nWarps = (float)(pwarps->n());
					const int c_maxWarps = 10;
					assert (pwarps->n() <= c_maxWarps);
					float   offset[c_maxWarps] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
					float   displacement;
					if (pwarps->n() > 1)
					{
						displacement =  sin(pi/nWarps) * 2.0f / 3.0f;
						for (int index = 0; (index < pwarps->n()); index++)
						{
							int swap = randomInt(0, pwarps->n() - 1);
							if (swap != index)
							{
								float   t = offset[index];
								offset[index] = offset[swap];
								offset[swap] = t;
							}
						}
					}
					else
						displacement = 2.0f / 3.0f;
					{
						float   bias = random(0.0f, 2.0f * pi);
						int     index = 0;

						for (WarpLinkIGC*   pwl = pwarps->first(); (pwl != NULL); pwl = pwl->next())
						{
							IwarpIGC*   pwarp = pwl->data();
							//float       r = pwarp->GetPosition().z * majorRadius;
							float       r = 0.6f * majorRadius;
							float       angle = bias + offset[index++] * (2.0f * pi) / nWarps;
							Vector      position;
							position = Vector::RandomPosition(r * displacement);
							position.x  += cos(angle) * r;
							position.y  += sin(angle) * r;
							position.z  *= 1.2f;
							pwarp->SetPosition(position);
							Orientation o(-position);
							pwarp->SetOrientation(o);
							// fix rotating alephs
							Rotation rot(0,0,1,0);
							pwarp->SetRotation(rot);
							pwarp->GetHitTest()->UpdateStaticBB();
						}
					}
				}
			}
		}
		//---------------------------------------
        float   amountHe3;
        {
            float   nHe3 = 0.0f;
            for (ClusterLinkIGC* pcl = m_clusters.first(); (pcl != NULL); pcl = pcl->next())
            {
                IclusterIGC*            pcluster = pcl->data();
                const AsteroidListIGC*  pasteroids = pcluster->GetAsteroids();
                if (pasteroids->n() == 0)
                {
                    nHe3 += float(pcluster->GetHomeSector()
                                  ? pmp->nPlayerSectorMineableAsteroids
                                  : pmp->nNeutralSectorMineableAsteroids);
                }
                else
                {
                    for (AsteroidLinkIGC*   pal = pasteroids->first(); (pal != NULL); pal = pal->next())
                    {
                        if (pal->data()->GetCapabilities() & c_aabmMineHe3)
                            nHe3 += 1.0f;
                    }
                }
            }

            if (nHe3 == 0.0f)
                amountHe3 = 0.0f;
            else
            {
                amountHe3 = GetFloatConstant(c_fcidAmountHe3) *
                            pmp->fHe3Density *
                            float(m_sides.n()) / nHe3;
            }
        }

        const StationTypeListIGC*   pstationtypes = GetStationTypes();
        for (ClusterLinkIGC* pcl = m_clusters.first(); (pcl != NULL); pcl = pcl->next())
        {
            IclusterIGC*    pcluster = pcl->data();

            {
                //Replace stations with ones they actually can get
                //Walk the list backwards because changing a station moves it to the end of the list.
                StationLinkIGC*   psl = pcluster->GetStations()->last();
                while (psl != NULL)
                {
                    IstationIGC*            pstation = psl->data();
                    psl = psl->txen();

                    IsideIGC*               pside = pstation->GetSide();
                    IstationTypeIGC*        pst = pstation->GetBaseStationType();

                    TechTreeBitMask  ttbm = pside->GetDevelopmentTechs() |
                                            (pst->GetLocalTechs() | pst->GetEffectTechs());
                    if (!(pst->GetRequiredTechs() <= ttbm))
                    {
                        StationClassID classID = pst->GetClassID();

                        for (StationTypeLinkIGC*    pstl = pstationtypes->first(); (pstl != NULL); pstl = pstl->next())
                        {
                            IstationTypeIGC*    pstPossible = pstl->data();
                            if ((pstPossible->GetClassID() == classID) &&
                                (pstPossible->GetRequiredTechs() <= ttbm))
                            {
                                pstation->SetBaseStationType(pstPossible);
                                break;
                            }
                        }
                    }
                }
            }

            PopulateCluster(this, pmp, pcluster,
                            amountHe3,
                            (pcluster->GetAsteroids()->n() == 0),
                            (pcluster->GetTreasures()->n() == 0));

            pcluster->SetActive(true);
        }

        if (pmp->bShowMap)
        {
            for (WarpLinkIGC * pwl = m_warps.first();
                (pwl != NULL);
                pwl = pwl->next())
            {
                IwarpIGC*   pwarp = pwl->data();

                for (SideLinkIGC * psl = m_sides.first();
                    (psl != NULL);
                    psl = psl->next())
                {
                    pwarp->SetSideVisibility(psl->data(), true);
                }
            }
        }
    }

    //Go through and set the territory amounts for each side
    if (pmp->IsTerritoryGame())
    {
        assert (c_cSidesMax == 6);
        unsigned char nTerritoriesPerSide[c_cSidesMax] = {0, 0, 0, 0, 0, 0};

        for (ClusterLinkIGC*    pcl = m_clusters.first(); (pcl != NULL); pcl = pcl->next())
        {
            IclusterIGC*    pcluster = pcl->data();

            IsideIGC*       psideOwner = NULL;
            StationLinkIGC*    psl;
            for (psl = pcluster->GetStations()->first(); (psl != NULL); psl = psl->next())
            {
                IsideIGC*   pside = psl->data()->GetSide();
                if (psideOwner == NULL)
                    psideOwner = pside;
                else if (psideOwner != pside)
                    break;
            }

            if (psideOwner && (psl == NULL))
                nTerritoriesPerSide[psideOwner->GetObjectID()]++;
        }

        for (SideLinkIGC*   psl = m_sides.first(); (psl != NULL); psl = psl->next())
            psl->data()->SetTerritoryCount(nTerritoriesPerSide[psl->data()->GetObjectID()]);
    }
}

void    CmissionIGC::GenerateTreasure(Time         now,
                                      IclusterIGC* pCluster,
                                      short        tsi)
{
    DataTreasureIGC dt;
    if (tsi >= 0)
    {
        ItreasureSetIGC*    pts = GetTreasureSet(tsi);
        if (pts)
        {
            // Pick a random part ID given the treasure distribution
            // specified by the treasure chance.
            const TreasureData& td = pts->GetRandomTreasureData();
            dt.treasureCode = td.treasureCode;
            dt.treasureID = td.treasureID;
        }
        else
            return;
    }
    else if (tsi == NA)
        return;
    else
    {
        dt.treasureCode = c_tcFlag;
        dt.treasureID = SIDE_TEAMLOBBY;
    }

    if ((dt.treasureCode != c_tcCash) || (m_missionParams.bAllowDevelopments))
    {
        {
            int                 nTriesLeft = (dt.treasureCode != c_tcFlag) ? 3 : 0x7fffffff;    //Tries lots & lots for flags  
            const ModelListIGC* models = pCluster->GetModels();

            float   radius = c_radiusUniverse;

            ModelLinkIGC*   pmlink;
            do
            {
                //Pick a random position for the asteroid
                //in a squashed disk
                dt.p0 = Vector::RandomDirection();

                //Start 0.1 ... 1 the universe radius
                dt.p0 *= radius * pow(random(0.001f, 1.0f), 1.0f/3.0f);
                dt.p0.z *= c_zLensMultiplier;

                //Verify that it is not close to any other model in the cluster
                for (pmlink = models->first(); (pmlink != NULL); pmlink = pmlink->next())
                {
                    ImodelIGC*  pm = pmlink->data();
                    float       r = (pm->GetObjectType() == OT_ship         //Never create a treasure starting within 500*sqrt(10) of a player
                                     ? 500.0f
                                     : 25.0f) + pm->GetRadius();

                    if ((pm->GetPosition() - dt.p0).LengthSquared() < 10.0f * r * r)  //Lots of extra space (sqrt(10))
                    {
                        //Too close ... try again
                        radius *= 1.05f;
                        break;
                    }
                }
            }
            while (pmlink && (--nTriesLeft > 0));

            if (nTriesLeft == 0)
                return;     //Couldn't find a spot for a treasure
        }

        dt.amount = NA;

        dt.clusterID = pCluster->GetObjectID();
        dt.lifespan = (dt.treasureCode != c_tcFlag) ? random(300.0f, 600.0f) : 10.0f * 24.0f * 3600.0f;
        dt.time0 = now;

        dt.objectID = GenerateNewTreasureID();

        dt.v0 = Vector::GetZero();

        IObject*    o = CreateObject(now, OT_treasure,
                                     &dt, sizeof(dt));
        assert (o);
        o->Release();
    }
}


short                   CmissionIGC::GetReplayCount(void) const
{
   return m_nReplayCount;
}


const char*             CmissionIGC::GetContextName(void)
{
  // Return if it has already been generated
  if (!m_strContextName.IsEmpty())
     return m_strContextName;

  // Get the mission creation time
  SYSTEMTIME st;
  ZSucceeded(::FileTimeToSystemTime(&m_ftCreated, &st));

  // Format the context string
  char szContext[24];
  sprintf(szContext, "%c%02d%02d%02d%02d%02d%04d%02d",
      (GetMissionParams()->bObjectModelCreated && GetMissionParams()->bClubGame) ? 'Z' : 'U',
      st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, GetMissionID() % 10000, GetReplayCount() % 100);

  // Save the context string for next time
  m_strContextName = szContext;

  // Return the string (becomes invalid when m_strContextName changes)
  return m_strContextName;
}

