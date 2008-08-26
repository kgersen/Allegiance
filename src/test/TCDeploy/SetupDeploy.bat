@echo off

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: SetupDeploy.bat: Installs TCDeploy.dll onto the local machine.
::

:: Ensure enough environment space under Win9x
if not '%OS%' == '' goto SkipEnv
  if '%1' == 'InternalEnv' goto SkipEnv
  %COMSPEC% /E:2048 /C \\research\root\oblivion\_Deploy\SetupDeploy.bat InternalEnv
  goto Exit
:SkipEnv

:: Determine the temporary directory log file name
if '%tmp%' == '' goto NoTMP
  set SetupLog=%tmp%\SetupDeploy.log
  set SetupStep=%tmp%\SetupStep.log
  set DeployStep=%tmp%\DeployStep.log
  goto TempEnd
:NoTMP
  if '%temp%' == '' goto NoTEMP
    set SetupLog=%temp%\SetupDeploy.log
    set SetupStep=%temp%\SetupStep.log
    set DeployStep=%temp%\DeployStep.log
    goto TempEnd
:NoTEMP
  if not '%OS%' == '' goto TempNT
    set SetupLog=SetupDeploy.log
    set SetupStep=SetupStep.log
    set DeployStep=DeployStep.log
    goto TempEnd
:TempNT
  set SetupLog=%windir%\SetupDeploy.log
  set SetupStep=%windir%\SetupStep.log
  set DeployStep=%windir%\DeployStep.log
:TempEnd

:: Determine WinNT vs. Win9x variables
if not '%OS%' == '' goto VarsNT
  set Redirect=
  set DeployEcho=
  set RUNDLL=start /WAIT RunDll32 %windir%\TCDeploy.dll
  set WinSys=system
  goto VarsEnd
:VarsNT
  set "Redirect=> %DeployStep%"
  set "DeployEcho=type %DeployStep% >> %SetupLog%&type %DeployStep%"
  set "RUNDLL=RunDll32 %windir%\TCDeploy.dll"
  set "WinSys=system32"
:VarsEnd
set RUNDLLCMD=%RUNDLL%,Command

:: Initialize the log file
echo DeploySetup.bat: Begin > %SetupLog%

:: Clean up current DLL, if installed
if not exist %windir%\TCDeploy.dll goto SkipExit

  :: Exit any currently running instance of the program
  echo Exiting any previous instance of TCDeploy.dll... > %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLLCMD% /Exit %Redirect%
    %DeployEcho%

  :: Unregister any currently installed library
  echo Unregistering any currently installed TCDeploy.dll... > %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLLCMD% /UnregServer %Redirect%
    %DeployEcho%

:: Temporary hack until TCDeploy works again under Win9x
GOTO SkipExit

  :: Wait for all processes using TCDeploy.dll to exit
  echo Waiting for all processes using TCDeploy.dll to exit... > %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLL%,CleanTempFiles
:WaitExit
  if exist %windir%\*.TCDeploy goto WaitExit
:SkipExit

:: Copy the TCDeploy.dll file to the Windows directory
echo Copying TCDeploy.dll... > %SetupStep%
  type %SetupStep% >> %SetupLog%
  type %SetupStep%
copy \\research\root\Oblivion\_Deploy\TCDeploy.dll %windir% > %SetupStep%
  type %SetupStep% >> %SetupLog%
  type %SetupStep%

:: Register the DLL differently for NT/9x
if not '%OS%' == '' goto RegNT
  
  :: Register the DLL
  echo Registering TCDeploy.dll... > %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLLCMD% /Regserver %Redirect%
    %DeployEcho%

:: Temporary hack until TCDeploy works again under Win9x
GOTO RegEnd

  :: Install the Startup shortcut item
  echo Installing Startup shortcut... > %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLLCMD% /Install %Redirect%
    %DeployEcho%

  :: Launch the object now
  echo Launching TCDeploy.dll... >> %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLLCMD% /NoExit %Redirect%
    %DeployEcho%
  goto RegEnd

:RegNT

  :: Register the DLL
  echo Registering TCDeploy.dll as "Interactive User"... > %SetupStep%
    type %SetupStep% >> %SetupLog%
    type %SetupStep%
  %RUNDLLCMD% /Regserver "Interactive User" %Redirect%
    %DeployEcho%
  goto RegEnd

:RegEnd

:: Output the exit message
echo DeploySetup.bat: End >> %SetupLog%

:: Reset environment variables
set SetupStep=
set SetupLog=
set RUNDLLCMD=
set RUNDLL=
set Redirect=
set DeployEcho=

:Exit
