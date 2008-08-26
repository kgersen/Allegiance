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


var nCorrectionInterval = 2.0;
var fTimerOn = true;
var fHostile = false;
var nThrottle = 100;
var strTarget = "Grendel";
var strOldTarget = "";
var mySector, myShip, myTarget;
var nShieldValue, nHullValue;

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

  // Attempt to join Grendel's game, if it's there
  try
  {
    JoinMission("Grendel's game");                                            // Pig.JoinMission
    return;
  }
  catch (e)
  {
  }

  // Attempt to join pig007's game, if it's there
  try
  {
    JoinMission("pig007's game");                                            // Pig.JoinMission
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

  // As soon as the mission starts or we join a team, we launch
  if (PigState_WaitingForMission == ePrev || PigState_JoiningTeam == ePrev)
  {
    // Launch into space!
    Launch();                                                              // Pig.Launch
  }
  else if (PigState_Flying == ePrev)
  {
	// If we were flying before, we need to schedule the Launch method
	CreateTimer(5.0, "OnDoLaunch();", -1, "LaunchTimer");
  }
}


function OnDoLaunch()
{
	Timer.Kill();
	Beep(1000, 500);
	Launch();
}


/////////////////////////////////////////////////////////////////////////////
function WhoAmI()
{
  var shiplist = Game.Ships;
  var x;
  Host.Trace("*********************************************\n");
  var e = new Enumerator (shiplist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    Host.Trace("ship: " + x.Name + "\n");
    if (x.Name == Name)
      myShip = x;
  }
  if (myShip)
    mySector = myShip.Sector;
  Host.Trace("my ship name is " + myShip + "\n");
  Host.Trace("my sector name is " + mySector + "\n");
}

/////////////////////////////////////////////////////////////////////////////
function FindTarget(strTargetName)
{
  if (strTarget == strOldTarget)
    return;
  else
    strOldTarget = strTarget;
  var shiplist = Game.Ships;
  var stationlist = Game.Stations;
  var x, result = null;
  Host.Trace("*********** Find Target **********************************\n");
  var e = new Enumerator (shiplist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    Host.Trace("ship: " + x.Name + "\n");
    if (x.Name == strTargetName)
      result = x;
  }
  if (result != null)
  {
    myTarget = result;
    Host.Trace("target ship found: " + result + myTarget.Position + "\n");
    //Game.SendMsg("target ship found: " + result + "\n");
    Host.Trace("*********** Find Target End **********************************\n");
    return (result);
  }
  e = new Enumerator (stationlist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    Host.Trace("station: " + x.Name + "\n");
    if (x.Name == strTargetName)
      result = x;
  }
  if (result != null)
  {
    Host.Trace("target station found: " + result + "\n");
    myTarget = result;
    Host.Trace("*********** Find Target End **********************************\n");
    return (result);
  }
  Host.Trace("*********** Find Target End Not Found **********************************\n");
  return (null);
}

/////////////////////////////////////////////////////////////////////////////
// Calculates the distance squared to an AGCShip or PigShip.
function Range2Ship(agcShip)
{
  return (Position.Subtract(agcShip.Position).LengthSquared);
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

  WhoAmI();
  // Set the ship's throttle to 0%
  //Ship.Throttle = 0;                                                       // Pig.Ship.Throttle

  // Set the timer's interval to 2 seconds
  Timer.Interval = nCorrectionInterval;
  Timer.ExpirationExpression = "CorrectOrbit()";
}


/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    Ship.Face(strTarget, "ThrustOrbit();");
    if (fHostile)
      nCorrectionInterval = 2.0;              
    else
      nCorrectionInterval = 2.0;
    FindTarget(strTarget);
  }
  catch (e)
  {
    //Ship.Face("Outpost", "ThrustOrbit();");
    Host.Trace ("$$$$$$$$$$$$$$$$$ bad stuff!! $$$$$$$$$$$$$$$$$$$$$\n");
    AllStop();
		FireWeapon(false);
		FireMissile(false);
    nCorrectionInterval = 5.0;
    Ship.Fraction(0);
  }
  if (fTimerOn)
    Timer.Interval = nCorrectionInterval;
}


/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{
		// Stop the ship
		//AllStop();

    Host.Trace(">>>>>>>>>>>>>>>>>>> my health: " + Ship.Fraction + "\n");

    var range = Range2Ship(myTarget);

    if (range < 60000)
    {
		  // Thrust to the left
		  Thrust(ThrustLeft, ThrustForward);
      Throttle = 80;
      if (fHostile)
		    FireWeapon(true);
    }
    else if (range > 1000000 && range < 3000000)
    {
      Throttle = 100;
      Thrust(ThrustForward);
      if (fHostile)
		    FireMissile(true);
    }
    else
    {
      Throttle = 100;
      Thrust(ThrustForward);
		  FireMissile(false);
		  FireWeapon(false);
    }
    Ship.Throttle = nThrottle;

		// Check for empty Ammo
		if (!Ammo)
			CommitSuicide();
	}
	catch (e)
	{
    Host.Echo("*************** Something's Wrong!!!! ***********************\n");
	}
}



// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

