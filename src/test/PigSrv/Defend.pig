<pigbehavior language="JScript">

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
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
  // Avoid repeated attempts to join a team
  if (PigState_JoiningTeam != eStatePrevious)
    JoinTeam();                                                            // Pig.JoinTeam
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Launches the pig as soon as it becomes docked.
function OnStateDocked(ePrev)
{
  try
  {
    // Get the list of hull types
    var objHullTypes = HullTypes;                                        // Pig.HullTypes (should be Pig.Station.HullTypes)
    var cHullTypes = objHullTypes.Count;
    Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");    // Host.Trace

    // Buy a random hull (or else we'll be in an eject pod)
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(1))                                      // Pig.Ship.BuyHull
//      Ship.BuyHull(objHullTypes(Random() % cHullTypes))                  // Pig.Ship.BuyHull, Host.Random
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
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  Defend("Outpost");
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

