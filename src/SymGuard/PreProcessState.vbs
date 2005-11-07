'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Dim appSym
Set appSym = CreateObject("SymGuard.Application")


Dim docXML
Set docXML = CreateObject("Microsoft.XMLDOM")

docXML.async = False
If docXML.load("C:\Temp\GetDump.xml") Then

    PreProcessXState docXML, "\\oblivion0\Symbols\2003", "D:\FedSrc", appSym

    docXML.save("C:\Temp\Output.xml")

End If

Dim str1, str2, str3, str4, str5

str1 = "::APPVERlsLo:: asdasdfasdf"
str2 = "::APPVERLsLow::"
str3 = "::ApPvEr::!::AppVer::"
str4 = "jolly good nothing!"

WScript.Echo str1 & " -> """ & ReplacePathVerKeywords("1.2.343.1999", str1, "AppVer") & """"
WScript.Echo str2 & " -> """ & ReplacePathVerKeywords("1.2.343.1999", str2, "AppVer") & """"
WScript.Echo str3 & " -> """ & ReplacePathVerKeywords("1.2.343.1999", str3, "AppVer") & """"
WScript.Echo str4 & " -> """ & ReplacePathVerKeywords("1.2.343.1999", str4, "AppVer") & """"




'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
'
Sub PreProcessXState(xml, sSymPath, sBldRoot, app)

    On Error Resume Next

    ' Keep track of SymGuard.Module objects for each module
    Dim dictMods
    Set dictMods = CreateObject("Scripting.Dictionary")

    ' Resolve short names (no drive:\path\) for all of the modules
    Dim nodesMods
    Set nodesMods = xml.selectNodes("/State/Modules/Module")

    Dim nodeMod
    Do
        Set nodeMod = nodesMods.nextNode
        If Not nodeMod Is Nothing Then

            Dim sName
            sName = GetModuleShortName(nodeMod.getAttribute("Name"))

            ' Attempt to load symbols for this module
            Dim oMod
            Set oMod = app.LoadModule(sSymPath, sName, _
                nodeMod.getAttribute("LoadAddr"),      _
                nodeMod.getAttribute("LoadSize"))

            ' Map the module name to the symbol module object
            If Not oMod Is Nothing Then
                Set dictMods(sName) = oMod
            End If

        End If

    Loop While Not nodeMod Is Nothing


    ' Attempt to resolve symbol names for each frame of each thread
    Dim nodesFrames
    Set nodesFrames = xml.selectNodes("/State/Threads/Thread/Stack/Frm")

    Dim nodeFrame
    Do
        Set nodeFrame = nodesFrames.nextNode
        If Not nodeFrame Is Nothing Then

            ' Lookup module information
            Dim sMod
            sMod = nodeFrame.getAttribute("Module")
            If "" <> sMod Then

                If dictMods.Exists(sMod) Then

                    Dim module
                    Set module = Nothing
                    Set module = dictMods(sMod)

                    Dim addr
                    addr = nodeFrame.getAttribute("ProgCnt")

                    Dim sym
                    Set sym = Nothing
                    Set sym = module.GetSymbolFromAddress(addr)
                    If Not sym Is Nothing Then

                        nodeFrame.setAttribute "SymName"        , sym.Name
                        nodeFrame.setAttribute "DecName"        , sym.DecoratedName
                        nodeFrame.setAttribute "SrcFile"        , sym.SourceFileName
                        nodeFrame.setAttribute "SrcLine"        , sym.LineNumber
                        nodeFrame.setAttribute "BytesFromSymbol", sym.BytesFromSymbol
                        nodeFrame.setAttribute "BytesFromLine"  , sym.BytesFromLine  

                    End If                    

                End If

            End If

        End If

    Loop While Not nodeFrame Is Nothing

End Sub


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
'
Function GetModuleShortName(sName)

    If "" = sName Then

        GetModuleShortName = sName

    Else

        ' Remove the path, if any
        Dim iWhack
        iWhack = InStrRev(sName, "\", -1, 1)
        If iWhack Then
            GetModuleShortName = Mid(sName, iWhack + 1)
        End If

    End If

End Function


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'
'
Function ReplacePathVerKeywords(sVersion, sPath, sVerPrefix)

    ' Break apart the four parts of the version
    Dim regEx, matches
    Set regEx = New RegExp
    regEx.Pattern = "(\d+)\.(\d+)\.(\d+)\.(\d+)"
    regEx.Global = False

    Dim sVersionMSHigh, sVersionMSLow, sVersionLSHight, sVerionsLSLow
    sVersionMSHigh = regEx.Replace(sVersion, "$1")
    sVersionMSLow  = regEx.Replace(sVersion, "$2")
    sVersionLSHigh = regEx.Replace(sVersion, "$3")
    sVersionLSLow  = regEx.Replace(sVersion, "$4")

    ' Do the replacements
    sPath = Replace(sPath, "::" & sVerPrefix & "MSHigh::", sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "MSHi::"  , sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "MSLow::" , sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "MSLo::"  , sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "LSHigh::", sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "LSHi::"  , sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "LSLow::" , sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "LSLo::"  , sVersionMSHigh, 1, -1, 1)
    sPath = Replace(sPath, "::" & sVerPrefix & "::"      , sVersion      , 1, -1, 1)

    ' Return the possibly modified string
    ReplacePathVerKeywords  = sPath

End Function

