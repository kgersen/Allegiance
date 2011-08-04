<pigbehavior language="VBScript" Name="BeeperVB">

<script>
<![CDATA[
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
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
'            'internal' session object.
'


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Globally declare the number of seconds that this pig will stay alive
Dim nLifeTime
nLifeTime = 6

' Globally declare the Ticker object, which is the timer that we'll create
Dim objTicker


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Description: Handles a state transition.
'
' This is the first state that a pig will ever enter. It is also the state
' into which a pig will transition from PigState_LoggingOff. To distinguish
' for which case this method is called, the eStatePrevious method argument
' should be examined.
'
' Parameters:
'   eStatePrevious - The state from which the pig is transitioning into the
' current state.
'
Sub OnStateNonExistant(eStatePrevious)

    ' Do nothing if the previous state is not PigState_Terminated
    If PigState_Terminated <> eStatePrevious Then
        Exit Sub
    End If

    ' Beep to indicate the opening of this sequence
    Beep 1600, 100                                                          ' Host.Beep

    ' Create a timer that will shutdown the pig after a short lifetime
    CreateTimer nLifeTime, "OnKillPig()"                                    ' This.CreateTimer

    ' Create a timer that will beep every second that the pig is alive
    Set objTicker = CreateTimer(1, "Beep 3200, 60")                         ' This.CreateTimer

End Sub


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Kills the ticker object, beeps, and shuts down the pig.
Sub OnKillPig()

    ' Kill the ticker object
    objTicker.Kill

    ' Beep to indicate the closing of this sequence
    Beep 240, 150

    ' Shutdown the pig
    Pig.Shutdown

End Sub


' End of script
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
]]>
</script>

</pigbehavior>

