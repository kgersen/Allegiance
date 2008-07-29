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

  // Connect to the Pig Server
  var objSession = new ActiveXObject("Pigs");

  // Get the current ScriptDir property
  var strOld = objSession.ScriptDir;

  // Either set or get the property
  var str;
  if (cArgs)
  {
    if ("-?" == args(0) || "/?" == args(0))
      return Syntax();

    // Set the property value
    objSession.ScriptDir = args(0);

    // Format the response message
    str = "The Pigs ScriptDir changed from \"" + strOld + "\" to \""
        + args(0) + "\".";
  }
  else
  {
    // Format the response message
    str = "The current Pigs ScriptDir is \"" + strOld + "\".";
  }

  // Display the response message
  WScript.Echo(str);
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str =
    "\nDisplays or changes the current Pigs ScriptDir\n\n"
    + "Usage:\n\n"
    + "  ScriptDir\n"
    + "  ScriptDir new_server_name\n\n"
    + "When no parameter is specified, the current Pig ScriptDir is displayed.\n"
    + "Otherwise, the Pig ScriptDir is set to the specified string.";
  WScript.Echo(str);
}



