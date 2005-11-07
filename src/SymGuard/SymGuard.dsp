# Microsoft Developer Studio Project File - Name="SymGuard" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=SymGuard - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SymGuard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SymGuard.mak" CFG="SymGuard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SymGuard - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "SymGuard - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "SymGuard - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\objs\Debug\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\Debug\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\objs\Debug\SymGuard"
# PROP Intermediate_Dir "..\..\objs\Debug\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sdebug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\sdebug\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\sdebug\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Retail\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\Retail\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Retail\SymGuard"
# PROP Intermediate_Dir "..\..\objs\Retail\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sretail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\sretail\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\sretail\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Test\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\Test\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Test\SymGuard"
# PROP Intermediate_Dir "..\..\objs\Test\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=stest=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\test\stest\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\test\stest\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BCheckerD\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\BCheckerD\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BCheckerD\SymGuard"
# PROP Intermediate_Dir "..\..\objs\BCheckerD\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sdebug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\SBCheckerD\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\SBCheckerD\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BChecker\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\BChecker\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BChecker\SymGuard"
# PROP Intermediate_Dir "..\..\objs\BChecker\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sretail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\SBChecker\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\SBChecker\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICapD\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\ICapD\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICapD\SymGuard"
# PROP Intermediate_Dir "..\..\objs\ICapD\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sdebug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\SICapD\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\SICapD\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICap\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\ICap\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICap\SymGuard"
# PROP Intermediate_Dir "..\..\objs\ICap\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sretail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\SICap\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\SICap\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTimeD\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTimeD\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTimeD\SymGuard"
# PROP Intermediate_Dir "..\..\objs\TrueTimeD\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sdebug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\STrueTimeD\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\STrueTimeD\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SymGuard - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTime\SymGuard"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTime\SymGuard"
# PROP BASE Cmd_Line "NMAKE /f SymGuard.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SymGuard.lib"
# PROP BASE Bsc_Name "SymGuard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTime\SymGuard"
# PROP Intermediate_Dir "..\..\objs\TrueTime\SymGuard"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=SymGuard "FLAVOR=sretail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\STrueTime\SymGuard\SymGuard.dll"
# PROP Bsc_Name "..\..\objs\STrueTime\SymGuard\SymGuard.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "SymGuard - Win32 Debug"
# Name "SymGuard - Win32 Retail"
# Name "SymGuard - Win32 Test"
# Name "SymGuard - Win32 BCheckerD"
# Name "SymGuard - Win32 BChecker"
# Name "SymGuard - Win32 ICapD"
# Name "SymGuard - Win32 ICap"
# Name "SymGuard - Win32 TrueTimeD"
# Name "SymGuard - Win32 TrueTime"

!IF  "$(CFG)" == "SymGuard - Win32 Debug"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 Retail"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 Test"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 BChecker"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 ICapD"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 ICap"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "SymGuard - Win32 TrueTime"

!ENDIF 

# Begin Group "Examples"

# PROP Default_Filter "js;vbs;wsc;htm;html;asp;"
# Begin Source File

SOURCE=.\Lookup.js
# End Source File
# Begin Source File

SOURCE=.\Lookup.vbs
# End Source File
# Begin Source File

SOURCE=.\Test.js
# End Source File
# End Group
# Begin Source File

SOURCE=.\dlldatax.c
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\pch.cpp
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SymGuard.cpp
# End Source File
# Begin Source File

SOURCE=.\SymGuard.def
# End Source File
# Begin Source File

SOURCE=.\SymGuard.idl
# End Source File
# Begin Source File

SOURCE=.\SymGuard.rc
# End Source File
# Begin Source File

SOURCE=.\SymGuard.rc2
# End Source File
# Begin Source File

SOURCE=.\SymGuardApp.cpp
# End Source File
# Begin Source File

SOURCE=.\SymGuardApp.h
# End Source File
# Begin Source File

SOURCE=.\SymGuardApp.rgs
# End Source File
# Begin Source File

SOURCE=.\SymGuardVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\SymGuardVersion.h
# End Source File
# Begin Source File

SOURCE=.\SymGuardVersion.rgs
# End Source File
# Begin Source File

SOURCE=.\SymInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SymInfo.h
# End Source File
# Begin Source File

SOURCE=.\SymInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\SymModule.cpp
# End Source File
# Begin Source File

SOURCE=.\SymModule.h
# End Source File
# Begin Source File

SOURCE=.\SymModule.rgs
# End Source File
# End Target
# End Project
