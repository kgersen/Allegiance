<pigbehavior language="JScript">

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Logs on to the mission server.
function OnStateNonExistant(eStatePrevious)
{
  // If previous state was PigState_Terminated, the pig is being created
  if (PigState_Terminated == eStatePrevious)
  {
    // Logon to the mission server
    try
    {
      UpdatesPerSecond = 20;
      Logon();                                                             // Pig.Logon
    }
    catch (e)
    {
      // Logon failed, so shutdown the pig
      Trace("\nLogon failed: " + e.description + "\n");
      Shutdown();                                                          // Pig.Shutdown
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins or creates a mission.
function OnStateMissionList(eStatePrevious)
{
  // Create a game and join it
  var objParams = new ActiveXObject("Pigs.MissionParams");
  objParams.TeamCount = 2;
  objParams.MinTeamPlayers = 1;
  objParams.MaxTeamPlayers = 10;
  CreateMission(objParams);

  // Create a timer to quit the game
//  CreateTimer(30, "QuitMission()", -1, "MissionTimer");
}



//
// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

