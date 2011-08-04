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
      Shutdown();
    }
  }
}


//
// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

