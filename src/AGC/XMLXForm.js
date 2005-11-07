
// Validate the specified number of arguments
if (WScript.Arguments.Count() < 3)
{
  WScript.Echo("Usage: xmlxform <xmlfile> <xslfile> <outputfile>");
  WScript.Quit();
}

// Get the specified arguments
var strFileXML = WScript.Arguments(0);
var strFileXSL = WScript.Arguments(1);
var strFileOut = WScript.Arguments(2);

// Load the XML file
var doc = new ActiveXObject("microsoft.xmldom");
if (!doc.load(strFileXML)) 
{
  var parseError = doc.parseError;
  var strErr = "Error loading " + strFileXML + "\n";
  strErr += parseError.reason   + "\n"
  strErr += parseError.srcText;
  WScript.Echo(strErr);
  WScript.Quit(2);
}

// Load the XSL file
var sheet = new ActiveXObject("microsoft.xmldom");
if (!sheet.load(strFileXSL)) 
{
  var parseError = doc.parseError;
  var strErr = "Error loading " + strFileXSL + "\n";
  strErr += parseError.reason   + "\n"
  strErr += parseError.srcText;
  WScript.Echo(strErr);
  WScript.Quit(2);
}

// Open the output file
var fso     = new ActiveXObject("Scripting.FileSystemObject");
var fileOut = fso.CreateTextFile(strFileOut, true);

// Add some values to the root element
var el = doc.documentElement;
el.setAttribute("time"          , (new Date).toLocaleString());
el.setAttribute("scriptfullname", WScript.ScriptFullname);
el.setAttribute("xmlfile"       , fso.GetFile(strFileXML).Path);
el.setAttribute("xslfile"       , fso.GetFile(strFileXSL).Path);
el.setAttribute("outputfile"    , fso.GetFile(strFileOut).Path);

// Transform the XML document
var outHTML = doc.transformNode(sheet);

// Write the transformed text to the output file
fileOut.Write(outHTML);


