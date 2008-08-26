/////////////////////////////////////////////////////////////////////////////
// Create the Mission Parameters object
var objMissParms = new ActiveXObject("Pigs.MissionParams");

objMissParms.TeamCount      = 2;
objMissParms.MaxTeamPlayers = 16;
objMissParms.MinTeamPlayers = 1;
objMissParms.MapType        = 0;

objMissParms.Validate();


// Display the properties

WScript.Echo("TeamCount      = " + objMissParms.TeamCount      + "\n"
           + "MaxTeamPlayers = " + objMissParms.MaxTeamPlayers + "\n"
           + "MinTeamPlayers = " + objMissParms.MinTeamPlayers + "\n"
           + "MapType        = " + objMissParms.MapType);


