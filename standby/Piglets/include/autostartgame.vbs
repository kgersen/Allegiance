'///////////////////////////////////////////////////////////////////////////
' AutoStartGame.js : Code needed to automatically start a game once the
' minimum number of players per team have joined the game.
'

'--- Error messages and codes
Const N_WRONGSTATE = &H800C0001
Const S_WRONGSTATE = "Method called in wrong state."

'///////////////////////////////////////////////////////////////////////////
' Description: Creates a timer to check for the presence of the minimum
' number of players in each team.
'
' Parameters:
'   cMinTeamPlayers - The minimum number of players per team required to
' start the game. This is passed-in as a convenience, since I'm not sure
' that this value can be derived from the current object model.
'
' See Also: AutoStartGame_Tick
'
sub AutoStartGame(cMinTeamPlayers)

  if PigState_WaitingForMission <> PigState then
    err.Raise N_WRONGSTATE, "Cannot call AutoStartGame unless PigState is PigState_WaitingForMission", S_WRONGSTATE
    exit sub
  end if

  Dim strExpression
  strExpression = "AutoStartGame_Tick(" & cMinTeamPlayers & ")"
  CreateTimer 1.0, strExpression, -1, "AutoStartGameTimer"

end sub


'///////////////////////////////////////////////////////////////////////////
' Description: Called for each interval of the AutoStartGameTimer.
'
' Parameters:
'   cMinTeamPlayers - The minimum number of players per team required to
' start the game. This is passed-in as a convenience, since I'm not sure
' that this value can be derived from the current object model.
'
' Checks for the minimum number of players on each team of the game. When
' this is true, the timer is killed and the game is started.
'
' See Also: AutoStartGame
'
sub AutoStartGame_Tick(cMinTeamPlayers)

  for each e in Game.Teams
    if e.Ships.Count < cMinTeamPlayers then
      exit sub
    end if
  next

  ' Kill the timer
  Timer.Kill()

  ' Start the game
  StartGame()

end sub

