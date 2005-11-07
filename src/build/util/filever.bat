
if "%2"=="" goto Usage

filever.exe /B %1 | awk -f %SCRIPTDIR%\util\filever.awk %2 > %TEMP%\filever.bat
call %TEMP%\filever.bat
goto End

:Usage
echo Usage: filever.bat [file] [variable name]
set BUILDERROR=1
goto End


:End


