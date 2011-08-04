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

// nCorrectionInterval controls the update rate of the ship's heading.
var nCorrectionInterval = 2.0;
// fTimerOn disables the updating of the ship rate heading interval.
var fTimerOn = true;
// strTarget is the master target variable.  It is a string containing the name of
// the current target.  The ship heading code reads this.  This must be a valid target
// at all times (the scout never stops flying, unless it dies).
var strTarget = "";
// FinalDestination is a pointer to a final destination sector.  When a scout is heading
// to a remote sector, using a path that takes it thru other sectors, this variable
// is the final sector.
var FinalDestination = null;

// The following three variables are dictionary objects containing the sector visitation
// lists.  VisitedSectors represents all of the sectors that all ships on the team have been to
// for any amount of time.  NonVisitedSectors is the list of sectors that have not been visited,
// but their locations (the aleph(s) that go to them) are known.
// Here are the set descriptions of the interactions of these variables:
// {VisitedSectors} intersect {NonVisitedSectors} == {null}
// {VisitedSectors} union {NonVisitedSectors} <= {All Sectors in Game}
// The size of VisitedSectors strictly increases over the course of the game.
// ClaimedSectors is not currently used.  It was intended to hold the non-visited sectors that
// had been "claimed" by other scout2 pigs, and therefore should not be explored unless no other
// non-visited sectors exist.  This would yield exploration efficiency improvements when multiple
// scouts are used.
var VisitedSectors;
var NotVisitedSectors;
var ClaimedSectors;

// EnemyStations is a list of the enemy stations that have been found by this scout.
var EnemyStations;

// SectorScanCounter represents the number of "scan intervals" a scout should spend flying around
// a sector, looking for stuff.  The interval length is set in FindAlephTargets.
var SectorScanCounter = 6;
// Retreating is 1 when the pig is trying to leave the current sector as quickly as possible.
var Retreating = 0;

// ChatCommandList is an array of string commands that can be received from chats.  The OnReceiveChat event handler
// in chatcommand.js uses this array.
// chat commands MUST be LOWERCASE.
var ChatCommandList = new Array ("report", "status off", "status on", "eliza off", "eliza on", "autodonate to me");

// OnActivate sets up a few data structures and such when the scout is activated.
// It also issues a OnDocked state transition.
function OnActivate(objPrevPig)
{
  Trace("\n\nActivating scout2...\n\n");
  InitAsteroidLists();
  InitPathGenerator();
  InitEliza();
  VisitedSectors = new ActiveXObject("Scripting.Dictionary");
  NotVisitedSectors = new ActiveXObject("Scripting.Dictionary");
  ClaimedSectors = new ActiveXObject("Scripting.Dictionary");
  EnemyStations = new ActiveXObject("Scripting.Dictionary");
  if (PigState == PigState_Docked)
  {
    OnStateDocked(PigState_Flying);
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
    JoinMission("Ribski's game");                                            // Pig.JoinMission
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
  else if (PigState_Flying == ePrev || PigState_Docked == ePrev)
  {
	  // If we were flying before, we need to schedule the Launch method
	  CreateTimer(3.0, "OnDoLaunch();", -1, "LaunchTimer");
  }
}

// launch helper.
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
  UpdateAsteroidLists();
  UpdateSectorLists();
  try {
    VisitedSectors.Add(Ship.Sector, "");
  } catch (e) {
  }
}

