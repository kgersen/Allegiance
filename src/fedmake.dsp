# Microsoft Developer Studio Project File - Name="fedmake" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=fedmake - Win32 Icecap
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fedmake.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fedmake.mak" CFG="fedmake - Win32 Icecap"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fedmake - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "fedmake - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "fedmake - Win32 Icecap" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion ""

!IF  "$(CFG)" == "fedmake - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f fedmake.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "fedmake.exe"
# PROP BASE Bsc_Name "fedmake.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake Client retail="
# PROP Rebuild_Opt "Full"
# PROP Target_File "d:\fed\objs\retail\wintrek\allegiance.exe"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "fedmake - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f fedmake.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "fedmake.exe"
# PROP BASE Bsc_Name "fedmake.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake Client"
# PROP Rebuild_Opt "Full"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "fedmake - Win32 Icecap"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fedmake___Win32_Icecap"
# PROP BASE Intermediate_Dir "fedmake___Win32_Icecap"
# PROP BASE Cmd_Line "nmake Client"
# PROP BASE Rebuild_Opt "Full"
# PROP BASE Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fedmake___Win32_Icecap"
# PROP Intermediate_Dir "fedmake___Win32_Icecap"
# PROP Cmd_Line "nmake icap= icapclient"
# PROP Rebuild_Opt "Full"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "fedmake - Win32 Release"
# Name "fedmake - Win32 Debug"
# Name "fedmake - Win32 Icecap"

!IF  "$(CFG)" == "fedmake - Win32 Release"

!ELSEIF  "$(CFG)" == "fedmake - Win32 Debug"

!ELSEIF  "$(CFG)" == "fedmake - Win32 Icecap"

!ENDIF 

# End Target
# End Project
