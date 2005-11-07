<pigbehavior language="JScript" Name="Mark0">

<script src="include\AutoStartGame.js"/>

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//
// Outside of the right margin, I have shown an alternate syntax for calling
// the properties/methods of the ActivePigs objects. The point is merely to
// demonstrate that several of the ActivePigs objects are in the global
// name space of the script. These objects are:
//
//     This - The currently running behavior object for which this script
//            defines the actions of the pig.
//
//     Pig  - The pig object upon which this behavior is assigned.
//
//     Host - An object that acts as this behavior's entry into the
//            ActivePigs object model. This can also be thought of as an
//            /internal/ session object.
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
      Logon();                                                             // Pig.Logon
    }
    catch (e)
    {
      // Logon failed, so shutdown the pig
      Shutdown();                                                          // Pig.Shutdown
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
    JoinMission("Pig Mission");                                            // Pig.JoinMission
    return;
  }
  catch (e)
  {
  }

  // Mission didn't exist, join any other current mission, if any
  try
  {
    JoinMission();                                                         // Pig.JoinMission
    return;
  }
  catch (e)
  {
  }

  // No missions exist, create one and join it
  var objParams = new ActiveXObject("Pigs.MissionParams");
  objParams.TeamCount = 2;
  objParams.Validate();
  CreateMission(objParams);                                                // Pig.CreateMission

  // Automatically start game when the minimum players per team have joined
  AutoStartGame(objParams.MinTeamPlayers);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
  // Avoid repeated attempts to join a team
  if (PigState_JoiningTeam != eStatePrevious)
    JoinTeam();                                                            // Pig.JoinTeam
}

/////////////////////////////////////////////////////////////////////////////
// Selects the best hull from a collection of hulls.  Scans the list, noting the
// index of the Fighter and the Interceptor.  If a Fighter was found, it returns
// that index.  Otherwise, the index of the Interceptor is returned.  If an
// Interceptor is not found, returns 0, which should be a scout or a recovery
// ship.
function SelectBestHull (hullCollection)
{
	var FighterHull = -1, InterceptorHull = 0;
	e = new Enumerator (hullCollection)
	Host.Trace("Hull types:\n");
  // loop thru collection
	for (var i=0; !e.atEnd(); e.moveNext(), i++)
	{
		hull = e.item();
		Host.Trace("" + i + ". " + hull.Name + "\n");
    // search for "Interceptor" in the name
		if (hull.Name.search("Interceptor") != -1)
			InterceptorHull = i;
    // search for "Fighter" in the name
		if (hull.Name.search("Fighter") != -1)
			FighterHull = i;		
	}
	Host.Trace("Fighter: " + FighterHull + " Interceptor: " + InterceptorHull + "\n");
  // look for valid Fighter index
	if (FighterHull != -1)
  {
    // fighter found, return index
    Host.Trace ("Selecting fighter.\n");
    return FighterHull;
  }
	else
  {
    // interceptor found, or default, return index
    Host.Trace ("Selecting interceptor.\n");
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
    var objHullTypes = HullTypes;                                        // Pig.HullTypes (should be Pig.Station.HullTypes)
    var cHullTypes = objHullTypes.Count;
    Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");    // Host.Trace

	v = SelectBestHull(objHullTypes);
    // Buy a random hull (or else we'll be in an eject pod)
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(v))                                      // Pig.Ship.BuyHull
//      Ship.BuyHull(objHullTypes(Random() % cHullTypes))                  // Pig.Ship.BuyHull, Host.Random
  }
  catch (e)
  {
    Trace("\n" + e.description + "\n");                                  // Host.Trace
  }

  // As soon as the mission starts or we join a team, we launch
  if (PigState_WaitingForMission == ePrev || PigState_JoiningTeam == ePrev)
  {
    // Launch into space!
    //Launch();                                                              // Pig.Launch
  }
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  // Set the ship's throttle to 100%
  Throttle = 100;                                                          // Pig.Ship.Throttle

  // Create a timer to reset the throttle
  CreateTimer(10.0, "OnInitialThrottle()", -1, "FlyingTimer");             // This.CreateTimer
}


/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the ship's throttle to 0%
  Ship.Throttle = 0;                                                       // Pig.Ship.Throttle

  // Thrust left for awhile
  Ship.Thrust(ThrustLeft);                                                 // Pig.Ship.Thrust

  // Adjust the timer to reset the thrust, kill the timer, and face the outpost
  Timer.Interval = 15.0;                                                   // This.Timer
  Timer.ExpirationExpression = "OnEndThrusting()";
}

/////////////////////////////////////////////////////////////////////////////
function OnEndThrusting()
{
  // Kill the timer
  Timer.Kill();

  // Stop the ship from thrusting
  Ship.AllStop();

  // Face the outpost
  try
  {
    Ship.Face("Outpost", "AllStop(); Beep(5000, 50);");
  }
  catch (e)
  {
  }
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

