<pigbehavior language="JScript" basebehavior="Default">

<script src="include\common.js"></script>

<script>
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
//
// Alephjumper.pig
//
// This pig finds the first aleph in its sector, flies through it, and then
// flies back to the original sector.  It repeats this over and over again.
// In mass quantities, this pig is an effective stress test against AllSrv.
//
// The pig must be able to see an aleph upon launch, or it will just hang in
// space and do nothing.  The automated solution is to throw in a scout2 pig.
//

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
var HomeSectorName, DestinationAlephName;
var nShieldValue, nHullValue;
var TestAlephName = "";

function OnActivate(objPrevPig)
{
  Trace("\n\nActivating alephjumper...\n\n");
  if (PigState == PigState_Docked)
  {
    OnStateDocked(PigState_Flying);
  }
}

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


function OnReceiveChat(strChat, objShip)
{
  Trace(strChat + "\n");
  if (strChat.search("testaleph") == 0)
  {
    TestAlephName = strChat.substr(10);
    objShip.SendChat(TestAlephName);
    return true;
  }
  return false;
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

	CreateTimer(3.0, "DoLaunch();", -1, "LaunchTimer");
}

function DoLaunch()
{
  Timer.Kill();
  Launch();
  Throttle = 0;
}

function PostLaunch()
{
  if (!FindTargetAleph())
    return;
  strTarget = DestinationAlephName;
  Timer.Kill();
  HomeSectorName = Ship.Sector.Name;

  // Set the ship's throttle to 100%
  Thrust(ThrustForward);
  Throttle = 100;                                                          // Pig.Ship.Throttle

  // Create a timer to reset the throttle
  var fDuration = 5.0 + (Random() % 10);
  CreateTimer(fDuration, "OnInitialThrottle()", -1, "FlyingTimer");             // This.CreateTimer
  CreateTimer(1.0, "Throttle = 50", -1, "ThrottleTimer");             // This.CreateTimer
}

function OnSectorChange(agcOldSector, agcNewSector)
{
  if (TestAlephName == "")
  {
    strTarget = agcOldSector.Name;
  }
  else
  {
    if (agcNewSector.Name == HomeSectorName)
      strTarget = TestAlephName;
    else
      strTarget = agcOldSector.Name;
    TestAlephName = "";
  }
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  Ship.WingID = 7;
	CreateTimer(5.0, "PostLaunch();", -1, "PostLaunchTimer");
}

/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the timer's interval to 1.5 seconds
  Timer.Interval = 1.5;
  Timer.ExpirationExpression = "CorrectOrbit()";
}

function FindTargetAleph()
{
  var AlephList = Ship.Sector.Alephs;
  if (AlephList.Count == 0)
    return false;  
  DestinationAlephName = AlephList(0).Destination.Sector.Name;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    Ship.Face(strTarget, "");
  }
  catch (e)
  {
    Ship.Team.SendChat("Invalid target aleph name.");
    if (TestAlephName != "")
    {
      strTarget = TestAlephName;
      TestAlephName = "";
    }
		FireWeapon(false);
		FireMissile(false);
  }
}

// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

