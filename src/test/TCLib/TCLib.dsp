# Microsoft Developer Studio Project File - Name="TCLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=TCLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TCLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TCLib.mak" CFG="TCLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TCLib - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "TCLib - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "TCLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\objs\Debug\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\Debug\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir "..\..\..\objs\Debug\Test\TCLib"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\objs\Debug\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\Debug\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\debug\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\debug\test\tclib\tclib.bsc"
# PROP Target_Dir "..\..\..\objs\Debug\Test\TCLib"

!ELSEIF  "$(CFG)" == "TCLib - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Retail\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\Retail\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Retail\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\Retail\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\retail\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\retail\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\objs\Test\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\Test\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\objs\Test\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\Test\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\test\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\test\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BCheckerD\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\BCheckerD\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BCheckerD\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\BCheckerD\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\BCheckerD\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\BCheckerD\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\BChecker\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\BChecker\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\BChecker\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\BChecker\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\BChecker\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\BChecker\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICapD\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICapD\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICapD\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\ICapD\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\ICapD\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\ICapD\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\ICap\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\ICap\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\ICap\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\ICap\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\ICap\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\ICap\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTimeD\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTimeD\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\TrueTimeD\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\..\objs\TrueTimeD\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\TrueTimeD\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "TCLib - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\..\objs\TrueTime\Test\TCLib"
# PROP BASE Intermediate_Dir "..\..\..\objs\TrueTime\Test\TCLib"
# PROP BASE Cmd_Line "NMAKE /f TCLib.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "TCLib.lib"
# PROP BASE Bsc_Name "TCLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\..\objs\TrueTime\Test\TCLib"
# PROP Intermediate_Dir "..\..\..\objs\TrueTime\Test\TCLib"
# PROP Cmd_Line "nmake -f ..\..\MkFromDS.mak /nologo TARGET=TCLib "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\..\objs\TrueTime\test\tclib\tclib.lib"
# PROP Bsc_Name "..\..\..\objs\TrueTime\test\tclib\tclib.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "TCLib - Win32 Debug"
# Name "TCLib - Win32 Retail"
# Name "TCLib - Win32 Test"
# Name "TCLib - Win32 BCheckerD"
# Name "TCLib - Win32 BChecker"
# Name "TCLib - Win32 ICapD"
# Name "TCLib - Win32 ICap"
# Name "TCLib - Win32 TrueTimeD"
# Name "TCLib - Win32 TrueTime"

!IF  "$(CFG)" == "TCLib - Win32 Debug"

!ELSEIF  "$(CFG)" == "TCLib - Win32 Retail"

!ELSEIF  "$(CFG)" == "TCLib - Win32 Test"

!ELSEIF  "$(CFG)" == "TCLib - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "TCLib - Win32 BChecker"

!ELSEIF  "$(CFG)" == "TCLib - Win32 ICapD"

!ELSEIF  "$(CFG)" == "TCLib - Win32 ICap"

!ELSEIF  "$(CFG)" == "TCLib - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "TCLib - Win32 TrueTime"

!ENDIF 

# Begin Source File

SOURCE=.\AdviseHolder.h
# End Source File
# Begin Source File

SOURCE=.\AutoCriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\AutoHandle.h
# End Source File
# Begin Source File

SOURCE=.\AutoMutex.h
# End Source File
# Begin Source File

SOURCE=.\BinString.h
# End Source File
# Begin Source File

SOURCE=.\CoInit.h
# End Source File
# Begin Source File

SOURCE=.\CommonControls.h
# End Source File
# Begin Source File

SOURCE=.\CompositeValue.h
# End Source File
# Begin Source File

SOURCE=.\DynaLib.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaLib.h
# End Source File
# Begin Source File

SOURCE=.\ExcepInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcepInfo.h
# End Source File
# Begin Source File

SOURCE=.\InvokeArgs.cpp
# End Source File
# Begin Source File

SOURCE=.\InvokeArgs.h
# End Source File
# Begin Source File

SOURCE=.\LookupTable.h
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\ObjectLock.h
# End Source File
# Begin Source File

SOURCE=.\pch.cpp
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSite.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSite.h
# End Source File
# Begin Source File

SOURCE=.\Range.h
# End Source File
# Begin Source File

SOURCE=.\RangeSet.h
# End Source File
# Begin Source File

SOURCE=.\RefCountedData.h
# End Source File
# Begin Source File

SOURCE=.\RelPath.cpp
# End Source File
# Begin Source File

SOURCE=.\RelPath.h
# End Source File
# Begin Source File

SOURCE=.\StrManip.cpp
# End Source File
# Begin Source File

SOURCE=.\StrManip.h
# End Source File
# Begin Source File

SOURCE=.\TCCAUUID.h
# End Source File
# Begin Source File

SOURCE=.\TCLib.cpp
# End Source File
# Begin Source File

SOURCE=..\Inc\TCLib.h
# End Source File
# Begin Source File

SOURCE=.\TCThread.cpp
# End Source File
# Begin Source File

SOURCE=.\TCThread.h
# End Source File
# Begin Source File

SOURCE=.\TraceScope.h
# End Source File
# Begin Source File

SOURCE=.\tstring.h
# End Source File
# Begin Source File

SOURCE=.\UtilImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\UtilImpl.h
# End Source File
# Begin Source File

SOURCE=.\UtilityThread.cpp
# End Source File
# Begin Source File

SOURCE=.\UtilityThread.h
# End Source File
# Begin Source File

SOURCE=.\WinApp.h
# End Source File
# Begin Source File

SOURCE=.\WindowThreadBase.h
# End Source File
# Begin Source File

SOURCE=.\WindowThreadImpl.h
# End Source File
# Begin Source File

SOURCE=.\WorkerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkerThread.h
# End Source File
# Begin Source File

SOURCE=.\WotLib.h
# End Source File
# End Target
# End Project
