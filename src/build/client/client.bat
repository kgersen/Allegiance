
rem @ECHO OFF
if "%DEBUGTHEBUILD%"=="1" @ECHO ON

call \\fedbuild\fedsrc\src\build\client\init.bat

rem
rem Print out the last build number and input new build number.
rem
call %SCRIPTDIR%\util\maxbld.bat  %PROPDIR% BUILD
call %SCRIPTDIR%\util\maxbld.bat  %DIRECTXDIR% DIRECTXBUILD
set BVTLOGDIR=%PROPDIR%\%BUILD%\BVTLog
set LOGDIR=%BVTLOGDIR%\Clients
if not exist %LOGDIR% mkdir %LOGDIR%

:WaitLoop
if exist %BVTLOGDIR%\servsucc.txt goto Success
if exist %BVTLOGDIR%\failure.txt goto Error

sleep 600
goto WaitLoop


:Success

rem
rem Install the client.
rem
%PROPDIR%\%BUILD%\setup.exe
call %SCRIPTDIR%\util\runvt.bat %SCRIPTDIR%\client\install.pcd
if "%BUILDERROR%"=="1" goto Error


rem
rem Run the client BVT.
rem
call %SCRIPTDIR%\verify\vercli.bat %BUILD%
if "%BUILDERROR%"=="1" goto Error
goto End


:Error
echo An error occurred!
goto End


:End
set BVTLOGDIR=
set LOGDIR=
call %SCRIPTDIR%\client\term.bat
pause


