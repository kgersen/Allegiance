<pigbehavior language="JScript" basebehavior="Default">

<script src="include\common.js"></script>
<script src="include\chatcommand.js"></script>
<script src="include\asteroids.js"></script>
<script src="include\dijkstra.js"></script>
<script src="include\eliza.js"></script>
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
var FinalDestination = null;
var objTarget;
var strOldTarget = "";
var nShieldValue, nHullValue;
var VisitedSectors;
var EnemyStations;
var SectorScanCounter = 6;
var currentSector = " ";
var Retreating = 0;
var shutup = 0;
var HomeSector;

var NotVisitedSectors;
var ClaimedSectors;

// chat commands MUST be LOWERCASE.
var ChatCommandList = new Array ("report", "status off", "status on", "eliza off", "eliza on", "autodonate to me");

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins or creates a mission.
function OnStateMissionList(eStatePrevious)
{
  InitAsteroidLists();
  InitPathGenerator();
  InitEliza();
  VisitedSectors = new ActiveXObject("Scripting.Dictionary");
  NotVisitedSectors = new ActiveXObject("Scripting.Dictionary");
  ClaimedSectors = new ActiveXObject("Scripting.Dictionary");
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
    JoinMission("Bugman's game");                                            // Pig.JoinMission
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
    var v = SelectBestHull(objHullTypes, "Fighter", "Interceptor");
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(v));                                      // Pig.Ship.BuyHull
    Ship.WingID = 9;
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
  var fDuration = 5.0;
  CreateTimer(fDuration, "OnInitialThrottle()", -1, "FlyingTimer");             // This.CreateTimer
  HomeSector = Ship.Sector;
  UpdateAsteroidLists();
  UpdateSectorLists();
  try {
    VisitedSectors.Add(HomeSector, "");
  } catch (e) {
  }
}

/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{

  // Set the timer's interval to 2 seconds
  Timer.Interval = nCorrectionInterval;
  Timer.ExpirationExpression = "CorrectOrbit()";

  ElizaEnabled = 1;
  CreateTimer(10.0, "FindAlephTargets()", -1, "AlephTimer");
  CreateTimer(10.0, "ScanForEnemies(Ship.Sector)", -1, "EnemyScanTimer");
  CreateTimer(10.0, "ScanAsteroidsForOres(Ship.Sector)", -1, "AsteroidScanTimer");
  //CreateTimer(30.0, "UpdateSectorLists()", -1, "SectorScanTimer");
}

/////////////////////////////////////////////////////////////////////////////
function OnReceiveCommand(strCommand, strParams, objShip)
{
  var ReportParams = new Array("u", "he", "si", "c", "enemies", "enemy");
  var paramID = -1;
  if (objShip.Team != Ship.Team)
  {
    objShip.SendChat("I'm not allowed to talk to you.");
    return true;
  }
  if (strCommand == "status off")
  {
    if (RemoveShipFromStatusList(objShip))
      objShip.SendChat("Okay " + objShip.Name + ", I'll stop talking to you now.");
    else
      objShip.SendChat("I'm already not talking to you.");
  }
  else if (strCommand == "status on")
  {
    if (AddShipToStatusList(objShip))
      objShip.SendChat("Okay " + objShip.Name + ", I'll send you status messages now.");
    else
      objShip.SendChat("You're already receiving status messages from me.");
  }
  else if (strCommand == "eliza off")
  {
    objShip.SendChat("Eliza chat responses are now disabled.");
    ElizaEnabled = 0;
  }
  else if (strCommand == "eliza on")
  {
    objShip.SendChat("Eliza chat responses are now enabled.");
    ElizaEnabled = 1;
  }
  else if (strCommand == "autodonate to me")
  {
    objShip.SendChat("I am now autodonating to " + objShip.Name + ".");
    AutoDonate = objShip;
  }
  else if (strCommand == "report")
  {
    // todo: send a message indicating what sector I'm patrolling
    objShip.SendChat("Not yet implemented.");
  }
  return true;
}

function OnShipDamaged(objShip, objModel, fAmount, fLeakage, objV1, objV2)
{
  // if it's not me, return
  if (objShip != Ship)
    return;

  // if the attacker is not a ship, return
  if (objModel.ObjectType != AGC_Ship)
    return;

  // send back a chat message
  if (objModel.Team == Ship.Team)
    objModel.SendChat("Hey, stop shooting me!");
  else
  {
    objModel.SendChat("I'm gonna get you!");
    StatusMessage("I'm being attacked by " + objModel.Name + "!");
  }
}

function OnSectorChange(agcOldSector, agcNewSector)
{
  StatusMessage("Switched from sector " + agcOldSector.Name + " to " + agcNewSector.Name);
  Retreating = 0;
  if (agcNewSector == FinalDestination)
  {
    Trace("\nfinal dest cleared.\n");
    FinalDestination = null;
  }

  if (!VisitedSectors.Exists(agcNewSector))
    SectorScanCounter = 5;
  else
    SectorScanCounter = 0;

  try {
    VisitedSectors.Add(agcNewSector, "");
    Trace("\nhave visited " + agcNewSector + ".\n");
  } catch (e) {
  }

  try {
    NotVisitedSectors.Remove(agcNewSector);
    Trace("\nremoving " + agcNewSector + " from NVS.\n");
  } catch (e) {
  }

}

