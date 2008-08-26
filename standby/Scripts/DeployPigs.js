/////////////////////////////////////////////////////////////////////////////
// 
// 
/////////////////////////////////////////////////////////////////////////////

// Constants
var CompareMode_Text = 1;

// Derived from options
var g_sTemp           = "";    // Location of temp directory (extracted from TEMP env var)
var g_sPropDir        = "\\\\oblivion0\\builds";    // Location of where build has exported files
var g_sLogDir         = "";    // Location to store log files while product is building
var g_bLogo           = true;  // Whether or not to display the logo
var g_bSilent         = false; // Whether or not to display any messages
var g_sSetupPigsList  = "";    // The list of computers on which to install ActivePigs
var g_sAllSrv         = "";    // The server against which the pigs will run
var g_sBuildNumber    = "";    // The build number of ActivePigs to deploy
var g_dictPigs;                // Used to store list of computers on which SetupPigs.vbs was run
var g_fileLog;                 // Log file for summary output
var g_fileLogDetail;           // Log file for detailed output

// Utility Objects
var g_oShell      = new ActiveXObject("WScript.Shell");
var g_fso         = new ActiveXObject("Scripting.FileSystemObject");
var g_dictResults = new ActiveXObject("Scripting.Dictionary");
    g_dictResults.CompareMode = CompareMode_Text;


/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

