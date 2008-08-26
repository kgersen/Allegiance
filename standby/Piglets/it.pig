<pigbehavior language="JScript" basebehavior="Default">

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
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
  // Set the ship's throttle to 100%
  Throttle = 100;                                                          // Pig.Ship.Throttle

  // Create a timer to reset the throttle
  CreateTimer(10.0, "OnInitialThrottle()", -1, "FlyingTimer");             // This.CreateTimer
}


/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the ship's throttle to 0%
  Ship.Throttle = 0;
  AllStop();

  // Thrust left for awhile
  //Ship.Thrust(ThrustLeft);                                                 // Pig.Ship.Thrust

  // Adjust the timer to start a collision test - give the command every 30 secs so that new pigs
  // get the command.  pigs that already received the command will pretty much ignore it.
  Timer.Interval = 30.0;                                                   // This.Timer
  Timer.ExpirationExpression = "StartCollisionTest()";
}

/////////////////////////////////////////////////////////////////////////////
// This function tells bulldozer pigs to ram into this pig.
function StartCollisionTest()
{
  Ship.Sector.SendChat("?CollisionTest(\"" + Name + "\")");
}

/////////////////////////////////////////////////////////////////////////////
// utter frivolous fluff.
function CollisionTest()
{
  Game.SendChat("Come and get me...");
}

// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

