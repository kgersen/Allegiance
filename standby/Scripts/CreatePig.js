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
  if (0 > cArgs || cArgs > 1)
    return Syntax();

  // Get the specified behavior type
  var strBehavior = cArgs ? args(0) : "Default";

  // Create the Session Information object
  //var objInfo = new ActiveXObject("Pigs.SessionInfo");
  //objInfo.ApplicationName = WScript.ScriptFullName;

  // Connect to the Pig Server
  objSession = new ActiveXObject("Pigs.Session");
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
      str +=   "  CreatePig [behavior]";
  WScript.Echo(str);
}


