# Microsoft Developer Studio Project File - Name="TCGuid" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=TCGuid - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TCGUID.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TCGUID.mak" CFG="TCGuid - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TCGuid - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "TCGuid - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "TCGuid - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\objs\Debug\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\Debug\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir "..\..\..\objs\Debug\Test\TCGuid"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\objs\Debug\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\Debug\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\debug\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\debug\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir "..\..\..\objs\Debug\Test\TCGuid"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Retail\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\Retail\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Retail\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\Retail\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\retail\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\retail\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Test\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\Test\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Test\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\Test\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\test\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\test\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BCheckerD\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\BCheckerD\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BCheckerD\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\BCheckerD\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\BCheckerD\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\BCheckerD\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BChecker\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\BChecker\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BChecker\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\BChecker\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\BChecker\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\BChecker\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICapD\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICapD\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICapD\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\ICapD\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\ICapD\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\ICapD\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICap\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICap\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICap\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\ICap\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\ICap\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\ICap\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTimeD\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTimeD\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\TrueTimeD\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\TrueTimeD\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCGuid - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTime\Test\TCGuid"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTime\Test\TCGuid"
# PROP BASE Cmd_Line "NMAKE /f TCGuid.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCGuid.lib"
# PROP BASE Bsc_Name "TCGuid.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTime\Test\TCGuid"
# PROP Intermediate_Dir "..\..\..\objs\TrueTime\Test\TCGuid"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCGuid "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\TrueTime\test\TCGuid\TCGuid.lib"
# PROP Bsc_Name "..\..\..\objs\TrueTime\test\TCGuid\TCGuid.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "TCGuid - Win32 Debug"
# Name "TCGuid - Win32 Retail"
# Name "TCGuid - Win32 Test"
# Name "TCGuid - Win32 BCheckerD"
# Name "TCGuid - Win32 BChecker"
# Name "TCGuid - Win32 ICapD"
# Name "TCGuid - Win32 ICap"
# Name "TCGuid - Win32 TrueTimeD"
# Name "TCGuid - Win32 TrueTime"

!IF  "$(CFG)" == "TCGuid - Win32 Debug"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 Retail"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 Test"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 BChecker"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 ICapD"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 ICap"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "TCGuid - Win32 TrueTime"

!ENDIF 

# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\TCGUID.c
# End Source File
# End Target
# End Project