/////////////////////////////////////////////////////////////////////////////
// OnInitialThrottle sets up the machinery for flight, navigation, and resource detection.
// * The timer that calls this changes its event to CorrectOrbit, which perpetually
//   points the scout in the right direction.
// * FindAlephTargets makes sure the scout always has a valid flight destination.
//   All of the navigation is kept in this routine.
// * ScanForEnemies keeps track of enemy activity around the pig.
// * ScanAsteroidsForOres looks for valuable rocks.
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
// OnReceiveCommand is called by OnReceiveChat in chatcommand.js.
// The command string is provided in two pieces - the command and the string
// following the command.  The commanding ship is also passed in for sending
// a reply message or what not.
// The structure is pretty straightforward - it selects on the command, and
// then does the associated action (possibly parsing the parameter string).
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
    for (key in ReportParams)
    {
      if (strParams.search(ReportParams[key]) != -1)
        paramID = new Number(key);
    }
    if (paramID == 0)
    {
      objShip.SendChat(SectorAsteroidCount("Uranium"));
    }
    else if (paramID == 1)
    {
      objShip.SendChat(SectorAsteroidCount("Helium3"));
    }
    else if (paramID == 2)
    {
      objShip.SendChat(SectorAsteroidCount("Silicon"));
    }
    else if (paramID == 3)
    {
      objShip.SendChat(SectorAsteroidCount("Carbon"));
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

// OnShipDamaged is called when the ship is hit.  It's pretty useless right now... it
// just makes the scout whine to its team when it is hit.  Pretty lame.
// The ideal would be to make the pig run away or fight when it is attacked.
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

// OnSectorChange is called when the pig changes sectors.  It updates a few data structures,
// and resets a few counters for navigation purposes.
function OnSectorChange(agcOldSector, agcNewSector)
{
  StatusMessage("Switched from sector " + agcOldSector.Name + " to " + agcNewSector.Name);
  Retreating = 0;
  // check for reaching final destination
  if (agcNewSector == FinalDestination)
  {
    Trace("\nfinal dest cleared.\n");
    FinalDestination = null;
  }
  
  // if the new sector has not been visited, set the scan counter so the scout will
  // fly around the sector for a while, explorng and stuff.
  if (!VisitedSectors.Exists(agcNewSector))
    SectorScanCounter = 5;
  else
    SectorScanCounter = 0;

  // add the new sector to the visited sectors list
  try {
    VisitedSectors.Add(agcNewSector, "");
    Trace("\nhave visited " + agcNewSector + ".\n");
  } catch (e) {
  }

  // remove the new sector from the not visited sectors list
  try {
    NotVisitedSectors.Remove(agcNewSector);
    Trace("\nremoving " + agcNewSector + " from NVS.\n");
  } catch (e) {
  }

}

// UpdateSectorLists rebuilds the visitation lists from the sector info contained
// in AGC.  Visited sectors are defined as sectors that have more than 0 asteroids
// visible (if you visit a sector, you're bound to expose at least 1 asteroid).
function UpdateSectorLists()
{
  // clear visitation lists
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

// ProcessEnemyStation reports a status message if the station has not been seen
// before.  It also adds it to the enemy station list if it isn't there already.
function ProcessEnemyStation(objStation)
{
  if (EnemyStations.Exists(objStation))
    return;

  StatusMessage("" + objStation.Team.Name + " enemy station found in " + objStation.Sector.Name);
  EnemyStations.Add(objStation, Ship.Name);
}

// ScanForEnemies assesses the enemy threat in a sector.  If the enemy count passes
// a threshold, the scout sets the Retreating variable to 1.
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
  if (enemycount > 0)
    StatusMessage("" + enemycount + " enem" + plural(enemycount, "y", "ies") + " spotted in " + Ship.Sector);

  // threshold: 2 enemies and a station or 3 enemies
  if ((enemystationcount > 0 && enemycount > 2) || (enemycount > 3))
  {
    StatusMessage("I'm getting the hell outta here!");
    Retreating = 1;
  }
}

// DumpEnemyStationList returns a string representation of the enemy stations
// found, and where they were found.  This info is obtained from the dictionary
// object dictStations that is passed in.
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

// ScanAsteroidsForTarget sets strTarget to the name of a random asteroid in the sector
// that is more than 3500 units away.
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
    if (dist > 12250000)    // 12250000 == 3500^2
    {
      maxasteroid = maxasteroid.concat(new Array(x.Name));
    }
    
  }

  // select one of the far asteroids at random
  if (maxasteroid.length > 0)
    strTarget = maxasteroid[Math.floor(Math.random() * maxasteroid.length)];
  else
    strTarget = asteroidList[0].Name;   // no asteroids over 3500 units away (weird) - select asteroid 0
}

