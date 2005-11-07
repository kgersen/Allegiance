# Microsoft Developer Studio Project File - Name="me" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=me - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "me.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "me.mak" CFG="me - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "me - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "me - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "me - Win32 IceCap" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion ""

!IF  "$(CFG)" == "me - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f me.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "me.exe"
# PROP BASE Bsc_Name "me.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake retail= me"
# PROP Rebuild_Opt "clean"
# PROP Target_File "c:/fed/objs/retail/me/me.exe"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "me - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f me.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "me.exe"
# PROP BASE Bsc_Name "me.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake me"
# PROP Rebuild_Opt "clean"
# PROP Bsc_Name "d:\fed\objs\debug\wintrek\allegiance.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "me - Win32 IceCap"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "me___Win32_IceCap"
# PROP BASE Intermediate_Dir "me___Win32_IceCap"
# PROP BASE Cmd_Line "nmake retail= me"
# PROP BASE Rebuild_Opt "clean"
# PROP BASE Target_File "c:/fed/objs/retail/me/me.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "me___Win32_IceCap"
# PROP Intermediate_Dir "me___Win32_IceCap"
# PROP Cmd_Line "nmake icap= icapme"
# PROP Rebuild_Opt "clean"
# PROP Target_File "c:/fed/objs/icap/me/me.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "me - Win32 Release"
# Name "me - Win32 Debug"
# Name "me - Win32 IceCap"

!IF  "$(CFG)" == "me - Win32 Release"

!ELSEIF  "$(CFG)" == "me - Win32 Debug"

!ELSEIF  "$(CFG)" == "me - Win32 IceCap"

!ENDIF 

# Begin Source File

SOURCE=.\me\me.cpp
# End Source File
# End Target
# End Project
