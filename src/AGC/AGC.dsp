# Microsoft Developer Studio Project File - Name="AGC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=AGC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AGC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AGC.mak" CFG="AGC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AGC - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "AGC - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "AGC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\objs\Debug\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\Debug\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\objs\Debug\AGC"
# PROP Intermediate_Dir "..\..\objs\Debug\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\debug\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\debug\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Retail\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\Retail\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Retail\AGC"
# PROP Intermediate_Dir "..\..\objs\Retail\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\retail\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\retail\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Test\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\Test\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Test\AGC"
# PROP Intermediate_Dir "..\..\objs\Test\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\test\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\test\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BCheckerD\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\BCheckerD\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BCheckerD\AGC"
# PROP Intermediate_Dir "..\..\objs\BCheckerD\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\BCheckerD\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\BCheckerD\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BChecker\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\BChecker\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BChecker\AGC"
# PROP Intermediate_Dir "..\..\objs\BChecker\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\BChecker\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\BChecker\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICapD\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\ICapD\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICapD\AGC"
# PROP Intermediate_Dir "..\..\objs\ICapD\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\ICapD\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\ICapD\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICap\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\ICap\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICap\AGC"
# PROP Intermediate_Dir "..\..\objs\ICap\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\ICap\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\ICap\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTimeD\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTimeD\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTimeD\AGC"
# PROP Intermediate_Dir "..\..\objs\TrueTimeD\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\TrueTimeD\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\TrueTimeD\AGC\AGC.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AGC - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTime\AGC"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTime\AGC"
# PROP BASE Cmd_Line "NMAKE /f AGC.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AGC.dll"
# PROP BASE Bsc_Name "AGC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTime\AGC"
# PROP Intermediate_Dir "..\..\objs\TrueTime\AGC"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AGC "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\TrueTime\AGC\AGC.dll"
# PROP Bsc_Name "..\..\objs\TrueTime\AGC\AGC.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "AGC - Win32 Debug"
# Name "AGC - Win32 Retail"
# Name "AGC - Win32 Test"
# Name "AGC - Win32 BCheckerD"
# Name "AGC - Win32 BChecker"
# Name "AGC - Win32 ICapD"
# Name "AGC - Win32 ICap"
# Name "AGC - Win32 TrueTimeD"
# Name "AGC - Win32 TrueTime"

!IF  "$(CFG)" == "AGC - Win32 Debug"

!ELSEIF  "$(CFG)" == "AGC - Win32 Retail"

!ELSEIF  "$(CFG)" == "AGC - Win32 Test"

!ELSEIF  "$(CFG)" == "AGC - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "AGC - Win32 BChecker"

!ELSEIF  "$(CFG)" == "AGC - Win32 ICapD"

!ELSEIF  "$(CFG)" == "AGC - Win32 ICap"

!ELSEIF  "$(CFG)" == "AGC - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "AGC - Win32 TrueTime"

!ENDIF 

# Begin Group "AGCEvents"

# PROP Default_Filter "xml;xsl;js;htm"
# Begin Source File

SOURCE=.\AGCEvents.xml
# End Source File
# Begin Source File

SOURCE=.\AGCEventsCPP.xsl
# End Source File
# Begin Source File

SOURCE=.\AGCEventsIDL.xsl
# End Source File
# Begin Source File

SOURCE=.\AGCEventsMC.xsl
# End Source File
# Begin Source File

SOURCE=.\AGCEventsRC2.xsl
# End Source File
# Begin Source File

SOURCE=.\AGCEventsRCH.xsl
# End Source File
# Begin Source File

SOURCE=.\EventBrowser.xsl
# End Source File
# Begin Source File

SOURCE=..\test\Scripts\EventViewer.htm
# End Source File
# Begin Source File

SOURCE=.\XMLXForm.js
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdminSessionHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminSessionHelper.h
# End Source File
# Begin Source File

SOURCE=.\AdminSessionHelper.rgs
# End Source File
# Begin Source File

SOURCE=.\AGC.cpp
# End Source File
# Begin Source File

SOURCE=.\AGC.def
# End Source File
# Begin Source File

SOURCE=.\agc.idl
# End Source File
# Begin Source File

SOURCE=.\AGC.rc
# End Source File
# Begin Source File

SOURCE=.\AGC.rc2
# End Source File
# Begin Source File

SOURCE=.\AGCAleph.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCAleph.h
# End Source File
# Begin Source File

SOURCE=.\AGCAleph.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCAlephs.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCAlephs.h
# End Source File
# Begin Source File

SOURCE=.\AGCAlephs.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCAsteroid.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCAsteroid.h
# End Source File
# Begin Source File

SOURCE=.\AGCAsteroid.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCAsteroids.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCAsteroids.h
# End Source File
# Begin Source File

SOURCE=.\AGCAsteroids.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCChat.h
# End Source File
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

SOURCE=.\AGCCP.h
# End Source File
# Begin Source File

