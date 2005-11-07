<pigbehavior language="JScript" Name="Manual">

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//


/////////////////////////////////////////////////////////////////////////////
//
function OnActivate(objDeactivated)
{
  Trace(Pig.Name + ": Manaul piglet Activated, previous was " +
    (objDeactivated ? objDeactivated.BehaviorType.Name : "(none)") + "\n");
}


/////////////////////////////////////////////////////////////////////////////
//
function OnDeactivate(objActivated)
{
  Trace(Pig.Name + ": Manaul piglet Dectivated, next is " +
    (objActivated ? objActivated.BehaviorType.Name : "(none)") + "\n");
}



/////////////////////////////////////////////////////////////////////////////
// Displays state transitions.
//
function DisplayStateTransition(eStatePrevious)
{
  Trace(Pig.Name + ": State changed from " + StateName(eStatePrevious)     // Host.Trace    Host.StateName
    + " to " + PigStateName + "\n");                                       // Pig.PigStateName
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateNonExistant(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateLoggingOn(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateLoggingOff(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateMissionList(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateCreatingMission(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateJoiningMission(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateQuittingMission(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateTeamList(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateJoiningTeam(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateWaitingForMission(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateDocked(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateLaunching(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateTeminated(eStatePrevious)
{
  DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles the 'mission started' notification.
function OnMissionStarted()
{
  // Output debug text
  Trace(Pig.Name + ": OnMissionStarted()\n");
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

