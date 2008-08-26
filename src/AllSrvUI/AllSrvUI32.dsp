# Microsoft Developer Studio Project File - Name="AllSrvUI32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=AllSrvUI32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AllSrvUI32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AllSrvUI32.mak" CFG="AllSrvUI32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AllSrvUI32 - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 Retail" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 Test" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 BCheckerD" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 BChecker" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 ICapD" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 ICap" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 TrueTimeD" (based on "Win32 (x86) External Target")
!MESSAGE "AllSrvUI32 - Win32 TrueTime" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "AllSrvUI32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\objs\Debug\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\Debug\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\objs\Debug\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\Debug\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=debug=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\debug\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\debug\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 Retail"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Retail\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\Retail\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Retail\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\Retail\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=retail=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\retail\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\retail\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\objs\Test\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\Test\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\Test\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\Test\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=test=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\test\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\test\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 BCheckerD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BCheckerD\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\BCheckerD\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BCheckerD\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\BCheckerD\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=debug= BCHK=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\BCheckerD\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\BCheckerD\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 BChecker"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\BChecker\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\BChecker\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\BChecker\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\BChecker\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=retail= BCHK=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\BChecker\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\BChecker\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 ICapD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICapD\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\ICapD\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICapD\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\ICapD\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=debug= ICAP=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\ICapD\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\ICapD\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 ICap"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\ICap\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\ICap\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\ICap\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\ICap\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=retail= ICAP=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\ICap\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\ICap\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 TrueTimeD"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTimeD\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTimeD\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTimeD\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\TrueTimeD\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=debug= TRUETIME=""
# PROP Rebuild_Opt "FULL="
# PROP Target_File "..\..\objs\TrueTimeD\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\TrueTimeD\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 TrueTime"

# PROP BASE Use_MFC 0
# PROP BASE Output_Dir "..\..\objs\TrueTime\AllSrvUI32"
# PROP BASE Intermediate_Dir "..\..\objs\TrueTime\AllSrvUI32"
# PROP BASE Cmd_Line "NMAKE /f AllSrvUI32.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AllSrvUI32.exe"
# PROP BASE Bsc_Name "AllSrvUI32.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Output_Dir "..\..\objs\TrueTime\AllSrvUI32"
# PROP Intermediate_Dir "..\..\objs\TrueTime\AllSrvUI32"
# PROP Cmd_Line "nmake -f ..\MkFromDS.mak /nologo TARGET=AllSrvUI32App "FLAVOR=retail= TRUETIME=""
# PROP Rebuild_Opt "FULL= "
# PROP Target_File "..\..\objs\TrueTime\AllSrvUI32\AllSrvUI32.exe"
# PROP Bsc_Name "..\..\objs\TrueTime\AllSrvUI32\AllSrvUI32.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "AllSrvUI32 - Win32 Debug"
# Name "AllSrvUI32 - Win32 Retail"
# Name "AllSrvUI32 - Win32 Test"
# Name "AllSrvUI32 - Win32 BCheckerD"
# Name "AllSrvUI32 - Win32 BChecker"
# Name "AllSrvUI32 - Win32 ICapD"
# Name "AllSrvUI32 - Win32 ICap"
# Name "AllSrvUI32 - Win32 TrueTimeD"
# Name "AllSrvUI32 - Win32 TrueTime"

!IF  "$(CFG)" == "AllSrvUI32 - Win32 Debug"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 Retail"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 Test"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 BCheckerD"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 BChecker"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 ICapD"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 ICap"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 TrueTimeD"

!ELSEIF  "$(CFG)" == "AllSrvUI32 - Win32 TrueTime"

!ENDIF 

# Begin Source File

SOURCE=.\AdminPageThreadWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminPageThreadWnd.h
# End Source File
# Begin Source File

SOURCE=..\FedSrv\AdminSessionSecure.h
# End Source File
# Begin Source File

SOURCE=.\AllSrvUI.cpp
# End Source File
# Begin Source File

SOURCE=.\AllSrvUI.h
# End Source File
# Begin Source File

SOURCE=.\AllSrvUI.ico
# End Source File
# Begin Source File

SOURCE=.\AllSrvUI.rc
# End Source File
# Begin Source File

SOURCE=.\AllSrvUI.rc2
# End Source File
# Begin Source File

SOURCE=.\AllSrvUISheet.cpp
# End Source File
# Begin Source File

SOURCE=.\AllSrvUISheet.h
# End Source File
# Begin Source File

SOURCE=.\ArtifactRun.ico
# End Source File
# Begin Source File

SOURCE=.\AutoSizer.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoSizer.h
# End Source File
# Begin Source File

SOURCE=.\Conquest.ico
# End Source File
# Begin Source File

SOURCE=.\CountDown.ico
# End Source File
# Begin Source File

SOURCE=.\DeathMatch.ico
# End Source File
# Begin Source File

SOURCE=.\DlgAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAbout.h
# End Source File
# Begin Source File

SOURCE=.\DlgChatPreferences.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChatPreferences.h
# End Source File
# Begin Source File

SOURCE=.\DlgGameSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGameSelect.h
# End Source File
# Begin Source File

SOURCE=.\Flags.ico
# End Source File
# Begin Source File

SOURCE=.\GamePageSpecifics.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePageSpecifics.h
# End Source File
# Begin Source File

SOURCE=.\GamePageTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePageTeam.h
# End Source File
# Begin Source File

SOURCE=.\GamePageWin.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePageWin.h
# End Source File
# Begin Source File

SOURCE=.\GameParamsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GameParamsSheet.h
# End Source File
# Begin Source File

SOURCE=.\Inbalance.ico
# End Source File
# Begin Source File

SOURCE=.\Lives.ico
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\MaxTeam.ico
# End Source File
# Begin Source File

SOURCE=.\MinTeam.ico
# End Source File
# Begin Source File

SOURCE=.\PageChat.cpp
# End Source File
# Begin Source File

SOURCE=.\PageChat.h
# End Source File
# Begin Source File

SOURCE=.\PageConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\PageConnect.h
# End Source File
# Begin Source File

SOURCE=.\PageDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDummy.h
# End Source File
# Begin Source File

SOURCE=.\PageGameCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\PageGameCreate.h
# End Source File
# Begin Source File

SOURCE=.\PagePlayers.cpp
# End Source File
# Begin Source File

SOURCE=.\PagePlayers.h
# End Source File
# Begin Source File

SOURCE=.\Prosperity.ico
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Skill.ico
# End Source File
# Begin Source File

SOURCE=.\StartMoney.ico
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TeamCount.ico
# End Source File
# Begin Source File

SOURCE=.\TotalMoney.ico
# End Source File
# Begin Source File

SOURCE=.\zone.bmp
# End Source File
# End Target
# End Project
