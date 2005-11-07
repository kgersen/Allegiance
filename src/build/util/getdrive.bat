
if "%2"=="" goto Usage

echo %1 | awk -f %SCRIPTDIR%\util\getdrive.awk %2 > %TEMP%\getdrive.bat
call %TEMP%\getdrive.bat
goto End


:Usage
echo Usage: getdrive.bat [path string] [variable name]
set BUILDERROR=1
goto End


:End



