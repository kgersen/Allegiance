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

  // Get the current ZoneAuthTimeout property
  var nOld = objSession.ZoneAuthTimeout;

  // Either set or get the property
  var str;
  if (cArgs)
  {
    // Set the property value
    objSession.ZoneAuthTimeout = args(0);

    // Format the response message
    str = "The Pigs ZoneAuthTimeout changed from \"" + nOld + "\" to \""
        + objSession.ZoneAuthTimeout + "\".";
  }
  else
  {
    // Format the response message
    str = "The current Pigs ZoneAuthTimeout is \"" + nOld + "\".";
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
    "\nDisplays or changes the current Pigs ZoneAuthTimeout\n\n"
  + "Usage:\n\n"
  + "  ZoneAuthTimeout\n"
  + "  ZoneAuthTimeout <new_timeout_value>\n\n"
  + "When <new_timeout_value> is not specified, the current Pig ZoneAuthTimeout is\n"
  + "displayed. Otherwise, the Pig ZoneAuthTimeout is set to the specified number\n"
  + "of milliseconds.";
  WScript.Echo(str);
}



