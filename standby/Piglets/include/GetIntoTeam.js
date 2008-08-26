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
      Logon();
    }
    catch (e)
    {
      // Logon failed, so shutdown the pig
      Shutdown();
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
    JoinMission("Pig Mission");
    return;
  } 
  catch (e)
  {
    // catch here just keeps JoinMission errors from being fatal
  }

  // Mission didn't exist, join any other current mission, if any
  try
  {
    JoinMission();
    return;
  }
  catch (e)
  {
    // catch here just keeps JoinMission errors from being fatal
  }

  // No missions exist, create one and join it
  var objParams = new ActiveXObject("Pigs.MissionParams");
  objParams.TeamCount = 2;
  objParams.MinTeamPlayers = 3;
  objParams.MaxTeamPlayers = 10;
  CreateMission(objParams);

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
      JoinTeam();
    }
    catch (e)
    {
      Trace("\n" + e.description + " - Attempting to QuitGame\n");
      QuitGame();
    }
  }
}

