
/////////////////////////////////////////////////////////////////////////////
// Create the Session Information object

//var objInfo = new ActiveXObject("Pigs.SessionInfo");
//objInfo.ApplicationName = WScript.ScriptFullName;


/////////////////////////////////////////////////////////////////////////////
// Connect to the Pig Server

var objSession = new ActiveXObject("Pigs.Session");
//objSession.SessionInfo = objInfo;


/////////////////////////////////////////////////////////////////////////////
// Create a single pig with a "Default" behavior

var objPig = objSession.CreatePig();

// Logon to an Allegiance server
WScript.Echo("Logging on to an Allegiance server...");
objPig.Logon("JTASLER-NT-000", "password", "JTASLER-NT");

// Join an existing mission
WScript.Echo("Joining an existing mission...");
objPig.JoinMission();
WScript.Echo("Mission Joined!");

// Join a team
WScript.Echo("Joining a team...");
objPig.JoinTeam();
WScript.Echo("Team Joined!");
