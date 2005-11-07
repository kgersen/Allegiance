'==============================================================================
'
'   This script will attempt to create an instance of the TCDeployment object.
'   If it fails, it will exit with the code 1, indicating that it needs to be
'   updated. Otherwise it will return 0, indicating that it is up to date.
'
'   TODO: Once created, the objects' Version property should be compared to
'   that of the latest DLL. However, we currently have no way to get the
'   latest DLL's version. Reading a text file would probably be sufficient.
'

' Require explicit variable Dim'ing
Option Explicit

' Continue after errors
On Error Resume Next

' Attempt to create the TCDeployment object
Err.Clear
Dim oDeploy
Set oDeploy = CreateObject("TCDeployment")

' Return 1 if object creation failed
If Err.Number Then
    WScript.Quit(1)
End If

' Get the latest object's version
Err.Clear
Dim oVersionNew
Set oVersionNew = CreateObject("TCDeployment.Version")

' Return 1 if object creation failed
If Err.Number Then
    WScript.Quit(1)
End If

' Get the path of this script file
Dim fso
Set fso = CreateObject("Scripting.FileSystemObject")
Dim sPath
sPath = fso.GetFile(Wscript.ScriptFullName).ParentFolder.Path

' Set the DLL name
oVersionNew.FileName = sPath & "\TCDeploy.dll"

' Get the latest object's version
Dim nVersionLatest
nVersionLatest = oVersionNew.FileBuildNumber
If Err.Number Then
    WScript.Quit(1)
End If

' Compare the latest object's version
If nVersionLatest > oDeploy.Version.FileBuildNumber Then
    WScript.Quit(1)
End If

' Return 0
WScript.Quit(0)

