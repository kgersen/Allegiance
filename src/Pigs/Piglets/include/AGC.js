
// ***
// *** 
// *** This is a generated file. Please do not modify this file by hand or your changes will be lost.
// ***
// ***
//
// These values bridge COM Enums defined in IDL into JS compatible enums that can be used for intellisense.
// These values are also available at the global level, but I'm putting them into an Enums wrapper so that 
// intellisense from *IDL.d.ts can be used at design time, but at run time, this file is used to bridge to 
// the real values.


// Enums is the top level namespace. All Enums will go under this namespace. This will avoid collisions
// with global COM variables like "PigState" which has the same name as the enum. 

var Enums = function () { };



////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCObjectType Enum

Enums.AGCObjectType = function () { };

Enums.AGCObjectType.AGCObjectType_Invalid = -1;
Enums.AGCObjectType.AGC_ModelBegin = 0;
Enums.AGCObjectType.AGC_Ship = 0;
Enums.AGCObjectType.AGC_Station = 1;
Enums.AGCObjectType.AGC_Missile = 2;
Enums.AGCObjectType.AGC_Mine = 3;
Enums.AGCObjectType.AGC_Probe = 4;
Enums.AGCObjectType.AGC_Asteroid = 5;
Enums.AGCObjectType.AGC_Projectile = 6;
Enums.AGCObjectType.AGC_Warp = 7;
Enums.AGCObjectType.AGC_Treasure = 8;
Enums.AGCObjectType.AGC_Buoy = 9;
Enums.AGCObjectType.AGC_Chaff = 10;
Enums.AGCObjectType.AGC_BuildingEffect = 11;
Enums.AGCObjectType.AGC_ModelEnd = 11;
Enums.AGCObjectType.AGC_Side = 12;
Enums.AGCObjectType.AGC_Cluster = 13;
Enums.AGCObjectType.AGC_Bucket = 14;
Enums.AGCObjectType.AGC_PartBegin = 15;
Enums.AGCObjectType.AGC_Weapon = 15;
Enums.AGCObjectType.AGC_Shield = 16;
Enums.AGCObjectType.AGC_Cloak = 17;
Enums.AGCObjectType.AGC_Pack = 18;
Enums.AGCObjectType.AGC_Afterburner = 19;
Enums.AGCObjectType.AGC_LauncherBegin = 20;
Enums.AGCObjectType.AGC_Magazine = 20;
Enums.AGCObjectType.AGC_Dispenser = 21;
Enums.AGCObjectType.AGC_LauncherEnd = 21;
Enums.AGCObjectType.AGC_PartEnd = 21;
Enums.AGCObjectType.AGC_StaticBegin = 22;
Enums.AGCObjectType.AGC_ProjectileType = 22;
Enums.AGCObjectType.AGC_MissileType = 23;
Enums.AGCObjectType.AGC_MineType = 24;
Enums.AGCObjectType.AGC_ProbeType = 25;
Enums.AGCObjectType.AGC_ChaffType = 36;
Enums.AGCObjectType.AGC_Civilization = 27;
Enums.AGCObjectType.AGC_TreasureSet = 28;
Enums.AGCObjectType.AGC_BucketStart = 29;
Enums.AGCObjectType.AGC_HullType = 29;
Enums.AGCObjectType.AGC_PartType = 30;
Enums.AGCObjectType.AGC_StationType = 31;
Enums.AGCObjectType.AGC_Development = 32;
Enums.AGCObjectType.AGC_DroneType = 33;
Enums.AGCObjectType.AGC_BucketEnd = 33;
Enums.AGCObjectType.AGC_StaticEnd = 33;
Enums.AGCObjectType.AGC_Constants = 34;
Enums.AGCObjectType.AGC_AdminUser = 35;
Enums.AGCObjectType.AGCObjectType_Max = 36;
Enums.AGCObjectType.AGC_Any_Objects = 36;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCHullAbility Enum

Enums.AGCHullAbility = function () { };

Enums.AGCHullAbility.AGCHullAbility_Board = 0x01;
Enums.AGCHullAbility.AGCHullAbility_Rescue = 0x02;
Enums.AGCHullAbility.AGCHullAbility_Lifepod = 0x04;
Enums.AGCHullAbility.AGCHullAbility_NoPickup = 0x08;
Enums.AGCHullAbility.AGCHullAbility_NoEjection = 0x10;
Enums.AGCHullAbility.AGCHullAbility_NoRipcord = 0x20;
Enums.AGCHullAbility.AGCHullAbility_CheatToDock = 0x40;
Enums.AGCHullAbility.AGCHullAbility_Fighter = 0x80;
Enums.AGCHullAbility.AGCHullAbility_Captital = 0x100;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCAsteroidAbility Enum

