# Microsoft Developer Studio Project File - Name="Utility" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Utility - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Utility.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Utility.mak" CFG="Utility - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Utility - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "Utility - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Utility - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\objs\Debug\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\Debug\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\objs\Debug\Utility"
# PROP Intermediate_Dir "..\..\objs\Debug\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\debug\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\debug\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Retail\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\Retail\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Retail\Utility"
# PROP Intermediate_Dir "..\..\objs\Retail\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\retail\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\retail\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Test\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\Test\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Test\Utility"
# PROP Intermediate_Dir "..\..\objs\Test\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\test\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\test\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BCheckerD\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\BCheckerD\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BCheckerD\Utility"
# PROP Intermediate_Dir "..\..\objs\BCheckerD\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\BCheckerD\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\BCheckerD\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BChecker\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\BChecker\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BChecker\Utility"
# PROP Intermediate_Dir "..\..\objs\BChecker\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\BChecker\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\BChecker\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICapD\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\ICapD\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICapD\Utility"
# PROP Intermediate_Dir "..\..\objs\ICapD\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\ICapD\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\ICapD\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICap\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\ICap\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICap\Utility"
# PROP Intermediate_Dir "..\..\objs\ICap\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\ICap\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\ICap\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTimeD\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTimeD\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTimeD\Utility"
# PROP Intermediate_Dir "..\..\objs\TrueTimeD\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\TrueTimeD\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\TrueTimeD\_utility\Utility.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Utility - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTime\Utility"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTime\Utility"
# PROP BASE Cmd_Line "NMAKE /f Utility.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Utility.lib"
# PROP BASE Bsc_Name "Utility.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTime\Utility"
# PROP Intermediate_Dir "..\..\objs\TrueTime\Utility"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Utility "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\TrueTime\_utility\Utility.lib"
# PROP Bsc_Name "..\..\objs\TrueTime\_utility\Utility.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Utility - Win32 Debug"
# Name "Utility - Win32 Retail"
# Name "Utility - Win32 Test"
# Name "Utility - Win32 BCheckerD"
# Name "Utility - Win32 BChecker"
# Name "Utility - Win32 ICapD"
# Name "Utility - Win32 ICap"
# Name "Utility - Win32 TrueTimeD"
# Name "Utility - Win32 TrueTime"

!IF  "$(CFG)" == "Utility - Win32 Debug"

!ELSEIF  "$(CFG)" == "Utility - Win32 Retail"

!ELSEIF  "$(CFG)" == "Utility - Win32 Test"

!ELSEIF  "$(CFG)" == "Utility - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "Utility - Win32 BChecker"

!ELSEIF  "$(CFG)" == "Utility - Win32 ICapD"

!ELSEIF  "$(CFG)" == "Utility - Win32 ICap"

!ELSEIF  "$(CFG)" == "Utility - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "Utility - Win32 TrueTime"

!ENDIF 

# Begin Source File

SOURCE=.\CollisionEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\CollisionQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\HitTest.cpp
# End Source File
# Begin Source File

SOURCE=.\KDnode.cpp
# End Source File
# Begin Source File

SOURCE=.\KDroot.cpp
# End Source File
# Begin Source File

SOURCE=.\listwrappers.h
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=..\Inc\messagecore.h
# End Source File
# Begin Source File

SOURCE=.\Messages.cpp
# End Source File
# Begin Source File

SOURCE=..\Inc\Messages.h
# End Source File
# Begin Source File

SOURCE=.\pch.cpp
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\Utility.h
# End Source File
# End Target
# End Project
