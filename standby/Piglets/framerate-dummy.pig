<pigbehavior language="JScript">

<script src="include\common.js"/>
<script src="include\AutoStartGame.js"/>
<script src="include\GetIntoTeam.js"/>
<script src="include\chatcommand.js"/>
<script src="include\eliza.js"/>

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
var mySector, myShip;
var myTarget = new ActiveXObject("AGC.AGCVector");
var nShieldValue, nHullValue;
var range, oldrange;

var ChatCommandList = new Array ("eliza off", "eliza on", "start demo", "explode", "count off");

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
  try
  {
    myTarget.X = 0;
    myTarget.Y = 0;
    myTarget.Z = 1000;
    Ship.WingID = 9;
    // Kill the flying timer, if one exists
    if ("object" == typeof(Properties("FlyingTimer")))
      Properties("FlyingTimer").Kill();
    if ("object" == typeof(Properties("AimTimer")))
      Properties("AimTimer").Kill();
    if ("object" == typeof(Properties("ThrottleTimer")))
      Properties("ThrottleTimer").Kill();
  } 
  catch (e)
  {
    Trace("\ndock1: " + e.description + "\n");                                  // //Host.Trace
  }

  try
  {
    // Get the list of hull types
    var objHullTypes = HullTypes;                                        // Pig.HullTypes (should be Pig.Station.HullTypes)
    var cHullTypes = objHullTypes.Count;
    //Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");    // //Host.Trace

    // Buy the best hull (or else we'll be in an eject pod)
    var v = SelectBestHull(objHullTypes, "Interceptor", "interceptor");
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(v))                                      // Pig.Ship.BuyHull
  }
  catch (e)
  {
    Trace("\ndock2: " + e.description + "\n");                                  // //Host.Trace
  }

  try
  {
    // As soon as the mission starts or we join a team, we launch
    Launch();
  } 
  catch (e)
  {
    Trace("\ndock3: " + e.description + "\n");                                  // //Host.Trace
  }
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  // Set the ship's throttle to 100%
  Thrust(ThrustForward);
  Throttle = 100;                                                          // Pig.Ship.Throttle

  // Create a timer to reset the throttle

  CreateTimer(2.0, "AllStop(); DoAiming()", -1, "AimTimer");
  InitEliza();
}

function DoAiming()
{
  //if ("object" == typeof(Properties("AimTimer")))
    //Properties("AimTimer").Kill();
  Timer.ExpirationExpression = "DoAiming()";
  range = 10000000;
  Ship.Face(myTarget, "ThrustOrbit()");
}

/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  ElizaEnabled = 1;
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
  else if (strCommand == "explode")
  {
    CommitSuicide();
  }
  else if (strCommand == "count off")
  {
    objShip.SendChat("count");
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    if (range < 900)
      AllStop();
    else
      Ship.Face(myTarget, "");
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
  Timer.Interval = 10.0;
}

/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
  if (range < 0)
  {
    AllStop();
    return;
  }
  try
  {
    range = Ship.Position.Subtract(myTarget).LengthSquared;
    //Ship.Team.SendChat("range: " + range);

    if (range < 900)
    {
      AllStop();
      range = -100;
      if ("object" == typeof(Properties("AimTimer")))
        Properties("AimTimer").Kill();
      //if ("object" == typeof(Properties("ThrottleTimer")))
        //Properties("ThrottleTimer").Kill();
    }
    else if (range < 10000)
    {
      Throttle = 2;
    }
    else if (range < 40000)
    {
      Throttle = 5;
    }
    else if (range < 90000)
    {
      Throttle = 10;
    }
    else
    {
      Throttle = 100;
    }

    //Timer.Interval = 1.0;

    // Check for empty Ammo
    if (!Ammo)
      CommitSuicide();
  }
  catch (e)
  {
    Trace("Error in ThrustOrbit():\n\t" + e.description);
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

