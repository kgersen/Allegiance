/////////////////////////////////////////////////////////////////////////////
// Constants
//
var CompareMode_Text = 1;


/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.
//
main(WScript.Arguments, WScript.Arguments.Count());


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args, cArgs)
{
  // Validate the command line arguments
  if (cArgs < 2)
    return Syntax();
  if ("-?" == args(0) || "/?" == args(0))
    return Syntax();

  // Create a utility object here
  var objUtil = new ActiveXObject("TCUtil");

  // Parse the argument string into a collection of servers
  var dict = StringToCollection(args(0));

  // Create the command line
  var sCmdLine = "";
  for (iArg = 1; iArg < cArgs; ++iArg)
  {
    if (iArg > 1)
      sCmdLine += " ";
    sCmdLine += args(iArg);
  }

  // Iterate through each server name and attempt to run the command line
  var it;
  for (it = new Enumerator(dict); !it.atEnd(); it.moveNext())
  {
    var sComputer = it.item();

    // Catch all errors within the next scope
    var sErrCtx;
    try
    {
      // Create the deployment object on the remote server
      sErrCtx = "create TCDeployment object";
      var oDeploy = objUtil.CreateObject("TCDeployment", sComputer);

      // Begin executing SetupPigs on the remote server
      sErrCtx = "execute command line";
      var oProcess = oDeploy.Execute(sCmdLine);
    }
    catch (e)
    {
      // Log the error
      WScript.Echo("Failed to " + sErrCtx + " on " + sComputer + ":\n  " + ErrorText(e));
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// 
function StringToCollection(strIn)
{
  // Create a dictionary object with text compare mode
  var d = new ActiveXObject("Scripting.Dictionary");
  d.CompareMode = CompareMode_Text;

  // Loop through the tokens in the specified string
  var s = strIn, n;
  do
  {
    // Find the next ';' token seperator
    var sTarget;
    n = s.indexOf(";"); // will return -1 or where the ';' was found
    if (n >= 0)
    {
      sTarget = s.substr(0, n);
      s = s.substr(n + 1);
    }
    else
    {
      sTarget = s;
      s = "";
    }

    // now look through sTarget for more complex range specification
    var nn = sTarget.indexOf("(");  // will return -1 or index of '('
    if (nn >= 0)
    {
      var ss = sTarget, ssBase, ssRange, ssR1, ssR2, nn, nsR1, nsR2, i, j, l;
      ssBase = ss.substr(0, nn);  // ssBase is the base
      ss = ss.substr(nn + 1);

      nn = ss.indexOf(")");
      if (nn >= 0)
      {
        ssRange = ss.substr(0, nn);  // ssRange is the replacement part
        ss = ss.substr(nn + 1);
      }
      else
        throw "Can't find end of range near: '" + ssBase + "'.";
      if ("" != ss)
        throw "Extra garbage after range specification near: '" + ssBase + "'.";
      nn = ssRange.indexOf("-");
      if (nn >= 0)
      {
        ssR1 = ssRange.substr(0, nn);
        ssR2 = ssRange.substr(nn + 1);
      }
      else 
      {
        ssR1 = ssRange;  // silly, but there's only one value in the range to substitute
        ssR2 = "";
      }
      l = ssR1.length;
      if (l < ssR2.length)
          l = ssR2.length;
      nsR1 = new Number(ssR1);
      nsR2 = new Number(ssR2);
      if (isNaN(nsR1) || isNaN(nsR2) )
      {
        throw "Non-numeric range specification near: '" + ssBase + "', '" + ssR1 + "'; '" + ssR2 + "'.";
      }
      nn = nsR2 - nsR1;
      if ( nn < 0 )
        throw "Invalid range specification near: '" + ssBase + "'. End of range less than start.";
      if ( nn > 100 )
        throw "Invalid range specification near: '" + ssBase + "'.  Too many iterations (max 100).";
      // loop over them and add each target to the dictionary
      for ( i = nsR1; i <= nsR2; i++)
      {
        var sZeroes = new String(i);
        while (sZeroes.length < l)
          sZeroes = "0" + sZeroes;
        ssTarget = ssBase + sZeroes;

        // Add it to the dictionary, if not empty and not already in there
        if ("" != ssTarget && !d.Exists(ssTarget))
          d.Add(ssTarget, "");
      }
      sTarget = "";
    }

    // Add it to the dictionary, if not empty and not already in there
    if ("" != sTarget && !d.Exists(sTarget))
      d.Add(sTarget, "");

  } while (n >= 0);

  // Return the dictionary object
  return d;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Formats the specified error object into a readable message.
//
function ErrorText(e)
{
  var sText = "0x" + (e.number + 0x100000000).toString(16) + " - ";
  sText += ("" == e.description) ? "(no description)" : e.description;
  return sText;
}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  // ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
  var str =
    "\nRuns the specified command line on each of the specified computers.\n\n"
  + "Usage:\n\n"
  + "  ForEach <computer_names> <command_line>\n"
  WScript.Echo(str);
}



