
if "%2"=="" goto Usage

type %1 | awk -f %SCRIPTDIR%\util\todaysdate.awk %2 > %TEMP%\todaysdate.bat
call %TEMP%\todaysdate.bat
goto End

:Usage
echo Usage: todaysdate.bat [file] [variable name]
set BUILDERROR=1
goto End


:End


