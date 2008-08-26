/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

main(WScript.Arguments);


/////////////////////////////////////////////////////////////////////////////
// Global Declarations

var objSession;


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args)
{
  // Validate the command line arguments
  if (args.Count() != 0)
    return Syntax();

  // Create the Session Information object
  //var objInfo = new ActiveXObject("TCObj.SessionInfo");
  //objInfo.ApplicationName = WScript.ScriptFullName;

  // Connect to the Pig Server
  objSession = new ActiveXObject("Pigs.Session");
  //objSession.SessionInfo = objInfo;

  // Get the collection of pigs
  var itPigs = new Enumerator(objSession.Pigs);
  if (itPigs.atEnd())
  {
    WScript.Echo("No pigs are currently running.");
    return;
  }

  // List the pigs that are currently running
  WScript.Echo("Listing the pigs that are currently running...");
  for (var nPigs = 0; !itPigs.atEnd(); itPigs.moveNext(), ++nPigs)
  {
    var objPig = itPigs.item();
    WScript.Echo("  \"" + objPig + "\" - State is " + objPig.PigStateName);
  }

  if (nPigs > 1)
    WScript.Echo(nPigs + " pigs are currently running.");
  else
    WScript.Echo("1 pig is currently running.");
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str  = "\nUsage:\n\n";
      str +=   "  ListPigs";
  WScript.Echo(str);
}


