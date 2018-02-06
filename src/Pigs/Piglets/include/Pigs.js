
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
// PigState Enum

Enums.PigState = function () { };

Enums.PigState.PigState_NonExistant = 0;
Enums.PigState.PigState_LoggingOn = 1;
Enums.PigState.PigState_LoggingOff = 2;
Enums.PigState.PigState_MissionList = 3;
Enums.PigState.PigState_CreatingMission = 4;
Enums.PigState.PigState_JoiningMission = 5;
Enums.PigState.PigState_QuittingMission = 6;
Enums.PigState.PigState_TeamList = 7;
Enums.PigState.PigState_JoiningTeam = 8;
Enums.PigState.PigState_WaitingForMission = 9;
Enums.PigState.PigState_Docked = 10;
Enums.PigState.PigState_Launching = 11;
Enums.PigState.PigState_Flying = 12;
Enums.PigState.PigState_Terminated = 13;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigMapType Enum

Enums.PigMapType = function () { };

Enums.PigMapType.PigMapType_SingleRing = 0;
Enums.PigMapType.PigMapType_DoubleRing = 1;
Enums.PigMapType.PigMapType_PinWheel = 2;
Enums.PigMapType.PigMapType_DiamondRing = 3;
Enums.PigMapType.PigMapType_Snowflake = 4;
Enums.PigMapType.PigMapType_SplitBases = 5;
Enums.PigMapType.PigMapType_Brawl = 6;
Enums.PigMapType.PigMapType_Count = 7;
Enums.PigMapType.PigMapType_Default = PigMapType_Brawl;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigLobbyMode Enum

Enums.PigLobbyMode = function () { };

Enums.PigLobbyMode.PigLobbyMode_Club = 0;
Enums.PigLobbyMode.PigLobbyMode_Free = 1;
Enums.PigLobbyMode.PigLobbyMode_LAN = 2;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigThrust Enum

Enums.PigThrust = function () { };

Enums.PigThrust.ThrustNone = 0;
Enums.PigThrust.ThrustLeft = 1;
Enums.PigThrust.ThrustRight = 2;
Enums.PigThrust.ThrustUp = 3;
Enums.PigThrust.ThrustDown = 4;
Enums.PigThrust.ThrustForward = 5;
Enums.PigThrust.ThrustBackward = 6;
Enums.PigThrust.ThrustBooster = 7;

