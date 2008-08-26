<pigbehavior language="JScript" basebehavior="Default">

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
var strTarget = "";
var strOldTarget = "";
var mySector, myShip, myTarget;
var nShieldValue, nHullValue;

/////////////////////////////////////////////////////////////////////////////
// Selects the best hull from a collection of hulls.  Scans the list, noting the
// index of the Fighter and the Interceptor.  If a Fighter was found, it returns
// that index.  Otherwise, the index of the Interceptor is returned.  If an
// Interceptor is not found, returns 0, which should be a scout or a recovery
// ship.
function SelectBestHull (hullCollection, strFirstChoice, strSecondChoice)
{
  var FirstHull = -1, SecondHull = -1, DefaultHull = -1;
  var e = new Enumerator (hullCollection)
  Host.Trace("Hull types:\n");

  // loop thru collection
  for (var i=0; !e.atEnd(); e.moveNext(), i++)
  {
    var hull = e.item();
    Host.Trace("" + i + ". " + hull.Name + "\n");

    // search for strFirstChoice in the name
    if (hull.Name.search(strFirstChoice) != -1)
      FirstHull = i;

    // search for strSecondChoice in the name
    if (hull.Name.search(strSecondChoice) != -1)
      SecondHull = i;		

    // search for strSecondChoice in the name
    if (hull.Name.search("Scout") != -1)
      DefaultHull = i;		
  }
  Host.Trace("First: " + FirstHull + " Second: " + SecondHull + "\n");

  // look for valid first choice index
  if (FirstHull != -1)
  {
    // first choice found, return index
    Host.Trace ("Selecting first choice.\n");
    return FirstHull;
  }
  else if (SecondHull != -1)
  {
    // second choice found, return index
    Host.Trace ("Selecting second choice.\n");
    return SecondHull;
  }
  else
  {
    // default found, return index
    Host.Trace ("Selecting default choice.\n");
    return DefaultHull;
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
    var v = SelectBestHull(objHullTypes, "Fighter", "Interceptor");
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
  Host.Trace("%%%%%%%%%%%%%% ship not found %%%%%%%%%%\n");
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
  }
  if (fTimerOn)
    Timer.Interval = nCorrectionInterval;
}

/////////////////////////////////////////////////////////////////////////////
// This function starts a ripcord operation.
function DoRipCord()
{
  // stop the ship, stop all weapons
  AllStop();
  FireMissile(false);
  FireWeapon(false);

  // start ripcording
  Ship.RipCord(true);

  // go back to normal in 15 seconds - set a timer
  CreateTimer(15, "EndRipCord()", 1, "RipcordTimer");
}

/////////////////////////////////////////////////////////////////////////////
// This function ends a ripcord operation - it should not be called directly.
function EndRipCord()
{
  // stop ripcording
  Ship.RipCord(false);

  // kill ripcord timer
  Timer.Kill();
}

/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{
		// Stop the ship
		//AllStop();

    //Host.Trace(">>>>>>>>>>>>>>>>>>> my health: " + Ship.Fraction + "\n");

    var range = Range2Ship(myTarget);

    if (range < 60000)
    {
		  // Thrust to the left
		  Thrust(ThrustForward);
      Throttle = 50;
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

function CollisionTest(strIt)
{
  strTarget = strIt;
}

// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

