/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

main(WScript.Arguments, WScript.Arguments.Count());


/////////////////////////////////////////////////////////////////////////////
// Global Declarations

var objSession;


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args, cArgs)
{
  // Validate the command line arguments
  if (1 > cArgs || cArgs > 2 || args(0) == "-?" || args(0) == "/?")
    return Syntax();

  // Get the specified behavior type
  var strBehavior = cArgs ? args(0) : "Default";
  var nCount = (cArgs == 2) ? args(1) : 1;

  // Create the Session Information object
  //var objInfo = new ActiveXObject("Pigs.SessionInfo");
  //objInfo.ApplicationName = WScript.ScriptFullName;

  // Connect to the Pig Server
  objSession = new ActiveXObject("Pigs.Session");
  //objSession.SessionInfo = objInfo;

  WScript.Echo("Creating " + nCount + " pigs with the '" + strBehavior + "' behavior...");
  for (var i = 0; i < nCount; i++)
  {
    // Create a single pig
    var objPig = objSession.CreatePig(strBehavior);
    WScript.Echo("1 pig named \"" + objPig + "\" created.");
  }

  WScript.Echo(nCount + " pigs created.");

}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str  = "\nUsage:\n\n";
      str +=   "  CreatePigs behavior count";
  WScript.Echo(str);
}


