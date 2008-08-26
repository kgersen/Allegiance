/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

main(WScript.Arguments, WScript.Arguments.Count());


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args, cArgs)
{
  // Validate the command line arguments
  if (cArgs > 2 || cArgs < 1)
    return Syntax();
  if ("-?" == args(0) || "/?" == args(0))
    return Syntax();

  // Create a utility object here
  var objUtil = new ActiveXObject("TCUtil");

  // Connect to the Pig Server
  var strServer  = args(0);
  var objSession = objUtil.CreateObject("Pigs", strServer);

  // Get the current MissionServer property
  var strOld = objSession.MissionServer;

  // Either set or get the property
  var str;
  if (2 == cArgs)
  {
    // Set the property value
    objSession.MissionServer = args(1);

    // Format the response message
    str = "The Pigs LobbyServer changed from \"" + strOld + "\" to \""
        + args(1) + "\".";
  }
  else
  {
    // Format the response message
    str = "The current Pigs LobbyServer is \"" + strOld + "\".";
  }

  // Display the response message
  WScript.Echo(str);
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  // ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
  var str =
    "\nDisplays or changes the current Pigs MissionServer\n\n"
  + "Usage:\n\n"
  + "  RemLobbyServer <computer_name>\n"
  + "  RemLobbyServer <computer_name> <new_server_name>\n\n"
  + "When <new_server_name> is not specified, the current Pig LobbyServer is\n"
  + "displayed. Otherwise, the Pig LobbyServer is set to the specified string.";
  WScript.Echo(str);
}



