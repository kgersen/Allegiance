
@ECHO OFF

if "%DEBUGTHEBUILD%"=="1" @ECHO ON

call \\fedbuild\fedsrc\src\build\server\init.bat

rem
rem Print out the last build number and input new build number.
rem
call %SCRIPTDIR%\util\maxbld.bat  %PROPDIR% BUILD
call %SCRIPTDIR%\util\maxbld.bat  %DIRECTXDIR% DIRECTXBUILD
set BVTLOGDIR=%PROPDIR%\%BUILD%\BVTLog
set LOGDIR=%BVTLOGDIR%\%COMPUTERNAME%
if not exist %LOGDIR% mkdir %LOGDIR%

:WaitLoop
if exist %BVTLOGDIR%\servsucc.txt goto Install
if exist %BVTLOGDIR%\failure.txt goto Install

sleep 600
goto WaitLoop


:Install

rem
rem Install the server and run the BVT.
rem
call %SCRIPTDIR%\server\servmain.bat %BUILD% %LOGDIR% oblsql %COMPUTERNAME%

set BVTLOGDIR=
set LOGDIR=
call %SCRIPTDIR%\server\term.bat



