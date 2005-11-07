
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Create the Session Information object

'Dim objInfo
'Set objInfo = CreateObject("Pigs.PigSessionInfo")
'objInfo.ApplicationName = WScript.ScriptFullName


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Connect to the Pig Server

Dim objSession
Set objSession = CreatObject("Pigs.PigSession")
'objSession.SessionInfo = objInfo


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Create a single pig with a "Default" behavior

Dim objPig
Set objPig = objSession.CreatePig()

' Logon to an Allegiance server
WScript.Echo("Logging on to an Allegiance server...")
objPig.Logon "JTASLER-NT-000", "password", "JTASLER-NT"

' Create a MissionParameters object
WScript.Echo("Creating mission parameters...")
Dim objMissionParams
Set objMissionParams = CreateObject("Pigs.MissionParams")
objMissionParams.TeamCount      = 2
objMissionParams.MaxTeamPlayers = 8
objMissionParams.MinTeamPlayers = 1
objMissionParams.MapType        = PigMapType_SingleRing

' Create a new mission
WScript.Echo("Creating a new mission...")
objSession.CreateMission(objMissionParameters)
WScript.Echo("Mission Created!")

