
if "%4"=="" goto Usage

set PIGSDRIVE=c:
set PIGSDIR=c:\pigs
set PIGSTEMPLOG=%TEMP%\pigs.log
if "%LOGDIR%"=="" set LOGDIR=%PIGSDIR%
set PIGSLOG=%LOGDIR%\servbvt.log
set PIGSERRLOG=%LOGDIR%\servbvt.err

rem
rem The first thing we need to do is copy pigs and its associated files.
rem
call %PROPDIR%\%1\test\setup.bat
if exist %PIGSDIR%\error.log del %PIGSDIR%\error.log
if exist %PIGSDIR%\event.log del %PIGSDIR%\event.log

rem
rem Make sure we have database accounts for pigs
rem
isql.exe -S %3 -E -d %4 -n -e -Q "exec CreatePigsMachine '%COMPUTERNAME%'"
isql.exe -S %3 -E -d %4 -n -e -Q "exec CreatePigsMachine '%COMPUTERNAME%L'"

rem
rem Run pigs
rem
%PIGSDRIVE%
cd %PIGSDIR%
start pigs -Server %2
sleep 30
cd losers
pigs -Server %2 -INIFile pigsl.ini
cd ..


rem
rem Check the pigs log for success or failure.
rem
findstr Successfully event.log > %PIGSTEMPLOG%
call %SCRIPTDIR%\util\filesize.bat %PIGSTEMPLOG% FILESIZE
if not "%FILESIZE%"=="0" goto Success
copy event.log %PIGSLOG%
copy error.log %PIGSERRLOG%
set BUILDERROR=1
goto AfterCheck


:Success
echo No errors found. > %PIGSERRLOG%
goto AfterCheck


:AfterCheck
set PIGSDRIVE=
set PIGSDIR=
set PIGSTEMPLOG=
set PIGSERRLOG=
set FILESIZE=
goto End


:Usage
echo Usage: verserv [Build] [Allegiance Server] [SQL Server] [SQL Database]
set BUILDERROR=1
goto End


:End


