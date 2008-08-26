<pigbehavior language="JScript">

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


function OnActivate(objPrevPig)
{
  Trace("\n\nActivating mark2s...\n\n");
  if (PigState == PigState_Docked)
  {
    OnStateDocked(PigState_Flying);
  }
}

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
      UpdatesPerSecond = 5;
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
  CreateMission(objParams); // Pig.CreateMission

  // Automatically start game when the minimum players per team have joined
  AutoStartGame(objParams.MinTeamPlayers);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
  // Avoid repeated attempts to join a team
  if (PigState_JoiningTeam != eStatePrevious)
  {
    try
    {
      JoinTeam();                                                          // Pig.JoinTeam
    }
    catch (e)
    {
      Trace("\n" + e.description + " - Attempting to QuitGame\n");         // Host.Trace
      QuitGame();
    }
  }
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
  var e = new Enumerator (hullCollection)
  Host.Trace("Hull types:\n");

  // loop thru collection
  for (var i=0; !e.atEnd(); e.moveNext(), i++)
  {
    var hull = e.item();
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

    // Buy the best hull (or else we'll be in an eject pod)
    var v = SelectBestHull(objHullTypes);
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(v))                                      // Pig.Ship.BuyHull
  }
  catch (e)
  {
    Trace("\n" + e.description + "\n");                                  // Host.Trace
  }

	CreateTimer(5.0, "OnDoLaunch();", -1, "LaunchTimer");
  
  /*
  // As soon as the mission starts or we join a team, we launch
  if (PigState_WaitingForMission == ePrev || PigState_JoiningTeam == ePrev)
  {
	  CreateTimer(5.0, "OnDoLaunch();", -1, "LaunchTimer");
    // Launch into space!
  }
  else if (PigState_Flying == ePrev)
  {
	// If we were flying before, we need to schedule the Launch method
	  CreateTimer(5.0, "OnDoLaunch();", -1, "LaunchTimer");
  }
  */
}


function OnDoLaunch()
{
	Timer.Kill();
	//Beep(1000, 500);
	Launch();
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  // Set the ship's throttle to 100%
  Throttle = 100;                                                          // Pig.Ship.Throttle

  // Create a timer to reset the throttle
  var fDuration = 5.0 + (Random() % 10);
  CreateTimer(fDuration, "OnInitialThrottle()", -1, "FlyingTimer");             // This.CreateTimer
}


/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the ship's throttle to 0%
  Ship.Throttle = 0;                                                       // Pig.Ship.Throttle

  // Set the timer's interval to 2 seconds
  Timer.Interval = 2.0;
  Timer.ExpirationExpression = "CorrectOrbit()";
}


/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    Ship.Face("Outpost", "ThrustOrbit();");
  }
  catch (e)
  {
  }
}


/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{
		// Stop the ship
		AllStop();

		// Thrust to the left
		Thrust(ThrustLeft, ThrustForward);

		// Toggle the firing of the weapon
		FireWeapon(!IsFiringWeapon);
		
		// Check for empty Ammo
		if (!Ammo)
			CommitSuicide();
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

