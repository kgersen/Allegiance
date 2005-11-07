
if "%2"=="" goto Usage

type %1 | awk -f %SCRIPTDIR%\util\getdrive.awk %2 > %TEMP%\getdrive.bat
call %TEMP%\getdrive.bat
goto End


:Usage
echo Usage: getdriv2.bat [file name] [variable name]
set BUILDERROR=1
goto End


:End



