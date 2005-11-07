
if "%2"=="" goto Usage

wc %1 | awk -f %SCRIPTDIR%\util\filesize.awk %2 > %TEMP%\filesize.bat
call %TEMP%\filesize.bat
goto End

:Usage
echo Usage: filesize.bat [file] [variable name]
set BUILDERROR=1
goto End


:End


