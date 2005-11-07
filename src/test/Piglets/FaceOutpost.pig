<pigbehavior language="JScript" basebehavior="Default">

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//


/////////////////////////////////////////////////////////////////////////////
function OnEndThrusting()
{
  // Perform default processing
  BaseBehavior.OnEndThrusting();

  // Face the outpost
  try
  {
    Ship.Face("Outpost", "AllStop(); Beep(5000, 50);");
  }
  catch (e)
  {
    Trace("\nOnEndThrusting(): " + e.description + "\n");
  }
}


// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

