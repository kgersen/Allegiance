
  ' Create the SymGuard.Application object
  '   (this is designed to be safely cached in an ASP Application object variable)
  Dim app
  Set app = CreateObject("SymGuard.Application")

  ' Declare input values
  Dim strSymbolPath, strModuleName, strImageBase, strImageSize
'  strSymbolPath = "\\allegiance\props\2006\Symbols"
  strSymbolPath = "C:\temp"
  strModuleName = "Allegiance" ' Get from XML
  strImageBase  = "00400000"   ' Get from XML
  strImageSize  = "001D8000"   ' Get from XML

  ' Create a SymGuard.Module object (may take a while)
  Dim module
  Set module = app.LoadModule(strSymbolPath, strModuleName, strImageBase, strImageSize)

  ' Display the properties of the module or symbol file
  WScript.Echo "SymbolPath      = "   &     module.SymbolPath
  WScript.Echo "ModuleName      = "   &     module.ModuleName
  WScript.Echo "ImageBase       = &H" & Hex(module.ImageBase)
  WScript.Echo "ImageSize       = &H" & Hex(module.ImageSize)
  WScript.Echo "TimeDateStamp   = "   &     module.TimeDateStamp
  WScript.Echo "CheckSum        = &H" & Hex(module.CheckSum)
  WScript.Echo "SymbolCount     = &H" & Hex(module.SymbolCount)
  WScript.Echo "SymbolType      = "   &     module.SymbolType
  WScript.Echo "ImageName       = "   &     module.ImageName
  WScript.Echo "LoadedImageName = "   &     module.LoadedImageName

  ' Set the path-formatting options
  module.BuildPathBase  = "D:\fedsrc"                       ' Gets stripped from beginning
'  module.SourcePathBase = "\\oblivion0\builds\2006\CD2\Src" ' Gets prepended to beginning

  ' Find the symbol information for an address
  Dim strAddr
  strAddr = "00426297"         ' Get from XML

  ' Display the symbol information for several addresses
  DisplaySymAddress module, "00426297"           ' Get from XML
  DisplaySymAddress module, "0042117B"           ' Get from XML
  DisplaySymAddress module, "0041F271"           ' Get from XML
  DisplaySymAddress module, "004C9CC1"           ' Get from XML
  DisplaySymAddress module, "004C9F50"           ' Get from XML
  DisplaySymAddress module, "004A6610"           ' Get from XML
  DisplaySymAddress module, "004A680A"           ' Get from XML
  DisplaySymAddress module, "004AB998"           ' Get from XML
  DisplaySymAddress module, "004757DF"           ' Get from XML
  DisplaySymAddress module, "004A2DF8"           ' Get from XML


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Displays all the known symbol information for the symbol at the specified
' address within the specified module.
'
Sub DisplaySymAddress(module, addr)

  ' Find the symbol information for the specified address
  Dim sym
  Set sym = module.GetSymbolFromAddress(addr)

  ' Display the properties of the symbol information object
  WScript.Echo "    " & module.ModuleName & "!" & Hex(sym.Address) & " " & sym.Name & " " & sym.DecoratedName
  If Len(sym.SourceFileName) And sym.LineNumber Then

    WScript.Echo "        " & sym.SourceFileName & "(" & sym.LineNumber & ")"
    WScript.Echo "        BytesFromSymbol = " & sym.BytesFromSymbol
    WScript.Echo "        BytesFromLine   = " & sym.BytesFromLine

  End If

End Sub


