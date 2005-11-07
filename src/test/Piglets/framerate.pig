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
var StopRadius = 400;
var strTarget = "";
var strOldTarget = "";
var mySector, myShip, myTarget;
var nShieldValue, nHullValue;
var ShipObservationVector = new ActiveXObject("AGC.AGCVector");
var dummycounter, dummyready = false;

var DUMMYPIGS = 6;

var ChatCommandList = new Array ("eliza off", "eliza on", "start demo", "count");

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
  dummyready = false;
  // Kill the flying timer, if one exists
  if ("object" == typeof(Properties("FlyingTimer")))
    Properties("FlyingTimer").Kill();
  if ("object" == typeof(Properties("LaunchTimer")))
    Properties("LaunchTimer").Kill();
  if ("object" == typeof(Properties("ThrottleTimer")))
    Properties("ThrottleTimer").Kill();

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
    Trace("\n" + e.description + "\n");                                  // //Host.Trace
  }

  CountDummyPigs();
  Ship.Team.SendChat("Switch to Wing 'hotel' to join the framerate test.");

  // As soon as the mission starts or we join a team, we launch
  if (PigState_WaitingForMission == ePrev || PigState_JoiningTeam == ePrev)
  {
    // Launch into space!
    CreateTimer(5.0, "PreLaunch();", -1, "LaunchTimer");
  }
  else if (PigState_Flying == ePrev)
  {
    // If we were flying before, we need to schedule the Launch method
    CreateTimer(30.0, "PreLaunch();", -1, "LaunchTimer");
  }
}

