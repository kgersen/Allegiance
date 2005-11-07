
if "%1"=="" goto Usage

set COMPILELOG=%LOGDIR%\compile.log

rem
rem Compile the project.
rem

%FEDDRIVE%
cd %FEDROOT%

if "%NOSSYNC%"=="1" goto NoSsync
ssync -r -f

:NoSsync

cd %FEDSOURCEROOT%

if not exist %PROPDIR%\%1 mkdir %PROPDIR%\%1

title Compiling build %1
run.exe /b /ttc %COMPILELOG% nmake daily VER=%1
call %SCRIPTDIR%\verify\vercom.bat %1
set COMPILELOG=
goto End


:Usage
echo Usage: compile.bat [build number]
set BUILDERROR=1
goto End

:End


