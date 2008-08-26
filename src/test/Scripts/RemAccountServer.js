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

  // Get the current AccountServer property
  var strOld = objSession.AccountServer;

  // Either set or get the property
  var str;
  if (2 == cArgs)
  {
    // Set the property value
    objSession.AccountServer = args(1);

    // Format the response message
    str = "The Pigs AccountServer changed from \"" + strOld + "\" to \""
        + args(1) + "\".";
  }
  else
  {
    // Format the response message
    str = "The current Pigs AccountServer is \"" + strOld + "\".";
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
    "\nDisplays or changes the current Pigs AccountServer\n\n"
  + "Usage:\n\n"
  + "  RemAccountServer <computer_name>\n"
  + "  RemAccountServer <computer_name> <new_server_name>\n\n"
  + "When <new_server_name> is not specified, the current Pig AccountServer is\n"
  + "displayed. Otherwise, the Pig AccountServer is set to the specified string.";
  WScript.Echo(str);
}



