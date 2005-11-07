<pigbehavior language="JScript" basebehavior="Default">

<script>
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//

var TargetVector = new ActiveXObject("AGC.AGCVector");

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  TargetVector.X = 0;
  TargetVector.Y = 0;
  TargetVector.Z = 0;
  try
  {
    // Set the ship's throttle to 100%
    Throttle = 100;

    // Create a timer to reset the throttle
    var fDuration = 5.0 + (Random() % 5);
    CreateTimer(fDuration, "OnInitialThrottle()", -1, "FlyingTimer");
  }
  catch (e)
  {
    Trace("Error in OnStateFlying():\n\t" + e.description);
  }
}


/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the ship's throttle to 0%
  Ship.Throttle = 0;

  // Set the timer's interval to 2 seconds
  Timer.Interval = 2.0;
  Timer.ExpirationExpression = "CorrectOrbit()";
}


/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    Ship.Face(TargetVector, "ThrustOrbit();");
    return;
  }
  catch (e)
  {
    Trace("Error in CorrectOrbit():\n\t" + e.description);
  }
}


/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{
		Thrust(ThrustForward);

		// Toggle the firing of the weapon
		FireWeapon(!IsFiringWeapon);
		
		// Check for empty Ammo
		if (!Ammo || Energy < 20)
			CommitSuicide();
	}
	catch (e)
	{
	}
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

