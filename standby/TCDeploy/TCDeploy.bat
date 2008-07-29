@echo off

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: TCDeploy.bat: Gives TCDeploy.dll a command line interface
::

:: Ensure that the DLL exists in the Windows directory
if exist %windir%\TCDeploy.dll goto SkipExist
  echo Error: %windir%\TCDeploy.dll does not exist!
  goto Exit
:SkipExist

:: Determine the temporary directory log file name
if '%tmp%' == '' goto NoTMP
  set SetupStep=%tmp%\SetupStep.log
  goto TempEnd
:NoTMP
  if '%temp%' == '' goto NoTEMP
    set SetupStep=%temp%\SetupStep.log
    goto TempEnd
:NoTEMP
  if not '%OS%' == '' goto TempNT
    set SetupStep=SetupStep.log
    goto TempEnd
:TempNT
  set SetupStep=%windir%\SetupStep.log
:TempEnd

:: Run the DLL's command line
if not '%OS%' == '' goto CommandNT
  start /WAIT RunDll32 %windir%\TCDeploy.dll,Command %1 %2 %3 %4 %5 %6 %7 %8 %9 > %SetupStep%
  goto CommandExit
:CommandNT
  RunDll32 %windir%\TCDeploy.dll,Command %1 %2 %3 %4 %5 %6 %7 %8 %9 > %SetupStep%
:CommandExit
type %SetupStep%

:: Exit the batch file
:Exit

