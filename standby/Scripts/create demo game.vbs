
'ServerName = InputBox("Enter the remote server (or nothing for local)", "  Note: Be sure server is running.")
'TotalGames = InputBox("Enter the number of games", "  Note: Be sure server is running.")


Dim objCreateParams

Set objCreateParams = CreateObject("AGC.GameParameters")


'
' Let's set a couple params, note that ALL mission
' params are currently changable.
'
objCreateParams.AllowJoiners = true
objCreateParams.Lives = 10

'
' Need to set these because the default values currently 
' cause the game to crash
'
objCreateParams.CivIDs(0) = 2
objCreateParams.CivIDs(1) = 2
objCreateParams.CivIDs(2) = 2 
objCreateParams.CivIDs(3) = 2
objCreateParams.CivIDs(4) = 2
objCreateParams.CivIDs(5) = 2

'
' Make some games with these params
'
Set objSession = CreateObject("AllSrv.AdminSession")
Set objGames = objSession.Games


'For i = 1 to TotalGames
'	objCreateParams.GameName = "Deathmatch " & i
'	objGames.Add(objCreateParams)
'Next

objCreateParams.MapType = 3
objCreateParams.MaxPlayers = 20
objCreateParams.MinPlayers = 1
objCreateParams.IsGoalConquest = false
objCreateParams.Teams = 6
objCreateParams.GameName = "Scout Test Game"
objGames.Add(objCreateParams)

