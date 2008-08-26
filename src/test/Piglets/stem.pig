<pigbehavior language="JScript" basebehavior="Default">

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
//
//  stem.pig
//
//  This pig is a "stem" pig - it joins the game and docks, and waits for a
//  "become x" chat command, where x is the name of a pig profile.
//  Upon receiving the become command, it will push the specified pig behavior
//  on the stack and activate that pig.
//
//  This pig is named "stem" because in biology, stem cells are cells that
//  have not specialized yet, such as the cells in a developing embryo.

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
// Handles state transition. Keep the pig docked.
function OnStateDocked(ePrev)
{
  Trace("\nstem pig is docked... doing nothing...\n");
  // do nothing
}

function OnActivate(objPig)
{
  Trace("I have been activated; prev pig = " + objPig + "\n");
}

function OnReceiveChat(strText, objShip)
{
  // look for "become" in the chat
  if (strText.indexOf("become") == 0)
  {
    // get behavior name
    var strBehaviorName = strText.substr(7);
    objShip.SendChat("Attempting to load behavior \"" + strBehaviorName + "\".");
    try
    {
      // try to push the specified behavior on the stack
      var pb = Stack.Push(strBehaviorName);
      // check for valid result
      if (!pb)
      {
        objShip.SendChat("Invalid behavior name.");
      }
      else
      {
        objShip.SendChat("I am now a " + strBehaviorName + " pig.");
      }
    } 
    catch (e)
    {
      objShip.SendChat("Invalid behavior name.");
    }
  }
}

// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

