<pigbehavior language="JScript" basebehavior="Default">

<script src="include\common.js"></script>
<script src="include\chatcommand.js"></script>
<script src="include\eliza.js"></script>

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


var nCorrectionInterval = 4.0;
var fTimerOn = true;
var fHostile = false;
var nThrottle = 100;
var strTarget = "";
var strOldTarget = "";
var mySector, myShip, myTarget;
var nShieldValue, nHullValue;

var ChatCommandList = new Array ("eliza off", "eliza on", "start demo");

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
  //Host.Trace("Hull types:\n");

  // loop thru collection
  for (var i=0; !e.atEnd(); e.moveNext(), i++)
  {
    var hull = e.item();
    //Host.Trace("" + i + ". " + hull.Name + "\n");

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
  //Host.Trace("First: " + FirstHull + " Second: " + SecondHull + "\n");

  // look for valid first choice index
  if (FirstHull != -1)
  {
    // first choice found, return index
    //Host.Trace ("Selecting first choice.\n");
    return FirstHull;
  }
  else if (SecondHull != -1)
  {
    // second choice found, return index
    //Host.Trace ("Selecting second choice.\n");
    return SecondHull;
  }
  else
  {
    // default found, return index
    //Host.Trace ("Selecting default choice.\n");
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
  if ("object" == typeof(Properties("ThrottleTimer")))
    Properties("ThrottleTimer").Kill();

  try
  {
    // Get the list of hull types
    var objHullTypes = HullTypes;                                        // Pig.HullTypes (should be Pig.Station.HullTypes)
    var cHullTypes = objHullTypes.Count;
    //Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");    // //Host.Trace

    // Buy the best hull (or else we'll be in an eject pod)
    var v = SelectBestHull(objHullTypes, "Scout", "scout");
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(v))                                      // Pig.Ship.BuyHull
  }
  catch (e)
  {
    Trace("\n" + e.description + "\n");                                  // //Host.Trace
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
	CreateTimer(5.0, "Launch();", -1, "LaunchTimer");
  }
}

/////////////////////////////////////////////////////////////////////////////
function WhoAmI()
{
  var shiplist = Game.Ships;
  var x;
  //Host.Trace("*********************************************\n");
  var e = new Enumerator (shiplist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    //Host.Trace("ship: " + x.Name + "\n");
    if (x.Name == Name)
      myShip = x;
  }
  if (myShip)
    mySector = myShip.Sector;
  //Host.Trace("my ship name is " + myShip + "\n");
  //Host.Trace("my sector name is " + mySector + "\n");
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
  //Host.Trace("*********** Find Target **********************************\n");
  var e = new Enumerator (shiplist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    //Host.Trace("ship: " + x.Name + "\n");
    if (x.Name == strTargetName)
      result = x;
  }
  if (result != null)
  {
    myTarget = result;
    //Host.Trace("target ship found: " + result + myTarget.Position + "\n");
    //Game.SendMsg("target ship found: " + result + "\n");
    //Host.Trace("*********** Find Target End **********************************\n");
    return (result);
  }
  //Host.Trace("%%%%%%%%%%%%%% ship not found %%%%%%%%%%\n");
  e = new Enumerator (stationlist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    //Host.Trace("station: " + x.Name + "\n");
    if (x.Name == strTargetName)
      result = x;
  }
  if (result != null)
  {
    //Host.Trace("target station found: " + result + "\n");
    myTarget = result;
    //Host.Trace("*********** Find Target End **********************************\n");
    return (result);
  }
  //Host.Trace("*********** Find Target End Not Found **********************************\n");
  return (null);
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
  CreateTimer(1.0, "ThrustOrbit()", -1, "ThrottleTimer");             // This.CreateTimer
  InitEliza();
}

/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  ElizaEnabled = 1;
  WhoAmI();
  // Set the ship's throttle to 0%
  //Ship.Throttle = 0;                                                       // Pig.Ship.Throttle

  // Set the timer's interval to 2 seconds
  Timer.Interval = nCorrectionInterval;
  Timer.ExpirationExpression = "CorrectOrbit()";
}


/////////////////////////////////////////////////////////////////////////////
function OnReceiveCommand(strCommand, strParams, objShip)
{
  if (objShip.Team != Ship.Team)
  {
    objShip.SendChat("I'm not allowed to talk to you.");
    return true;
  }
  if (strCommand == "eliza off")
  {
    objShip.SendChat("Eliza chat responses are now disabled.");
    ElizaEnabled = 0;
  }
  else if (strCommand == "eliza on")
  {
    objShip.SendChat("Eliza chat responses are now enabled.");
    ElizaEnabled = 1;
  }
  else if (strCommand == "start demo")
  {
    objShip.SendChat("The demo has started.");
    strTarget = objShip.Name;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    if (Ship.Velocity.LengthSquared != 0)
      Ship.Face(strTarget, "CollisionTest(strTarget)");
    nCorrectionInterval = 2.0;
    FindTarget(strTarget);
  }
  catch (e)
  {
    //Game.SendChat("bad stuff");
    //Ship.Face("Outpost", "ThrustOrbit();");
    //Host.Trace ("$$$$$$$$$$$$$$$$$ bad stuff!! $$$$$$$$$$$$$$$$$$$$$\n");
    //AllStop();
		FireWeapon(false);
		FireMissile(false);
    nCorrectionInterval = 2.0;
  }
  Timer.Interval = nCorrectionInterval;
}

/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{
    var range = Range2Ship(myTarget);

    if (range < 20000)
    {
      AllStop();
    }
    else
    {
      Thrust(ThrustForward);
      Throttle = 100;
    }

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

