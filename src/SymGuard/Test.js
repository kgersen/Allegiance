
  // Create the SymGuard.Application object
  //   (this is designed to be safely cached in an ASP Application object variable)
  var app = new ActiveXObject("SymGuard.Application");

  // Declare input values
  var strSymbolPath = "C:\\temp";
  var strModuleName = "Allegiance.exe"; // Get from XML
  var strImageBase  = "00400000";   // Get from XML
  var strImageSize  = "001D8000";   // Get from XML

  // Create a SymGuard.Module object
  var timeBegin = new Date();
    var module1 = app.LoadModule(strSymbolPath, strModuleName, strImageBase, strImageSize);
  var timeEnd = new Date();
    DisplaySymModule(module1);
  WScript.Echo("First LoadModule: " + (timeEnd - timeBegin) + " ms\n");


function DisplaySymModule(module)
{
  // Display the properties of the module or symbol file
  WScript.Echo("SymbolPath      = "   +     module.SymbolPath     );
  WScript.Echo("ModuleName      = "   +     module.ModuleName     );
  WScript.Echo("ImageBase       = 0x" + Hex(module.ImageBase)     );
  WScript.Echo("ImageSize       = 0x" + Hex(module.ImageSize)     );
  WScript.Echo("TimeDateStamp   = "   +     module.TimeDateStamp  );
  WScript.Echo("CheckSum        = 0x" + Hex(module.CheckSum)      );
  WScript.Echo("SymbolCount     = 0x" + Hex(module.SymbolCount)   );
  WScript.Echo("SymbolType      = "   +     module.SymbolType     );
  WScript.Echo("ImageName       = "   +     module.ImageName      );
  WScript.Echo("LoadedImageName = "   +     module.LoadedImageName);

  // Set the path-formatting options
  module.BuildPathBase  = "D:\\fedsrc";                            // Gets stripped from beginning
  module.SourcePathBase = "\\\\oblivion0\\builds\\2006\\CD2\\Src"; // Gets prepended to beginning

  // Display the symbol information for several addresses
  DisplaySymAddress(module, "00426297");         // Get from XML
  DisplaySymAddress(module, "0042117B");         // Get from XML
  DisplaySymAddress(module, "0041F271");         // Get from XML
  DisplaySymAddress(module, "004C9CC1");         // Get from XML
  DisplaySymAddress(module, "004C9F50");         // Get from XML
  DisplaySymAddress(module, "004A6610");         // Get from XML
  DisplaySymAddress(module, "004A680A");         // Get from XML
  DisplaySymAddress(module, "004AB998");         // Get from XML
  DisplaySymAddress(module, "004757DF");         // Get from XML
  DisplaySymAddress(module, "004A2DF8");         // Get from XML
}


/////////////////////////////////////////////////////////////////////////////
// Displays all the known symbol information for the symbol at the specified
// address within the specified module.
//
function DisplaySymAddress(module, addr)
{
  // Find the symbol information for the specified address
  var sym = module.GetSymbolFromAddress(addr);

  // Display the properties of the symbol information object
  WScript.Echo("    " + module.ModuleName + "!" + Hex(sym.Address) + " " + sym.Name + " " + sym.DecoratedName);
  if (sym.SourceFileName.length && sym.LineNumber)
  {
    WScript.Echo("        " + sym.SourceFileName + "(" + sym.LineNumber + ")");
    WScript.Echo("        BytesFromSymbol = " + sym.BytesFromSymbol);
    WScript.Echo("        BytesFromLine   = " + sym.BytesFromLine);
  }
}


/////////////////////////////////////////////////////////////////////////////
// Converts the specified value to an 8-character hex string.
//
function Hex(val)
{
  var str = "0000000" + new Number(val).toString(16);
  return str.substr(str.length - 8);
}


