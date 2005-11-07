<pigbehavior language="JScript">

<script src="include\AutoStartGame.js"/>

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//


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
      UpdatesPerSecond = 20;
      Logon();                                                             // Pig.Logon
    }
    catch (e)
    {
      // Logon failed, so shutdown the pig
      Trace("\nLogon failed: " + e.description + "\n");
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
  objParams.MinTeamPlayers = 3;
  objParams.MaxTeamPlayers = 10;
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
// Selects the best hull from a collection of hulls.  Scans the list, noting
// the index of the Fighter and the Interceptor.  If a Fighter was found, it
// returns that index.  Otherwise, the index of the Interceptor is returned.
// If an Interceptor is not found, returns 0, which should be a scout or a
// recovery ship.
//
function SelectBestHull(objHullTypes)
{
  var FighterHull = -1, InterceptorHull = 0;
  var e = new Enumerator(objHullTypes)

  // loop thru collection
  var strTrace = "Hull types:\n";
  for (var i = 0; !e.atEnd(); e.moveNext(), ++i)
  {
    hull = e.item();
    strTrace += "  " +  i + ". " + hull.Name + "\n";

    // search for "Interceptor" in the name
    if (hull.Name.search("Interceptor") != -1)
      InterceptorHull = i;

    // search for "Fighter" in the name
    if (hull.Name.search("Fighter") != -1)
      FighterHull = i;    
  }
  strTrace += "Fighter: " + FighterHull + " Interceptor: " + InterceptorHull + "\n";

  // look for valid Fighter index
  if (FighterHull != -1)
  {
    // fighter found, return index
    Trace(strTrace + "Selecting fighter.\n");
    return FighterHull;
  }
  else
  {
    // interceptor found, or default, return index
    Trace (strTrace + "Selecting interceptor.\n");
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
    var objHullTypes = HullTypes;                                          // Pig.HullTypes (should be Pig.Station.HullTypes)
    var cHullTypes = objHullTypes.Count;
    Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");      // Host.Trace

    // Select and buy the best hull available
    var iHull = SelectBestHull(objHullTypes);
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(iHull))                                    // Pig.Ship.BuyHull
  }
  catch (e)
  {
    Trace("\n" + e.description + "\n");                                    // Host.Trace
  }

  // Schedule the Launch method (to avoid an IGC assert)
	CreateTimer(1.0, "Timer.Kill(); Launch();", -1, "LaunchTimer");
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  // Set the ship's throttle to 100%
  Throttle = 100;                                                          // Pig.Ship.Throttle

  // Generate a random number of seconds to throttle
  var nDuration = 5.0 + (Random() % 7);

  // Create a timer to reset the throttle
  CreateTimer(nDuration, "OnInitialThrottle()", -1, "FlyingTimer");        // This.CreateTimer
}


/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the ship's throttle to 0%
  Ship.Throttle = 0;                                                       // Pig.Ship.Throttle

  // Thrust left for awhile
  Ship.Thrust(ThrustLeft);                                                 // Pig.Ship.Thrust

  // Generate a random number of seconds to thrust left
  var nDuration = 5.0 + (Random() % 10);

  // Adjust the timer to reset the thrust, kill the timer, and face the outpost
  Timer.Interval = nDuration;                                              // This.Timer
  Timer.ExpirationExpression = "OnEndThrusting()";
}


/////////////////////////////////////////////////////////////////////////////
function OnEndThrusting()
{
  // Kill the timer
  Timer.Kill();

  // Stop the ship from thrusting
  Ship.AllStop();
}


/////////////////////////////////////////////////////////////////////////////
// Chat globals
//
var g_strChatText = "";


/////////////////////////////////////////////////////////////////////////////
function StartChatting(cch)
{
  if (cch < 1)
    return "Must specify a number of characters that is at least 1";


  var strAll = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  var cchAll = strAll.length;

  g_strChatText = "";
  var cchRemain = cch;
  while (cchRemain > 0)
  {
    var cchCopy = Math.min(cchRemain, cchAll);
    g_strChatText += strAll.substr(0, cchCopy);
    cchRemain -= cchCopy;
  }

  // Set a timer, if it doesn't already exist
  if ("object" != typeof(Properties("ChatTimer")))
    CreateTimer(0, "Game.SendChat(g_strChatText);", -1, "ChatTimer");
}

function StopChatting()
{
  // Kill the chatting timer, if one exists
  if ("object" == typeof(Properties("ChatTimer")))
    Properties("ChatTimer").Kill();
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

