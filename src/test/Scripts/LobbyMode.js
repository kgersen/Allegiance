/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

main(WScript.Arguments, WScript.Arguments.Count());


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args, cArgs)
{
  // Validate the command line arguments
  if (cArgs > 1)
    return Syntax();
  if (1 == cArgs && ("-?" == args(0) || "/?" == args(0)))
    return Syntax();

  // Connect to the Pig Server
  var objSession = new ActiveXObject("Pigs");

  // Get the current LobbyMode property
  var strOld = LobbyModeName(objSession.LobbyMode);

  // Either set or get the property
  var str;
  if (cArgs)
  {
    // Set the property value
    objSession.LobbyMode = LobbyModeFromName(args(0));

    // Format the response message
    str = "The Pigs LobbyMode changed from \"" + strOld + "\" to \""
        + LobbyModeName(objSession.LobbyMode) + "\".";
  }
  else
  {
    // Format the response message
    str = "The current Pigs LobbyMode is \"" + strOld + "\".";
  }

  // Display the response message
  WScript.Echo(str);
}


/////////////////////////////////////////////////////////////////////////////
//
function LobbyModeName(eMode)
{
  switch (eMode)
  {
    case 0:
      return "Club";
    case 1:
      return "Free";
    case 2:
      return "LAN";
    default:
      return "(unknown)";
  }
}


/////////////////////////////////////////////////////////////////////////////
//
function LobbyModeFromName(strMode)
{
  strMode = strMode.toUpperCase();
  if ("CLUB" == strMode)
    return 0;
  else if ("FREE" == strMode)
    return 1;
  else if ("LAN" == strMode)
    return 2;
  else
    return -1;
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  // ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
  var str =
    "\nDisplays or changes the current Pigs LobbyMode\n\n"
  + "Usage:\n\n"
  + "  LobbyMode\n"
  + "  LobbyMode <lobby_mode_name>\n\n"
  + "When <lobby_mode_name> is not specified, the current Pig LobbyMode is\n"
  + "displayed. Otherwise, the Pig LobbyMode is set as specified, with the following\n"
  + "names being valid:\n"
  + "  Club\n"
  + "  Free\n"
  + "  LAN.";
  WScript.Echo(str);
}