try
{
  main(WScript.Arguments.Count(), WScript.Arguments);
}
catch (e)
{
  var strError = ("string" == typeof(e)) ? e : ErrorText(e);
  WScript.Echo("ERROR: " + strError);
  g_dictResults("Error") = strError;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(cArgs, args)
{
  // Parse command line parameters and validate usage.
  if (!Init(cArgs, args))
    return;

  // Echo the command line
  var sCommandLine = WScript.ScriptName;
  for (i = 0; i < cArgs; ++i)
    sCommandLine += " " + args(i);
  DoLog("Command line: <<" + sCommandLine + ">>\n");

  // Display the logo, unless otherwise specified
  if (g_bLogo)
  {
    DoMessage(
      "ActivePigs Deployment Script, 1.0\n" +
      "  by John A. Tasler (jtasler)\n"     +
      " and Mark E. Snyder (marksn)\n"      );
  }

  // Go setup the pigs
  SetupPigs();
}

/////////////////////////////////////////////////////////////////////////////
// 
function DoMessage(sMessage)
{
  DoLog(sMessage);
  if (!g_bSilent)
    WScript.Echo(sMessage);
}

/////////////////////////////////////////////////////////////////////////////
// 
function DoLog(sMessage)
{
  DoLogDetail(sMessage);
  if (g_fileLog)
    g_fileLog.WriteLine(sMessage);
}

/////////////////////////////////////////////////////////////////////////////
// 
function DoLogDetail(sMessage)
{
  if (g_fileLogDetail)
    g_fileLogDetail.WriteLine(sMessage);
}


/////////////////////////////////////////////////////////////////////////////
// 
function Init(cArgs, args)
{
  // Get the command line parameters as a string-to-string dictionary
  var dictArgs = GetCommandList(cArgs, args);

  // Check for an error
  if (dictArgs.Exists("Error"))
    throw dictArgs("Error");

  // Check for a syntax query option
  if (dictArgs.Exists("?") || dictArgs.Exists("H") || !dictArgs.Count)
  {
    WScript.Echo(SyntaxText());
    return false;
  }

  // Check for /SetupPigs option
  if (!dictArgs.Exists("SetupPigs"))
    throw "/SetupPigs option was not specified";
  g_sSetupPigsList = dictArgs("SetupPigs");
  if ("" == g_sSetupPigsList)
  {
    //          0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
    var sText = "/SetupPigs option requires a list of one or more server names\n\n"
              + "Example:\n"
              + "  /SetupPigs FedStress01;Fedstress02;Fedstress03\n"
              + "  /SetupPigs FedStress(01-07);Fedstress10\n"
    throw sText;
  }

  // Check for /AllSrv option
  if (!dictArgs.Exists("AllSrv"))
    throw "/AllSrv option was not specified";
  g_sAllSrv = dictArgs("AllSrv");
  if ("" == g_sAllSrv)
    throw "/AllSrv option requires a server name";

  // Check for /Silent option
  if (dictArgs.Exists("Silent"))
    g_bSilent = true;

  // Check for /NoLogo option
  if (dictArgs.Exists("NoLogo"))
    g_bLogo = false;

  // Check for /Label option
  if (dictArgs.Exists("Label"))
  {
    g_sBuildNumber = dictArgs("Label");
    if ("" != g_sBuildNumber)
    {
      // Ensure that the label is a four-digit number
      if (4 != g_sBuildNumber.length)
        throw "/Label, when specified, must be a 4-digit number";
      var reNumber = /[0-9]/;
      for (i = 0; i < g_sBuildNumber.length; ++i)
        if (-1 == g_sBuildNumber.substr(0, 1).search(reNumber))
          throw "/Label, when specified, must be a 4-digit number";

      // Check for the existance of the specified build number of ActivePigs
      var sSrc = g_sPropDir + "\\" + g_sBuildNumber + "\\cd2\\tools\\SetupPigs.vbs"
      if (!g_fso.FileExists(sSrc))
        throw "Build " + g_sBuildNumber + " does not exist:\n  " + sSrc;
    }
  }

  // Check for /Results option
  if (dictArgs.Exists("Results"))
  {
    try
    {
      g_dictResults = GetObject(dictArgs("Results"));
    }
    catch (e)
    {
      throw "Creation of object reference specified in /Results option failed:\n  " + ErrorText(e);
    }
  }

  // Retrieve process environment variables
  var objProcEnv = g_oShell.Environment("Process");
  g_sTemp = objProcEnv("temp");
  g_sLogDir = g_sTemp + "\\Log";

  // Create the log folder, if it does not exist
  if (!g_fso.FolderExists(g_sLogDir))
    g_fso.CreateFolder(g_sLogDir);

  // Create the log files
  g_fileLog       = g_fso.CreateTextFile(g_sLogDir + "\\DeployPigs.log"      , true);
  g_fileLogDetail = g_fso.CreateTextFile(g_sLogDir + "\\DeployPigsDetail.log", true);

  // Initialize the dictionary of succeeded Pigs Setups
  g_dictPigs = new ActiveXObject("Scripting.Dictionary");

  // Indicate success
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// 
function SetupPigs()
{
  // Parse the argument string into a collection of servers
  var dict = StringToCollection(g_sSetupPigsList);

  // Create the command line
  var sCmdLine = g_sPropDir + "\\";
  sCmdLine += ("" == g_sBuildNumber) ?
    "SetupPigs.bat" : "SetupPigsBuild.bat " + g_sBuildNumber;
  sCmdLine += " /D C:\\Pigs /AllSrv " + g_sAllSrv + " /NoUpdateDeployment /Silent"
  
  // Iterate through each server name and attempt to set it up
  var it;
  for (it = new Enumerator(dict); !it.atEnd(); it.moveNext())
  {
    var sPigSrv = it.item();

    // Log the intention to setup pigs
    DoLogDetail("------------------------------------------------------------------------");
    DoLogDetail("Deployment:    Setup Pigs on " + sPigSrv);
    DoLogDetail("Command Line:  " + sCmdLine);

    // Catch all errors within the next scope
    var sErrCtx;
    try
    {
      // Create the deployment object on the remote server
      sErrCtx = "create TCDeployment object";
      var oDeploy = GetDeployment().CreateObject("TCDeployment", sPigSrv);

      // Begin executing SetupPigs on the remote server
      sErrCtx = "execute SetupPigs script";
      var oProcess = oDeploy.Execute(sCmdLine);

      // Associate the process object with the server name
      dict(sPigSrv) = oProcess;
    }
    catch (e)
    {
      // Log the error
      g_dictResults(sPigSrv) = "Failed to " + sErrCtx + ":\n  " + ErrorText(e);
      DoMessage(sPigSrv + ":\n  " + g_dictResults(sPigSrv));
      DoMessage("------------------------------------------------------------------------");

      // Remove the server name from the dictionary
      dict.Remove(sPigSrv);
    }
  }

  // Wait up to 5 minutes for the servers to complete the setup process
  DoLogDetail("Waiting for pig machines to complete setup...");
  var bStillRunning = true;
  var timeStop = new Date;
  timeStop.setMinutes(timeStop.getMinutes() + 5);
  while (bStillRunning && (new Date).valueOf() < timeStop.valueOf())
  {
    // Sleep for 2 seconds
    GetDeployment().Sleep(2 * 1000);

    // Determine if any process is still working
    bStillRunning = false;
    for (it = new Enumerator(dict); !it.atEnd(); it.moveNext())
    {
      var sPigSrv = it.item();

      if (!dict(sPigSrv).IsCompleted)
      {
        bStillRunning = true;
      }
      else
      {
        // Add it to the list of servers that completed setup
        g_dictPigs(sPigSrv) = sPigSrv;

        // Remove if from the list of servers that are still running
        dict.Remove(sPigSrv);

        // Display a message that the setup completed on the server 
        DoMessage("Setup Pigs on '" + sPigSrv + "' completed.");
      }
    }
  }

  // Report which server processes were abandoned, if any
  for (it = new Enumerator(dict); !it.atEnd(); it.moveNext())
  {
    var sPigSrv = it.item();
    g_dictResults(sPigSrv) = "Setup Pigs did not complete soon enough";
    DoMessage(sPigSrv + ": " + g_dictResults(sPigSrv));
  }

  // Report the build number running on each computer
  for (it = new Enumerator(g_dictPigs); !it.atEnd(); it.moveNext())
  {
    var sPigSrv = it.item();

    // Catch all errors within the next scope
    var sErrCtx;
    try
    {
      // Create a pig session on the server
      sErrCtx = "create Pig session";
      var oSession = GetDeployment().CreateObject("Pigs", sPigSrv);

      // Get the build number of the pig session
      sErrCtx = "retrieve Pigs build number";
      var nBuildNumber = oSession.Version.ProductBuildNumber;

      // Report the build number of the pigs
      sErrCtx = "report Pigs build number";
      g_dictResults(sPigSrv) = "Running build " + nBuildNumber;
      DoMessage(sPigSrv + ": " + g_dictResults(sPigSrv));
    }
    catch (e)
    {
      // Log the error
      g_dictResults(sPigSrv) = "Failed to " + sErrCtx + ":\n  " + ErrorText(e);
      DoMessage(sPigSrv + ":\n  " + g_dictResults(sPigSrv));
      DoMessage("------------------------------------------------------------------------");

      // Remove the server name from the dictionary
      dict.Remove(sPigSrv);
    }
  }

  // Output a seperator line to the log
  DoMessage("------------------------------------------------------------------------");
}


/////////////////////////////////////////////////////////////////////////////
// 
function Run(sCmd, sStartDir, sTempName)
{
  // Declare Constants
  var ForReading = 1, ShowCmd_Hide = 0;

  // Determine the file name for capturing stdout/stderr
  var strOut = g_sLogDir + "\\" + sTempName + ".log";
  var strErr = strOut;

  // Execute the full command line
  DoLogDetail("------------------------------------------------------------------------");
  DoLogDetail("Setup Step:     " + sTempName);
  DoLogDetail("Command Line:   " + sCmd);
  DoLogDetail("From Directory: " + sStartDir);
  DoLogDetail("StdOut File:    " + strOut);
  DoLogDetail("StdErr File:    " + strErr);
  var nResult = GetDeployment().ExecuteAndWait(sCmd, sStartDir, strOut, strErr, "", ShowCmd_Hide);
  DoLogDetail("Return Code:    " + Run);

  // Copy the output results of the command
  var ResultFile, sLine;

  // Copy the StdOut captured file results of the command
  var ResultFile = g_fso.OpenTextFile(strOut, ForReading, false);
  if (ResultFile.AtEndOfStream)
  {
    DoLogDetail("StdOut Interlaced with StdErr: (empty)----------------------------------");
  }
  else
  {
    DoLogDetail("StdOut Interlaced with StdErr:------------------------------------------");
    while (!ResultFile.AtEndOfStream)
    {
      var sLine = ResultFile.ReadLine;
      DoLogDetail("> " + sLine);
    }
  }

  // Output a seperator line to the log
  DoLogDetail("------------------------------------------------------------------------");
}


/////////////////////////////////////////////////////////////////////////////
// 
function GetCommandList(cArgs, args)
{
  // Create a dictionary object with text compare mode
  var d = new ActiveXObject("Scripting.Dictionary");
  d.CompareMode = CompareMode_Text;

  try
  {
    // Loop through each argument
    for (i = 0; i < cArgs; ++i)
    {
      // Get the argument and its first character
      var sArg = args(i), sArgName = sArg.substr(1);
      var sChar = sArg.charAt(0);

      // Ensure that the option begins with '/' or '-'
      if ("/" != sChar && "-" != sChar)
        throw "Option does not start with '/' or '-': '" + sArg + "'.";

      // Ensure that the option is more than just '/' or '-'
      if (sArg.length < 2)
        throw "Invalid option: '" + sArg + "'.";

      // Ensure that the option has not already been specified
      if (d.Exists(sArgName))
        throw "Duplicate option: '" + sArg + "'.";

      // Get the next argument and its first character
      var sArgNext = (i < cArgs - 1) ? args(i + 1) : "";
      sChar = sArgNext.charAt(0);

      // The next argument is either this option's value or another option
      var sValue = "";
      if ("/" != sChar && "-" != sChar)
      {
        sValue = sArgNext;
        ++i;
      }

      // Add the value to the dictionary
      d(sArgName) = sValue;
    }
  }
  catch (e)
  {
    // Re-throw the error if it's not a string
    if ("string" != typeof(e))
      throw e;

    // Set the "Error" key to the error string
    d("Error") = e;
  }

  // Return the dictionary object
  return d;
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
// Description: Gets the syntax of the script usage.
//
function SyntaxText()
{
  //  0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
  var sText =
      "Usage:  DeployPigs <args>\n"
    + "Where:  <args> can be one or more of the following:\n\n"
    + "  /SetupPigs <list> The list of computers on which to install ActivePigs.\n"
    + "  /AllSrv <server>  The name of the computer running the Allegiance Game Server\n"
    + "                    against which ActivePigs will test.\n"
    + "  /Silent           Suppresses all output messages.\n"
    + "  /NoLogo           Suppresses output of the initial logo text.\n"
    + "  /Label <label>    The build number of ActivePigs to setup.\n"
    + "                    Defaults to the latest build.\n";
  return sText;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Used by the GetDeployment function to ensure that only one
// instance of the TCDeployment object is created.
//
var g_oDeploy;

/////////////////////////////////////////////////////////////////////////////
// Description: Creates the TCDeployment object as g_oDeploy.
// 
function GetDeployment()
{
  if (!g_oDeploy)
    g_oDeploy = new ActiveXObject("TCDeployment");
  return g_oDeploy;
}

