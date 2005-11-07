
if "%2"=="" goto Usage

dir /AD /B /O-N %1 | awk.exe -f %SCRIPTDIR%\util\maxbld.awk %2 > %TEMP%\maxbld.bat
call %TEMP%\maxbld.bat

goto End


:Usage
echo Usage: maxbld.bat [directory] [variable name]
set BUILDERROR=1
goto End


:End

