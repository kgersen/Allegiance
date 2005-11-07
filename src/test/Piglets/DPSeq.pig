<pigbehavior language="JScript">

<script src="include\common.js"/>
<script src="include\AutoStartGame.js"/>
<script src="include\GetIntoTeam.js"/>

<script>
<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//

var g_shipChatTo = null;
var g_nChatSeq = 0;

function TestChats(shipChatTo, nIterations)
{
  g_shipChatTo = shipChatTo;
  if (Properties.Exists("ChattyTimer"))
    Properties.Remove("ChattyTimer");
  CreateTimer(0, "SendChats();", nIterations, "ChattyTimer");
}

function SendChats()
{
  var objChatTo = (g_nChatSeq % 2) ? g_shipChatTo : g_shipChatTo.Team;
  objChatTo.SendChat("This is chat number " + FormatSeq());
}

function FormatSeq()
{
  var str = "000000000" + g_nChatSeq++;
  return str.substring(str.length - 10);
}


//
// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>

</pigbehavior>

