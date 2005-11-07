
if "%1"=="" goto Usage

set BUILDLOG=%LOGDIR%\build.log

if not exist %LOGDIR%\compile.err  echo Not run. > %LOGDIR%\compile.err
if not exist %LOGDIR%\compile.wrn  echo Not run. > %LOGDIR%\compile.wrn
if not exist %LOGDIR%\copy.err     echo Not run. > %LOGDIR%\copy.err
if not exist %LOGDIR%\verdbtbl.err echo Not run. > %LOGDIR%\verdbtbl.err
if not exist %LOGDIR%\verdbdta.wrn echo Not run. > %LOGDIR%\verdbdta.wrn

echo Log for build %1 > %BUILDLOG%
echo ------------------ >> %BUILDLOG%
echo Compilation errors: >> %BUILDLOG%
type %LOGDIR%\compile.err >> %BUILDLOG%
echo Compilation warnings: >> %BUILDLOG%
type %LOGDIR%\compile.wrn >> %BUILDLOG%
echo Copy errors: >> %BUILDLOG%
type %LOGDIR%\copy.err >> %BUILDLOG%
echo Database install errors: >> %BUILDLOG%
type %LOGDIR%\verdbtbl.err >> %BUILDLOG%
echo Database data warnings: >> %BUILDLOG%
type %LOGDIR%\verdbdta.wrn >> %BUILDLOG%

set BUILDLOG=
goto End


:Usage
echo Usage: buildlog.bat [build number]
goto End


:End


