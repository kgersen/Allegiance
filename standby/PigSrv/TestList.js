
// Create the Session Information object
//var objInfo = new ActiveXObject("Pigs.SessionInfo");
//objInfo.ApplicationName = WScript.ScriptFullName;


// Connect to the Pig Server
var objSession = new ActiveXObject("Pigs.Session");
//objSession.SessionInfo = objInfo;


// Create several pigs with the "Default" behavior
WScript.Echo("Creating several pigs...");
for (i = 0; i < 3; ++i)
  var objPig = objSession.CreatePig();

// List the pigs that are currently running
WScript.Echo("Listing the " + objSession.Pigs.Count + " pigs that are currently running...");
for (var it = new Enumerator(objSession.Pigs); !it.atEnd(); it.moveNext())
{
  var objItem = it.item();
  WScript.Echo("Name = \"" + objItem.Name + "\"");
}

