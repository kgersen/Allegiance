# Microsoft Developer Studio Project File - Name="mdledit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=mdledit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mdledit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mdledit.mak" CFG="mdledit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mdledit - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "mdledit - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "mdledit - Win32 IceCap" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion ""

!IF  "$(CFG)" == "mdledit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f mdledit.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "mdledit.exe"
# PROP BASE Bsc_Name "mdledit.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake retail= mdledit"
# PROP Rebuild_Opt "clean"
# PROP Target_File "c:/fed/objs/retail/mdledit/mdledit.exe"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "mdledit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f mdledit.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "mdledit.exe"
# PROP BASE Bsc_Name "mdledit.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake mdledit"
# PROP Rebuild_Opt "clean"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "mdledit - Win32 IceCap"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mdledit___Win32_IceCap"
# PROP BASE Intermediate_Dir "mdledit___Win32_IceCap"
# PROP BASE Cmd_Line "nmake retail= mdledit"
# PROP BASE Rebuild_Opt "clean"
# PROP BASE Target_File "c:/fed/objs/retail/mdledit/mdledit.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "mdledit___Win32_IceCap"
# PROP Intermediate_Dir "mdledit___Win32_IceCap"
# PROP Cmd_Line "nmake icap= icapmdledit"
# PROP Rebuild_Opt "clean"
# PROP Target_File "c:/fed/objs/icap/mdledit/mdledit.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "mdledit - Win32 Release"
# Name "mdledit - Win32 Debug"
# Name "mdledit - Win32 IceCap"

!IF  "$(CFG)" == "mdledit - Win32 Release"

!ELSEIF  "$(CFG)" == "mdledit - Win32 Debug"

!ELSEIF  "$(CFG)" == "mdledit - Win32 IceCap"

!ENDIF 

# Begin Source File

SOURCE=.\mdledit\mdledit.cpp
# End Source File
# End Target
# End Project
