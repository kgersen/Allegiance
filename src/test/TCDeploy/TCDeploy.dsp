# Microsoft Developer Studio Project File - Name="TCDeploy" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=TCDeploy - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TCDeploy.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TCDeploy.mak" CFG="TCDeploy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TCDeploy - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "TCDeploy - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "TCDeploy - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\objs\Debug\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\Debug\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\objs\Debug\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\Debug\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sdebug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\sdebug\test\TCDeploy\TCDeploy.dll"
# PROP Bsc_Name "..\..\..\objs\sdebug\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Retail\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\Retail\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Retail\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\Retail\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sretail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\sretail\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\sretail\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Test\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\Test\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Test\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\Test\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=stest=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\test\stest\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\test\stest\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BCheckerD\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\BCheckerD\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BCheckerD\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\BCheckerD\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sdebug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\SBCheckerD\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\SBCheckerD\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BChecker\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\BChecker\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BChecker\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\BChecker\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sretail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\SBChecker\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\SBChecker\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICapD\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICapD\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICapD\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\ICapD\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sdebug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\SICapD\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\SICapD\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICap\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICap\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICap\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\ICap\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sretail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\SICap\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\SICap\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTimeD\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTimeD\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sdebug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\STrueTimeD\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\STrueTimeD\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTime\Test\TCDeploy"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTime\Test\TCDeploy"
# PROP BASE Cmd_Line "NMAKE /f TCDeploy.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCDeploy.lib"
# PROP BASE Bsc_Name "TCDeploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTime\Test\TCDeploy"
# PROP Intermediate_Dir "..\..\..\objs\TrueTime\Test\TCDeploy"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCDeploy "FLAVOR=sretail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\STrueTime\test\TCDeploy\TCDeploy.lib"
# PROP Bsc_Name "..\..\..\objs\STrueTime\test\TCDeploy\TCDeploy.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "TCDeploy - Win32 Debug"
# Name "TCDeploy - Win32 Retail"
# Name "TCDeploy - Win32 Test"
# Name "TCDeploy - Win32 BCheckerD"
# Name "TCDeploy - Win32 BChecker"
# Name "TCDeploy - Win32 ICapD"
# Name "TCDeploy - Win32 ICap"
# Name "TCDeploy - Win32 TrueTimeD"
# Name "TCDeploy - Win32 TrueTime"

!IF  "$(CFG)" == "TCDeploy - Win32 Debug"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 Retail"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 Test"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 BChecker"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 ICapD"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 ICap"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "TCDeploy - Win32 TrueTime"

!ENDIF 

# Begin Group "Batch Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IsNewer.vbs
# End Source File
# Begin Source File

SOURCE=.\SetupDeploy.bat
# End Source File
# End Group
# Begin Group "Test Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TCDeploymentTestPage.htm
# End Source File
# Begin Source File

SOURCE=.\TCDInproc.htm
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

SOURCE=.\TCDeploy.cpp
# End Source File
# Begin Source File

SOURCE=.\TCDeploy.def
# End Source File
# Begin Source File

SOURCE=.\TCDeploy.idl
# End Source File
# Begin Source File

SOURCE=.\TCDeploy.rc
# End Source File
# Begin Source File

SOURCE=.\TCDeploy.rc2
# End Source File
# Begin Source File

SOURCE=.\TCDeployment.cpp
# End Source File
# Begin Source File

SOURCE=.\TCDeployment.h
# End Source File
# Begin Source File

SOURCE=.\TCDeployment.rgs
# End Source File
# Begin Source File

SOURCE=.\TCDeploymentProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\TCDeploymentProcess.h
# End Source File
# Begin Source File

SOURCE=.\TCDeploymentProcess.rgs
# End Source File
# Begin Source File

SOURCE=.\TCDeploymentVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\TCDeploymentVersion.h
# End Source File
# Begin Source File

SOURCE=.\TCDeploymentVersion.rgs
# End Source File
# End Target
# End Project