function PreLaunch()
{
  if (dummyready)
  {
    StopTimer("CountTimer");
    Ship.Team.SendChat("My state is " + PigStateName, 8);
    Ship.Team.SendChat("cheat-board " + Ship.Name, 8);
    Ship.Team.SendChat("My state is " + PigStateName, 8);
    Timer.ExpirationExpression = "Timer.Kill(); Launch();";
    Timer.Interval = 3.0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  StepOne();
  InitEliza();
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

function CountDummyPigs()
{
  Ship.Team.SendChat("Waiting for dummy pigs to respond.");
  dummycounter = 0;
  Ship.Team.SendChat("count off", 9);
  CreateTimer(10.0, "CountPigResponses()", -1, "CountTimer");
}

function CountPigResponses()
{
  Ship.Team.SendChat("I found " + dummycounter + " dummy pigs.");
  StopTimer("CountTimer");
  if (dummycounter >= DUMMYPIGS)
  {
    Ship.Team.SendChat("All clear for launch.");
    dummyready = true;
  }
  else
  {
    Ship.Team.SendChat("Creating " + (DUMMYPIGS - dummycounter) + " dummy pigs.");
    CreateDummyPigs(DUMMYPIGS - dummycounter);
    CreateTimer(20.0, "WaitForNewPigs()", -1, "CountTimer");
    Ship.Team.SendChat("Waiting for new pigs.");
  }
}

function WaitForNewPigs()
{
  Ship.Team.SendChat("All clear for launch.");
  StopTimer("CountTimer");
  dummyready = true;
}

function CreateDummyPigs(nPigCount)
{
  CreateTimer(1.0, "Host.CreatePig('framerate-dummy');", nPigCount, "CreateDummyPigTimer");
}

function ResetTimer(strTimerName, strFunction, nInterval)
{
  if ("object" == typeof(Properties(strTimerName)))
    Properties(strTimerName).Kill();
  CreateTimer(nInterval, strFunction, -1, strTimerName);
}

function StopTimer(strTimerName)
{
  if ("object" == typeof(Properties(strTimerName)))
    Properties(strTimerName).Kill();
}

// startframeraterecording sends a cheat message to wing 8.
function StartFramerateRecording()
{
  Ship.Team.SendChat("cheat-log 1", 8);
}

// stopframeraterecording sends a cheat message to wing 8.
function StopFramerateRecording()
{
  Ship.Team.SendChat("cheat-log 0", 8);
}

// step1 makes the pig fly outwards from the station, and calls step2 5 secs later.
function StepOne()
{
  ResetTimer("StepTimer", "StepTwo()", 5.0);
  Throttle = 100;
}

// step2 stops the pig, and calls step2b 5 secs later.
function StepTwo()
{
  AllStop();
  ResetTimer("StepTimer", "StepTwoB()", 5.0);
}

// step2b stops the pig, starts framerate recording on the clients, and starts random flipping (spinning around).
// schedules step3 for 30 secs later.
function StepTwoB()
{
  AllStop();
  StartFramerateRecording();
  RandomFlipping();
  ResetTimer("StepTimer", "StepThree()", 30.0);
}

// step3 stops framerate recording, stops the pig, and calls step4 3 secs later.
function StepThree()
{
  StopFramerateRecording();
  AllStop();
  ResetTimer("StepTimer", "StepFour()", 3.0);
}

// step4 points the pig towards <0, 0, 1180>.  calls step5 60 secs later.
function StepFour()
{
  //Ship.Face(Ship.Sector.Stations(0), "AllStop()");
  ResetTimer("StepTimer", "StepFive()", 60.0);
  ShipObservationVector.X = 0;
  ShipObservationVector.Y = 0;
  ShipObservationVector.Z = 1180;

  Ship.Face(ShipObservationVector, "StepFourB()");
}

// step4b turns the pig on to go to the <0, 0, 1180> vector.
function StepFourB()
{
  AllStop();
  CreateTimer(6.0, "ThrustOrbit(ShipObservationVector)", -1, "ThrottleTimer");             // This.CreateTimer
}

// step5 kills the throttle timer, and points the pig back at the station (and also the dummy pigs,
// which are at <0, 0, 1000>).
function StepFive()
{
  if ("object" == typeof(Properties("ThrottleTimer")))
    Properties("ThrottleTimer").Kill();

  AllStop();
  Ship.Face(Ship.Sector.Stations(0), "StepFiveB()");
}

// step5b starts framerate recording, calls step6 30 secs later.
function StepFiveB()
{
  AllStop();
  StartFramerateRecording();
  ResetTimer("StepTimer", "StepFiveC()", 5.0);
}

// step5c thrusts backwards and calls step5d 20 secs later.
function StepFiveC()
{
  Thrust(ThrustBackward);
  ResetTimer("StepTimer", "StepFiveD()", 20.0);
}

// step5d stops and calls step5e 10 secs later.
function StepFiveD()
{
  AllStop();
  ResetTimer("StepTimer", "StepFiveE()", 10.0);
}

// step5e thrusts backwards and calls step5f 10 secs later.
function StepFiveE()
{
  Thrust(ThrustForward);
  ResetTimer("StepTimer", "StepFiveF()", 10.0);
}

// step5f stops and calls step6 5 secs later.
function StepFiveF()
{
  AllStop();
  ResetTimer("StepTimer", "StepSix()", 5.0);
}

// step6 stops framerate recording, calls step7 3 secs later.
function StepSix()
{
  AllStop();
  StopFramerateRecording();
  ResetTimer("StepTimer", "StepSeven()", 3.0);
}

// step7 points the pig at asteroid 0, which should be away from the station somewhere.
function StepSeven()
{
  AllStop();
  StopTimer("StepTimer");
  Ship.Face(Ship.Sector.Asteroids(0), "StepEight()");
}

// step8 starts framerate recording, and calls step8b 5 secs later.
function StepEight()
{
  AllStop();
  StartFramerateRecording();
  ResetTimer("StepTimer", "StepEightB()", 5.0);
}

// step8b starts firing the gun, and calls step8b 20 secs later.
function StepEightB()
{
  ResetTimer("StepTimer", "StepEightC()", 20.0);
  FireWeapon(true);
}

// step8c stops firing the weapon, and calls step8d 5 secs later.
function StepEightC()
{
  FireWeapon(false);
  ResetTimer("StepTimer", "StepEightD()", 5.0);
}

// step8d starts firing missiles, and calls step8e 20 secs later.
function StepEightD()
{
  ResetTimer("StepTimer", "StepEightE()", 20.0);
  FireMissile(true);
}

// step8e stops firing missiles, and calls step8f 5 secs later.
function StepEightE()
{
  FireMissile(false);
  ResetTimer("StepTimer", "StepEightF()", 5.0);
}

// step8f stops framerate recording, and calls step9 3 secs later.
function StepEightF()
{
  ResetTimer("StepTimer", "StepNine()", 3.0);
  StopFramerateRecording();
}

// step9 makes the pig face the station (and the dummy pigs) again.
function StepNine()
{
  StopTimer("StepTimer");
  Ship.Face(Ship.Sector.Stations(0), "StepNineB()");
}

// step9b turns on framerate recording and calls step9c 3 secs later.
function StepNineB()
{
  AllStop();
  StartFramerateRecording();
  ResetTimer("StepTimer", "StepNineC()", 3.0);
}

// step9c tells the dummy pigs to explode and calls step9d 15 secs later.
function StepNineC()
{
  ResetTimer("StepTimer", "StepNineD()", 15.0);
  Ship.Team.SendChat("explode", 9);
}

// step9d stops framerate recording, stops all timers, and displays a message ending the framerate test.
function StepNineD()
{
  StopFramerateRecording();
  StopTimer("StepTimer");
  Ship.Team.SendChat("Framerate Test Complete");
  CommitSuicide();
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
  else if (strCommand == "count")
  {
    dummycounter++;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit(objTarget)
{
  try
  {
    if (range < 900)
      AllStop();
    else
      Ship.Face(objTarget, "");
  }
  catch (e)
  {
    FireWeapon(false);
    FireMissile(false);
    nCorrectionInterval = 2.0;
  }
  Timer.Interval = 2.0;
}

/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit(objTarget)
{
  try
  {
    range = Ship.Position.Subtract(objTarget).LengthSquared;

    if (range < 900)
    {
      AllStop();
    }
    else if (range < 10000)
    {
      Throttle = 5;
    }
    else if (range < 40000)
    {
      Throttle = 10;
    }
    else if (range < 250000)
    {
      Throttle = 20;
    }
    else
    {
      Throttle = 100;
    }

    // Check for empty Ammo
    if (!Ammo)
      CommitSuicide();
    Timer.Interval = 1.0;
  }
  catch (e)
  {
    Trace("Error in ThrustOrbit():\n\t" + e.description);
  }
}

function RandomFlipping()
{
  Roll = (Random() % 200.0) - 100.0;
  Pitch = (Random() % 200.0) - 100.0;
  Yaw = (Random() % 200.0) - 100.0;
}

function GoTo(strIt, radius)
{
  strTarget = strIt;
  StopRadius = radius;
  Throttle = 100;
}

// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

