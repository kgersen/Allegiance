
if "%1"=="" goto Usage

set COPYLOG=%LOGDIR%\copy.log

rem
rem Copy the built files.
rem

run /b /ttc  %COPYLOG% %SCRIPTDIR%\copy\copybin.bat %1
run /b /ttca %COPYLOG% %SCRIPTDIR%\copy\copysrc.bat %1
%SCRIPTDIR%\verify\vercopy.bat %1

set COPYLOG=
goto End


:Usage
echo Usage: copy.bat [build number]
set BUILDERROR=1
goto End


:End


