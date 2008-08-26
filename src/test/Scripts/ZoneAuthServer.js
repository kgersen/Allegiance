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

  // Get the current ZoneAuthServer property
  var strOld = objSession.ZoneAuthServer;

  // Either set or get the property
  var str;
  if (cArgs)
  {
    // Set the property value
    objSession.ZoneAuthServer = args(0);

    // Format the response message
    str = "The Pigs ZoneAuthServer changed from \"" + strOld + "\" to \""
        + args(0) + "\".";
  }
  else
  {
    // Format the response message
    str = "The current Pigs ZoneAuthServer is \"" + strOld + "\".";
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
    "\nDisplays or changes the current Pigs ZoneAuthServer\n\n"
  + "Usage:\n\n"
  + "  ZoneAuthServer\n"
  + "  ZoneAuthServer <new_server_name>\n\n"
  + "When <new_server_name> is not specified, the current Pig ZoneAuthServer is\n"
  + "displayed. Otherwise, the Pig ZoneAuthServer is set to the specified string.";
  WScript.Echo(str);
}



