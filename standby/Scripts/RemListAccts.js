/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

main(WScript.Arguments, WScript.Arguments.Count());


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args, cArgs)
{
  // Validate the command line arguments
  if (1 > cArgs || cArgs > 2)
    return Syntax();

  // Get the specified server name
  var strServer = args(0).toUpperCase();

  // Get the specified listing type
  var strListing = (2 == cArgs) ? args(1).toUpperCase() : "";

  // Create a utility object here
  var objUtil = new ActiveXObject("TCUtil");

  // Connect to the Pig Account Dispenser
  var objAccts = objUtil.CreateObject("Pigs.AccountDispenser", strServer);

  // Get the list of all accounts and accounts in use
  var objAll   = objAccts.Names;
  var objInUse = objAccts.NamesInUse;

  // List the specified accounts
  var cLines = 0, cPerEcho = 20;
  var str = "";
  var cch = strListing.length;
  if (0 == cch)
  {
    for (var it = new Enumerator(objAll); !it.atEnd(); it.moveNext())
    {
      str += FormatLine(it.item(), !FindString(objInUse, it.item()));
      if (0 == (++cLines % cPerEcho))
        WScript.Echo(str), str = "";
      else
        str += "\n";
    }
  }
  else if ("AVAILABLE".substr(0, cch) == strListing)
  {
    for (var it = new Enumerator(objAll); !it.atEnd(); it.moveNext())
    {
      if (!FindString(objInUse, it.item()))
      {
        str += FormatLine(it.item(), true);
        if (0 == (++cLines % cPerEcho))
          WScript.Echo(str), str = "";
        else
          str += "\n";
      }
    }
  }
  else if ("INUSE".substr(0, cch) == strListing)
  {
    var objNames = objAccts.NamesInUse;
    for (var it = new Enumerator(objInUse); !it.atEnd(); it.moveNext())
    {
      str += FormatLine(it.item(), false);
      if (0 == (++cLines % cPerEcho))
        WScript.Echo(str), str = "";
      else
        str += "\n";
    }
  }
  else
    return Syntax();

  // Output the string
  WScript.Echo(str);

  // Format the total line
  str  = "Total Accounts: \t" + objAll.Count + "\n";
  str += "Available:      \t" + (objAll.Count - objInUse.Count) + "\n";
  str += "InUse:          \t" + objInUse.Count;

  // Output the string
  WScript.Echo(str);
}


/////////////////////////////////////////////////////////////////////////////
// Description:
//
function FormatLine(strName, bAvailable)
{
  return strName + " \t" + (bAvailable ? "Available" : "In Use");
}


/////////////////////////////////////////////////////////////////////////////
// Desciption: Finds the specified string in the specified collection of
// strings.
//
// Parameters:
//   objStrings - The collection of strings in which to search.
//   strString - The string to search for in /objStrings/.
//
// Return Value: *true* if the specified string is in the collection,
// otherwise *false*.
//   
function FindString(objStrings, strString)
{
  for (var it = new Enumerator(objStrings); !it.atEnd(); it.moveNext())
    if (it.item() == strString)
      return true;
  return false;
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str  = "\nUsage:\n\n";
      str +=   "  RemListAccts <computer_name> [Available | Inuse]";
  WScript.Echo(str);
}



