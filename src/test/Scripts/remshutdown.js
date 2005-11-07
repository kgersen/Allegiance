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
  if (args.Count() != 2)
    return Syntax();
  var strArg = args(0).toLowerCase();
  if ("/?" == strArg || "-?" == strArg || "/help" == strArg || "-help" == strArg)
    return Syntax();

  // Get the specified server name
  var strServer = strArg.toUpperCase();

  // Get the specified pig name
  strArg = args(1).toLowerCase();

  // Create a utility object here
  var objUtil = new ActiveXObject("TCUtil");

  // Create the Session Information object
  //var objInfo = new ActiveXObject("Pigs.SessionInfo");
  //objInfo.ApplicationName = WScript.ScriptFullName;

  // Connect to the Pig Server
  objSession = objUtil.CreateObject("Pigs.Session", strServer);
  //objSession.SessionInfo = objInfo;

  // Determine if "all" was specified or a possible pig name
  return ("all" == strArg) ? ShutdownAll() : ShutdownPig(strArg);
}


/////////////////////////////////////////////////////////////////////////////
// Shuts down the specified pig, if one by that name is running.
//
function ShutdownPig(strName)
{
  // Get a pig with the specified name
  var objPig = objSession.Pigs(strName);
  if (!objPig)
  {
    WScript.Echo("Could not find a pig named \"" + strName + "\".");
    return;
  }

  // Shutdown the pig
  WScript.Echo("Shutting down the pig named \"" + objPig + "\"...");
  objPig.Shutdown();

  // Display the number of pigs that were shutdown
  WScript.Echo("1 pig was shutdown.");
}


/////////////////////////////////////////////////////////////////////////////
// Shuts down all running pigs, if any
//
function ShutdownAll()
{
  // Get the collection of pigs
  var itPigs = new Enumerator(objSession.Pigs);
  if (itPigs.atEnd())
  {
    WScript.Echo("No pigs are currently running.");
    return;
  }

  // Shutdown each pig in the list
  var nPigs = 0;
  WScript.Echo("Shutting down all pigs that are currently running...");
  for (; !itPigs.atEnd(); itPigs.moveNext(), ++nPigs)
  {
    var objPig = itPigs.item();
    WScript.Echo("Shutting down \"" + objPig + "\"...");
    objPig.Shutdown();
  }

  // Display the number of pigs that were shutdown
  if (nPigs > 1)
    WScript.Echo(nPigs + " pigs were shutdown.");
  else
    WScript.Echo("1 pig was shutdown.");
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str  = "\nUsage:\n\n";
      str +=   "  RemShutdown <computer_name> ALL\n\n";
      str +=   "  RemShutdown <computer_name> <pigname>";
  WScript.Echo(str);
}


