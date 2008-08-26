# Microsoft Developer Studio Project File - Name="_All" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=_All - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "_All.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "_All.mak" CFG="_All - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "_All - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "_All - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "_All - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\objs\Debug"
# PROP BASE Intermediate_Dir "..\objs\Debug"
# PROP BASE Cmd_Line "NMAKE /f _All.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\objs\Debug"
# PROP Intermediate_Dir "..\objs\Debug"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\objs\debug\NUL"
# PROP Bsc_Name "..\objs\debug\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\objs\Retail"
# PROP BASE Intermediate_Dir "..\objs\Retail"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\objs\Retail"
# PROP Intermediate_Dir "..\objs\Retail"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\objs\retail\NUL"
# PROP Bsc_Name "..\objs\retail\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\objs\Test"
# PROP BASE Intermediate_Dir "..\objs\Test"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\objs\Test"
# PROP Intermediate_Dir "..\objs\Test"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\objs\test\NUL"
# PROP Bsc_Name "..\objs\test\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\objs\BCheckerD"
# PROP BASE Intermediate_Dir "..\objs\BCheckerD"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\objs\BCheckerD"
# PROP Intermediate_Dir "..\objs\BCheckerD"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\objs\BCheckerD\NUL"
# PROP Bsc_Name "..\objs\BCheckerD\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\objs\BChecker"
# PROP BASE Intermediate_Dir "..\objs\BChecker"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\objs\BChecker"
# PROP Intermediate_Dir "..\objs\BChecker"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\objs\BChecker\NUL"
# PROP Bsc_Name "..\objs\BChecker\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\objs\ICapD"
# PROP BASE Intermediate_Dir "..\objs\ICapD"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\objs\ICapD"
# PROP Intermediate_Dir "..\objs\ICapD"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\objs\ICapD\NUL"
# PROP Bsc_Name "..\objs\ICapD\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\objs\ICap"
# PROP BASE Intermediate_Dir "..\objs\ICap"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\objs\ICap"
# PROP Intermediate_Dir "..\objs\ICap"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\objs\ICap\NUL"
# PROP Bsc_Name "..\objs\ICap\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\objs\TrueTimeD"
# PROP BASE Intermediate_Dir "..\objs\TrueTimeD"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\objs\TrueTimeD"
# PROP Intermediate_Dir "..\objs\TrueTimeD"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\objs\TrueTimeD\NUL"
# PROP Bsc_Name "..\objs\TrueTimeD\NUL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "_All - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\objs\TrueTime"
# PROP BASE Intermediate_Dir "..\objs\TrueTime"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NUL"
# PROP BASE Bsc_Name "NUL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\objs\TrueTime"
# PROP Intermediate_Dir "..\objs\TrueTime"
# PROP Cmd_Line "nmake -f .\MkFromDS.mak /nologo TARGET=All "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\objs\TrueTime\NUL"
# PROP Bsc_Name "..\objs\TrueTime\NUL"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "_All - Win32 Debug"
# Name "_All - Win32 Retail"
# Name "_All - Win32 Test"
# Name "_All - Win32 BCheckerD"
# Name "_All - Win32 BChecker"
# Name "_All - Win32 ICapD"
# Name "_All - Win32 ICap"
# Name "_All - Win32 TrueTimeD"
# Name "_All - Win32 TrueTime"

!IF  "$(CFG)" == "_All - Win32 Debug"

!ELSEIF  "$(CFG)" == "_All - Win32 Retail"

!ELSEIF  "$(CFG)" == "_All - Win32 Test"

!ELSEIF  "$(CFG)" == "_All - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "_All - Win32 BChecker"

!ELSEIF  "$(CFG)" == "_All - Win32 ICapD"

!ELSEIF  "$(CFG)" == "_All - Win32 ICap"

!ELSEIF  "$(CFG)" == "_All - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "_All - Win32 TrueTime"

!ENDIF 

# Begin Source File

SOURCE=.\machines.txt
# End Source File
# Begin Source File

SOURCE=.\makedep.inc
# End Source File
# Begin Source File

SOURCE=.\makeexe.inc
# End Source File
# Begin Source File

SOURCE=.\makeexes
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\makefile.inc
# End Source File
# Begin Source File

SOURCE=.\MkFromDS.mak
# End Source File
# Begin Source File

SOURCE=.\path.mak
# End Source File
# Begin Source File

SOURCE=.\phones.txt
# End Source File
# End Target
# End Project
