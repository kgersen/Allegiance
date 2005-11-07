# Microsoft Developer Studio Project File - Name="TestOld" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=TCDeployExportOld - Win32 Retail
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TCDeployExportOld.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TCDeployExportOld.mak" CFG="TCDeployExportOld - Win32 Retail"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TCDeployExportOld - Win32 Retail" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Retail"
# PROP BASE Intermediate_Dir "Retail"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\\research\root\Oblivion\_Deploy"
# PROP Intermediate_Dir "\\research\root\Oblivion\_Deploy"
# PROP Target_Dir ""
# Begin Target

# Name "TCDeployExportOld - Win32 Retail"
# Begin Source File

SOURCE=.\Export.bat
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Exporting Deployment Files to the Network...
InputPath=.\Export.bat

BuildCmds= \
	$(InputPath)

"\\research\root\Oblivion\_Deploy\TCDeploy.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"\\research\root\Oblivion\_Deploy\SetupDeploy.bat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"\\research\root\Oblivion\_Deploy\IsNewer.vbs" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build
# End Source File
# End Target
# End Project