function UpdateSectorLists()
{
  // clear lists
  NotVisitedSectors.RemoveAll();
  VisitedSectors.RemoveAll();
  ClaimedSectors.RemoveAll();

  // iterate thru sectors
  e = new Enumerator (Game.Sectors);
  for (; !e.atEnd; e.moveNext())
  {
    x = e.item();
    if (x.Asteroids.Count == 0 && x.Alephs.Count > 0)
      NotVisitedSectors.Add(x, "");
    else if (x.Asteroids.Count > 0)
      VisitedSectors.Add(x, "");
  }
}

function ProcessEnemyStation(objStation)
{
  if (EnemyStations.Exists(objStation))
    return;

  StatusMessage("" + objStation.Team.Name + " enemy station found in " + objStation.Sector.Name);
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
    try {
      // look for ships on a different team
      if (x.Team != Ship.Team)
        enemycount++;
    } catch (e) {
      Trace ("------------ bad enemy found ----------\n");
    }
  }

  // iterate thru stations
  var e = new Enumerator (stationlist);
  for(; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    try {
      // look for stations on a different team
      if (x.Team != Ship.Team)
      {
        ProcessEnemyStation(x);
        enemystationcount++;
      }
    } catch (e) {
      Trace("--------------- bad enemy station found --------------\n");
    }
  }

  // report results
  if (shutup == 0 && enemycount > 0)
    StatusMessage("" + enemycount + " enem" + plural(enemycount, "y", "ies") + " spotted in " + Ship.Sector);
  if ((enemystationcount > 0 && enemycount > 2) || (enemycount > 3))
  {
    StatusMessage("I'm getting the hell outta here!");
    Retreating = 1;
  }
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

function FindClosestAleph()
{
  var dist, mindist = 10000000000;
  var closestAleph;
  e = new Enumerator (Ship.Sector.Alephs);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    dist = Ship.Position.Subtract(x.Position).LengthSquared;
    if (dist < mindist)
    {
      mindist = dist;
      closestAleph = x;
    }
  }
  return closestAleph;
}

function ProcessSectorAlephs(agcSector)
{
  e = new Enumerator(agcSector.Alephs);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    s = x.Destination.Sector;
    if (VisitedSectors.Exists(s) || NotVisitedSectors.Exists(s))
      continue;
    NotVisitedSectors.Add(s, "");
  }
}

function NonVisitedAlephInSector(agcSector)
{
  var destinations = new Array();

  e = new Enumerator (agcSector.Alephs);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    if (!VisitedSectors.Exists(x.Destination.Sector))
      destinations = destinations.concat(new Array(x.Destination.Sector));
  }
  if (destinations.length == 0)
    return null;
  else
    return destinations[Math.floor(Math.random() * destinations.length)];
}

function NonVisitedRemoteSector()
{
  var destinations = new Array();
  mindist = 100;

  sectorarray = (new VBArray(NotVisitedSectors.Keys())).toArray();

  return sectorarray[Math.floor(Math.random() * sectorarray.length)];
}

function FindAlephTargets()
{
  var choice = "";

  // if pig is retreating, find the closest aleph and go to it.
  if (Retreating == 1)
  {
    Timer.Interval = 5;
    //Trace("\n\nsection 1\n");
    aleph = FindClosestAleph();
    strTarget = aleph.Name;
    StatusMessage("Retreating to " + strTarget);
    return;
  }

  // if pig is scanning, do the scan.
  if (SectorScanCounter > 0)
  {
    Timer.Interval = 15;
    //Trace("\n\nsection 3\n");
    ScanAsteroidsForTarget();
    StatusMessage("I'm looking for stuff in " + Ship.Sector.Name + " (" + SectorScanCounter + ")");
    SectorScanCounter--;
    return;
  }

  // if pig is on a mission to another sector, find the path aleph and go to it
  if (FinalDestination != null)
  {
    Timer.Interval = 10;
    //Trace("\n\nsection 2\n");
    closestAleph = NextAlephInShortestPath(Ship.Sector, FinalDestination);
    strTarget = closestAleph.Name;
    StatusMessage("Flying to " + FinalDestination + " via " + strTarget);
    return;
  }

  Timer.Interval = 10;

  //Trace("\n\nsection 4\n");
  // add recently scanned alephs to database
  ProcessSectorAlephs(Ship.Sector);

  //Trace("\n\nsection 5\n");
  // pick a local non-visited aleph to go to
  choice = NonVisitedAlephInSector(Ship.Sector);

  //Trace("\n\nsection 6\n");
  if (choice == null)
  {
    FinalDestination = NonVisitedRemoteSector();
    closestAleph = NextAlephInShortestPath(Ship.Sector, FinalDestination);
    strTarget = closestAleph.Name;
    StatusMessage("Flying to " + FinalDestination + " via " + strTarget);
  }
  else
  {
    FinalDestination = choice;
    strTarget = choice;
    StatusMessage("Flying to " + strTarget);
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

