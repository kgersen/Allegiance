# Microsoft Developer Studio Project File - Name="engineapp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=engineapp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "engineapp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "engineapp.mak" CFG="engineapp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "engineapp - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "engineapp - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "engineapp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f engineapp.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "engineapp.exe"
# PROP BASE Bsc_Name "engineapp.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake retail= NOBSC=x"
# PROP Rebuild_Opt "clean"
# PROP Target_File "c:/fed/objs/retail/engineapp/engineapp.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "engineapp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f engineapp.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "engineapp.exe"
# PROP BASE Bsc_Name "engineapp.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake NOBSC=x"
# PROP Rebuild_Opt "clean"
# PROP Target_File "c:/fed/objs/debug/engineapp/engineapp.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "engineapp - Win32 Release"
# Name "engineapp - Win32 Debug"

!IF  "$(CFG)" == "engineapp - Win32 Release"

!ELSEIF  "$(CFG)" == "engineapp - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=.\engineapp.cpp
# End Source File
# Begin Source File

SOURCE=.\pch.cpp
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# End Target
# End Project
