<pigbehavior language="JScript" basebehavior="Default">

<script>
<![CDATA[

////////////////
// docker.pig
/*

  This pig undocks and docks.
  The dockcommander profile has been merged into this profile, so it is no longer
  necessary.  The pigs do all traffic control communally.

  */

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
  Ship.WingID = 8;
  if ("object" == typeof(Properties("RepeatTimer")))
    Properties("RepeatTimer").Kill();
  //Ship.AutoAcceptCommands(true);
  CreateTimer(3.0, "StepOne()", -1, "Step1Timer");             // This.CreateTimer
}

function StepOne()
{
  Timer.Kill();
  FaceRandomAsteroid();
}

function FaceRandomAsteroid()
{
  if ("object" == typeof(Properties("RepeatTimer")))
    Properties("RepeatTimer").Kill();
  var alist = Ship.Sector.Asteroids;
  var index = Random() % alist.Count;
  Ship.Face(alist(index), "StepTwo()");
}

function StepTwo()
{
  Throttle = 80;
  CreateTimer(80.0 - (Random() % 60), "SendDockCommand2()", -1, "CommandTimer");
}

/*
function SendDockCommand()
{
  Timer.Kill();
  Ship.Team.SendChat("sendcommand1", 8);
  CreateTimer(20.0, "SendDockCommand2()", -1, "CommandTimer");
}
*/

function SendDockCommand2()
{
  AllStop();
  Timer.Kill();
  Ship.Team.SendChat("sendcommand2", 8);
  CreateTimer(120.0 - (Random() % 15), "FaceRandomAsteroid()", -1, "RepeatTimer");
}

function OnReceiveChat(strText, objShip)
{
  if (PigState != PigState_Flying)
    return false;
  if (strText.indexOf("sendcommand2") == 0)
  {
    objShip.SendCommand("goto", Ship.Sector.Stations(0));
    return true;
  }
}

]]>
</script>

</pigbehavior>

