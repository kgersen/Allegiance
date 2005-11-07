/*-------------------------------------------------------------------------
 * Queries.h
 * 
 * All the queries used by fedsrv
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#ifndef _QUERIES_H_
#define _QUERIES_H_

#if !defined(ALLSRV_STANDALONE)
// for now just to be sure
//#define CbTechBits 32

/*
Use the following macros to define the statement parameters, 
  where INOUT is one of then enum SQLPARM
  and the number is the size of type.

SQL_STR_PARM( NAME, LENGTH, INOUT) // char*. LENGTH=field size. do not include string null termination character
SQL_INT4_PARM(NAME,         INOUT) // int
SQL_INT2_PARM(NAME,         INOUT) // short
SQL_INT1_PARM(NAME,         INOUT) // char
SQL_FLT4_PARM(NAME,         INOUT) // float
SQL_FLT8_PARM(NAME,         INOUT) // double

The NAME part of the parameter macros end up as global variables, so name them uniquely.
That is where you set and get the parameters
*/

BEGIN_SQL_DEF(&g.siteFedSrv)
  DEF_STMT(ShipTypeIDs, "SELECT ShipTypeID FROM ShipTypes ORDER BY SortOrder")
    SQL_INT2_PARM(ShipTypeIDs_ShipTypeID,           SQL_OUT_PARM)

  DEF_STMT(ShipTypes, "SELECT " FEDSTR(Description) ", " FEDSTR(Name) ", " FEDSTR(Model) ", " FEDSTR(PRIcon) 
          ", Length, "
          FEDNUM(SuccessorHullID) ", "
          "Weight, Price, SecondsToBuild, GroupID, BaseMaxSpeed, BaseHitPoints, DefenseType, BaseScannerRange, "
          "MaxRollRate, MaxPitchRate, MaxYawRate, DriftRoll, "
          "DriftPitch, DriftYaw, Acceleration, AccelSideMultiplier, "
          "AccelBackMultiplier, "
          "ShieldPartMask, AfterburnerPartMask, CloakPartMask, MagazinePartMask, DispenserPartMask, ChaffPartMask,"
          "MagazineCapacity, DispenserCapacity, ChaffCapacity, "
          FEDNUM(PreferredPartID1) ", " FEDNUM(PreferredPartID2) ", " FEDNUM(PreferredPartID3) ", " 
          FEDNUM(PreferredPartID4) ", " FEDNUM(PreferredPartID5) ", " FEDNUM(PreferredPartID6) ", " 
          FEDNUM(PreferredPartID7) ", " FEDNUM(PreferredPartID8) ", " FEDNUM(PreferredPartID9) ", " 
          FEDNUM(PreferredPartID10) ", " FEDNUM(PreferredPartID11) ", " FEDNUM(PreferredPartID12) ", " 
          FEDNUM(PreferredPartID13) ", " FEDNUM(PreferredPartID14) ", "
          "EnergyMax, RateRechargeEnergy, "
          "BaseSignature, RTRIM(TechBitsRequired), RTRIM(TechBitsEffect), Capabilities, MaxAmmo, MaxFuel, "
          "InteriorSound, ExteriorSound, ThrustInteriorSound, ThrustExteriorSound, "
          "TurnInteriorSound, TurnExteriorSound, RipcordSpeed, ecm, RipcordCost "
          "FROM ShipTypes WHERE ShipTypeID=?  ORDER BY SortOrder")
    SQL_STR_PARM( ShipTypes_Description,    c_cbDescriptionDB, SQL_OUT_PARM)
    SQL_STR_PARM( ShipTypes_Name,           c_cbNameDB,        SQL_OUT_PARM)
    SQL_STR_PARM( ShipTypes_ShipModelName,  c_cbFileNameDB,    SQL_OUT_PARM)
    SQL_STR_PARM( ShipTypes_ShipIconName,   c_cbFileNameDB,    SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flLength,               SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_SuccessorID,            SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flWeight,               SQL_OUT_PARM)
    SQL_INT4_PARM(ShipTypes_Price,                  SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_TimeToBuild,            SQL_OUT_PARM)
    SQL_INT1_PARM(ShipTypes_GroupID,                SQL_OUT_PARM)
    SQL_INT4_PARM(ShipTypes_SpeedMax,               SQL_OUT_PARM)
    SQL_INT4_PARM(ShipTypes_HitPoints,              SQL_OUT_PARM)
    SQL_INT1_PARM(ShipTypes_DefenseType,            SQL_OUT_PARM)
    SQL_INT4_PARM(ShipTypes_RangeScanner,           SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flRateRoll,             SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flRatePitch,            SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flRateYaw,              SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flDriftRoll,            SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flDriftPitch,           SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flDriftYaw,             SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flAcceleration,         SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flAccelSideMult,        SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_flAccelBackMult,        SQL_OUT_PARM)

    SQL_INT2_PARM(ShipTypes_ShieldPartMask,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_AfterburnerPartMask,    SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_CloakPartMask,          SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_MagazinePartMask,       SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_DispenserPartMask,      SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_ChaffPartMask,          SQL_OUT_PARM)

    SQL_INT2_PARM(ShipTypes_MagazineCapacity,       SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_DispenserCapacity,      SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_ChaffCapacity,          SQL_OUT_PARM)

    SQL_INT2_PARM(ShipTypes_PreferredPart1,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart2,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart3,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart4,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart5,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart6,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart7,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart8,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart9,         SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart10,        SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart11,        SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart12,        SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart13,        SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_PreferredPart14,        SQL_OUT_PARM)

    SQL_FLT4_PARM(ShipTypes_EnergyMax,              SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_RateRechargeEnergy,     SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_BaseSignature,          SQL_OUT_PARM)
    SQL_STR_PARM( ShipTypes_TechBitsReqd,   CbTechBits, SQL_OUT_PARM)
    SQL_STR_PARM( ShipTypes_TechBitsEffect, CbTechBits, SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_Capabilities,           SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_MaxAmmo,                SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_MaxFuel,                SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_InteriorSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_ExteriorSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_ThrustInteriorSound,    SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_ThrustExteriorSound,    SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_TurnInteriorSound,      SQL_OUT_PARM)
    SQL_INT2_PARM(ShipTypes_TurnExteriorSound,      SQL_OUT_PARM)

    SQL_FLT4_PARM(ShipTypes_ripcordSpeed,           SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_ecm,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(ShipTypes_ripcordCost,            SQL_OUT_PARM)

    SQL_INT2_PARM(ShipTypes_ShipTypeID,             SQL_IN_PARM)
    
    DEF_STMT(MapsCount, "SELECT COUNT(*) FROM Maps")
    SQL_INT4_PARM(g_cMapsCount, SQL_OUT_PARM)

    DEF_STMT(MapIDs, "SELECT MapID, " FEDSTR(FileName) " FROM Maps")
    SQL_INT2_PARM(MapIDs_MapID,                     SQL_OUT_PARM)
    SQL_STR_PARM( MapIDs_FileName,  c_cbFileNameDB, SQL_OUT_PARM)

/*
struct  DataWarpIGC
{
    WarpDef             warpDef;
    char                name[c_cbName];
    Vector              position;
    Vector              forward;
    Rotation            rotation;
    float               signature;
    SectorID            clusterID;
};

struct  WarpDef
{
    WarpID              warpID;
    WarpID              destinationID;
    short               radius;
    char                textureName[c_cbFileName];
    char                iconName[c_cbFileName];
};
*/
    DEF_STMT(AlephsCount, "SELECT COUNT(*) FROM AlephInstances")
    SQL_INT4_PARM(g_cAlephs, SQL_OUT_PARM)

    DEF_STMT(Alephs, 
        "SELECT " 
        "AlephID, SectorID, TargetAlephID, "
        "LocationX, LocationY, LocationZ, "
        "RotationR, "
        "ForwardX, ForwardY, ForwardZ, "
        "Radius, Signature, "
        FEDSTR(Texture) ", " FEDSTR(PRIcon) " "
        "FROM AlephInstances "
        "WHERE MapID = ?"
        )
    SQL_INT4_PARM(AlephInst_ID,                         SQL_OUT_PARM)
    SQL_INT2_PARM(AlephInst_SectorID,                   SQL_OUT_PARM)
    SQL_INT4_PARM(AlephInst_TargetAlephID,              SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_xLocation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_yLocation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_zLocation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_rRotation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_xForward,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_yForward,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_zForward,                   SQL_OUT_PARM)
    SQL_INT2_PARM(AlephInst_Radius,                     SQL_OUT_PARM)
    SQL_FLT4_PARM(AlephInst_Signature,                  SQL_OUT_PARM)
    SQL_STR_PARM( AlephInst_Texture,    c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( AlephInst_PRIcon,     c_cbFileNameDB, SQL_OUT_PARM)
    SQL_INT2_PARM(AlephInst_MapID,                      SQL_IN_PARM)


/*
struct AsteroidDef
{
    float                   ore;
    AsteroidAbilityBitMask  aabmCapabilities;
    AsteroidID              asteroidID;
    HitPoints               hitpoints;
    short                   radius;
    char                    modelName[c_cbFileName];
    char                    textureName[c_cbFileName];
    char                    iconName[c_cbFileName];
};

struct  DataAsteroidIGC
{
    float               mass;
    float               signature;
    Vector              position;
    Vector              up;
    Vector              forward;
    Rotation            rotation;
    AsteroidDef         asteroidDef;
    SectorID            clusterID;
    char                name[c_cbName];
    BytePercentage      fraction;
};
*/

    DEF_STMT(Asteroids, "SELECT "
        "SectorID, AsteroidID, "
        "Ore, AbilitiesMask, HitPoints, "
        "Radius, Signature, "
        "LocationX, LocationY, LocationZ, "
        FEDNUM(ForwardX) ", " FEDNUM(ForwardY) ", " FEDNUM(ForwardZ) ", "
        FEDNUM(UpX) ", " FEDNUM(UpY) ", " FEDNUM(UpZ) ", "
        FEDNUM(RotationAx) ", " FEDNUM(RotationAy) ", " FEDNUM(RotationAz) ", " FEDNUM(RotationR) ", "
        FEDSTR(FileModel) ", " FEDSTR(FileTexture) ", " FEDSTR(FileIcon) ", "
        FEDSTR(Name) " "
        "FROM Asteroids "
        "WHERE MapID = ?"
        )
    SQL_INT2_PARM(Asteroids_ClusterID,                  SQL_OUT_PARM)
    SQL_INT2_PARM(Asteroids_AsteroidID,                 SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_Ore,                        SQL_OUT_PARM)
    SQL_INT2_PARM(Asteroids_Abilities,                  SQL_OUT_PARM)
    SQL_INT2_PARM(Asteroids_HitPoints,                  SQL_OUT_PARM)
    SQL_INT2_PARM(Asteroids_Radius,                     SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_Signature,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_x,                          SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_y,                          SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_z,                          SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_xForward,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_yForward,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_zForward,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_xUp,                        SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_yUp,                        SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_zUp,                        SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_xRotation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_yRotation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_zRotation,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Asteroids_rRotation,                  SQL_OUT_PARM)
    SQL_STR_PARM( Asteroids_Model,      c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( Asteroids_Texture,    c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( Asteroids_PRIcon,     c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( Asteroids_Name,       c_cbNameDB,     SQL_OUT_PARM)
    SQL_INT2_PARM(Asteroids_MapID,                      SQL_IN_PARM)

/*
    XXX current IGC contraints mean that only one planet and poster per sector is allowed,
    XXX and those are created as part of the sector, so this is not necessary. If we 
    XXX decide to re-enable this feature later on, we will need to modify IGC to accept
    XXX the additional posters. -BSW 7/19/99
    DEF_STMT(PosterInstance, "SELECT " FEDSTR(PosterName) ", " FEDSTR(Texture)
          ", PosterID, Radius, Longitude, Latitude, SectorID "
          "FROM PosterInstances")
    SQL_STR_PARM( PosterInst_Name,    c_cbNameDB,     SQL_OUT_PARM)
    SQL_STR_PARM( PosterInst_Texture, c_cbFileNameDB, SQL_OUT_PARM)
    SQL_INT4_PARM(PosterInst_PosterID,          SQL_OUT_PARM)
    SQL_INT4_PARM(PosterInst_Radius,            SQL_OUT_PARM)
    SQL_INT2_PARM(PosterInst_Longitude,         SQL_OUT_PARM)
    SQL_INT2_PARM(PosterInst_Latitude,          SQL_OUT_PARM)
    SQL_INT2_PARM(PosterInst_SectorID,          SQL_OUT_PARM)
*/
  
/*
struct  DataClusterIGC
{
    unsigned int        starSeed;
    Vector              lightDirection;
    D3DCOLOR            lightColor;
    float               screenX;
    float               screenY;
    SectorID            clusterID;
    short               nDebris;
    short               nStars;
    char                name[c_cbName];
    char                posterName[c_cbFileName];
    char                planetName[c_cbFileName];
    BytePercentage      planetSinLatitude;
    BytePercentage      planetLongitude;
    unsigned char       planetRadius;
    bool                activeF;
    bool                bHomeSector;
};
*/

    DEF_STMT(SectorInfo, "SELECT " 
        "SectorID, " FEDSTR(SectorName) ", "
        "LightX, LightY, LightZ, LightColor, "
        "StarsCount, DebrisCount, starSeed, "
        "ScreenX, ScreenY, "
        FEDSTR(PosterFile) ", " FEDSTR(PlanetFile) ", "
        "PlanetSinLatitude, PlanetLongitude, PlanetRadius, "
        "IsHomeSector "
        "FROM SectorInfo "
        "WHERE MapID = ?"
        )
    SQL_INT2_PARM(SectorInfo_SectorID,                          SQL_OUT_PARM)
    SQL_STR_PARM( SectorInfo_Name,              c_cbNameDB,     SQL_OUT_PARM)
    SQL_FLT4_PARM(SectorInfo_LightX,                            SQL_OUT_PARM)
    SQL_FLT4_PARM(SectorInfo_LightY,                            SQL_OUT_PARM)
    SQL_FLT4_PARM(SectorInfo_LightZ,                            SQL_OUT_PARM)
    SQL_INT4_PARM(SectorInfo_LightColor,                        SQL_OUT_PARM)
    SQL_INT2_PARM(SectorInfo_StarsCount,                        SQL_OUT_PARM)
    SQL_INT2_PARM(SectorInfo_DebrisCount,                       SQL_OUT_PARM)
    SQL_INT4_PARM(SectorInfo_starSeed,                          SQL_OUT_PARM)
    SQL_FLT4_PARM(SectorInfo_ScreenX,                           SQL_OUT_PARM)
    SQL_FLT4_PARM(SectorInfo_ScreenY,                           SQL_OUT_PARM)
    SQL_STR_PARM( SectorInfo_PosterName,        c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( SectorInfo_PlanetName,        c_cbFileNameDB, SQL_OUT_PARM)
    SQL_INT1_PARM(SectorInfo_planetSinLatitude,                 SQL_OUT_PARM)
    SQL_INT1_PARM(SectorInfo_planetLongitude,                   SQL_OUT_PARM)
    SQL_INT1_PARM(SectorInfo_planetRadius,                      SQL_OUT_PARM)
    SQL_INT1_PARM(SectorInfo_IsHomeSector,                      SQL_OUT_PARM)
    SQL_INT2_PARM(SectorInfo_MapID,                             SQL_IN_PARM)

/*
struct  DataStationIGC
{
    Vector              position;
    Vector              up;
    Vector              forward;
    Rotation            rotation;
    SectorID            clusterID;
    SideID              sideID;
    StationID           stationID;
    StationTypeID       stationTypeID;
    char                name[c_cbName];
};
*/
    DEF_STMT(StationInstance, "SELECT " 
        "StationID, " FEDSTR(Name) ", "
        "SideID, SectorID, "
        "LocationX, LocationY, LocationZ, "
        "RotationAx, RotationAy, RotationAz, RotationR, "
        "ForwardX, ForwardY, ForwardZ, "
        "UpX, UpY, UpZ, "
        "StationTypeID "
        "FROM StationInstances "
        "WHERE MapID = ?"
        )
    SQL_INT2_PARM(StationInstance_StationID,                   SQL_OUT_PARM)
    SQL_STR_PARM( StationInstance_Name,        c_cbNameDB,     SQL_OUT_PARM)
    SQL_INT1_PARM(StationInstance_SideID,                      SQL_OUT_PARM)
    SQL_INT2_PARM(StationInstance_SectorID,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_xLocation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_yLocation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_zLocation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_xRotation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_yRotation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_zRotation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_rRotation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_xForward,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_yForward,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_zForward,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_xUp,                         SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_yUp,                         SQL_OUT_PARM)
    SQL_FLT4_PARM(StationInstance_zUp,                         SQL_OUT_PARM)
    SQL_INT2_PARM(StationInstance_StationTypeID,               SQL_OUT_PARM)
    SQL_INT2_PARM(StationInstance_MapID,                       SQL_IN_PARM)

/*
struct  DataProbeBase
{
    Vector              p0;
    Time                time0;
    ProbeID             probeID;
    bool                exportF;
};

struct  DataProbeIGC : public DataProbeBase
{
    IprobeTypeIGC*      pprobetype;
    IsideIGC*           pside;
    IclusterIGC*        pcluster;
};
*/
    DEF_STMT(ProbeInstance, "SELECT " 
        "ProbeID, "
        "ProbeTypeID, "
        "SideID, SectorID, "
        "LocationX, LocationY, LocationZ "
        "FROM ProbeInstances "
        "WHERE MapID = ?"
        )
    SQL_INT2_PARM(ProbeInstance_ProbeID,                        SQL_OUT_PARM)
    SQL_INT2_PARM(ProbeInstance_ProbeTypeID,                    SQL_OUT_PARM)
    SQL_INT1_PARM(ProbeInstance_SideID,                         SQL_OUT_PARM)
    SQL_INT2_PARM(ProbeInstance_SectorID,                       SQL_OUT_PARM)
    SQL_FLT4_PARM(ProbeInstance_xLocation,                      SQL_OUT_PARM)
    SQL_FLT4_PARM(ProbeInstance_yLocation,                      SQL_OUT_PARM)
    SQL_FLT4_PARM(ProbeInstance_zLocation,                      SQL_OUT_PARM)
    SQL_INT2_PARM(ProbeInstance_MapID,                          SQL_IN_PARM)

/*
struct  DataMineBase
{
    Vector              p0;
    Time                time0;
    MineID              mineID;
    bool                exportF;
};

struct DataMineExport : public DataMineBase
{
    SectorID            clusterID;
    ExpendableTypeID    minetypeID;
    ShipID              launcherID;
    SideID              sideID;
    BytePercentage      fraction;
    bool                createNow;
};

*/
    DEF_STMT(MineInstance, "SELECT " 
        "MineID, "
        "MineTypeID, "
        "SideID, SectorID, "
        "LocationX, LocationY, LocationZ "
        "FROM MineInstances "
        "WHERE MapID = ?"
        )
    SQL_INT2_PARM(MineInstance_MineID,                          SQL_OUT_PARM)
    SQL_INT2_PARM(MineInstance_MineTypeID,                      SQL_OUT_PARM)
    SQL_INT1_PARM(MineInstance_SideID,                          SQL_OUT_PARM)
    SQL_INT2_PARM(MineInstance_SectorID,                        SQL_OUT_PARM)
    SQL_FLT4_PARM(MineInstance_xLocation,                       SQL_OUT_PARM)
    SQL_FLT4_PARM(MineInstance_yLocation,                       SQL_OUT_PARM)
    SQL_FLT4_PARM(MineInstance_zLocation,                       SQL_OUT_PARM)
    SQL_INT2_PARM(MineInstance_MapID,                           SQL_IN_PARM)

/*
struct  DataTreasureIGC
{
    Vector              p0;
    Vector              v0;
    float               lifespan;
    Time                time0;
    ObjectID            objectID;
    TreasureID          treasureID;
    SectorID            clusterID;
    short               amount;
    TreasureCode        treasureCode;
    bool                createNow;
};
*/
    DEF_STMT(TreasureInstance, "SELECT " 
        "Lifespan, "
        "TreasureID, "
        "LocationX, LocationY, LocationZ, "
        "SectorID, "
        "Amount, TreasureCode "
        "FROM TreasureInstances "
        "WHERE MapID = ?"
        )
    SQL_FLT4_PARM(TreasureInstance_Lifespan,                    SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureInstance_TreasureID,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(TreasureInstance_xLocation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(TreasureInstance_yLocation,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(TreasureInstance_zLocation,                   SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureInstance_SectorID,                    SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureInstance_Amount,                      SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureInstance_TreasureCode,                SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureInstance_MapID,                       SQL_IN_PARM)

  // for stuff that we're going to cache, we need to know how many rows there are,
  // so we can preallocate each array 
  DEF_STMT(ShipsCount, "SELECT MAX(ShipTypeID) + 1 FROM ShipTypes")
    SQL_INT4_PARM(g_cShipTypes, SQL_OUT_PARM)

  DEF_STMT(PostersCount,      
          "SELECT COUNT(*) FROM PosterInstances")
    SQL_INT4_PARM(g_cPosters, SQL_OUT_PARM)

  DEF_STMT(AttachPointsCount, "SELECT COUNT(*) FROM AttachPoints WHERE ShipTypeID=?")
    SQL_INT4_PARM(g_cAttachPoints,            SQL_OUT_PARM)
    SQL_INT2_PARM(AttachPointsCount_ShipTypeID, SQL_IN_PARM)
  
  DEF_STMT(EffectsCount, "SELECT Count(*) FROM Effects")
    SQL_INT4_PARM(g_cEffects,                 SQL_OUT_PARM)

  DEF_STMT(AttachPoints, "SELECT ShipTypeID, AttachPointID, "
          FEDSTR(FrameName) ", PartMask, "
          "PartTypeID, InteriorSound, TurnSound, " FEDSTR(LocationAbreviation)
          "FROM AttachPoints WHERE ShipTypeID=? ORDER BY PartTypeID")
    SQL_INT2_PARM(AttachPoints_ShiptTypeID,           SQL_OUT_PARM)
    SQL_INT2_PARM(AttachPoints_AttachPointID,         SQL_OUT_PARM)
    SQL_STR_PARM( AttachPoints_FrameName,  c_cbFileNameDB, SQL_OUT_PARM)
    SQL_INT2_PARM(AttachPoints_PartMask,              SQL_OUT_PARM)
    SQL_INT1_PARM(AttachPoints_PartTypeID,            SQL_OUT_PARM)
    SQL_INT2_PARM(AttachPoints_InteriorSound,         SQL_OUT_PARM)
    SQL_INT2_PARM(AttachPoints_TurnSound,             SQL_OUT_PARM)
    SQL_INT2_PARM(AttachPoints_ShipTypeID,            SQL_IN_PARM)
    SQL_STR_PARM( AttachPoints_LocationAbreviation, c_cbLocAbrevDB, SQL_OUT_PARM)

  DEF_STMT(Projectiles, "SELECT ProjectileID, TimeDuration, "
          "HitPointsInflict, DamageType, " FEDSTR(FileModel) ", "
          FEDSTR(FileTexture) ", SpeedMax, Acceleration, "
          "Size_cm, RadiusBlast_m, percentRed, percentGreen, percentBlue, percentAlpha, "
          "PercentPenetration, RateRotation, IsAbsoluteSpeed, IsDirectional, IsExplodeOnExpire, "
          "IsProximityFuse, BlastPower, BlastRadius, WidthOverHeight, AmbientSound "
          "FROM Projectiles")
    SQL_INT2_PARM(Projectiles_ProjectileID,       SQL_OUT_PARM)
    SQL_INT2_PARM(Projectiles_TimeDuration,       SQL_OUT_PARM)
    SQL_FLT4_PARM(Projectiles_HitPointsInflict,   SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_DamageType,         SQL_OUT_PARM)
    SQL_STR_PARM( Projectiles_FileModel,   c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( Projectiles_FileTexture, c_cbFileNameDB, SQL_OUT_PARM)
    SQL_INT2_PARM(Projectiles_SpeedMax,           SQL_OUT_PARM)
    SQL_INT2_PARM(Projectiles_Acceleration,       SQL_OUT_PARM)
    SQL_INT2_PARM(Projectiles_Size_cm,            SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_RadiusBlast_m,      SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_Red,                SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_Green,              SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_Blue,               SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_Alpha,              SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_PercentPenetration, SQL_OUT_PARM) //NYI weapons no longer penetrate
    SQL_FLT4_PARM(Projectiles_RateRotation,       SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_IsAbsoluteSpeed,    SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_IsDirectional,      SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_IsExplodeOnExpire,  SQL_OUT_PARM)
    SQL_INT1_PARM(Projectiles_IsProximityFuse,    SQL_OUT_PARM)
    SQL_FLT4_PARM(Projectiles_BlastPower,         SQL_OUT_PARM)
    SQL_FLT4_PARM(Projectiles_BlastRadius,        SQL_OUT_PARM)
    SQL_FLT4_PARM(Projectiles_WidthOverHeight,    SQL_OUT_PARM)
    SQL_INT2_PARM(Projectiles_AmbientSound,       SQL_OUT_PARM)


// ****** Common SQL Stuff For Each Part ******
// All parts types need to get base part info. Make it common to prevent screw ups
#define SZPARTSFIELDS "P.PartID, " FEDNUM(P.SuccessorPartID) ", P.Price, P.SecondsToBuild, P.GroupID, P.PartMask, " FEDSTR(P.Name) ", " \
        FEDSTR(P.FileModel) ", " FEDSTR(P.PRIcon) ", " FEDSTR(P.FileTexture) ", " FEDSTR(P.Description) ", P.PartTypeID, P.Mass, " \
        "dSignatureActive, RTRIM(P.TechBitsRequired), RTRIM(P.TechBitsEffect), " FEDSTR(P.InventoryLineMDL)
#define PARTPARMS(PARTTYPE) \
    SQL_INT2_PARM(PARTTYPE##_PartID,                SQL_OUT_PARM) \
    SQL_INT2_PARM(PARTTYPE##_SuccessorID,           SQL_OUT_PARM) \
    SQL_INT4_PARM(PARTTYPE##_Price,                 SQL_OUT_PARM) \
    SQL_INT2_PARM(PARTTYPE##_TimeToBuild,           SQL_OUT_PARM) \
    SQL_INT1_PARM(PARTTYPE##_GroupID,               SQL_OUT_PARM) \
    SQL_INT2_PARM(PARTTYPE##_PartMask,              SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_Name,        c_cbNameDB,       SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_FileModel,   c_cbFileNameDB,   SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_FileIcon,   c_cbFileNameDB,   SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_FileTexture, c_cbFileNameDB,   SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_Description, c_cbDescriptionDB, SQL_OUT_PARM) \
    SQL_INT1_PARM(PARTTYPE##_PartTypeID,            SQL_OUT_PARM) \
    SQL_INT2_PARM(PARTTYPE##_Mass,                  SQL_OUT_PARM) \
    SQL_INT2_PARM(PARTTYPE##_Signature,             SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_TechBitsReqd,   CbTechBits, SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_TechBitsEffect, CbTechBits, SQL_OUT_PARM) \
    SQL_STR_PARM( PARTTYPE##_InventoryLineMDL, c_cbFileNameDB, SQL_OUT_PARM)


  DEF_STMT(Afterburners, "SELECT " SZPARTSFIELDS ", A.BurnRate, A.MaxThrust, "
          "A.rateOn, A.rateOff, "
          "A.InteriorSound, A.ExteriorSound "
          "FROM Afterburners AS A, Parts AS P WHERE P.PartID=A.PartID"
          " ORDER BY P.SortOrder")
    PARTPARMS(Afterburners)
    SQL_FLT4_PARM(Afterburners_RateBurn,          SQL_OUT_PARM)
    SQL_FLT4_PARM(Afterburners_ThrustMax,         SQL_OUT_PARM)
    SQL_FLT4_PARM(Afterburners_RateOn,            SQL_OUT_PARM)
    SQL_FLT4_PARM(Afterburners_RateOff,           SQL_OUT_PARM)
    SQL_INT2_PARM(Afterburners_InteriorSound,     SQL_OUT_PARM)
    SQL_INT2_PARM(Afterburners_ExteriorSound,     SQL_OUT_PARM)
    
  DEF_STMT(Ammo, "SELECT " SZPARTSFIELDS ", A.Qty, A.AmmoType "
          "FROM Ammo AS A, Parts AS P WHERE P.PartID=A.PartID"
          " ORDER BY P.SortOrder")
    PARTPARMS(Ammo)
    SQL_INT2_PARM(Ammo_Qty,                       SQL_OUT_PARM)
    SQL_INT1_PARM(Ammo_AmmoType,                  SQL_OUT_PARM)    
  
  DEF_STMT(Weapons, "SELECT " SZPARTSFIELDS ", W.dTimeBurstShots, W.dTimeReady, "
          "W.EnergyPerShot, W.ProjectileID1, "
          "W.Dispersion, "
          "W.cBulletsPerShot, "
          "W.ActivateSound, W.SingleShotSound, W.BurstShotSound "
          "FROM Weapons AS W, Parts AS P WHERE P.PartID=W.PartID"
          " ORDER BY P.SortOrder")
    PARTPARMS(Weapons)
    SQL_INT2_PARM(Weapons_dTimeBurstShots,        SQL_OUT_PARM)
    SQL_INT2_PARM(Weapons_dTimeReady,             SQL_OUT_PARM)
    SQL_FLT4_PARM(Weapons_EnergyPerShot,          SQL_OUT_PARM)
    SQL_INT2_PARM(Weapons_ProjectileID1,          SQL_OUT_PARM)
    SQL_FLT4_PARM(Weapons_Dispersion,             SQL_OUT_PARM)
    SQL_INT2_PARM(Weapons_cBulletsPerShot,        SQL_OUT_PARM)
    SQL_INT2_PARM(Weapons_ActivateSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(Weapons_SingleShotSound,        SQL_OUT_PARM)
    SQL_INT2_PARM(Weapons_BurstShotSound,         SQL_OUT_PARM)

  DEF_STMT(Shields, "SELECT " SZPARTSFIELDS ", S.RegenRate, "
          "S.Hitpoints, "
          "S.DefenseType, "
          "S.ActivateSound, S.DeactivateSound "
          "FROM Shields AS S, Parts AS P WHERE P.PartID=S.PartID"
          " ORDER BY P.SortOrder")
    PARTPARMS(Shields)
    SQL_FLT4_PARM(Shields_RegenRate,              SQL_OUT_PARM)
    SQL_INT2_PARM(Shields_MaxHitPoints,           SQL_OUT_PARM)
    SQL_INT1_PARM(Shields_DefenseType,            SQL_OUT_PARM)
    SQL_INT2_PARM(Shields_ActivateSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(Shields_DeactivateSound,        SQL_OUT_PARM)

  DEF_STMT(Magazines, "SELECT " SZPARTSFIELDS ", M.Amount, M.LaunchCount, M.ExpendableID "
          "FROM Magazines AS M, Parts AS P WHERE P.PartID=M.PartID"
          " ORDER BY P.SortOrder")
    PARTPARMS(Magazine)
    SQL_INT2_PARM(Magazine_Amount,                      SQL_OUT_PARM)
    SQL_INT2_PARM(Magazine_LaunchCount,                 SQL_OUT_PARM)
    SQL_INT2_PARM(Magazine_ExpendableID,                SQL_OUT_PARM)

  DEF_STMT(StationTypesCount, "SELECT COUNT(*) FROM StationTypes")
    SQL_INT2_PARM(g_cStationTypes, SQL_OUT_PARM)

  DEF_STMT(StationTypes, "SELECT StationTypeID, "
          FEDSTR(Name) ", " FEDSTR(Description) ", " FEDSTR(Model) ", " FEDSTR(PRIcon) ", " FEDSTR(BuilderName)
          ", RTRIM(TechBitsLocal), RTRIM(TechBitsRequired), "
          "RTRIM(TechBitsEffect), Radius, HitPointsArmor, DefenseTypeArmor, HitPointsShield, DefenseTypeShield, "
          "RateRegenArmor, RateRegenShield, Price, Income, Signature, SecondsToBuild, "
          FEDNUM(UpgradeStationTypeID) ", "
          "Capabilities, ScanRange,  AsteroidDiscountMask, "
          "BuildLocationSound, ConstructorAttackedSound, ConstructorDestroyedSound, CompletedSound, "
          "ExteriorSound, InteriorSound, InteriorAlertSound, UnderAttackSound, CriticalSound, "
          "DestroyedSound, CapturedSound, EnemyDestroyedSound, EnemyCapturedSound, "
          "GroupID, ClassID, ConstructionDroneTypeID "
          "FROM StationTypes ORDER BY StationTypeID")
    SQL_INT4_PARM(Stations_ID,                          SQL_OUT_PARM)
    SQL_STR_PARM( Stations_Name,      c_cbNameDB,       SQL_OUT_PARM)
    SQL_STR_PARM( Stations_Description, c_cbDescriptionDB, SQL_OUT_PARM)
    SQL_STR_PARM( Stations_Model,     c_cbFileNameDB,   SQL_OUT_PARM)
    SQL_STR_PARM( Stations_IconName,  c_cbFileNameDB,   SQL_OUT_PARM)
    SQL_STR_PARM( Stations_BuilderName,  c_cbNameDB,   SQL_OUT_PARM)
    SQL_STR_PARM( Stations_TBLocal,   CbTechBits,       SQL_OUT_PARM)
    SQL_STR_PARM( Stations_TBReqd,    CbTechBits,       SQL_OUT_PARM)
    SQL_STR_PARM( Stations_TBEffect,  CbTechBits,       SQL_OUT_PARM)
    SQL_FLT4_PARM(Stations_Radius,                      SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_HPArmor,                     SQL_OUT_PARM)
    SQL_INT1_PARM(Stations_DTArmor,                     SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_HPShield,                    SQL_OUT_PARM)
    SQL_INT1_PARM(Stations_DTShield,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(Stations_RateRegenArmor,              SQL_OUT_PARM)
    SQL_FLT4_PARM(Stations_RateRegenShield,             SQL_OUT_PARM)
    SQL_INT4_PARM(Stations_Price,                       SQL_OUT_PARM)
    SQL_INT4_PARM(Stations_Income,                      SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_Signature,                   SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_SecondsToBuild,              SQL_OUT_PARM)
    SQL_INT4_PARM(Stations_UpgradeStationTypeID,        SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_Capabilities,                SQL_OUT_PARM)
    SQL_FLT4_PARM(Stations_ScanRange,                   SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_AsteroidDiscountMask,        SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_BuildLocationSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_ConstructorAttackedSound,    SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_ConstructorDestroyedSound,   SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_CompletedSound,              SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_ExteriorSound,               SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_InteriorSound,               SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_InteriorAlertSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_UnderAttackSound,            SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_CriticalSound,               SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_DestroyedSound,              SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_CapturedSound,               SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_EnemyDestroyedSound,         SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_EnemyCapturedSound,          SQL_OUT_PARM)
    SQL_INT1_PARM(Stations_GroupID,                     SQL_OUT_PARM)
    SQL_INT1_PARM(Stations_ClassID,                     SQL_OUT_PARM)
    SQL_INT2_PARM(Stations_ConstructionDroneTypeID,     SQL_OUT_PARM)

  DEF_STMT(SectorCount, "SELECT Count(*) FROM SectorInfo")
    SQL_INT4_PARM(g_cSectors,                       SQL_OUT_PARM)

  DEF_STMT(Effects, "SELECT EffectID, Description, RTRIM(FileSound), IsLooping "
                    "FROM Effects")
    SQL_INT2_PARM(Effect_ID,                            SQL_OUT_PARM)
    SQL_STR_PARM (Effect_Description, c_cbDescriptionDB,SQL_OUT_PARM)
    SQL_STR_PARM (Effect_FileSound,   c_cbFileNameDB,   SQL_OUT_PARM)
    SQL_INT1_PARM(Effect_IsLooping,                     SQL_OUT_PARM)
                    
  DEF_STMT(Drones, "SELECT DroneID, " FEDSTR(Name) ", " FEDSTR(Description) ", Price, "
                   "ShipTypeID, " FEDNUM(ExpendableID) 
                   ", RTRIM(TechBitsRequired), RTRIM(TechBitsEffect), SecondsToBuild, DroneType FROM Drones"
                   " ORDER BY SortOrder")
    SQL_INT2_PARM(Drones_ID,                            SQL_OUT_PARM)
    SQL_STR_PARM( Drones_Name,        c_cbNameDB,       SQL_OUT_PARM)
    SQL_STR_PARM( Drones_Description, c_cbDescriptionDB,SQL_OUT_PARM)
    SQL_INT4_PARM(Drones_Price,                         SQL_OUT_PARM)
    SQL_INT2_PARM(Drones_ShipTypeID,                    SQL_OUT_PARM)
    SQL_INT2_PARM(Drones_ExpendableID,                  SQL_OUT_PARM)
    SQL_STR_PARM( Drones_TechBitsReqd,   CbTechBits,    SQL_OUT_PARM)
    SQL_STR_PARM( Drones_TechBitsEffect, CbTechBits,    SQL_OUT_PARM)
    SQL_INT2_PARM(Drones_SecToBuild,                    SQL_OUT_PARM)
    SQL_INT4_PARM(Drones_DroneType,                     SQL_OUT_PARM)
    SQL_INT1_PARM(Drones_GroupID,                       SQL_OUT_PARM)

  DEF_STMT(CivsCount, "SELECT MAX(CivID) + 1 FROM Civs")
    SQL_INT4_PARM(g_cCivs, SQL_OUT_PARM)

  DEF_STMT(Civs, "SELECT " FEDSTR(Name) ", " FEDSTR(Description) ", "
                 "EscapePodShipTypeID, RTRIM(TechBitsCiv), " FEDNUM(StartingGlobalAttributeID) 
                 ", InitialStationTypeID, RTRIM(IconName), RTRIM(StationHUD), RTRIM(TechBitsCivNoDev), BonusMoney, IncomeMoney "
                 "FROM Civs WHERE CivID=?")
    SQL_STR_PARM( Civs_Name,        c_cbNameDB,         SQL_OUT_PARM)
    SQL_STR_PARM( Civs_Description, c_cbDescriptionDB,  SQL_OUT_PARM)
    SQL_INT2_PARM(Civs_DefaultShipType,                 SQL_OUT_PARM)
    SQL_STR_PARM( Civs_TechBits,    800,         SQL_OUT_PARM)
    SQL_INT2_PARM(Civs_GlobalAttr,                      SQL_OUT_PARM)
    SQL_INT2_PARM(Civs_InitialStationTypeID,            SQL_OUT_PARM)
    SQL_STR_PARM( Civs_IconName,    c_cbFileNameDB,     SQL_OUT_PARM)
    SQL_STR_PARM( Civs_HUDName,     c_cbFileNameDB,     SQL_OUT_PARM)
    SQL_STR_PARM( Civs_TechBitsNoDev,    800,           SQL_OUT_PARM)
    SQL_FLT4_PARM(Civs_BonusMoney,                      SQL_OUT_PARM)
    SQL_FLT4_PARM(Civs_IncomeMoney,                     SQL_OUT_PARM)
    SQL_INT2_PARM(Civs_ID,                              SQL_IN_PARM)

  // Until we have IGC object, it's easiest to just get global attributes when we need them
  DEF_STMT(GetGlobalAttrs, "SELECT MaxSpeed, Thrust, "
          "TurnRate, TurnTorque, MaxArmorStation, ArmorRegenerationStation, MaxShieldStation, "
          "ShieldRegenerationStation, MaxArmorShip, MaxShieldShip, "
          "ShieldRegenerationShip, ScanRange, Signature, MaxEnergy, SpeedAmmo, "
          "LifespanEnergy, TurnRateMissile, MiningRate, MiningYield, "
          "RipcordTime, DamageGuns, DamageMissiles, DevelopmentCost, DevelopmentTime, MiningCapacity FROM GlobalAttributes "
          "WHERE GlobalAttributeID=?")
    SQL_INT2_PARM(GlobalAttrs_MaxSpeed,                     SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_Thrust,                       SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_TurnRate,                     SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_TurnTorque,                   SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MaxArmorStation,              SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_ArmorRegenerationStation,     SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MaxShieldStation,             SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_ShieldRegenerationStation,    SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MaxArmorShip,                 SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MaxShieldShip,                SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_ShieldRegenerationShip,       SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_ScanRange,                    SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_Signature,                    SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MaxEnergy,                    SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_SpeedAmmo,                    SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_LifespanEnergy,               SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_TurnRateMissile,              SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MiningRate,                   SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MiningYield,                  SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_RipcordTime,                  SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_DamageGuns,                   SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_DamageMissiles,               SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_DevelopmentCost,              SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_DevelopmentTime,              SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_MiningCapacity,               SQL_OUT_PARM)
    SQL_INT2_PARM(GlobalAttrs_ID,                           SQL_IN_PARM)

  DEF_STMT(DevelopmentsCount, "SELECT MAX(DevelopmentID) + 1 FROM Developments")
    SQL_INT4_PARM(g_cDevelopments, SQL_OUT_PARM)

  DEF_STMT(DevelopmentsIDs, "SELECT DevelopmentID FROM Developments ORDER BY SortOrder")
    SQL_INT2_PARM(DevelopmentIDs_DevelopmentID,         SQL_OUT_PARM)

  DEF_STMT(Developments, "SELECT Price, " FEDSTR(LoadoutBitmap) ", "
          FEDSTR(Name) ", " FEDSTR(PRIcon) ", "  FEDSTR(Description)
          ", RTRIM(TechBitsRequired), RTRIM(TechBitsEffect), "
          "SecondsToBuild, GroupID, " FEDNUM(GlobalAttributeID) ", CompletedSound "
          "FROM Developments WHERE DevelopmentID=?")
    SQL_INT4_PARM(Developments_Price,                   SQL_OUT_PARM)
    SQL_STR_PARM( Developments_Bitmap,          c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM( Developments_Name,            c_cbNameDB,     SQL_OUT_PARM)
    SQL_STR_PARM( Developments_IconName,        c_cbFileNameDB,     SQL_OUT_PARM)
    SQL_STR_PARM( Developments_Description,     c_cbDescription,SQL_OUT_PARM)
    SQL_STR_PARM( Developments_TechBitsReqd,    CbTechBits, SQL_OUT_PARM)
    SQL_STR_PARM( Developments_TechBitsEffect,  CbTechBits, SQL_OUT_PARM)
    SQL_INT2_PARM(Developments_SecondsToBuild,          SQL_OUT_PARM)
    SQL_INT1_PARM(Developments_GroupID,                 SQL_OUT_PARM)
    SQL_INT2_PARM(Developments_GlobalAttribute,         SQL_OUT_PARM)
    SQL_INT2_PARM(Developments_CompletedSound,          SQL_OUT_PARM)
    SQL_INT2_PARM(Developments_ID,                      SQL_IN_PARM)

  DEF_STMT(SectorForSide, "SELECT SectorID FROM SectorInfo WHERE "
          "BaseForSide=? + 1")
    SQL_INT2_PARM(SectorForSide_SectorID,               SQL_OUT_PARM)
    SQL_INT2_PARM(SectorForSide_SideID,                 SQL_IN_PARM)

  DEF_STMT(Cloaks, "SELECT " SZPARTSFIELDS ", C.EnergyConsumption, "
          "C.MaxCloaking, C.OnRate, C.OffRate, "
          "C.EngageSound, C.DisengageSound "
          "FROM Cloaks AS C, Parts AS P WHERE P.PartID=C.PartID"
          " ORDER BY P.SortOrder")
    PARTPARMS(Cloaks)
    SQL_FLT4_PARM(Cloaks_EnergyConsumption,             SQL_OUT_PARM)
    SQL_FLT4_PARM(Cloaks_MaxCloaking,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(Cloaks_OnRate,                        SQL_OUT_PARM)
    SQL_FLT4_PARM(Cloaks_OffRate,                       SQL_OUT_PARM)
    SQL_INT2_PARM(Cloaks_EngageSound,                   SQL_OUT_PARM)
    SQL_INT2_PARM(Cloaks_DisengageSound,                SQL_OUT_PARM)

#define SZEXPENDFIELDS "E.ExpendableID, E.Price, E.SecondsToBuild, E.GroupID, " FEDSTR(E.FileModel) ", " FEDSTR(E.PRIcon) ", " \
        FEDSTR(E.Name) ", " FEDSTR(E.Description) ", RTRIM(E.TechBitsRequired), "\
        "RTRIM(E.TechBitsEffect), E.SignatureShip, E.Mass, E.PartMask, E.ExpendableSize, E.Capabilities, " \
        "E.LoadTime, E.LifeSpan, E.SignatureInstance, E.HitPoints, E.DefenseType, " \
        FEDSTR(E.FileModelInstance) ", " FEDSTR(E.PRIconInstance) ", " FEDSTR(E.FileTextureInstance) \
        ", E.Radius, E.RateRotation, E.percentRed, E.percentGreen, E.percentBlue, " \
        "E.percentAlpha"
#define EXPENDPARMS(EXPENDTYPE) \
    SQL_INT2_PARM(EXPENDTYPE##_ExpendableID,            SQL_OUT_PARM) \
    SQL_INT4_PARM(EXPENDTYPE##_Price,                   SQL_OUT_PARM) \
    SQL_INT2_PARM(EXPENDTYPE##_TimeToBuild,             SQL_OUT_PARM) \
    SQL_INT1_PARM(EXPENDTYPE##_GroupID,                 SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_Model,         c_cbFileNameDB, SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_Icon,          c_cbFileNameDB, SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_Name,          c_cbNameDB,     SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_Description,   c_cbDescriptionDB, SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_TechBitsReqd,  CbTechBits,     SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_TechBitsEffect, CbTechBits,    SQL_OUT_PARM) \
    SQL_INT2_PARM(EXPENDTYPE##_SignatureShip,           SQL_OUT_PARM) \
    SQL_FLT4_PARM(EXPENDTYPE##_Mass,                    SQL_OUT_PARM) \
    SQL_INT2_PARM(EXPENDTYPE##_PartMask,                SQL_OUT_PARM) \
    SQL_INT2_PARM(EXPENDTYPE##_ExpendableSize,          SQL_OUT_PARM) \
    SQL_INT2_PARM(EXPENDTYPE##_Capabilities,            SQL_OUT_PARM) \
    SQL_FLT4_PARM(EXPENDTYPE##_LoadTime,                SQL_OUT_PARM) \
    SQL_FLT4_PARM(EXPENDTYPE##_LifeSpan,                SQL_OUT_PARM) \
    SQL_FLT4_PARM(EXPENDTYPE##_SignatureInstance,       SQL_OUT_PARM) \
    SQL_INT2_PARM(EXPENDTYPE##_HitPoints,               SQL_OUT_PARM) \
    SQL_INT1_PARM(EXPENDTYPE##_DefenseType,             SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_ModelInstance,   c_cbFileNameDB, SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_IconInstance,    c_cbFileNameDB, SQL_OUT_PARM) \
    SQL_STR_PARM( EXPENDTYPE##_TextureInstance, c_cbFileNameDB, SQL_OUT_PARM) \
    SQL_FLT4_PARM(EXPENDTYPE##_Radius,                  SQL_OUT_PARM) \
    SQL_FLT4_PARM(EXPENDTYPE##_Rotation,                SQL_OUT_PARM) \
    SQL_INT1_PARM(EXPENDTYPE##_PercentRed,              SQL_OUT_PARM) \
    SQL_INT1_PARM(EXPENDTYPE##_PercentGreen,            SQL_OUT_PARM) \
    SQL_INT1_PARM(EXPENDTYPE##_PercentBlue,             SQL_OUT_PARM) \
    SQL_INT1_PARM(EXPENDTYPE##_PercentAlpha,            SQL_OUT_PARM) 

  DEF_STMT(Missiles, "SELECT " SZEXPENDFIELDS
          ", M.MaxLock, M.ChaffResistance, M.Acceleration, M.TurnRate, M.InitialSpeed, M.LockTime, "
          "M.ReadyTime, M.Dispersion, M.LockAngle, M.Power, M.DamageType, "
          "M.BlastPower, M.BlastRadius, M.IsDirectional, M.WidthOverHeight, "
          "M.LaunchSound, M.FlightSound "
          "FROM Missiles AS M, Expendables AS E WHERE E.ExpendableID=M.ExpendableID")
    EXPENDPARMS(Missiles)
    SQL_FLT4_PARM(Missiles_MaxLock,                     SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_Resistance,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_Acceleration,                SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_TurnRate,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_InitialSpeed,                SQL_OUT_PARM)
    //SQL_FLT4_PARM(Missiles_ArmTime,                     SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_LockTime,                    SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_ReadyTime,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_Dispersion,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_LockAngle,                   SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_Power,                       SQL_OUT_PARM)
    SQL_INT1_PARM(Missiles_DamageType,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_BlastPower,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_BlastRadius,                 SQL_OUT_PARM)
    SQL_INT1_PARM(Missiles_IsDirectional,               SQL_OUT_PARM)
    SQL_FLT4_PARM(Missiles_WidthOverHeight,             SQL_OUT_PARM)
    SQL_INT2_PARM(Missiles_LaunchSound,                 SQL_OUT_PARM)
    SQL_INT2_PARM(Missiles_FlightSound,                 SQL_OUT_PARM)

  DEF_STMT(Chaff, "SELECT " SZEXPENDFIELDS
          ", C.Strength "
          "FROM Chaff AS C, Expendables AS E WHERE E.ExpendableID=C.ExpendableID")
    EXPENDPARMS(Chaff)
    SQL_FLT4_PARM(Chaff_Strength,                       SQL_OUT_PARM)

  DEF_STMT(Mines, "SELECT " SZEXPENDFIELDS ", M.PlacementRadius, M.MunitionCount, "
          "M.Power, M.DamageType "
          "FROM Mines AS M, Expendables AS E WHERE E.ExpendableID=M.ExpendableID")
    EXPENDPARMS(Mines)
    SQL_FLT4_PARM(Mines_PlacementRadius,                SQL_OUT_PARM)
    SQL_INT1_PARM(Mines_MunitionCount,                  SQL_OUT_PARM)
    SQL_FLT4_PARM(Mines_Power,                          SQL_OUT_PARM)
    SQL_INT1_PARM(Mines_DamageType,                     SQL_OUT_PARM)

  DEF_STMT(Probes, "SELECT " SZEXPENDFIELDS ", P.ScanRange, " FEDNUM(P.ProjectileID)
          ", P.dtimeBurst, P.accuracy, P.dispersion, P.Ammo, P.dtRipcord, P.AmbientSound "
          "FROM Probes AS P, Expendables AS E WHERE E.ExpendableID=P.ExpendableID")
    EXPENDPARMS(Probes)
    SQL_FLT4_PARM(Probes_ScanRange,                     SQL_OUT_PARM)
    SQL_INT2_PARM(Probes_ProjectileTypeID,              SQL_OUT_PARM) 
    SQL_INT2_PARM(Probes_dtimeBurst,                    SQL_OUT_PARM) 
    SQL_INT1_PARM(Probes_accuracy,                      SQL_OUT_PARM) 
    SQL_FLT4_PARM(Probes_dispersion,                    SQL_OUT_PARM) 
    SQL_INT2_PARM(Probes_ammo,                          SQL_OUT_PARM) 
    SQL_INT1_PARM(Probes_dtRipcord,                     SQL_OUT_PARM) 
    SQL_INT2_PARM(Probes_AmbientSound,                  SQL_OUT_PARM)

  DEF_STMT(MaxPartID, "SELECT MAX(PartID)+1 FROM Parts")
    SQL_INT2_PARM(MaxPartID_ID,                         SQL_OUT_PARM)
    
  DEF_STMT(TreasureSets, "SELECT TreasureSetName, TreasureSetID, IsZoneOnly "
          "FROM TreasureSets ORDER BY TreasureSetID DESC")
    SQL_STR_PARM( TreasureSets_Name, CbTechBitName,     SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureSets_ID,                      SQL_OUT_PARM)
    SQL_INT1_PARM(TreasureSets_IsZoneOnly,              SQL_OUT_PARM)
    
  DEF_STMT(TreasureChance, "SELECT TreasureCode, ObjectID, Chance, TreasureSetID FROM TreasureChances")
    SQL_INT1_PARM(TreasureChance_TreasureCode,          SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureChance_ObjectID,              SQL_OUT_PARM)
    SQL_INT1_PARM(TreasureChance_Chance,                SQL_OUT_PARM)
    SQL_INT2_PARM(TreasureChance_SetID,                 SQL_OUT_PARM)

  DEF_STMT(GetICQID, "Select " FEDNUM(ICQID) " From CharacterInfo Where CharacterName=?")
    SQL_INT4_PARM(GetICQID_ID,                          SQL_OUT_PARM)
    SQL_STR_PARM( GetICQID_CharName, c_cbNameDB,        SQL_IN_PARM)

  DEF_STMT(TechBitsAll, "SELECT BitID, RTRIM(BitShortName) From TechBits ORDER BY BitShortName")
    SQL_INT4_PARM(TechBitsAll_BitID,                    SQL_OUT_PARM)
    SQL_STR_PARM( TechBitsAll_Name, CbTechBitName,      SQL_OUT_PARM)

  DEF_STMT(GetRankCount, "Select Count(*) From Ranks") 
    SQL_INT2_PARM(GetRankCount_Count,                   SQL_OUT_PARM)  

  DEF_STMT(GetRanks, "SELECT Rank, " FEDNUM(CivID) ", Requirement, Name FROM Ranks ORDER BY CivID, Rank") 
    SQL_INT2_PARM(GetRanks_Rank,                    SQL_OUT_PARM)
    SQL_INT2_PARM(GetRanks_CivID,                   SQL_OUT_PARM)  
    SQL_INT4_PARM(GetRanks_Requirement,             SQL_OUT_PARM)  
    SQL_STR_PARM( GetRanks_Name,        c_cbNameDB, SQL_OUT_PARM)

  DEF_STMT(GetStaticMapCount, "Select Count(*) From Maps Where IsUserOption = 1") 
    SQL_INT2_PARM(GetStaticMapCount_Count,                   SQL_OUT_PARM)  

  DEF_STMT(GetStaticMaps, "SELECT NumTeams, " FEDSTR(FileName) ", " FEDSTR(MapName) 
        "FROM Maps Where IsUserOption = 1 ORDER BY MapID") 
    SQL_INT1_PARM(GetStaticMaps_NumTeams,                   SQL_OUT_PARM)
    SQL_STR_PARM(GetStaticMaps_FileName,    c_cbFileNameDB, SQL_OUT_PARM)
    SQL_STR_PARM(GetStaticMaps_MapName,     c_cbNameDB,     SQL_OUT_PARM)

END_SQL_DEF


#endif // !defined(ALLSRV_STANDALONE)
#endif // !_QUERIES_H_
