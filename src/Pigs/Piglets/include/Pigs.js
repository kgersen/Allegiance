
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

Enums.PigState.PigState_NonExistant = PigState_NonExistant;
Enums.PigState.PigState_LoggingOn = PigState_LoggingOn;
Enums.PigState.PigState_LoggingOff = PigState_LoggingOff;
Enums.PigState.PigState_MissionList = PigState_MissionList;
Enums.PigState.PigState_CreatingMission = PigState_CreatingMission;
Enums.PigState.PigState_JoiningMission = PigState_JoiningMission;
Enums.PigState.PigState_QuittingMission = PigState_QuittingMission;
Enums.PigState.PigState_TeamList = PigState_TeamList;
Enums.PigState.PigState_JoiningTeam = PigState_JoiningTeam;
Enums.PigState.PigState_WaitingForMission = PigState_WaitingForMission;
Enums.PigState.PigState_Docked = PigState_Docked;
Enums.PigState.PigState_Launching = PigState_Launching;
Enums.PigState.PigState_Flying = PigState_Flying;
Enums.PigState.PigState_Terminated = PigState_Terminated;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigMapType Enum

Enums.PigMapType = function () { };

Enums.PigMapType.PigMapType_SingleRing = PigMapType_SingleRing;
Enums.PigMapType.PigMapType_DoubleRing = PigMapType_DoubleRing;
Enums.PigMapType.PigMapType_PinWheel = PigMapType_PinWheel;
Enums.PigMapType.PigMapType_DiamondRing = PigMapType_DiamondRing;
Enums.PigMapType.PigMapType_Snowflake = PigMapType_Snowflake;
Enums.PigMapType.PigMapType_SplitBases = PigMapType_SplitBases;
Enums.PigMapType.PigMapType_Brawl = PigMapType_Brawl;
Enums.PigMapType.PigMapType_Count = PigMapType_Count;
Enums.PigMapType.PigMapType_Default = PigMapType_Default;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigLobbyMode Enum

Enums.PigLobbyMode = function () { };

Enums.PigLobbyMode.PigLobbyMode_Club = PigLobbyMode_Club;
Enums.PigLobbyMode.PigLobbyMode_Free = PigLobbyMode_Free;
Enums.PigLobbyMode.PigLobbyMode_LAN = PigLobbyMode_LAN;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigThrust Enum

Enums.PigThrust = function () { };

Enums.PigThrust.ThrustNone = ThrustNone;
Enums.PigThrust.ThrustLeft = ThrustLeft;
Enums.PigThrust.ThrustRight = ThrustRight;
Enums.PigThrust.ThrustUp = ThrustUp;
Enums.PigThrust.ThrustDown = ThrustDown;
Enums.PigThrust.ThrustForward = ThrustForward;
Enums.PigThrust.ThrustBackward = ThrustBackward;
Enums.PigThrust.ThrustBooster = ThrustBooster;

////////////////////////////////////////////////////////////////////////////////////////////////////
// This is the global pig object. It is created by the COM layer when the pig script is 
// loaded by PigSrv. This is your main hook into all things pig.

/**
 * Interface to a pig player object.
 */
var Pig = function() { };

Pig.PigState = PigState;
Pig.PigStateName = PigStateName;
Pig.Stack = Stack;
Pig.Name = Name;
Pig.Ship = Ship;

Pig.HullTypes = function () {
    return HullTypes;
};

Pig.Money = Money;
Pig.Game = Game;
Pig.Me = Me;
Pig.ChatTarget = ChatTarget;
Pig.Host = Host;


Pig.UpdatesPerSecond = function(nPerSecond)
{
	return UpdatesPerSecond(nPerSecond);
};

Pig.Logon = function()
{
	return Logon();
};

Pig.Logoff = function()
{
	return Logoff();
};

Pig.CreateMission = function(bstrServerName, bstrServerAddr, pMissionParams)
{
	return CreateMission(bstrServerName, bstrServerAddr, pMissionParams);
};

Pig.JoinMission = function(bstrMissionOrPlayer)
{
	return JoinMission(bstrMissionOrPlayer);
};

Pig.JoinTeam = function(bstrCivName, bstrTeamOrPlayer)
{
	return JoinTeam(bstrCivName, bstrTeamOrPlayer);
};

Pig.QuitGame = function()
{
	return QuitGame();
};

Pig.Launch = function()
{
	return Launch();
};

Pig.Shutdown = function()
{
	return Shutdown();
};

Pig.IsMissionOwner = function()
{
	return IsMissionOwner();
};

Pig.IsTeamLeader = function()
{
	return IsTeamLeader();
};

Pig.SetSkills = function(fShoot, fTurn, fGoto)
{
	return SetSkills(fShoot, fTurn, fGoto);
};

Pig.StartCustomGame = function(pMissionParams)
{
	return StartCustomGame(pMissionParams);
};

Pig.StartGame = function()
{
	return StartGame();
};

Pig.ShipAngleThreshold1 = function(rRadians)
{
	return ShipAngleThreshold1(rRadians);
};

Pig.ShipAngleThreshold2 = function(rRadians)
{
	return ShipAngleThreshold2(rRadians);
};

Pig.ShipAngleThreshold3 = function(rRadians)
{
	return ShipAngleThreshold3(rRadians);
};

Pig.ShipDistanceThreshold1 = function(rDistance)
{
	return ShipDistanceThreshold1(rDistance);
};

Pig.ShipDistanceThreshold2 = function(rDistance)
{
	return ShipDistanceThreshold2(rDistance);
};

Pig.ShipDistanceThreshold3 = function(rDistance)
{
	return ShipDistanceThreshold3(rDistance);
};

Pig.ShipsUpdateLatencyThreshold1 = function(nMilliseconds)
{
	return ShipsUpdateLatencyThreshold1(nMilliseconds);
};

Pig.ShipsUpdateLatencyThreshold2 = function(nMilliseconds)
{
	return ShipsUpdateLatencyThreshold2(nMilliseconds);
};

Pig.ShipsUpdateLatencyThreshold3 = function(nMilliseconds)
{
	return ShipsUpdateLatencyThreshold3(nMilliseconds);
};

Pig.SendBytesToLobby = function(pvBytes, bGuaranteed)
{
	return SendBytesToLobby(pvBytes, bGuaranteed);
};

Pig.SendBytesToGame = function(pvBytes, bGuaranteed)
{
	return SendBytesToGame(pvBytes, bGuaranteed);
};