Enums.AGCAsteroidAbility = function () { };

Enums.AGCAsteroidAbility.AGCAsteroidAbility_MineHe3 = 0x01;
Enums.AGCAsteroidAbility.AGCAsteroidAbility_MineIce = 0x02;
Enums.AGCAsteroidAbility.AGCAsteroidAbility_MineGold = 0x04;
Enums.AGCAsteroidAbility.AGCAsteroidAbility_Buildable = 0x08;
Enums.AGCAsteroidAbility.AGCAsteroidAbility_Special = 0x10;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCAxis Enum

Enums.AGCAxis = function () { };

Enums.AGCAxis.AGCAxis_Yaw = 0;
Enums.AGCAxis.AGCAxis_Pitch = 1;
Enums.AGCAxis.AGCAxis_Roll = 2;
Enums.AGCAxis.AGCAxis_Throttle = 3;
Enums.AGCAxis.AGCAxis_Max = 4;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCEquipmentType Enum

Enums.AGCEquipmentType = function () { };

Enums.AGCEquipmentType.AGCEquipmentType_ChaffLauncher = 0;
Enums.AGCEquipmentType.AGCEquipmentType_Weapon = 1;
Enums.AGCEquipmentType.AGCEquipmentType_Magazine = 2;
Enums.AGCEquipmentType.AGCEquipmentType_Dispenser = 3;
Enums.AGCEquipmentType.AGCEquipmentType_Shield = 4;
Enums.AGCEquipmentType.AGCEquipmentType_Cloak = 5;
Enums.AGCEquipmentType.AGCEquipmentType_Pack = 6;
Enums.AGCEquipmentType.AGCEquipmentType_Afterburner = 7;
Enums.AGCEquipmentType.AGCEquipmentType_MAX = 8;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCTreasureType Enum

Enums.AGCTreasureType = function () { };

Enums.AGCTreasureType.AGCTreasureType_Part = 0;
Enums.AGCTreasureType.AGCTreasureType_Development = 1;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCChatTarget Enum

Enums.AGCChatTarget = function () { };

Enums.AGCChatTarget.AGCChat_Everyone = 0;
Enums.AGCChatTarget.AGCChat_Leaders = 1;
Enums.AGCChatTarget.AGCChat_Admin = 2;
Enums.AGCChatTarget.AGCChat_Ship = 3;
Enums.AGCChatTarget.AGCChat_Allies = 4;
Enums.AGCChatTarget.AGCChat_Team = 5;
Enums.AGCChatTarget.AGCChat_Individual = 6;
Enums.AGCChatTarget.AGCChat_Individual_NoFilter = 7;
Enums.AGCChatTarget.AGCChat_Wing = 8;
Enums.AGCChatTarget.AGCChat_Echo = 9;
Enums.AGCChatTarget.AGCChat_AllSector = 10;
Enums.AGCChatTarget.AGCChat_FriendlySector = 11;
Enums.AGCChatTarget.AGCChat_Group = 12;
Enums.AGCChatTarget.AGCChat_GroupNoEcho = 13;
Enums.AGCChatTarget.AGCChat_NoSelection = 14;
Enums.AGCChatTarget.AGCChat_Max = 15;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCGameStage Enum

Enums.AGCGameStage = function () { };

Enums.AGCGameStage.AGCGameStage_NotStarted = 0;
Enums.AGCGameStage.AGCGameStage_Starting = 1;
Enums.AGCGameStage.AGCGameStage_Started = 2;
Enums.AGCGameStage.AGCGameStage_Over = 3;
Enums.AGCGameStage.AGCGameStage_Terminate = 4;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FileAttributes Enum

Enums.FileAttributes = function () { };

Enums.FileAttributes.FileAttribute_None = 0x00000000;
Enums.FileAttributes.FileAttribute_ReadOnly = 0x00000001;
Enums.FileAttributes.FileAttribute_Hidden = 0x00000002;
Enums.FileAttributes.FileAttribute_System = 0x00000004;
Enums.FileAttributes.FileAttribute_Directory = 0x00000010;
Enums.FileAttributes.FileAttribute_Archive = 0x00000020;
Enums.FileAttributes.FileAttribute_Encrypted = 0x00000040;
Enums.FileAttributes.FileAttribute_Normal = 0x00000080;
Enums.FileAttributes.FileAttribute_Temporary = 0x00000100;
Enums.FileAttributes.FileAttribute_SparseFile = 0x00000200;
Enums.FileAttributes.FileAttribute_ReparsePoint = 0x00000400;
Enums.FileAttributes.FileAttribute_Compressed = 0x00000800;
Enums.FileAttributes.FileAttribute_Offline = 0x00001000;
Enums.FileAttributes.FileAttribute_NotContentIndexed = 0x00002000;

