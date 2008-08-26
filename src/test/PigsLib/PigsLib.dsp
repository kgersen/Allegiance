# Microsoft Developer Studio Project File - Name="PigsLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=PigsLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PigsLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PigsLib.mak" CFG="PigsLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PigsLib - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "PigsLib - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "PigsLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\objs\Debug\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\Debug\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\objs\Debug\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\Debug\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\debug\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\debug\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Retail\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\Retail\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Retail\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\Retail\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\retail\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\retail\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Test\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\Test\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Test\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\Test\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\test\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\test\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BCheckerD\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\BCheckerD\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BCheckerD\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\BCheckerD\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\BCheckerD\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\BCheckerD\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BChecker\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\BChecker\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BChecker\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\BChecker\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\BChecker\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\BChecker\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICapD\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICapD\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICapD\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\ICapD\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\ICapD\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\ICapD\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICap\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICap\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICap\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\ICap\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\ICap\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\ICap\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTimeD\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTimeD\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\TrueTimeD\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\TrueTimeD\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PigsLib - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTime\Test\PigsLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTime\Test\PigsLib"
# PROP BASE Cmd_Line "NMAKE /f PigsLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PigsLib.dll"
# PROP BASE Bsc_Name "PigsLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTime\Test\PigsLib"
# PROP Intermediate_Dir "..\..\..\objs\TrueTime\Test\PigsLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=PigsLib "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\TrueTime\test\PigsLib\PigsLib.dll"
# PROP Bsc_Name "..\..\..\objs\TrueTime\test\PigsLib\PigsLib.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "PigsLib - Win32 Debug"
# Name "PigsLib - Win32 Retail"
# Name "PigsLib - Win32 Test"
# Name "PigsLib - Win32 BCheckerD"
# Name "PigsLib - Win32 BChecker"
# Name "PigsLib - Win32 ICapD"
# Name "PigsLib - Win32 ICap"
# Name "PigsLib - Win32 TrueTimeD"
# Name "PigsLib - Win32 TrueTime"

!IF  "$(CFG)" == "PigsLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 Retail"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 Test"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 BChecker"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 ICapD"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 ICap"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "PigsLib - Win32 TrueTime"

!ENDIF 

# Begin Source File

SOURCE=.\AGCCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCCommand.h
# End Source File
# Begin Source File

SOURCE=.\AGCCommand.rgs
# End Source File
# Begin Source File

SOURCE=..\Inc\Allegiance.h
# End Source File
# Begin Source File

SOURCE=.\guid.cpp
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

SOURCE=.\PigMissionParams.cpp
# End Source File
# Begin Source File

SOURCE=.\PigMissionParams.h
# End Source File
# Begin Source File

SOURCE=.\PigMissionParams.rgs
# End Source File
# Begin Source File

SOURCE=..\Inc\PigsIDL.idl
# End Source File
# Begin Source File

SOURCE=.\PigsLib.cpp
# End Source File
# Begin Source File

SOURCE=.\PigsLib.def
# End Source File
# Begin Source File

SOURCE=.\PigsLib.idl
# End Source File
# Begin Source File

SOURCE=.\PigsLib.rc
# End Source File
# Begin Source File

SOURCE=.\PigsLib.rc2
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SrcInc.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcInc.h
# End Source File
# End Target
# End Project
