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
  if (0 > cArgs || cArgs > 2)
    return Syntax();

  // Get the specified behavior type
  var strBehavior = (2 == cArgs) ? args(1) : "Default";

  // Create the utility object
  var objUtil = new ActiveXObject("TCUtil");

  // Create the Session Information object
  //var objInfo = new ActiveXObject("Pigs.SessionInfo");
  //objInfo.ApplicationName = WScript.ScriptFullName;

  // Connect to the Pig Server on the specified machine
  objSession = objUtil.CreateObject("Pigs.Session", args(0));
  //objSession.SessionInfo = objInfo;

  // Create a single pig
  WScript.Echo("Creating a pig with the '" + strBehavior + "' behavior...");
  var objPig = objSession.CreatePig(strBehavior);

  WScript.Echo("1 pig named \"" + objPig + "\" created.");
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str  = "\nUsage:\n\n";
      str +=   "  RemCreatePig computer [behavior]";
  WScript.Echo(str);
}


