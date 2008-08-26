
/////////////////////////////////////////////////////////////////////////////
// Create the Session Information object

//var objInfo = new ActiveXObject("Pigs.SessionInfo");
//objInfo.ApplicationName = WScript.ScriptFullName;


/////////////////////////////////////////////////////////////////////////////
// Connect to the Pig Server

var objSession = new ActiveXObject("Pigs.Session");
//objSession.SessionInfo = objInfo;


/////////////////////////////////////////////////////////////////////////////
// Create a single pig with a "BeeperVB" behavior

var objPig = objSession.CreatePig("beeperVB");

// Logon to an Allegiance server
WScript.Echo("Logging on to an Allegiance server...");
objPig.Logon("JTASLER-NT-000", "password", "JTASLER-NT");

// Create a MissionParameters object
WScript.Echo("Creating mission parameters...");
var objMissionParams = new ActiveXObject("Pigs.MissionParams");
objMissionParams.TeamCount      = 2;
objMissionParams.MaxTeamPlayers = 8;
objMissionParams.MinTeamPlayers = 2;
objMissionParams.MapType        = 0;

// Create a new mission
WScript.Echo("Creating a new mission...");
objPig.CreateMission(objMissionParams);
WScript.Echo("Mission Created!");

