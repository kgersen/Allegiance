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
  // Attempt to join the current pig mission, if any
  try
  {
    JoinMission("Pig Mission");
    return;
  }
  catch (e)
  {
  }

  // Mission didn't exist, join any other current mission, if any
  try
  {
    JoinMission();
    return;
  }
  catch (e)
  {
  }

  // No missions exist, create one and join it
  var objParams = new ActiveXObject("Pigs.MissionParams");
  objParams.TeamCount = 2;
  objParams.MinTeamPlayers = 3;
  objParams.MaxTeamPlayers = 10;
  CreateMission(objParams);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
  // Avoid repeated attempts to join a team
  if (PigState_JoiningTeam == eStatePrevious)
  {
    QuitGame();
  }
  else
  {
    try
    {
      JoinTeam();
    }
    catch (e)
    {
      Trace("\n" + e.description + " - Attempting to QuitGame\n");
      QuitGame();
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Selects the best hull from a collection of hulls.  Scans the list, noting
// the index of the Fighter and the Interceptor.  If a Fighter was found, it
// returns that index.  Otherwise, the index of the Interceptor is returned.
// If an Interceptor is not found, returns 0, which should be a scout or a
// recovery ship.
//
function SelectBestHull(objHullTypes)
{
  var FighterHull = -1, InterceptorHull = 0;
  var e = new Enumerator(objHullTypes)

  // loop thru collection
  var strTrace = "Hull types:\n";
  for (var i = 0; !e.atEnd(); e.moveNext(), ++i)
  {
    hull = e.item();
    strTrace += "  " +  i + ". " + hull.Name + "\n";

    // search for "Interceptor" in the name
    if (hull.Name.search("Interceptor") != -1)
      InterceptorHull = i;

    // search for "Fighter" in the name
    if (hull.Name.search("Fighter") != -1)
      FighterHull = i;    
  }
  strTrace += "Fighter: " + FighterHull + " Interceptor: " + InterceptorHull + "\n";

  // look for valid Fighter index
  if (FighterHull != -1)
  {
    // fighter found, return index
    Trace(strTrace + "Selecting fighter.\n");
    return FighterHull;
  }
  else
  {
    // interceptor found, or default, return index
    Trace (strTrace + "Selecting interceptor.\n");
    return InterceptorHull;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Launches the pig as soon as it becomes docked.
function OnStateDocked(ePrev)
{
  // Kill the flying timer, if one exists
  if ("object" == typeof(Properties("FlyingTimer")))
    Properties("FlyingTimer").Kill();

  try
  {
    // Get the list of hull types
    var objHullTypes = HullTypes;
    var cHullTypes = objHullTypes.Count;
    Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");

    // Select and buy the best hull available
    var iHull = SelectBestHull(objHullTypes);
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(iHull));
  }
  catch (e)
  {
    Trace("\n" + e.description + "\n");
  }

  // Schedule the Launch method (to avoid an IGC assert)
	CreateTimer(1.0, "Timer.Kill(); Launch();", -1, "LaunchTimer");
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  try
  {
    // Set the ship's throttle to 100%
    Throttle = 100;

    // Turn on the thruster
    Thrust(ThrustBooster);
  }
  catch (e)
  {
    Trace("Error in OnStateFlying():\n\t" + e.description);
  }
}

// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

