

if "%2"=="" goto Usage

set ISBUILDLOG=%LOGDIR%\isbuild.log

title Running InstallShield for build %1
call %SCRIPTDIR%\isbuild\isinit.bat


rem
rem Delete the old temp files.
rem
delnode /q "%ISMEDIADISK%"
if exist %ISTEMP% delnode /q %ISTEMP%
mkdir %ISTEMP%
mkdir %ISTEMP%\help

rem
rem Some files are copied from the build tree.  Since slm makes these
rem files read only, we have to copy them somewhere else so they don't
rem get installed as read only files.
rem
copy %FEDROOT%\src\bin\oblivwiz.exe     %ISTEMP%
copy %FEDROOT%\src\bin\tahomabd.ttf     %ISTEMP%
copy %FEDROOT%\src\bin\tahoma.ttf       %ISTEMP%
copy %FEDROOT%\src\bin\fsmon.exe        %ISTEMP%
copy %FEDROOT%\src\setup\msvcrt.dll     %ISTEMP%
copy %FEDROOT%\src\setup\msvcrtd.dll    %ISTEMP%
copy %FEDROOT%\src\setup\zticketc.dll   %ISTEMP%
copy %FEDROOT%\src\setup\zticketc.pdb   %ISTEMP%
xcopy /E /I %FEDROOT%\src\artwork\help\*   %ISTEMP%\help

rem
rem Run the InstallShield build process.
rem
run /b /ttc %ISBUILDLOG% isbuild.exe -p"%ISROOT%" -mDefault -b"%ISMEDIAROOT%"


rem
rem Copy the InstallShield build to the file share location.
rem
if not exist %PROPDIR%\%1 mkdir %PROPDIR%\%1
run /b /ttca %ISBUILDLOG% xcopy /I /S "%ISMEDIADISK%\*.*" %PROPDIR%\%1
run /b /ttca %ISBUILDLOG% xcopy /I /S %2 %PROPDIR%\%1

set ISBUILDLOG=
call %SCRIPTDIR%\isbuild\isterm.bat
goto End


:Usage
echo Usage: isbuild.bat [build number] [directx file location]
set BUILDERROR=1
goto End


:End


