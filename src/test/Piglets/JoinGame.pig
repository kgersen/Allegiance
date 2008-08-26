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
      UpdatesPerSecond = 8;
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
  // Join any current mission, if any
  try
  {
    JoinMission();
    return;
  }
  catch (e)
  {
    // Logon failed, so shutdown the pig
    Trace("\JoinMission failed: " + e.description + "\n");

    // Shutdown if there's a problem
    Shutdown();
  }
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

