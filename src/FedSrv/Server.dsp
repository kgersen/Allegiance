# Microsoft Developer Studio Project File - Name="Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Server - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "Server - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\objs\Debug\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\Debug\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\objs\Debug\FedSrv"
# PROP Intermediate_Dir "..\..\objs\Debug\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\debug\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\debug\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Retail\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\Retail\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Retail\FedSrv"
# PROP Intermediate_Dir "..\..\objs\Retail\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\retail\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\retail\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Test\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\Test\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Test\FedSrv"
# PROP Intermediate_Dir "..\..\objs\Test\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\test\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\test\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BCheckerD\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\BCheckerD\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BCheckerD\FedSrv"
# PROP Intermediate_Dir "..\..\objs\BCheckerD\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\BCheckerD\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\BCheckerD\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BChecker\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\BChecker\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BChecker\FedSrv"
# PROP Intermediate_Dir "..\..\objs\BChecker\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\BChecker\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\BChecker\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICapD\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\ICapD\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICapD\FedSrv"
# PROP Intermediate_Dir "..\..\objs\ICapD\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\ICapD\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\ICapD\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICap\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\ICap\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICap\FedSrv"
# PROP Intermediate_Dir "..\..\objs\ICap\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\ICap\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\ICap\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTimeD\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTimeD\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTimeD\FedSrv"
# PROP Intermediate_Dir "..\..\objs\TrueTimeD\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\TrueTimeD\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\TrueTimeD\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Server - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTime\FedSrv"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTime\FedSrv"
# PROP BASE Cmd_Line "NMAKE /f Server.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrv.exe"
# PROP BASE Bsc_Name "AllSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTime\FedSrv"
# PROP Intermediate_Dir "..\..\objs\TrueTime\FedSrv"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=Server "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\TrueTime\FedSrv\AllSrv.exe"
# PROP Bsc_Name "..\..\objs\TrueTime\FedSrv\AllSrv.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Server - Win32 Debug"
# Name "Server - Win32 Retail"
# Name "Server - Win32 Test"
# Name "Server - Win32 BCheckerD"
# Name "Server - Win32 BChecker"
# Name "Server - Win32 ICapD"
# Name "Server - Win32 ICap"
# Name "Server - Win32 TrueTimeD"
# Name "Server - Win32 TrueTime"

!IF  "$(CFG)" == "Server - Win32 Debug"

!ELSEIF  "$(CFG)" == "Server - Win32 Retail"

!ELSEIF  "$(CFG)" == "Server - Win32 Test"

!ELSEIF  "$(CFG)" == "Server - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "Server - Win32 BChecker"

!ELSEIF  "$(CFG)" == "Server - Win32 ICapD"

!ELSEIF  "$(CFG)" == "Server - Win32 ICap"

!ELSEIF  "$(CFG)" == "Server - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "Server - Win32 TrueTime"

!ENDIF 

# Begin Group "ShareMem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\sharemem\pch.cpp
# End Source File
# Begin Source File

SOURCE=..\sharemem\pch.h
# End Source File
# Begin Source File

SOURCE=..\sharemem\sharemem.cpp
# End Source File
# End Group
# Begin Group "FedPerf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\fedperf\cntrmap.cpp
# End Source File
# Begin Source File

SOURCE=..\fedperf\cntrmap.h
# End Source File
# Begin Source File

SOURCE=..\fedperf\collect.cpp
# End Source File
# Begin Source File

SOURCE=..\fedperf\util\fedperf.h
# End Source File
# Begin Source File

SOURCE=..\fedperf\pch.cpp
# End Source File
# Begin Source File

SOURCE=..\fedperf\pch.h
# End Source File
# Begin Source File

SOURCE=..\fedperf\perfdll.cpp
# End Source File
# Begin Source File

SOURCE=..\fedperf\perfutil.cpp
# End Source File
# Begin Source File

SOURCE=..\fedperf\perfutil.h
# End Source File
# End Group
# Begin Group "Sentinal"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\sentinal\config.cpp
# End Source File
# Begin Source File

SOURCE=..\sentinal\config.h
# End Source File
# Begin Source File

SOURCE=..\sentinal\dbgutil.h
# End Source File
# Begin Source File

SOURCE=..\sentinal\dplaychk.cpp
# End Source File
# Begin Source File

SOURCE=..\sentinal\dplaychk.h
# End Source File
# Begin Source File

SOURCE=..\sentinal\makefile
# End Source File
# Begin Source File

SOURCE=..\sentinal\resource.h
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.cpp
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.def
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.h
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.ico
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.ini
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.rc
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentinal.reg
# End Source File
# Begin Source File