SOURCE=.\AGCDBParams.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCDBParams.h
# End Source File
# Begin Source File

SOURCE=.\AGCDBParams.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCDebugHook.h
# End Source File
# Begin Source File

SOURCE=.\agcevent.cpp
# End Source File
# Begin Source File

SOURCE=.\agcevent.h
# End Source File
# Begin Source File

SOURCE=.\agcevent.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCEventData.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCEventData.h
# End Source File
# Begin Source File

SOURCE=.\AGCEventDef.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCEventDef.h
# End Source File
# Begin Source File

SOURCE=.\AGCEventIDRange.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCEventIDRange.h
# End Source File
# Begin Source File

SOURCE=.\AGCEventIDRange.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCEventIDRanges.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCEventIDRanges.h
# End Source File
# Begin Source File

SOURCE=.\AGCEventIDRanges.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCEventLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCEventLogger.h
# End Source File
# Begin Source File

SOURCE=.\AGCEventLogger.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCEventThread.h
# End Source File
# Begin Source File

SOURCE=.\AGCGame.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCGame.h
# End Source File
# Begin Source File

SOURCE=.\AGCGame.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCGameParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCGameParameters.h
# End Source File
# Begin Source File

SOURCE=.\AGCGameParameters.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCGlobal.h
# End Source File
# Begin Source File

SOURCE=.\AGCGlobal.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCGuid.c
# End Source File
# Begin Source File

SOURCE=.\AGCHullType.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCHullType.h
# End Source File
# Begin Source File

SOURCE=.\AGCHullType.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCHullTypes.rgs
# End Source File
# Begin Source File

SOURCE=..\Inc\AGCIDL.idl
# End Source File
# Begin Source File

SOURCE=.\AGCModel.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCModel.h
# End Source File
# Begin Source File

SOURCE=.\AGCModel.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCModels.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCModels.h
# End Source File
# Begin Source File

SOURCE=.\AGCModels.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCModule.h
# End Source File
# Begin Source File

SOURCE=.\AGCOrientation.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCOrientation.h
# End Source File
# Begin Source File

SOURCE=.\AGCOrientation.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCProbe.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCProbe.h
# End Source File
# Begin Source File

SOURCE=.\AGCProbe.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCProbes.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCProbes.h
# End Source File
# Begin Source File

SOURCE=.\AGCProbes.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCSector.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCSector.h
# End Source File
# Begin Source File

SOURCE=.\AGCSector.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCSectors.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCSectors.h
# End Source File
# Begin Source File

SOURCE=.\AGCSectors.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCShip.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCShip.h
# End Source File
# Begin Source File

SOURCE=.\AGCShip.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCShips.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCShips.h
# End Source File
# Begin Source File

SOURCE=.\AGCShips.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCSite.h
# End Source File
# Begin Source File

SOURCE=.\AGCStation.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCStation.h
# End Source File
# Begin Source File

SOURCE=.\AGCStation.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCStations.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCStations.h
# End Source File
# Begin Source File

SOURCE=.\AGCStations.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCTeam.h
# End Source File
# Begin Source File

SOURCE=.\AGCTeam.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCTeams.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCTeams.h
# End Source File
# Begin Source File

SOURCE=.\AGCTeams.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCUtil.h
# End Source File
# Begin Source File

SOURCE=.\AGCVector.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCVector.h
# End Source File
# Begin Source File

SOURCE=.\AGCVector.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AGCVersionInfo.h
# End Source File
# Begin Source File

SOURCE=.\AGCVersionInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\AGCWinApp.h
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# End Source File
# Begin Source File

SOURCE=.\gameparamdata.h
# End Source File
# Begin Source File

SOURCE=.\IAGCAlephImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCAsteroidImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCBaseImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCBuyableImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCCollectionImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCCommonImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCDamageImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCGameImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCModelImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCProbeImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCRangeImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCRangesImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCScannerImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCSectorImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCShipImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCStationImpl.h
# End Source File
# Begin Source File

SOURCE=.\IAGCTeamImpl.h
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

SOURCE=.\TCMarshalByValue.cpp
# End Source File
# Begin Source File

SOURCE=.\TCMarshalByValue.h
# End Source File
# Begin Source File

SOURCE=.\TCMarshalByValue.rgs
# End Source File
# Begin Source File

SOURCE=.\TCNullStream.cpp
# End Source File
# Begin Source File

SOURCE=.\TCNullStream.h
# End Source File
# Begin Source File

SOURCE=.\TCNullStream.rgs
# End Source File
# Begin Source File

SOURCE=.\TCStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\TCStrings.h
# End Source File
# Begin Source File

SOURCE=.\TCStrings.rgs
# End Source File
# Begin Source File

SOURCE=.\TCUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\TCUtility.h
# End Source File
# Begin Source File

SOURCE=.\TCUtility.rgs
# End Source File
# End Target
# End Project
