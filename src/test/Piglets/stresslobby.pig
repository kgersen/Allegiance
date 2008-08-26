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
      Logon();
    }
    catch (e)
    {
      // Logon failed, so shutdown the pig
      Trace("\nLogon failed: " + e.description + "\n");
      Shutdown();
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins or creates a mission.
function OnStateMissionList(eStatePrevious)
{
  // Hang-out for a while, then join a team
  var nDuration = (Random() % 50) / 5.0;
  CreateTimer(nDuration, "DoMissionList();", -1, "JoinGameTimer");
}


/////////////////////////////////////////////////////////////////////////////
function DoMissionList()
{
  Timer.Kill();

  // Join any current mission, if any
  try
  {
    JoinMission();
  }
  catch (e)
  {
    Trace("\nCouldn't join a mission. Pig is shutting down.\n");
    Shutdown();
  }

  return;
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
  // Quit game after waiting for a while
  var nDuration = (Random() % 10) / 5.0;
  CreateTimer(nDuration, "DoQuitGame();", -1, "QuitGameTimer");
}


/////////////////////////////////////////////////////////////////////////////
function DoQuitGame()
{
  Timer.Kill();
  Logoff();
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