SOURCE=..\sentinal\sentmsg.msg
# End Source File
# Begin Source File

SOURCE=..\sentinal\srvdbg.cpp
# End Source File
# Begin Source File

SOURCE=..\sentinal\srvdbg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\_fedsrv.h
# End Source File
# Begin Source File

SOURCE=.\AdminEventLoggerHook.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminEventLoggerHook.h
# End Source File
# Begin Source File

SOURCE=.\AdminGame.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminGame.h
# End Source File
# Begin Source File

SOURCE=.\AdminGame.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminGames.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminGames.h
# End Source File
# Begin Source File

SOURCE=.\AdminGames.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminServer.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminServer.h
# End Source File
# Begin Source File

SOURCE=.\AdminServer.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminSession.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminSession.h
# End Source File
# Begin Source File

SOURCE=.\AdminSession.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminSessionClass.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminSessionClass.h
# End Source File
# Begin Source File

SOURCE=.\AdminSessionEventSink.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminSessionEventSink.h
# End Source File
# Begin Source File

SOURCE=.\AdminSessionSecure.h
# End Source File
# Begin Source File

SOURCE=.\AdminSessionSecureHost.h
# End Source File
# Begin Source File

SOURCE=.\AdminShip.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminShip.h
# End Source File
# Begin Source File

SOURCE=.\AdminShip.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminUser.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminUser.h
# End Source File
# Begin Source File

SOURCE=.\AdminUser.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminUsers.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminUsers.h
# End Source File
# Begin Source File

SOURCE=.\AdminUsers.rgs
# End Source File
# Begin Source File

SOURCE=.\AdminUtil.h
# End Source File
# Begin Source File

SOURCE=.\allsrv.rgs
# End Source File
# Begin Source File

SOURCE=.\AllSrv32.rgs
# End Source File
# Begin Source File

SOURCE=.\allsrvmodule.cpp
# End Source File
# Begin Source File

SOURCE=.\allsrvmodule.h
# End Source File
# Begin Source File

SOURCE=.\AllSrvModuleIDL.idl
# End Source File
# Begin Source File

SOURCE=.\BitArray.cpp
# End Source File
# Begin Source File

SOURCE=.\BitArray.h
# End Source File
# Begin Source File

SOURCE=.\config.cpp
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\counters.h
# End Source File
# Begin Source File

SOURCE=.\dbgutil.h
# End Source File
# Begin Source File

SOURCE=.\DIALOGS.CPP
# End Source File
# Begin Source File

SOURCE=.\dirmon.cpp
# End Source File
# Begin Source File

SOURCE=.\dirmon.h
# End Source File
# Begin Source File

SOURCE=.\DPMono.rgs
# End Source File
# Begin Source File

SOURCE=.\FedSrv.CPP
# End Source File
# Begin Source File

SOURCE=.\FedSrv.H
# End Source File
# Begin Source File

SOURCE=.\fedsrv.ico
# End Source File
# Begin Source File

SOURCE=.\FedSrv.RC
# End Source File
# Begin Source File

SOURCE=.\fedsrv.rc2
# End Source File
# Begin Source File

SOURCE=.\FedSrvApp.h
# End Source File
# Begin Source File

SOURCE=.\fedsrvsql.h
# End Source File
# Begin Source File

SOURCE=.\fscluster.cpp
# End Source File
# Begin Source File

SOURCE=.\fscluster.h
# End Source File
# Begin Source File

SOURCE=.\fslobby.cpp
# End Source File
# Begin Source File

SOURCE=.\fsmission.cpp
# End Source File
# Begin Source File

SOURCE=.\fsmission.h
# End Source File
# Begin Source File

SOURCE=.\fsship.cpp
# End Source File
# Begin Source File

SOURCE=.\fsship.h
# End Source File
# Begin Source File

SOURCE=.\fsside.cpp
# End Source File
# Begin Source File

SOURCE=.\fsside.h
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

SOURCE=.\queries.h
# End Source File
# Begin Source File

SOURCE=.\regini.txt
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\sqlhelp.cpp
# End Source File
# Begin Source File

SOURCE=..\srvconfig\srvconfig.ico
# End Source File
# Begin Source File

SOURCE=.\srvdbg.cpp
# End Source File
# Begin Source File

SOURCE=.\srvdbg.h
# End Source File
# Begin Source File

SOURCE=.\srvqguids.cpp
# End Source File
# Begin Source File

SOURCE=.\srvqueries.h
# End Source File
# Begin Source File

SOURCE=.\Status.htm
# End Source File
# Begin Source File

SOURCE=.\SWMRG.CPP
# End Source File
# Begin Source File

SOURCE=.\SWMRG.H
# End Source File
# End Target
# End Project