// FindClosestAleph returns the closest aleph to the ship's current location.
// This is used when the pig is retreating and needs to make a hasty exit.
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

// ProcessSectorAlephs checks all of the alephs in a given sector.
// Any aleph destination sectors that are not visited are added to
// the not visited sectors list.
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

// NonVisitedAlephInSector returns an aleph in the given sector that goes to
// a sector that is not visited.  If no such aleph exists, null is returned.
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

// NonVisitedRemoteSector returns a random non-visited sector from the not
// visited sectors list.  If there are no sectors in the not visited sectors
// list, null is returned.
function NonVisitedRemoteSector()
{
  var destinations = new Array();
  mindist = 100;

  sectorarray = (new VBArray(NotVisitedSectors.Keys())).toArray();

  if (sectorarray.length == 0)
    return null;

  return sectorarray[Math.floor(Math.random() * sectorarray.length)];
}

// FindAlephTargets is the main navigation routine.  Based on the state of a few
// variables, the target variable strTarget is set to the desired destination
// within the current sector.  Based on the conditions, FinalDestination may be
// set as well.
function FindAlephTargets()
{
  var choice = "";

  // if pig is retreating, find the closest aleph and go to it.
  if (Retreating == 1)
  {
    Timer.Interval = 5;
    aleph = FindClosestAleph();
    strTarget = aleph.Name;
    StatusMessage("Retreating to " + strTarget);
    return;
  }

  // if pig is scanning for stuff, do the scan.
  if (SectorScanCounter > 0)
  {
    Timer.Interval = 25;
    // find an asteroid to fly to
    ScanAsteroidsForTarget();
    StatusMessage("I'm looking for stuff in " + Ship.Sector.Name + " (" + SectorScanCounter + ")");
    // decrement the scan counter
    SectorScanCounter--;
    return;
  }

  // if pig is on a mission to another sector, find the path aleph and go to it
  if (FinalDestination != null)
  {
    Timer.Interval = 10;
    // find the next aleph in the path to the final destination (dijkstra.js)
    closestAleph = NextAlephInShortestPath(Ship.Sector, FinalDestination);
    strTarget = closestAleph.Name;
    StatusMessage("Flying to " + FinalDestination + " via " + strTarget);
    return;
  }

  Timer.Interval = 10;

  // add recently scanned alephs to database
  ProcessSectorAlephs(Ship.Sector);

  // pick a local non-visited aleph to go to
  choice = NonVisitedAlephInSector(Ship.Sector);

  // check if the selection is valid
  if (choice == null)
  {
    // find a remote sector to visit
    FinalDestination = NonVisitedRemoteSector();
    // if FinalDestination is null, then there is no more non-visited sectors to explore
    if (FinalDestination == null)
    {
      // pick a random destination sector not equal to the current sector
      do
      {
        FinalDestination = Game.Sectors(Math.floor(Math.random() * Game.Sectors.Count));
      }
      while (FinalDestination == Ship.Sector);
    }
    // select the local aleph that will take us on the path there
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
// CorrectOrbit continually faces the target specified by strTarget.
function CorrectOrbit()
{
  try
  {
    Ship.Face(strTarget, "ThrustOrbit();");
    nCorrectionInterval = 1.5;
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

// ProperVelocity sets the proper velocity for the ship.  It's in its own function'
// so it can be changed to something more intelligent later.
function ProperVelocity()
{
  Throttle = 100;
  Thrust(ThrustForward);
}   

/////////////////////////////////////////////////////////////////////////////
// ThrustOrbit maintains the proper speed of the ship.  Not too exciting in the
// scout, because it always flies at 100 throttle straight ahead.
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

