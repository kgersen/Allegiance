
if "%2"=="" goto Usage

dir /AD /B /O-N %1 | awk.exe -f %SCRIPTDIR%\util\nextbld.awk %2 > %TEMP%\nextbld.bat
call %TEMP%\nextbld.bat

goto End


:Usage
echo Usage: nextbld.bat [directory] [variable name]
set BUILDERROR=1
goto End


:End

