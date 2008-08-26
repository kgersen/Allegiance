<pigbehavior language="VBScript">

<script src="include\AutoStartGame.vbs"/>

<script>
<![CDATA[
'///////////////////////////////////////////////////////////////////////////
' Beginning of script
'
' Outside of the right margin, I have shown an alternate syntax for calling
' the properties/methods of the ActivePigs objects. The point is merely to
' demonstrate that several of the ActivePigs objects are in the global
' name space of the script. These objects are:
'
'     This - The currently running behavior object for which this script
'            defines the actions of the pig.
'
'     Pig  - The pig object upon which this behavior is assigned.
'
'     Host - An object that acts as this behavior's entry into the
'            ActivePigs object model. This can also be thought of as an
'            /internal/ session object.
'

sub HandleError(str, strSubName)
  if err.Number then
      Trace("\nError in '" & str & "': " & Err.Number & " - " & Err.Description & "\n")
      if strSubName <> "" then
        Execute("call " & strSubName)
      end if
  end if
end sub

'///////////////////////////////////////////////////////////////////////////
' Handles state transition. Logs on to the mission server.
sub OnStateNonExistant(eStatePrevious)

  On Error Resume Next

  ' If previous state was PigState_Terminated, the pig is being created
  if PigState_Terminated = eStatePrevious then
    ' Logon to the mission server
      UpdatesPerSecond = 8
      err.Clear()
      Logon()                                                         ' Pig.Logon
      HandleError "Logon", "Shutdown()"
  end if

end sub


'///////////////////////////////////////////////////////////////////////////
' Handles state transition. Joins or creates a mission.
sub OnStateMissionList(eStatePrevious)

  On Error Resume Next

  ' Attempt to join the current pig mission, if any
  err.Clear()
  JoinMission("Pig Mission")                                         ' Pig.JoinMission
  if err.Number then

      ' Mission didn't exist, join any other current mission, if any
      err.Clear()
      JoinMission()                                                      ' Pig.JoinMission

      if err.Number then

          ' No missions exist, create one and join it
          Dim objParams
          Set objParams = CreateObject("Pigs.MissionParams")
          objParams.TeamCount = 2
          objParams.MinTeamPlayers = 3
          objParams.MaxTeamPlayers = 10
          err.Clear()
          CreateMission(objParams)                                                 ' Pig.CreateMission
          HandleError "Create Mission", "Shutdown()"

          ' Automatically start game when the minimum players per team have joined
          'AutoStartGame(objParams.MinTeamPlayers)
          AutoStartGame(1)

      end if
  end if

end sub


'///////////////////////////////////////////////////////////////////////////
' Handles state transition. Joins a random team.
sub OnStateTeamList(eStatePrevious)

  ' Avoid repeated attempts to join a team
  if PigState_JoiningTeam <> eStatePrevious then

    err.Clear()
    JoinTeam()                                                        ' Pig.JoinTeam
    HandleError "JoinTeam", "QuitGame()"

  end if

end sub


'///////////////////////////////////////////////////////////////////////////
' Selects the best hull from a collection of hulls.  Scans the list, noting
' the index of the Fighter and the Interceptor.  If a Fighter was found, it
' returns that index.  Otherwise, the index of the Interceptor is returned.
' If an Interceptor is not found, returns 0, which should be a scout or a
' recovery ship.
'
function SelectBestHull(objHullTypes)

    Dim nHullClass ' 0 = other, 1 = interceptor, 2 = fighter (preferred)

    For each e in objHullTypes
        Dim nHullClassTry
        nHullClassTry = 0
        if Instr(lcase(e.Name), "intercept") then
            nHullClassTry = 1
        elseif Instr(lcase(e.Name), "fight") then
            nHullClassTry = 2
        end if

        ' now see if this is better than our current best hull
        if nHullClassTry > nHullClass then
            nHullClass = nHullClassTry
            SelectBestHull = e
        elseif nHullClassTry = nHullClass then
            if e.Mass > objHull.Mass then
                SelectBestHull = e
            end if
        end if
    Next

end function

'///////////////////////////////////////////////////////////////////////////
' Handles state transition. Launches the pig as soon as it becomes docked.
sub OnStateDocked(ePrev)

  ' Kill the flying timer, if one exists
  if Properties.Exists("FlyingTimer") then
    Properties.Kill("FlyingTimer")
  end if

  Dim objHullTypes, cHullTypes, objHullType
  objHullTypes = HullTypes
  cHullTypes = objHullTypes.Count
  
  Host.Trace("\nOnStateDocked; " & cHullTypes & " hull types available.\n")      ' Host.Trace
  
  objHullType = SelectBestHull(objHullTypes)

  Ship.BuyHull(objHullType)
  HandleError "Buy Hull", ""
     
  ' Schedule the Launch method (to avoid an IGC assert)
  CreateTimer 1.0, "OnLaunchTimer()", -1, "LaunchTimer"
end sub

sub OnLaunchTimer()
    Timer.Kill() 
    Launch()
end sub

'///////////////////////////////////////////////////////////////////////////
' Handles state transition. Currently just outputs debug text.
sub OnStateFlying(eStatePrevious)

  ' Set the ship's throttle to 100%
  Ship.Throttle = 100                                                          ' Pig.Ship.Throttle

  ' Generate a random number of seconds to throttle
  Dim nDuration
  nDuration = 5.0 + (Random() Mod 7)

  ' Create a timer to reset the throttle
  CreateTimer nDuration, "OnInitialThrottle()", -1, "FlyingTimer"        ' This.CreateTimer

end sub


'///////////////////////////////////////////////////////////////////////////
sub OnInitialThrottle()

  ' Set the ship's throttle to 0%
  Ship.Throttle = 0                                                       ' Pig.Ship.Throttle

  ' Thrust left for awhile
  Ship.Thrust(ThrustLeft)                                                 ' Pig.Ship.Thrust

  ' Generate a random number of seconds to thrust left
  Dim nDuration
  nDuration = 5.0 + (Random() Mod 10)

  ' Adjust the timer to reset the thrust, kill the timer, and face the outpost
  Timer.Interval = nDuration                                              ' This.Timer
  Timer.ExpirationExpression = "OnEndThrusting()"

end sub


'///////////////////////////////////////////////////////////////////////////
sub OnEndThrusting()

  ' Kill the timer
  Timer.Kill()

  ' Stop the ship from thrusting
  Ship.AllStop()

end sub


' End of script
'///////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

