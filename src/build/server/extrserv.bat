
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
if exist %BVTLOGDIR%\servsucc.txt goto Success
if exist %BVTLOGDIR%\failure.txt goto Stress

sleep 600
goto WaitLoop


:Success

rem
rem Install the server and run the BVT.
rem
call %SCRIPTDIR%\server\servmain.bat %BUILD% %LOGDIR% oblsql %COMPUTERNAME%
if "%BUILDERROR%"=="1" goto End


rem
rem Copy new pigs binaries to the stress share.
rem
copy %PROPDIR%\%BUILD%\test\pigs.exe \\%COMPUTERNAME%\stress\pigs\bin
copy %PROPDIR%\%BUILD%\test\pigs.pdb \\%COMPUTERNAME%\stress\pigs\bin
goto Stress


:Stress
rem
rem Start perfmon
rem
call %SCRIPTDIR%\server\perfmon.bat %SCRIPTDIR%\server\%COMPUTERNAME%.pml C:\fedsrv.log

rem
rem Run stress.
rem
call \\%COMPUTERNAME%\stress\startit.bat


rem
rem Sleep for ten minutes and then get rid of the stress file.
rem
sleep 600
call \\%COMPUTERNAME%\stress\stopit.bat


rem
rem Sleep for four hours, ten minutes to allow stress to run and stop.
rem
sleep 15000


rem
rem Run the server BVT to make sure we're working after stress.
rem
call %SCRIPTDIR%\verify\verserv.bat %BUILD% %COMPUTERNAME% oblsql %COMPUTERNAME%
if "%BUILDERROR%"=="1" goto Error
echo Success. > %LOGDIR%\servsucc.txt
goto End


:Error
echo An error occurred!
echo Failure. > %LOGDIR%\servfail.txt
goto End


:End
set BVTLOGDIR=
set LOGDIR=
call %SCRIPTDIR%\server\term.bat



