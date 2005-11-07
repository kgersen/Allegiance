
if "%2"=="" goto Usage

echo %1 | awk.exe -f %SCRIPTDIR%\util\escstr.awk %2 > %TEMP%\escstr.bat
call %TEMP%\escstr.bat

goto End


:Usage
echo Usage: escstr.bat [string] [variable name]
set BUILDERROR=1
goto End


:End

