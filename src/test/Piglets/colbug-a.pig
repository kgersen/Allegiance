<pigbehavior language="JScript" basebehavior="Default">

<script>
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//

var CurrentAsteroid;
var AsteroidPosition = new ActiveXObject("AGC.AGCVector");

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  try
  {
    if ("object" == typeof(Properties("FlyingTimer")))
      Properties("FlyingTimer").Kill();
    if ("object" == typeof(Properties("TargetTimer")))
      Properties("TargetTimer").Kill();
    // find closest asteroid
    CurrentAsteroid = FindClosestAsteroid();
    UpdatePositionVector();

    // Set the ship's throttle to 100%
    Throttle = 100;

    // Create a timer to reset the throttle
    var fDuration = 5.0 + (Random() % 5);
    CreateTimer(fDuration, "OnInitialThrottle()", -1, "FlyingTimer");
    CreateTimer(fDuration, "CurrentAsteroid = FindClosestAsteroid(); UpdatePositionVector();", -1, "TargetTimer");
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

function UpdatePositionVector()
{
  AsteroidPosition.X = CurrentAsteroid.Position.X;
  AsteroidPosition.Y = CurrentAsteroid.Position.Y;
  AsteroidPosition.Z = CurrentAsteroid.Position.Z;
}

function FindClosestAsteroid()
{
  var alist = Ship.Sector.Asteroids;

  if (alist.Count == 0)
  {
    ShutDown();
    return null;
  }

  var result = alist(0);
  var mindist = 10000 * 10000;

  var e = new Enumerator(alist);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    dist = Ship.Position.Subtract(x.Position).LengthSquared;
    if (dist < mindist)
    {
      result = x;
      mindist = dist;
    }
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    Ship.Face(AsteroidPosition, "ThrustOrbit();");
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

