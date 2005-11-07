
if "%2"=="" goto Usage

dir /s %1 | awk -f %SCRIPTDIR%\util\findpath.awk %2 > %TEMP%\findpath.bat
call %TEMP%\findpath.bat
goto End


:Usage
echo Usage: findpath.bat [file and path] [variable name]
set BUILDERROR=1
goto End


:End


