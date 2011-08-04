<pigbehavior language="JScript" basebehavior="Default">

<script src="include\common.js"></script>
<script src="include\chatcommand.js"></script>
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
var nThrottle = 100;
var strTarget = "";
var objTarget;
var strOldTarget = "";
var nShieldValue, nHullValue;
var visitedSectors;
var UAsteroids;
var SiAsteroids;
var HeAsteroids;
var CAsteroids;
var EnemyStations;
var GiveUpCounter = 2;
var currentSector = " ";
var Retreating = 0;
var shutup = 0;

// chat commands MUST be LOWERCASE.
var ChatCommandList = new Array ("report", "hello", "hi", "shut up", "speak");

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins or creates a mission.
function OnStateMissionList(eStatePrevious)
{
  visitedSectors = new ActiveXObject("Scripting.Dictionary");
  UAsteroids = new ActiveXObject("Scripting.Dictionary");
  SiAsteroids = new ActiveXObject("Scripting.Dictionary");
  HeAsteroids = new ActiveXObject("Scripting.Dictionary");
  CAsteroids = new ActiveXObject("Scripting.Dictionary");
  EnemyStations = new ActiveXObject("Scripting.Dictionary");

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
    JoinMission("Scout Test Game");                                            // Pig.JoinMission
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
// Handles state transition. Launches the pig as soon as it becomes docked.
function OnStateDocked(ePrev)
{
  // Kill the flying timer, if one exists
  if ("object" == typeof(Properties("FlyingTimer")))
    Properties("FlyingTimer").Kill();
  if ("object" == typeof(Properties("AlephTimer")))
    Properties("AlephTimer").Kill();
  if ("object" == typeof(Properties("EnemyScanTimer")))
    Properties("EnemyScanTimer").Kill();
  if ("object" == typeof(Properties("AsteroidScanTimer")))
    Properties("AsteroidScanTimer").Kill();

  try
  {
    // Get the list of hull types
    var objHullTypes = HullTypes;                                        // Pig.HullTypes (should be Pig.Station.HullTypes)
    var cHullTypes = objHullTypes.Count;
    Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");    // Host.Trace

    // Buy the best hull (or else we'll be in an eject pod)
    var v = SelectBestHull(objHullTypes, "scout", "Scout");
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
	CreateTimer(3.0, "OnDoLaunch();", -1, "LaunchTimer");
  }
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

  // Set the timer's interval to 2 seconds
  Timer.Interval = nCorrectionInterval;
  Timer.ExpirationExpression = "CorrectOrbit()";

  CreateTimer(10.0, "FindAlephTargets()", -1, "AlephTimer");
  CreateTimer(10.0, "ScanForEnemies(Ship.Sector)", -1, "EnemyScanTimer");
  CreateTimer(10.0, "ScanAsteroidsForOres(Ship.Sector)", -1, "AsteroidScanTimer");
}

/////////////////////////////////////////////////////////////////////////////
function OnReceiveCommand(strCommand, strParams, objShip)
{
  var ReportParams = new Array("u", "he", "si", "c", "enemies", "enemy");
  var paramID = 0;
  //Trace(objShip + " gave the command '" + strCommand + "' with params '" + strParams + "'\n");
  if (objShip.Team != Ship.Team)
  {
    objShip.SendChat("I'm not allowed to talk to you.");
    return true;
  }
  if (strCommand == "hello" || strCommand == "hi")
  {
    objShip.SendChat("Hello there!");
  }
  else if (strCommand == "shut up")
  {
    objShip.SendChat("Okay, I'll stop talking now.");
    shutup = 1;
  }
  else if (strCommand == "speak")
  {
    objShip.SendChat("I'll be reporting my status now.");
    shutup = 0;
  }
  else if (false && strCommand == "report")
  {
    objShip.SendChat("Sorry, a bandaid fix for a bug is in place.  Check back another day.");
  }
  else if (strCommand == "report")
  {
    for (key in ReportParams)
    {
      if (strParams.search(ReportParams[key]) != -1)
        paramID = new Number(key);
    }
    if (paramID == 0)
    {
      objShip.SendChat(DumpAsteroidList(UAsteroids, "Uranium"));
    }
    else if (paramID == 1)
    {
      objShip.SendChat(DumpAsteroidList(HeAsteroids, "Helium3"));
    }
    else if (paramID == 2)
    {
      objShip.SendChat(DumpAsteroidList(SiAsteroids, "Silicon"));
    }
    else if (paramID == 3)
    {
      objShip.SendChat(DumpAsteroidList(CAsteroids, "Carbon"));
    }
    else if (paramID == 4 || paramID == 5)
    {
      objShip.SendChat(DumpEnemyStationList(EnemyStations));
    }
    else
    {
      objShip.SendChat("I can't report anything on that. Try U, Si, C, He, or Enemies.");
    }
  }
  return true;
}

/*
function OnShipDamaged(objShip, objModel, fAmount, fLeakage, objV1, objV2)
{
  if (objShip != Ship)
    return;
  if (objModel.Team == Ship.Team)
    objModel.SendChat("Hey, stop shooting me!");
  else
    objModel.SendChat("I'm gonna get you!");
}
*/

function ProcessEnemyStation(objStation)
{
  if (EnemyStations.Exists(objStation))
    return;

  if (shutup == 0)
    Ship.Team.SendChat("" + objStation.Team.Name + " enemy station found in " + objStation.Sector.Name);
  EnemyStations.Add(objStation, Ship.Name);
}

function ScanForEnemies(agcSector)
{
  var shiplist = agcSector.Ships;
  var stationlist = agcSector.Stations;
  var enemycount = 0;
  var enemystationcount = 0;
  var x;

  // iterate thru ships
  var e = new Enumerator (shiplist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    // look for ships on a different team
    if (x.Team != Ship.Team)
      enemycount++;
  }

  // iterate thru stations
  var e = new Enumerator (stationlist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    // look for stations on a different team
    if (x.Team != Ship.Team)
    {
      ProcessEnemyStation(x);
      enemystationcount++;
    }
  }

  // report results
  if (shutup == 0 && enemycount > 0)
    Ship.Team.SendChat("" + enemycount + " enem" + plural(enemycount, "y", "ies") + " spotted in " + Ship.Sector);
  if ((enemystationcount > 0 && enemycount > 2) || (enemycount > 3))
  {
    if (shutup == 0)
      Ship.Team.SendChat("I'm getting the hell outta here!");
    Retreating = 1;
  }
}

function CheckOreAsteroid(objAsteroid)
{
  if (objAsteroid.Name.charAt(0) == 'a')
    return;

  if (UAsteroids.Exists(objAsteroid) || SiAsteroids.Exists(objAsteroid) || CAsteroids.Exists(objAsteroid) || HeAsteroids.Exists(objAsteroid))
    return;

  if (objAsteroid.Name.charAt(0) == 'U')
  {
    if (shutup == 0)
      Ship.Team.SendChat("Uranium asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    UAsteroids.Add(objAsteroid, Ship.Name);
  } 
  else if (objAsteroid.Name.charAt(0) == 'S')
  {
    if (shutup == 0)
      Ship.Team.SendChat("Silicon asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    SiAsteroids.Add(objAsteroid, Ship.Name);
  } 
  else if (objAsteroid.Name.charAt(0) == 'H')
  {
    if (shutup == 0)
      Ship.Team.SendChat("Helium3 asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    HeAsteroids.Add(objAsteroid, Ship.Name);
  } 
  else if (objAsteroid.Name.charAt(0) == 'C')
  {
    if (shutup == 0)
      Ship.Team.SendChat("Carbon asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    CAsteroids.Add(objAsteroid, Ship.Name);
  }
}

function DumpAsteroidList(dictAsteroid, strTitle)
{
  if (dictAsteroid.Count == 0)
  {
    return ("I haven't found any " + strTitle + " asteroids.");
  }
  var header = strTitle + " asteroids in ";
  var s = "";
  e = new Enumerator(dictAsteroid);
  Trace("*******************************************\n");
  while (!e.atEnd())
  {
    try
    {
      Trace("getting x\n");
      x = e.item();
      Trace("getting x.sector.name\n");
      s += x.Sector.Name;
      Trace("moving to next item\n");
      e.moveNext();
      if (!e.atEnd())
        s += ", ";
    }
    catch (error)
    {
      Trace("bad asteroid found\n");
      try
      {
        Trace ("removing bad asteroid.\n");
        dictAsteroid.Remove(x);
      }
      catch (removeerror)
      {
        Trace("removal failed.\n");
      }
      e.moveNext();
    }
  }
  return s;
}

function DumpEnemyStationList(dictStations)
{
  if (dictStations.Count == 0)
  {
    return ("I haven't found any enemy stations.");
  }
  var s = "I found ";
  e = new Enumerator(dictStations);
  while (!e.atEnd())
  {
    try
    {
      x = e.item();
      s += "a " + x.Team.Name + " enemy station in " + x.Sector.Name;
      e.moveNext();
      if (!e.atEnd())
        s += ", ";
    }
    catch (error)
    {
      e.moveNext();
      Trace("bad station found\n");
    }
  }
  return s;
}

function ScanAsteroidsForTarget()
{
  Timer.Interval = 30;

  // get collection of asteroids in the current sector
  var asteroidList = Ship.Sector.Asteroids;
  var dist;
  var maxasteroid = new Array();

  // iterate through asteroid collection
  e = new Enumerator (asteroidList);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    // calc distance squared to asteroid
    dist = Ship.Position.Subtract(x.Position).LengthSquared;
    // save names of asteroids over 3500 units away (arbitrary)
    if (dist > 12250000)
    {
      maxasteroid = maxasteroid.concat(new Array(x.Name));
    }
    
  }

  // select one of the far asteroids at random
  if (maxasteroid.length > 0)
    strTarget = maxasteroid[Math.floor(Math.random() * maxasteroid.length)];
  else
    strTarget = asteroidList[0].Name;
}

function ScanAsteroidsForOres(agcSector)
{
  // get collection of asteroids in the current sector
  var asteroidList = agcSector.Asteroids;
  var dist;
  var maxasteroid = new Array();

  // iterate through asteroid collection
  e = new Enumerator (asteroidList);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    CheckOreAsteroid(x);
  }

}

function FindAlephTargets()
{
  var alephList = Ship.Sector.Alephs;
  var destinations = new Array();
  var result = Ship.Sector.Name;
  var dist, mindist = 10000000000;
  var closestAleph;

  try {
    visitedSectors.add (Ship.Sector.Name, "visited");
  } catch (e) {
  }

  if (currentSector == Ship.Sector.Name)
    return;

  if (Retreating == 0 && alephList.Count < 2)
  {
    if (GiveUpCounter > 0)
    {
      ScanAsteroidsForTarget();
      GiveUpCounter--;
    }
    else
    {
      if (shutup == 0)
        Ship.Team.SendChat("Returning to previously visited sector.");
      //visitedSectors.RemoveAll();
    }
  }
  else
  {
    e = new Enumerator (alephList);
    for (; !e.atEnd(); e.moveNext())
    {
      x = e.item();
      if (!visitedSectors.exists(x.Name) || GiveUpCounter == 0)
        destinations = destinations.concat(new Array(x.Name));
      dist = Ship.Position.Subtract(x.Position).LengthSquared;
      if (dist < mindist)
      {
        mindist = dist;
        closestAleph = x;
      }
    }
  
    if (Retreating == 0 && destinations.length == 0)
    {
      if (GiveUpCounter > 0)
      {
        ScanAsteroidsForTarget();
        GiveUpCounter--;
      }
      else
      {
        if (shutup == 0)
          Ship.Team.SendChat("Returning to previously visited sector.");
        //visitedSectors.RemoveAll();
      }
    }
    else
    {
      if (Retreating == 0)
        strTarget = destinations[Math.floor(Math.random() * destinations.length)];
      else
        strTarget = closestAleph.Name;
      currentSector = Ship.Sector.Name;
      GiveUpCounter = 4;
      if (shutup == 0)
        Ship.Team.SendChat("Flying to " + strTarget);
      Timer.Interval = 10;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    Ship.Face(strTarget, "ThrustOrbit();");
    nCorrectionInterval = 2.0;
  }
  catch (e)
  {
    AllStop();
		FireWeapon(false);
		FireMissile(false);
    nCorrectionInterval = 5.0;
  }
  if (fTimerOn)
    Timer.Interval = nCorrectionInterval;
}

function ProperVelocity()
{
  Throttle = 100;
  Thrust(ThrustForward);
}   

/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{

    ProperVelocity();

		// Check for empty Ammo
		if (!Ammo)
			CommitSuicide();
	}
	catch (e)
	{
    Host.Trace("*************** Something's Wrong!!!! ***********************\n");
	}
}



// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

